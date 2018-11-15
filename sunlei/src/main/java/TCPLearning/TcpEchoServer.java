package TCPLearning;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketAddress;

public class TcpEchoServer {
    private static Logger log = LoggerFactory.getLogger(TcpDemo1.class);
    private static final int BUFSIZE = 32;
    public static void main(String[] args) throws IOException {
        if(args.length!=1){
            throw new IllegalArgumentException("parameter(s):<Port>");
        }
        int serverPort = Integer.parseInt(args[0]);

        ServerSocket serverSocket = new ServerSocket(serverPort);

        int recvMsgSize;
        byte[] receviedBuf = new byte[BUFSIZE];

        while (true){
            Socket socket = serverSocket.accept();
            SocketAddress socketAddress = socket.getRemoteSocketAddress();
            log.info("Handing client at "+socketAddress);
            InputStream in = socket.getInputStream();
            OutputStream out = socket.getOutputStream();
            while ((recvMsgSize=in.read(receviedBuf))!=1){
                out.write(receviedBuf,0,recvMsgSize);
            }
            socket.close();
        }

    }
}
