package LengthFramer;

import java.io.*;

public class LengthFramer {
    public static final int MAXMESSAGELENGTH = 65535;
    public static final int BYTEMASK = 0xff;
    public static final int SHORTMASK = 0xffff;
    public static final int BYTESHIFT = 8;

    private DataInputStream in;

    public LengthFramer(InputStream in) {
        this.in = new DataInputStream(in);
    }
    public void framerMsg(byte[] message, OutputStream out) throws IOException {
        if(message.length>MAXMESSAGELENGTH){
            throw new IOException("消息长度超过最大限制");
        }
        out.write((message.length>>BYTESHIFT)& BYTEMASK);
        out.write(message.length&BYTEMASK);
        out.write(message);
        out.flush();
    }
    public byte[] nextMsg() throws IOException {
        int length;
        try{
            length = in.readUnsignedShort();
        }catch (EOFException e){
            e.printStackTrace();
            return null;
        }
        byte[] msg = new byte[length];
        in.read(msg);
        return msg;
    }
}
