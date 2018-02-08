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
               *  为了上传和下载中文文件，有些地方建议使用以下两句代替
               *  new String(remotePath.getBytes(encoding),"iso-8859-1")转码。
               *  经过测试，通不过。
               */
//              FTPClientConfig conf = new FTPClientConfig(FTPClientConfig.SYST_NT);
//              conf.setServerLanguageCode("zh");

              ftpClient.connect(url, port);
              // 如果采用默认端口，可以使用ftp.connect(url)的方式直接连接FTP服务器
              ftpClient.login(username, password);// 登录
              // 设置文件传输类型为二进制
              ftpClient.setFileType(FTPClient.BINARY_FILE_TYPE);
              // 获取ftp登录应答代码
              reply = ftpClient.getReplyCode();
              // 验证是否登陆成功
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
     * Description: 从FTP服务器下载文件
     * 
     * @Version1.0
     * @param url
     *            FTP服务器hostname
     * @param port
     *            FTP服务器端口
     * @param username
     *            FTP登录账号
     * @param password
     *            FTP登录密码
     * @param remotePath
     *            FTP服务器上的相对路径
     * @param fileName
     *            要下载的文件名
     * @param localPath
     *            下载后保存到本地的路径
     * @return
     */
    public  boolean downFile( String remotePath, String fileName,
            String localPath,FTPClient ftpClient) {
        boolean result = false;
        try {
            // 转移到FTP服务器目录至指定的目录下
            ftpClient.changeWorkingDirectory(new String(remotePath.getBytes(encoding),"iso-8859-1"));
            // 获取文件列表
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