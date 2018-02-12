package TCPEcho;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketAddress;

public class TCPEchoServer {
    private static final int BUFFSIZE = 32;

    public static void main(String[] args) throws IllegalAccessException, IOException {
        if (args.length!=1){
            throw new IllegalAccessException("Parmaeter(s):<port>");
        }
        int serverPort = Integer.parseInt(args[0]);
        ServerSocket serverSocket = new ServerSocket(serverPort);
        int recvMsgSize;
        byte[] recvBuf = new byte[BUFFSIZE];
        while(true){
            Socket clientSocket = serverSocket.accept();
            SocketAddress add = clientSocket.getRemoteSocketAddress();
            System.out.println("add:"+add);
            InputStream in = clientSocket.getInputStream();
            OutputStream out = clientSocket.getOutputStream();
            while((recvMsgSize=in.read(recvBuf))!=-1){
                out.write(recvBuf,0,recvMsgSize);
            }
            clientSocket.close();
        }
    }
}
