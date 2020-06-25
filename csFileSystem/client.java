package csFileSystem;
import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Scanner;

public class client {
	public static void main(String[] args) {
		try {
			InetAddress acceptorHost=InetAddress.getByName("localhost");
			int acceptorPort=10000;
			Socket clientSocket=null;
			String fileName="C:\\Users\\Administrator.DESKTOP-CEG7N0B\\Desktop\\计算机分布式计算技术\\1-实验\\接收文件";
			while(true) {
				Scanner cin=new Scanner(System.in);
				System.out.println("输入-1，则关闭客户");
				int flag=cin.nextInt();
				if(flag==-1)
					break;
				clientSocket=new Socket(acceptorHost,acceptorPort);
				System.out.println("输入要下载的文件名\n");
				BufferedReader consoleReader = new BufferedReader(new InputStreamReader(System.in));
				String temp=consoleReader.readLine();
				String fullfileName=fileName+'\\'+temp;
				OutputStream sendFileName=clientSocket.getOutputStream();
				PrintWriter sendFile=new PrintWriter(new OutputStreamWriter(sendFileName));
				sendFile.println(temp);
				sendFile.flush();
				DataInputStream clientInput=new DataInputStream(clientSocket.getInputStream());
				DataOutputStream clientOutput=new DataOutputStream(new FileOutputStream(fullfileName));
				int buffersize =8192;
				byte[] buff=new byte[buffersize];
				while(true) {
					int read=0;
					if(clientInput !=null)
						read=clientInput.read(buff);
					if(read==-1)
						break;
					clientOutput.write(buff,0,read);	
				}
				sendFile.close();
				clientOutput.flush();
				clientInput.close();
				clientOutput.close();
				 System.out.println("file is received");     
			}
			clientSocket.close();
			System.out.println("Client closed");
			}
		catch (UnknownHostException e) {
			 e.printStackTrace();
		 }
		catch (IOException e) {
			e.printStackTrace();
			}
		
		}
		
	}
	

	
	
	
	
	
	
	
	

