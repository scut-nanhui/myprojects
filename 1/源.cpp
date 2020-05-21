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

int s = 0, ais = 1, s0;//s���ֵ�˭��,s=1,2��s=1��ai�£�s=2����ң�s=s0�Ǻڷ��£������ǰ׷���
int manu[2][300], manukey = 0;
int points[n][n + 1];//����������

int dx[8] = { 1, 1, 0, -1, -1, -1, 0, 1 }; //flat����
int dy[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };//��dx,dy����8����������
int checkPiece[16][17];//��¼�������޼���ɫ��1Ϊ��ɫ��2Ϊ��ɫ,3Ϊ����
int type;//�˻���������
bool b=0, d = 0, is_end = false;//����Ƿ��������Ѿ�������
int chessColor = 2;//���Ժ�ɫ˫�����˵�����ɫ����
POINT cursor, a, re;
RECT rect;
HWND hwnd;
HANDLE hInput;
HANDLE hOut;
INPUT_RECORD keyrec;      //���������¼��ṹ��
DWORD res;      //���ڴ洢��ȡ��¼



//�ܷ�����
bool ok(int row, int col)
{
	return checkPiece[row][col] == 0;
}

//�Ƿ���������
bool inboard(int row, int col)
{
	if (row <0 || row >15 )return false;
	return col >= 1 && col <= 16;
}

//�ж���������ɫ�Ƿ���ͬ
bool same(int i, int j, int key)
{
	if (checkPiece[i][j] == key)
		return true;
	else
		return false;
}

//������
bool find1(int x, int y)
{
	int i, num = 1;
	int temp = checkPiece[x][y];
	for (i = x + 1; i < 16; i++)
	{// ������
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
	{// ������
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

//������
bool find2(int x, int y)
{
	int i, num = 1;
	int temp = checkPiece[x][y];
	for (i = y +1; i < 17; i++)
	{ // ������
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
	{// ������
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

//.\����
bool find3(int x, int y)
{
	int i, j, num = 1;
	int temp = checkPiece[x][y ];
	for (i = x + 1, j = y + 1; i < 16 && j < 17; i++, j++) {// ���·�
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
	{// ���Ϸ�
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

//./����
bool find4(int x, int y)
{
	int i, j, num = 1;
	int temp = checkPiece[x][y ];
	for (i = x + 1, j = y-1; i < 16 && j >=1; i++, j--)
	{// ����
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
	{ // ����
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

//�ж�ʤ��
bool isWin(int x, int y)
{
	return (find1(x, y) || find2(x, y) || find3(x, y) || find4(x, y));

}

//���꣨row,col������������u
int num(int row, int col, int u)
{
	int i = row + dx[u], j = col + dy[u], sum = 0, ref = checkPiece[row][col];
	if (ref == 0)return 0;
	while (same(i, j, ref))sum++, i += dx[u], j += dy[u];
	return sum;
}

//�������ּ���������ϵ�����
bool overline(int row, int col)
{
	bool flag = false;
	int u;
	for (u = 0; u < 4; u++)
		if (num(row, col, u) + num(row, col, u + 4) > 4)
			flag = true;
	return flag;
}

//��2������
int live2(int row, int col)
{
	int key = checkPiece[row][col], sum = 0, i, u;
	for (u = 0; u < 4; u++)//�����Ļ���
	{
		int sumk = 1;
		sumkadd off i++;  off;
		sumksub off i--;  off;//��������ʾ����Ӧ����i--��д������˺ܾ��ˣ�����ȷ��������
		if (sumk == 2)sum++;
		return sum;
	}
}

//��3������
int live3(int row, int col)
{
	int key = checkPiece[row][col], sum = 0, i, u;
	for (u = 0; u < 4; u++)//�����Ļ���
	{
		int sumk = 1;
		sumkadd off i++;  off;
		sumksub off i--;  off;//��������ʾ����Ӧ����i--��д������˺ܾ��ˣ�����ȷ��������
		if (sumk == 3)sum++;
	}
	for (u = 0; u < 8; u++)//8������ÿ���������1���������Ļ���
	{
		int  sumk = 0;
		bool flag = true;
		for (i = 1; samekey || flag; i++)//�ɻ��ĵ�ķ���
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

//��4������
int live4(int row, int col)
{
	int key = checkPiece[row][col], sum = 0, i, u;
	for (u = 0; u < 4; u++)//4������ÿ���������1��
	{
		int sumk = 1;
		sumkadd off sumksub off
			if (sumk == 4)sum++;
	}
	return sum;
}

//��4������
int chong4(int row, int col)
{
	int key = checkPiece[row][col], sum = 0, i, u;
	for (u = 0; u < 8; u++)//8������ÿ���������1��
	{
		int  sumk = 0;
		bool flag = true;
		for (i = 1; samekey || flag; i++)//�����ķ���
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

//�ж����Ӻ��Ƿ�ɽ���
bool ban(int row, int col)
{
	bool flag = false;
	if ((live3(row, col) > 1) || ((live4(row, col) + chong4(row, col)) > 1))
		flag = true;
	return flag;
}

//����һ��
void go(int i, int j)//����һ��
{
	if (chessColor % 2 == 1)
	{
		//��������׵�
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
		//���Ի�����һ���������˫
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

//��ͼ����ʼ����������
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

//����������ͼ
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
								MessageBox(hwnd, L"���ܹ���������", L"����", 0);
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
				//���ܹ���return;
			}
		}

	}
	else
	{
		return false;
	}
}

//����������¼�
void personChess()
{
	b = 0;
	while (b == 0)
	{
		b = clickAndCircle();
	}
}

/*
//�Ǹ���ֵ
int point(int row, int col)
{
	if (ban(row, col))
		return 0;//����0��
	if (isWin(row, col))
	{
		is_end = false;
		return 10000;
	}
	int ret = live4(row, col) * 1000 + (chong4(row, col) + live3(row, col)) * 100, u;
	for (u = 0; u < 8; u++)
		if (checkPiece[row + dx[u]][col + dy[u]])
			ret++;//��Ч��0��
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
		if (tempp - p2 * 2 > keyp)keyp = tempp - p2 * 2;//������ȡ����
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
		if (tempp < keyp)keyp = tempp;//�ڶ���ȡ��С
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
			temp0//��Ч��֦���ܿ��˽��ֵ����Ч��
				if (tempp == 10000)
					return go(i, j);
			tempp = AI2();
			checkPiece[i][j] = 0;
			if (tempp > keyp)
				keyp = tempp, keyi = i, keyj = j;//��һ��ȡ����
		}
	}
	return go(i, j);
}

*/

//�˻������ֵ�㷨
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

//�˻������㷨
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
		MessageBox(hwnd, L"ƽ��", L"������", 0);
	}
	else
		go(keyi, keyj);
}

//main����
int main()
{
	//int type = MessageBox(hwnd, L"�˻���ս", L"ѡ������", MB_OKCANCEL);
	s= MessageBox(hwnd, L"��������", L"ѡ������", MB_OKCANCEL);
	chessColor += s;
	MessageBox(hwnd, L"û�л���", L"������", 0);
	drawMap();

	while (d == 0)
	{
		if (chessColor % 2 == 1)
		{
			//���������㷨

			keyrec.Event.KeyEvent.wVirtualKeyCode = NULL;
			hInput = GetStdHandle(STD_INPUT_HANDLE);      //��ñ�׼�����豸���
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
			d = MessageBox(hwnd, L"������Ϸ��", L"ѡ��", MB_OKCANCEL) - 1;
			if (d == 0)
			{
				s = MessageBox(hwnd, L"��������", L"ѡ������", MB_OKCANCEL);
				chessColor = 2 +s ;
				drawMap();
			}
		}
	}

}

/*
//��������Ļ���
	switch (type)
	{
	case 1:
	{
		while (d == 0)
		{
			if (chessColor % 2 == 1)
			{
				//���������㷨
				keyrec.Event.KeyEvent.wVirtualKeyCode = NULL;
				hInput = GetStdHandle(STD_INPUT_HANDLE);      //��ñ�׼�����豸���
				ReadConsoleInput(hInput, &keyrec, 1, &res);
				if (keyrec.EventType == KEY_EVENT && keyrec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN)
					personChess();
			}
			else
			{
				keyrec.Event.KeyEvent.wVirtualKeyCode = NULL;
				hInput = GetStdHandle(STD_INPUT_HANDLE);      //��ñ�׼�����豸���
				ReadConsoleInput(hInput, &keyrec, 1, &res);
				if (keyrec.EventType == KEY_EVENT && keyrec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN)
					personChess();
			}
			if (d == 1)
			{
				d = MessageBox(hwnd, L"������Ϸ��", L"ѡ��", MB_OKCANCEL) - 1;
				if (d == 0)
				{
					chessColor = 2 + MessageBox(hwnd, L"��������", L"ѡ������", MB_OKCANCEL);
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
				//���������㷨

				keyrec.Event.KeyEvent.wVirtualKeyCode = NULL;
				hInput = GetStdHandle(STD_INPUT_HANDLE);      //��ñ�׼�����豸���
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
				d = MessageBox(hwnd, L"������Ϸ��", L"ѡ��", MB_OKCANCEL) - 1;
				if (d == 0)
				{
					chessColor = 2 + MessageBox(hwnd, L"��������", L"ѡ������", MB_OKCANCEL);
					drawMap();
				}
			}
		}
	}
	default:
		system("pause");
*/

//��귽��ʧ�ܴ���
/*
if (keyrec.EventType == KEY_EVENT && keyrec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN)
{

	cout << 1 << endl;
}

	hOut = GetStdHandle(STD_OUTPUT_HANDLE); // ��ȡ��׼����豸���
		
		pos = mouserec.Event.MouseEvent.dwMousePosition;    //��õ�ǰ���λ��

		CloseHandle(hInput); // �رձ�׼�����豸���



		//�ҵ��㷨
	



		*/
