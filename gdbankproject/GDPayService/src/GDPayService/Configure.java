package GDPayService;


import java.io.File;
import java.util.ArrayList;
import java.util.List;

import org.dom4j.Document;
import org.dom4j.Element;
import org.dom4j.io.SAXReader;

/** 
 * @author sky
 * 项目名称：GDPayService
 * 文件名称：Configure.java
 * 创建时间：2014-12-10 下午2:58:25 
 * 类说明 :
 */
public class Configure {
	
	public static String programVersion;
	public static String DTDT_DIRECTORY_URL;
	public static int PORT;
	public static String Key;
	public static String log4jPropertiesName;
	
	static
	{
		parseXml();
	}
	
	private static void parseXml(){
		try {
			SAXReader reader = new SAXReader();
	        Document document = reader.read(new File("configuration.xml"));
	        Element root = document.getRootElement();
	        //解析ProgramVersion
	        List programVersionList  = root.elements("ProgramVersion");
	        for(int i = 0 ; i < programVersionList.size();i++){
	        	Element resourcenum = (Element)programVersionList.get(i);
	        	programVersion = resourcenum.getText();
	        }
	        //解析具体版本的详细配置信息
	        List parameterList = root.elements(programVersion);
	        for(int i = 0 ; i < parameterList.size();i++){
	        	Element resourceitem = (Element)parameterList.get(i);
	        	DTDT_DIRECTORY_URL = resourceitem.element("ipAddress").getText();
	        	PORT = Integer.parseInt(resourceitem.element("port").getText());
	        	Key = resourceitem.element("key").getText();
	        	log4jPropertiesName = resourceitem.element("log4jPropertiesName").getText();	
	        }      
			
		} catch (Exception e) {
			GDPayService.log.error(e);
		}
		
		
	}
	
	

}
