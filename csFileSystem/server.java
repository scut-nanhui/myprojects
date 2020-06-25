package csFileSystem;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Scanner;

public class server {
	public static void main(String arg[]) {
		//�����������Socket�˿ں�
		final int port = 10000;
		ServerSocket myServerSocket;
		
		try {
			//��ʼ����������Socket
			myServerSocket = new ServerSocket(port);
			 Socket dataSocket = null;
			//�����Ŵ������ļ��ķ�����·��
			 String fileName = "C:\\Users\\Administrator.DESKTOP-CEG7N0B\\Desktop\\������ֲ�ʽ���㼼��\\1-ʵ��\\�����ļ�";
			 while(true) {
				 Scanner cin = new Scanner(System.in);          
				//����-1����رս�������Socket������������������ַ�����������һ���ļ�
				 System.out.println("-1 to shut the Server or any other to stay\n");         
				 int flag = cin.nextInt();   
				 if(flag == -1)
					 break;            
				//�ȴ��ͻ��˷�����������
				 System.out.println("Receiving and Waiting...");
				//��������
				 dataSocket = myServerSocket.accept();          
				 System.out.println("Connected!");
				//��ȡ��������ͨ����������ȡ�ļ���          
				 BufferedReader readName = new BufferedReader(new InputStreamReader(dataSocket.getInputStream()));
				 String temp = readName.readLine();
				 System.out.println("file name is: " + temp);
				//�½�һ�����浱ǰ�����ļ����ļ�·����String���󣬷�ֹ�̶�·��fileName���޸�
				 String fullfileName = fileName + temp;
				//����File��������ȡ�ļ�����
				 File fi = new File(fullfileName);
				 System.out.println("�ļ�����: " + (int)fi.length());         
				//DataInputStream��DataOutputStream������ѡjava.io����org.omg.CORBA�� ����ʵ����ڱ��������Բ����ڲ�ͬ�����µ�ͨѶ��������java.io��ļ��ɡ�
				//InputStream �ǳ���ĳ��࣬����new�������þ����DataInputStream�ࡣ���ϵ�OutputStreamȫ����ֱ�ӱ���ֵ�ģ�����new.
				 DataInputStream fileIn = new DataInputStream(new FileInputStream(fullfileName));
				 DataOutputStream fileOut = new DataOutputStream(dataSocket.getOutputStream());
				//���建����
				 int bufferSize = 8192;
				 byte[] buff = new byte[bufferSize];
				//���ļ�������byte���飬�������Data Socket�Ļ�������
				 while(true) {
					 int read = 0;
					 if(fileIn != null)
						 read = fileIn.read(buff);
					 if(read == -1)
						 break;
					 fileOut.write(buff,0,read);
				 }
				//��ˢ��������ʹ����ȫ��д�뻺�������Է�Socket������ر�
				 fileOut.flush();
				 fileIn.close();
				 fileOut.close();
				 System.out.println("file has been sent");
				//�ر�����Socket
				 dataSocket.close();
				 System.out.println("data socket is closed");
			   }
			//�رս�������Socket�������
			 myServerSocket.close();
			 System.out.println("Sever is closed");
		 } catch (IOException e) {
			 // TODO Auto-generated catch block
			 e.printStackTrace();
		 }
	}
}
