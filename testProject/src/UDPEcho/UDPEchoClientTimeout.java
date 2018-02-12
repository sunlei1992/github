package UDPEcho;

import java.io.IOException;
import java.net.*;

public class UDPEchoClientTimeout {
    private static final int TIMROUT = 3000;
    private static final int MAXTRIES = 5;

    public static void main(String[] args) throws IllegalAccessException, IOException {
        if (args.length < 2 || args.length > 3) {
            throw new IllegalAccessException("Paramete(s):<server><word>[<port>]");
        }
        InetAddress serverAddress = InetAddress.getByName(args[0]);
        byte[] bytesToSend = args[1].getBytes();
        int serverPort = args.length == 3 ? Integer.parseInt(args[2]) : 8080;


        try (DatagramSocket datagramSocket = new DatagramSocket()) {
            datagramSocket.setSoTimeout(TIMROUT);
            DatagramPacket datagramPacket = new DatagramPacket(
                    bytesToSend,
                    bytesToSend.length,
                    serverAddress,
                    serverPort
            );
            DatagramPacket recvDatagramPacket = new DatagramPacket(new byte[bytesToSend.length], bytesToSend.length);
            int tries = 0;
            boolean recvdResponse = false;
            do {
                datagramSocket.send(datagramPacket);
                try {
                    datagramSocket.receive(recvDatagramPacket);
                    if (!recvDatagramPacket.getAddress().equals(serverAddress)) {
                        throw new IOException("recvd msg from a unknow add");
                    }
                    recvdResponse = true;
                } catch (SocketTimeoutException e) {
                    tries += 1;
                    System.out.println("timeout try:" + tries);
                }
            } while (!recvdResponse && tries < MAXTRIES);
            if (recvdResponse) {
                System.out.println("recv:" + recvDatagramPacket.getData().toString());
                System.out.println("recv:" + new String(recvDatagramPacket.getData()));
                System.out.println("recv:" + recvDatagramPacket.getLength());
            } else {
                System.out.println("尝试5次超时,放弃");
            }


             tries = 0;
             recvdResponse = false;
            do {
//                datagramSocket.send(datagramPacket);
                try {
                    datagramSocket.receive(recvDatagramPacket);
                    if (!recvDatagramPacket.getAddress().equals(serverAddress)) {
                        throw new IOException("recvd msg from a unknow add");
                    }
                    recvdResponse = true;
                } catch (SocketTimeoutException e) {
                    tries += 1;
                    System.out.println("timeout try:" + tries);
                }
            } while (!recvdResponse && tries < MAXTRIES);
            if (recvdResponse) {
                System.out.println("recv:" + recvDatagramPacket.getData().toString());
                System.out.println("recv:" + new String(recvDatagramPacket.getData()));
                System.out.println("recv:" + recvDatagramPacket.getLength());
            } else {
                System.out.println("尝试5次超时,放弃");
            }
        }


    }
}
