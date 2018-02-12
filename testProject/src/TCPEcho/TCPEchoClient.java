package TCPEcho;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.net.SocketException;

public class TCPEchoClient {
    public static void main(String[] args) throws IllegalAccessException, IOException {
        if((args.length<2)||(args.length>3)){
            throw new IllegalAccessException("parameter(s):<sever><word>[<port>]");
        }
        String server = args[0];
        byte[] data = args[1].getBytes();

        int serverPort = (args.length==3)?Integer.parseInt(args[2]):8080;

        Socket socket = new Socket(server,serverPort);
        System.out.println("conn to ->"+server+":"+serverPort);

        InputStream in = socket.getInputStream();
        OutputStream out = socket.getOutputStream();
        out.write(data);
//        out.flush();
        int totalBytesRecvd = 0;
        int bytesRecvd;
        while (totalBytesRecvd<data.length){
            if((bytesRecvd = in.read(data,totalBytesRecvd,data.length-totalBytesRecvd))==-1){
                throw new SocketException("coon close prematurely");
            }
            totalBytesRecvd+=bytesRecvd;
        }
        System.out.println("Recvd:"+new String(data));
        socket.close();
    }
}
