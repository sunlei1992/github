package TCPLearning;



import TCPLearning.logback.InitLog;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.net.*;
import java.util.Enumeration;

/**
 * tcp实验小程序，建立一个简单的tcp程序
 */
public class TcpDemo1 {

    private static Logger log = LoggerFactory.getLogger(TcpDemo1.class);

    public static void main(String[] args) throws SocketException {

        InitLog.init();
        TcpDemo1 tcpDemo1 = new TcpDemo1();
        tcpDemo1.interAddressExample();

    }

    public void interAddressExample() throws SocketException {
        Enumeration<NetworkInterface> interfaceList = NetworkInterface.getNetworkInterfaces();
        if(interfaceList==null){
            log.info("--No interface found");
        }else {
            while (interfaceList.hasMoreElements()){
                NetworkInterface networkInterface = interfaceList.nextElement();
                log.info("Interface: "+networkInterface.getName());
                Enumeration<InetAddress> adderList = networkInterface.getInetAddresses();
                if(!adderList.hasMoreElements()){
                    log.info("\t(No address for this interface)");
                }
                while (adderList.hasMoreElements()){
                    InetAddress address = adderList.nextElement();
                    log.info("\tAddress "+((address instanceof Inet4Address ? "(v4)":(address instanceof Inet6Address ? "(v6)" : "(?)" )))+": "+address.getHostAddress());
                }

            }

        }
    }

    public void testLog(){

        String name = "Aub";
        String message = "3Q";
        String[] fruits = { "apple", "banana" };

        // logback提供的可以使用变量的打印方式，结果为"Hello,Aub!"
        log.info("Hello,{}!", name);

        // 可以有多个参数,结果为“Hello,Aub! 3Q!”
        log.info("Hello,{}!   {}!", name, message);

        // 可以传入一个数组，结果为"Fruit:  apple,banana"
        log.info("Fruit:  {},{}", fruits);

        log.error("dddddddddddd");
        log.error("#file sssdddddddddddd");
    }


}
