package csFileSystem;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Scanner;

public class server {
	public static void main(String arg[]) {
		//定义接收连接Socket端口号
		final int port = 10000;
		ServerSocket myServerSocket;
		
		try {
			//初始化接收连接Socket
			myServerSocket = new ServerSocket(port);
			 Socket dataSocket = null;
			//定义存放待传送文件的服务器路径
			 String fileName = "C:\\Users\\Administrator.DESKTOP-CEG7N0B\\Desktop\\计算机分布式计算技术\\1-实验\\传输文件";
			 while(true) {
				 Scanner cin = new Scanner(System.in);          
				//输入-1，则关闭接收连接Socket与服务器；输入其余字符继续传送下一个文件
				 System.out.println("-1 to shut the Server or any other to stay\n");         
				 int flag = cin.nextInt();   
				 if(flag == -1)
					 break;            
				//等待客户端发起连接请求
				 System.out.println("Receiving and Waiting...");
				//接收连接
				 dataSocket = myServerSocket.accept();          
				 System.out.println("Connected!");
				//获取输入流，通过输入流获取文件名          
				 BufferedReader readName = new BufferedReader(new InputStreamReader(dataSocket.getInputStream()));
				 String temp = readName.readLine();
				 System.out.println("file name is: " + temp);
				//新建一个保存当前所需文件的文件路径的String对象，防止固定路径fileName被修改
				 String fullfileName = fileName + temp;
				//创建File变量，获取文件长度
				 File fi = new File(fullfileName);
				 System.out.println("文件长度: " + (int)fi.length());         
				//DataInputStream和DataOutputStream都可以选java.io或是org.omg.CORBA。 本次实验基于本机，所以不存在不同环境下的通讯，所以用java.io里的即可。
				//InputStream 是抽象的超类，不能new，所以用具体的DataInputStream类。书上的OutputStream全部是直接被赋值的，不是new.
				 DataInputStream fileIn = new DataInputStream(new FileInputStream(fullfileName));
				 DataOutputStream fileOut = new DataOutputStream(dataSocket.getOutputStream());
				//定义缓冲区
				 int bufferSize = 8192;
				 byte[] buff = new byte[bufferSize];
				//从文件流读入byte数组，再输出到Data Socket的缓冲流里
				 while(true) {
					 int read = 0;
					 if(fileIn != null)
						 read = fileIn.read(buff);
					 if(read == -1)
						 break;
					 fileOut.write(buff,0,read);
				 }
				//冲刷缓冲区，使数据全部写入缓冲区，以防Socket的意外关闭
				 fileOut.flush();
				 fileIn.close();
				 fileOut.close();
				 System.out.println("file has been sent");
				//关闭数据Socket
				 dataSocket.close();
				 System.out.println("data socket is closed");
			   }
			//关闭接收连接Socket与服务器
			 myServerSocket.close();
			 System.out.println("Sever is closed");
		 } catch (IOException e) {
			 // TODO Auto-generated catch block
			 e.printStackTrace();
		 }
	}
}
