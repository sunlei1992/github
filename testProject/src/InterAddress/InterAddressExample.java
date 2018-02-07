package InterAddress;

import java.net.InetAddress;
import java.net.InterfaceAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Enumeration;

public class InterAddressExample {
    public static void main(String[] args) {
        try{
            Enumeration<NetworkInterface> interfaceList = NetworkInterface.getNetworkInterfaces();
            if(interfaceList == null){
                System.out.println("--No interfaces found--");
            }else {
                while (interfaceList.hasMoreElements()){
                    NetworkInterface iface = interfaceList.nextElement();
                    System.out.println("interface:"+iface.getName());
                    Enumeration<InetAddress> addressList = iface.getInetAddresses();
                    if(!addressList.hasMoreElements()){
                        System.out.println("\t (no address for this interface)");
                        while(addressList.hasMoreElements()){
                            InetAddress address = addressList.nextElement();
                            System.out.println("\tAddress:"+address.getHostName());
                        }
                    }

                }
            }

        } catch (SocketException e) {
            e.printStackTrace();
        }
    }
}
