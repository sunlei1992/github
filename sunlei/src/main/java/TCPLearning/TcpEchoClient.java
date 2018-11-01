package TCPLearning;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.*;
import java.net.Socket;
import java.net.SocketException;
import java.util.Properties;

public class TcpEchoClient {
    private static Logger log = LoggerFactory.getLogger(TcpDemo1.class);
    public static void main(String[] args) throws IOException{
        //config.properties
        Properties properties = new Properties();
        properties.load(TcpEchoClient.class.getResourceAsStream("/config.properties"));
        if(args.length<2||args.length>3){
            throw new IllegalArgumentException("Parameter(s):<Server>:<port>[<port>]");
        }
        String server = properties.getProperty("server.address");
        byte[] data = args[1].getBytes();
        int serverPort = Integer.parseInt(properties.getProperty("server.port","7"));

        Socket socket = new Socket(server,serverPort);
        log.info("Connected to server...sending echo string");

        InputStream in = socket.getInputStream();
        OutputStream out = socket.getOutputStream();

        out.write(data);

        int totalByteRcvd = 0;
        int bytesRcvd;
        while (totalByteRcvd<data.length){
            if((bytesRcvd=in.read(data,totalByteRcvd,data.length-totalByteRcvd))==-1){
                throw new SocketException("Connection closed prematurely");
            }
            totalByteRcvd+=bytesRcvd;
        }
        log.info("Received: "+new String(data));
        socket.close();
    }
}
