package UDPEcho;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;

public class UDPEchoServer {
    private static final int ECHOMAX = 255;

    public static void main(String[] args) throws IllegalAccessException, IOException {
        if(args.length!=1){
            throw new IllegalAccessException("Paramest(s):<port>");
        }
        int serverPort = Integer.parseInt(args[0]);
        DatagramSocket datagramSocket = new DatagramSocket(serverPort);
        DatagramPacket datagramPacket = new DatagramPacket(new byte[ECHOMAX],ECHOMAX);
        while(true){
            datagramSocket.receive(datagramPacket);
            System.out.println(
                    "Handling client at "+ datagramPacket.getAddress().getHostAddress()+
                            " port "+datagramPacket.getPort()
            );
            System.out.println("data:"+new String(datagramPacket.getData()));
//            datagramPacket.setData((new String("ok i get it")).getBytes());
            byte[] bytesToSend = (
                    "0:ok test for len" +
                    "1:ok test for len" +
                    "2:ok test for len" +
                    "3:ok test for len" +
                    "4:ok test for len" +
                    "5:ok test for len" +
                    "6:ok test for len").getBytes();

            DatagramPacket recvPacket = new DatagramPacket(
                    bytesToSend,
                    bytesToSend.length,
                    InetAddress.getByName(datagramPacket.getAddress().getHostAddress()),
                    datagramPacket.getPort()
            );
            datagramSocket.send(recvPacket);
            datagramSocket.send(recvPacket);
            System.out.println(datagramPacket.getLength());
            datagramPacket.setLength(ECHOMAX);
            System.out.println(datagramPacket.getLength());

        }
    }
}
