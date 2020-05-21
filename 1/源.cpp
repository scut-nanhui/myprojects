#include <stdio.h>
#include<iostream>
#include<string>
#include<time.h>
#include<ctime>
#include<windows.h>
#include<graphics.h>
#include<string>
#include <conio.h>
using namespace std;
#define n 16
#define samekey same(row + dx[u] * i, col + dy[u] * i, key)
#define sumkadd for (i = 1; samekey; i++)sumk++; 
#define sumksub for (i = -1; samekey; i--)sumk++;
#define off if(checkPiece[row + dx[u] * i][col + dy[u] * i] == 2||checkPiece[row + dx[u] * i][col + dy[u] * i] == 1||inboard(row + dx[u] * i, col + dy[u] * i) )continue;
#define temp0 if (tempp == 0){checkPiece[i][j] = 0;continue;}

int s = 0, ais = 1, s0;//s是轮到谁下,s=1,2，s=1是ai下，s=2是玩家，s=s0是黑方下，否则是白方下
int manu[2][300], manukey = 0;
int points[n][n + 1];//计算各点分数

int dx[8] = { 1, 1, 0, -1, -1, -1, 0, 1 }; //flat技术
int dy[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };//（dx,dy）是8个方向向量
int checkPiece[16][17];//记录棋子有无及颜色，1为白色，2为黑色,3为禁区
int type;//人机还是人人
bool b=0, d = 0, is_end = false;//检查是否有棋子已经被下入
int chessColor = 2;//电脑红色双数，人单数白色棋子
POINT cursor, a, re;
RECT rect;
HWND hwnd;
HANDLE hInput;
HANDLE hOut;
INPUT_RECORD keyrec;      //定义输入事件结构体
DWORD res;      //用于存储读取记录



//能否落子
bool ok(int row, int col)
{
	return checkPiece[row][col] == 0;
}

//是否在棋盘内
bool inboard(int row, int col)
{
	if (row <0 || row >15 )return false;
	return col >= 1 && col <= 16;
}

//判断两个点颜色是否相同
bool same(int i, int j, int key)
{
	if (checkPiece[i][j] == key)
		return true;
	else
		return false;
}

//横向找
bool find1(int x, int y)
{
	int i, num = 1;
	int temp = checkPiece[x][y];
	for (i = x + 1; i < 16; i++)
	{// 向左找
		if (checkPiece[i][y] == temp)
		{
			num++;
			if (num == 5)
			{
				return true;
			}
		}
		else
		{
			break;
		}
	}
	for (i = x - 1; i >= 0; i--)
	{// 向右找
		if (checkPiece[i][y] == temp)
		{
			num++;
			if (num == 5)
			{
				return true;
			}
		}
		else
		{
			break;
		}
	}
	return false;
}

//纵向找
bool find2(int x, int y)
{
	int i, num = 1;
	int temp = checkPiece[x][y];
	for (i = y +1; i < 17; i++)
	{ // 向上找
		if (checkPiece[x][i] == temp)
		{
			num++;
			if (num == 5)
			{
				return true;
			}
		}
		else
		{
			break;
		}
	}
	for (i = y-1; i >= 1; i--)
	{// 向下找
		if (checkPiece[x][i] == temp)
		{
			num++;
			if (num == 5)
			{
				return true;
			}
		}
		else
		{
			break;
		}
	}
	return false;
}

//.\方向
bool find3(int x, int y)
{
	int i, j, num = 1;
	int temp = checkPiece[x][y ];
	for (i = x + 1, j = y + 1; i < 16 && j < 17; i++, j++) {// 向下方
		if (checkPiece[i][j] == temp)
		{
			num++;
			if (num == 5)
			{
				return true;
			}
		}
		else
		{
			break;
		}
	}
	for (i = x - 1, j = y-1; i >= 0 && j >= 1; i--, j--)
	{// 向上方
		if (checkPiece[i][j] == temp)
		{
			num++;
			if (num == 5)
			{
				return true;
			}
		}
		else
		{
			break;
		}
	}
	return false;
}

//./方向
bool find4(int x, int y)
{
	int i, j, num = 1;
	int temp = checkPiece[x][y ];
	for (i = x + 1, j = y-1; i < 16 && j >=1; i++, j--)
	{// 向下
		if (checkPiece[i][j] == temp)
		{
			num++;
			if (num == 5)
			{
				return true;
			}
		}
		else
		{
			break;
		}
	}
	for (i = x - 1, j = y + 1; i >= 0 && j < 17; i--, j++)
	{ // 向上
		if (checkPiece[i][j] == temp)
		{
			num++;
			if (num == 5)
			{
				return true;
			}
		}
		else
		{
			break;
		}
	}
	return false;
}

//判断胜负
bool isWin(int x, int y)
{
	return (find1(x, y) || find2(x, y) || find3(x, y) || find4(x, y));

}

//坐标（row,col），方向向量u
int num(int row, int col, int u)
{
	int i = row + dx[u], j = col + dy[u], sum = 0, ref = checkPiece[row][col];
	if (ref == 0)return 0;
	while (same(i, j, ref))sum++, i += dx[u], j += dy[u];
	return sum;
}

//长连禁手及五个及以上的棋子
bool overline(int row, int col)
{
	bool flag = false;
	int u;
	for (u = 0; u < 4; u++)
		if (num(row, col, u) + num(row, col, u + 4) > 4)
			flag = true;
	return flag;
}

//活2的数量
int live2(int row, int col)
{
	int key = checkPiece[row][col], sum = 0, i, u;
	for (u = 0; u < 4; u++)//三连的活三
	{
		int sumk = 1;
		sumkadd off i++;  off;
		sumksub off i--;  off;//据网友提示这里应该是i--，写代码过了很久了，懒得确认真相了
		if (sumk == 2)sum++;
		return sum;
	}
}

//活3的数量
int live3(int row, int col)
{
	int key = checkPiece[row][col], sum = 0, i, u;
	for (u = 0; u < 4; u++)//三连的活三
	{
		int sumk = 1;
		sumkadd off i++;  off;
		sumksub off i--;  off;//据网友提示这里应该是i--，写代码过了很久了，懒得确认真相了
		if (sumk == 3)sum++;
	}
	for (u = 0; u < 8; u++)//8个方向，每个方向最多1个非三连的活三
	{
		int  sumk = 0;
		bool flag = true;
		for (i = 1; samekey || flag; i++)//成活四点的方向
		{
			if (!samekey)
			{
				if (flag&&checkPiece[row + dx[u] * i][col + dy[u] * i])sumk -=10;
				flag = false;
			}
			sumk++;
		}off
			if (checkPiece[row + dx[u] * --i][col + dy[u] * i] == 0)continue;
		sumkadd off
			if (sumk == 3)sum++;
	}
	return sum;
}

//活4的数量
int live4(int row, int col)
{
	int key = checkPiece[row][col], sum = 0, i, u;
	for (u = 0; u < 4; u++)//4个方向，每个方向最多1个
	{
		int sumk = 1;
		sumkadd off sumksub off
			if (sumk == 4)sum++;
	}
	return sum;
}

//冲4的数量
int chong4(int row, int col)
{
	int key = checkPiece[row][col], sum = 0, i, u;
	for (u = 0; u < 8; u++)//8个方向，每个方向最多1个
	{
		int  sumk = 0;
		bool flag = true;
		for (i = 1; samekey || flag; i++)//成五点的方向
		{
			if (!samekey)
			{
				if (flag&&checkPiece[row + dx[u] * i][col + dy[u] * i])sumk -= 10;
				flag = false;
			}
			sumk++;
		}
		sumksub
			if (sumk == 4)sum++;
	}
	return sum - live4(row, col) * 2;
}

//判断落子后是否成禁手
bool ban(int row, int col)
{
	bool flag = false;
	if ((live3(row, col) > 1) || ((live4(row, col) + chong4(row, col)) > 1))
		flag = true;
	return flag;
}

//落下一子
void go(int i, int j)//落下一子
{
	if (chessColor % 2 == 1)
	{
		//人物走棋白单
		checkPiece[i][j] = 1;
		a.x = 50 + 40 * i; a.y = 10 + 40 * j;
		setfillcolor(WHITE);
		solidcircle(a.x, a.y, 10);
		++chessColor;
		
		if (isWin(i, j))
		{
			MessageBox(hwnd, L"white win", L"congraduations", 0);
			d = 1;
			Sleep(2000);
		}
		
	}
	else
	{
		//电脑或者另一个人走棋红双
		checkPiece[i][j] = 2;
		setfillcolor(RED);
		a.x = 50 + 40 * i; a.y = 10 + 40 * j;
		solidcircle(a.x, a.y, 10);
		++chessColor;
		
		if (isWin(i, j))
		{
			MessageBox(hwnd, L"red win", L"don't be unhappy", 0);
			d = 1;
			Sleep(2000);
		}
		
	}
	
}

//画图及初始化部分数据
void drawMap()
{
	initgraph(700, 700);
	setlinecolor(WHITE);
	for (int i = 0; i < n; ++i)
	{
		line(50, 50 + 40 * i, 650, 50 + 40 * i);
		line(50 + 40 * i, 50, 50 + 40 * i, 650);

	}
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n+1; ++j)
		{
			if(j==0)
			checkPiece[i][j] = 3;
			else
				checkPiece[i][j] = 0;
		}
	}
}

//人物点击及作图
bool clickAndCircle()
{
	a.x = 0; a.y = 0;
	hwnd = GetForegroundWindow();
	GetWindowRect(hwnd, &rect);
	GetCursorPos(&cursor);
	if ((cursor.x >= rect.left + 10)
		&& (cursor.x <= rect.left + 690)
		&& (cursor.y >= rect.top + 10)
		&& (cursor.y <= rect.top + 690)
		)
	{
		re.x = cursor.x - rect.left;
		re.y = cursor.y - rect.top;
		for (int i = 0; i < n; ++i)
		{
			for (int j = 1; j < n+1; ++j)
			{
				if (re.x > (50 + 40 * i - 15) && re.x < (50 + 40 * i + 15) && re.y >(50 + 40 * j - 15) && re.y < (50 + 40 * j + 15))
				{
					a.x = 50 + 40 * i; a.y = 10 + 40 * j;
					if (checkPiece[i][j] == 0)
					{
						/*
						if (s == 1)
						{
							if (ban(i, j))
							{
								MessageBox(hwnd, L"不能够走这里啦", L"禁棋", 0);
								return false;
							}
						}
						*/
						go(i, j);
						return true;
					}
					else
					{
						return false;
					}

				}
				//不能够加return;
			}
		}

	}
	else
	{
		return false;
	}
}

//获得人物点击事件
void personChess()
{
	b = 0;
	while (b == 0)
	{
		b = clickAndCircle();
	}
}

/*
//非负分值
int point(int row, int col)
{
	if (ban(row, col))
		return 0;//禁手0分
	if (isWin(row, col))
	{
		is_end = false;
		return 10000;
	}
	int ret = live4(row, col) * 1000 + (chong4(row, col) + live3(row, col)) * 100, u;
	for (u = 0; u < 8; u++)
		if (checkPiece[row + dx[u]][col + dy[u]])
			ret++;//无效点0分
	return ret;
}

//
int AI3(int p2)
{
	int i, j;
	int keyp = -100000, tempp;
	for (i = 0; i <= 15; i++)
		for (j = 1; j <= 16; j++)
	{
		if (!ok(i, j))continue;
		tempp = point(i, j);
		temp0
			if (tempp == 10000)
			{
				checkPiece[i][j] = 0;
				return 10000;
			}
		checkPiece[i][j] = 0;
		if (tempp - p2 * 2 > keyp)keyp = tempp - p2 * 2;//第三层取极大
	}
	return keyp;
}

//
int AI2()
{
	int i, j;
	int keyp = 100000, tempp;
	for (i = 0; i <= 15; i++)
		for (j = 1; j <= 16; j++)
	{
		if (!ok(i, j))continue;
		tempp = point(i, j);
		temp0
			if (tempp == 10000)
			{
				checkPiece[i][j] = 0;
				return -10000;
			}
		tempp = AI3(tempp);
		checkPiece[i][j] = 0;
		if (tempp < keyp)keyp = tempp;//第二层取极小
	}
	return keyp;
}

//
void AI()
{
	int i, j;
	int keyp = -100000, keyi, keyj, tempp;
	if (checkPiece[8][8] == 0)
		return go(8, 8);
	for (i = 0; i <= 15; i++)
	{
		for (j = 1; j <= 16; j++)
		{
			if (!ok(i, j))continue;
			tempp = point(i, j);
			temp0//高效剪枝，避开了禁手点和无效点
				if (tempp == 10000)
					return go(i, j);
			tempp = AI2();
			checkPiece[i][j] = 0;
			if (tempp > keyp)
				keyp = tempp, keyi = i, keyj = j;//第一层取极大
		}
	}
	return go(i, j);
}

*/

//人机计算分值算法
int point(int i,int j)
{
	int p = 0;
	int temp = 1;
	if (checkPiece[i][j] == 0)
	{
		checkPiece[i][j] = 1;
		if (isWin(i, j))
		{
			return 10000000;
		}
		p += live4(i, j) * 1000 + (chong4(i, j) + live3(i, j)) * 1000 + live2(i, j) * 100;
		for (int k = 0; k < 8; ++k)
		{
			if (checkPiece[i + dx[k]][j + dy[k]]==1)
				p += 10;
		}
		checkPiece[i][j] = 2;
		if (isWin(i, j))
		{
			return 10000000;
		}
		p += live4(i, j) * 1000 + (chong4(i, j) + live3(i, j)) * 1000 + live2(i, j) * 100;
		for (int k = 0; k < 8; ++k)
		{
			if ( checkPiece[i + dx[k]][j + dy[k]] == 2)
				p += 10;
		}
		checkPiece[i][j] = 0;
		return p;
	}
	else
		return -100000;
}

//人机搜索算法
void AI()
{
	int keyp = 0, keyi, keyj, temp=-10000000;
	if (checkPiece[8][8] == 0)
		return go(8, 8);
	else
	{
		for (int i = 0; i < n; ++i)
			for (int j = 1; j < n + 1; ++j)
			{
				points[i][j] = point(i, j);
			}
	}
	for (int i = 0; i < n; ++i)
		for (int j = 1; j < n + 1; ++j)
		{
			if (points[i][j] > temp)
			{
				temp = points[i][j];
				keyi = i; keyj = j;
			}
		}
	if (temp == -10000000)
	{
		MessageBox(hwnd, L"平局", L"哈哈哈", 0);
	}
	else
		go(keyi, keyj);
}

//main函数
int main()
{
	//int type = MessageBox(hwnd, L"人机对战", L"选择类型", MB_OKCANCEL);
	s= MessageBox(hwnd, L"我先走棋", L"选择先手", MB_OKCANCEL);
	chessColor += s;
	MessageBox(hwnd, L"没有悔棋", L"哈哈哈", 0);
	drawMap();

	while (d == 0)
	{
		if (chessColor % 2 == 1)
		{
			//人物走棋算法

			keyrec.Event.KeyEvent.wVirtualKeyCode = NULL;
			hInput = GetStdHandle(STD_INPUT_HANDLE);      //获得标准输入设备句柄
			ReadConsoleInput(hInput, &keyrec, 1, &res);
			if (keyrec.EventType == KEY_EVENT && keyrec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN)
				personChess();

		}
		else
		{
			AI();
		}
		if (d == 1)
		{
			d = MessageBox(hwnd, L"继续游戏？", L"选择", MB_OKCANCEL) - 1;
			if (d == 0)
			{
				s = MessageBox(hwnd, L"我先走棋", L"选择先手", MB_OKCANCEL);
				chessColor = 2 +s ;
				drawMap();
			}
		}
	}

}

/*
//两种下棋的机制
	switch (type)
	{
	case 1:
	{
		while (d == 0)
		{
			if (chessColor % 2 == 1)
			{
				//人物走棋算法
				keyrec.Event.KeyEvent.wVirtualKeyCode = NULL;
				hInput = GetStdHandle(STD_INPUT_HANDLE);      //获得标准输入设备句柄
				ReadConsoleInput(hInput, &keyrec, 1, &res);
				if (keyrec.EventType == KEY_EVENT && keyrec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN)
					personChess();
			}
			else
			{
				keyrec.Event.KeyEvent.wVirtualKeyCode = NULL;
				hInput = GetStdHandle(STD_INPUT_HANDLE);      //获得标准输入设备句柄
				ReadConsoleInput(hInput, &keyrec, 1, &res);
				if (keyrec.EventType == KEY_EVENT && keyrec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN)
					personChess();
			}
			if (d == 1)
			{
				d = MessageBox(hwnd, L"继续游戏？", L"选择", MB_OKCANCEL) - 1;
				if (d == 0)
				{
					chessColor = 2 + MessageBox(hwnd, L"我先走棋", L"选择先手", MB_OKCANCEL);
					drawMap();
				}
			}
		}
	}
	case 2:
	{
		while (d == 0)
		{
			if (chessColor % 2 == 1)
			{
				//人物走棋算法

				keyrec.Event.KeyEvent.wVirtualKeyCode = NULL;
				hInput = GetStdHandle(STD_INPUT_HANDLE);      //获得标准输入设备句柄
				ReadConsoleInput(hInput, &keyrec, 1, &res);
				if (keyrec.EventType == KEY_EVENT && keyrec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN)
					personChess();

			}
			else
			{
				AI();
			}
			if (d == 1)
			{
				d = MessageBox(hwnd, L"继续游戏？", L"选择", MB_OKCANCEL) - 1;
				if (d == 0)
				{
					chessColor = 2 + MessageBox(hwnd, L"我先走棋", L"选择先手", MB_OKCANCEL);
					drawMap();
				}
			}
		}
	}
	default:
		system("pause");
*/

//鼠标方法失败代码
/*
if (keyrec.EventType == KEY_EVENT && keyrec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN)
{

	cout << 1 << endl;
}

	hOut = GetStdHandle(STD_OUTPUT_HANDLE); // 获取标准输出设备句柄
		
		pos = mouserec.Event.MouseEvent.dwMousePosition;    //获得当前鼠标位置

		CloseHandle(hInput); // 关闭标准输入设备句柄



		//我的算法
	



		*/
