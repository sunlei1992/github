package gdftpserver;
import java.io.File;

import java.io.FileOutputStream;

import java.io.OutputStream;



import org.apache.commons.net.ftp.FTPClient;  
import org.apache.commons.net.ftp.FTPFile;  
import org.apache.commons.net.ftp.FTPReply;  



public class FtpApche {
   
    private static String encoding = System.getProperty("file.encoding");
    public  FTPClient connecttoftp(String url, int port, String username,String password) {
    FTPClient ftpClient = new FTPClient();
    	try {
              int reply;
              ftpClient.setControlEncoding(encoding);
              
              /*
               *  Ϊ���ϴ������������ļ�����Щ�ط�����ʹ�������������
               *  new String(remotePath.getBytes(encoding),"iso-8859-1")ת�롣
               *  �������ԣ�ͨ������
               */
//              FTPClientConfig conf = new FTPClientConfig(FTPClientConfig.SYST_NT);
//              conf.setServerLanguageCode("zh");

              ftpClient.connect(url, port);
              // �������Ĭ�϶˿ڣ�����ʹ��ftp.connect(url)�ķ�ʽֱ������FTP������
              ftpClient.login(username, password);// ��¼
              // �����ļ���������Ϊ������
              ftpClient.setFileType(FTPClient.BINARY_FILE_TYPE);
              // ��ȡftp��¼Ӧ�����
              reply = ftpClient.getReplyCode();
              // ��֤�Ƿ��½�ɹ�
              if (!FTPReply.isPositiveCompletion(reply)) {
                  ftpClient.disconnect();
                  System.err.println("FTP server refused connection.");
                  return null;
              }      
          return ftpClient;
          } catch (Exception e) {  
        	  MainServer.log.error(e);
          } 
    	return ftpClient;
	}
    /**
     * Description: ��FTP�����������ļ�
     * 
     * @Version1.0
     * @param url
     *            FTP������hostname
     * @param port
     *            FTP�������˿�
     * @param username
     *            FTP��¼�˺�
     * @param password
     *            FTP��¼����
     * @param remotePath
     *            FTP�������ϵ����·��
     * @param fileName
     *            Ҫ���ص��ļ���
     * @param localPath
     *            ���غ󱣴浽���ص�·��
     * @return
     */
    public  boolean downFile( String remotePath, String fileName,
            String localPath,FTPClient ftpClient) {
        boolean result = false;
        try {
            // ת�Ƶ�FTP������Ŀ¼��ָ����Ŀ¼��
            ftpClient.changeWorkingDirectory(new String(remotePath.getBytes(encoding),"iso-8859-1"));
            // ��ȡ�ļ��б�
            FTPFile[] fs = ftpClient.listFiles();
            for (FTPFile ff : fs) {
                if (ff.getName().equals(fileName)) {
                    File localFile = new File(localPath +  ff.getName());
                    OutputStream is = new FileOutputStream(localFile);
                    ftpClient.retrieveFile(ff.getName(), is);
                    is.close();
                }
            }

            
            result = true;
        } catch (Exception e) {  
        	 MainServer.log.error(e);
        } 
        return result;
    }
    public void loginout(FTPClient ftpclient){
    	try {
    		ftpclient.logout();
		} catch (Exception e) {
			 MainServer.log.error(e);
		}
    }
    
    
}