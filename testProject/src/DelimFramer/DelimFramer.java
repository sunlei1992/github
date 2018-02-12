package DelimFramer;

import java.io.*;

public class DelimFramer {
    private InputStream in;
    private static final byte DELIMITER = '\n';

    public DelimFramer(InputStream in) {
        this.in = in;
    }
    public void frameMsg(byte[] message, OutputStream out) throws IllegalAccessException, IOException {
        for(byte b : message){
            if(b == DELIMITER){
                throw new IllegalAccessException("消息里包含定界符");
            }
        }
        out.write(message);
        out.write(DELIMITER);
        out.flush();
    }

    public byte[] nextMsg() throws IOException {
        ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
        int nextByte;
        while ((nextByte=in.read())!=DELIMITER){
            if(nextByte == -1){
                if(byteArrayOutputStream.size()==0){
                    return null;
                }else {
                    throw new EOFException("非空消息但是没有定界符");
                }
            }
            byteArrayOutputStream.write(nextByte);
        }
        return byteArrayOutputStream.toByteArray();
    }
}
