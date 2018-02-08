package GDPayService;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.lang.reflect.Array;
import java.net.ConnectException;
import java.net.HttpURLConnection;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketTimeoutException;
import java.net.URL;
import java.sql.Connection;
import java.sql.SQLException;
import java.sql.Statement;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Calendar;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;
import java.util.Random;

import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;
import org.json.JSONException;
import org.json.JSONObject;

public class GDPayService extends Thread {
	public GDPayService(Socket c) {
		this.client = c;
	}

	//private static String DTDT_DIRECTORY_URL = "http://10.27.109.138:8080/notify/notify_start?"; // 正式环境
	//private static int PORT = 9340;// 正式环境
	//private static String Key = "z19EReMdHJwGsw2p";// 正式环境
	
	
	
	//private static String DTDT_DIRECTORY_URL = "http://10.27.109.140:8080/notify/notify_start?"; // 测试环境
	//private static int PORT = 9330;// 测试环境
	//private static String Key = "f0mcn6r0ZLYg4EZZ"; // 测试环境
	
	
	/*private static String DTDT_DIRECTORY_URL ="http://localhost:8080/notify/notify_start?"; //debug环境
	private static int PORT = 9320;// debug环境
	private static String Key = "f0mcn6r0ZLYg4EZZ"; // debug环境
*/	
	
	public static Map<String, String> CHdiquyingshe = new HashMap<String, String>() {
		{
			put("029 ", "西安");
			put("0913", "渭南");
			put("0910", "咸阳");
			put("0917", "宝鸡");
			put("0916", "汉中");
			put("0919", "铜川");
			put("0915", "安康");
			put("0914", "商洛");
			put("0911", "延安");
			put("0912", "榆林");
		}
	};
	public static Map<String, String> diquyingshe = new HashMap<String, String>() {
		{
			put("029 ", "610100");
			put("0913", "610500");
			put("0910", "610400");
			put("0917", "610300");
			put("0916", "610700");
			put("0919", "610200");
			put("0915", "610900");
			put("0914", "611000");
			put("0911", "610600");
			put("0912", "610800");
		}
	};
	// 银行交易渠道和广电订单渠道映射
	/*
	 * public static Map<String, String> jiaoyiqudaoyingshe = new
	 * HashMap<String, String>() { { put("BK010112", "1"); put("BK010870", "3");
	 * put("BK010821", "3"); } };
	 */
	public static Map<String, String> CHjiaoyiqudaoyingshe = new HashMap<String, String>() {
		{
			put("BK010112", "光大柜台");
			put("BK010821", "光大网银或手机银行");
			put("BK010001", "支付宝");
			put("BK010003", "财付通");
			put("BK010020", "利安社区");
			put("BK010002", "联动优势");
			put("BK010004", "中国移动");
			put("BK010005", "银联");
			put("BK010007", "东亚银行");
			put("BK010008", "环迅");
			put("BK010011", "广发银行");
			put("BK010010", "拉卡拉");
			put("BK010012", "富有");
			put("BK010014", "盛付通");
			put("BK010015", "易宝支付");
		}
	};
	private static String IParam[] = { // 广电支付平台输入参数
	"requestId", "tradeProcess", "totalBizType", "totalPrice", "description",
			"mersignature", "goodsDesc", "area", "orderChannel", "encode", };

	private static String PParam[] = { // 广电支付平台产品参数
	"productId", "productName", "fund", "merAcct", "bizType", "productNumber", };

	private static String BParam[] = { // 广电支付平台业务参数
	"busiRequestId", "busiCommandId", "busiPermark", "busiCardObject",
			"busiPackageCode", "busiBoxNo", "busiCaCard", "busiCaId",
			"busiOrderArea", "busiReceiveName", "busiAddress",
			"busiPostalcode", "busiPhone", "busiProductId", "busiProductName",
			"busiFund", "busiMerAcct", "busiBizType", "busiProductNumber",
			"busiProductDateRange", };

	private static String OParam[] = { // 广电支付平台输出参数
	"requestId", "payId", "fiscalDate", "busiOpenedResult", "description",
			"resultSignature", };
	private static int FromBankJFeiFieldCount = 22;// 来自银行缴费包字段个数
	private static int FromGDianUserQueryFieldCount = 8;// 来自广电boos用户查询返回包字段个数
	private static int FromGDianTCQueryFieldCount = 8;// 来自广电boos套餐目录查询返回包字段个数
	
	private static int TIME_OUT = 30000; // 超时时间
	private static int SEND_C_FIELD_NO = 10;// 发送给本地C程序的字段数量
	public static final Logger log = Logger.getLogger(GDPayService.class); // 记录日志
	private Socket client;

	// 从银行读取数据
	public void ReadDataFromBank(ArrayList<String> FromBankJFei,
			ArrayList<String> FromGDianUserQuery,
			ArrayList<String> FromGDianTCQuery) {

		try {
			log.info("函数开始执行");

			DataInputStream in = new DataInputStream(client.getInputStream());
			// Mutil User but can't parallel

			// 读取字符串
			for (int i = 0; i < FromBankJFeiFieldCount; i++) {

				byte readfrombanktemp[] = new byte[4];
				int readlength = 0;
				int sumreadlength = 0;
				while (sumreadlength != 4) {
					readlength = in.read(readfrombanktemp, readlength,
							4 - sumreadlength);
					sumreadlength += readlength;
					// System.out.println("     "+readlength+"  "+sumreadlength);
				}

				// System.out.print(readfrombanktemp);
				String baotoulengthString;
				baotoulengthString = new String(readfrombanktemp, "gb2312");
				log.info("获得报文长度" + baotoulengthString);
				// System.out.println("length:"+baotoulengthString.length());
				// System.out.println(readfrombank);
				// System.out.println(baotoulengthString.replaceFirst("^0*",
				// ""));
				int baotoulengint = Integer.parseInt(baotoulengthString.trim());
				// System.out.println(readfrombank);
				// System.out.println(baotoulengint);
				readlength = 0;
				sumreadlength = 0;
				byte readfrombanktemp1[] = new byte[baotoulengint];
				while (sumreadlength != baotoulengint) {
					readlength = in.read(readfrombanktemp1, readlength,
							baotoulengint - sumreadlength);
					sumreadlength += readlength;
				}
				String readfrombank;
				readfrombank = new String(readfrombanktemp1, "gb2312");
				// System.out.println("[" + readfrombank.length() + "]["
				// + readfrombank + "]");
				log.info("ID[" + i + "][" + readfrombank.length() + "]["
						+ readfrombank + "]");
				FromBankJFei.add(readfrombank);
			}

			for (int i = 0; i < FromGDianUserQueryFieldCount; i++) {
				String readfrombank = "";
				byte readfrombanktemp[] = new byte[4];
				int readlength = 0;
				int sumreadlength = 0;
				while (sumreadlength != 4) {
					readlength = in.read(readfrombanktemp, readlength,
							4 - sumreadlength);
					sumreadlength += readlength;
					// System.out.println("     "+readlength+"  "+sumreadlength);
				}

				// System.out.print(readfrombanktemp);
				String baotoulengthString;
				baotoulengthString = new String(readfrombanktemp, "gb2312");
				log.info("获得报文长度" + baotoulengthString);
				// System.out.println("length:"+baotoulengthString.length());
				// System.out.println(readfrombank);
				// System.out.println(baotoulengthString.replaceFirst("^0*",
				// ""));
				int baotoulengint = Integer.parseInt(baotoulengthString.trim());
				// System.out.println(readfrombank);
				// System.out.println(baotoulengint);
				readlength = 0;
				sumreadlength = 0;
				byte readfrombanktemp1[] = new byte[baotoulengint];
				while (sumreadlength != baotoulengint) {

					readlength = in.read(readfrombanktemp1, readlength,
							baotoulengint - sumreadlength);
					sumreadlength += readlength;
				}
				readfrombank = new String(readfrombanktemp1, "gb2312");
				// System.out.println("[" + readfrombank.length() + "]"
				// + readfrombank);
				log.info("ID[" + i + "][" + readfrombank.length() + "]["
						+ readfrombank + "]");
				FromGDianUserQuery.add(readfrombank);
			}

			for (int i = 0; i < FromGDianTCQueryFieldCount; i++) {
				String readfrombank = "";
				byte readfrombanktemp[] = new byte[4];
				int readlength = 0;
				int sumreadlength = 0;
				while (sumreadlength != 4) {
					readlength = in.read(readfrombanktemp, readlength,
							4 - sumreadlength);
					sumreadlength += readlength;
					// System.out.println("     "+readlength+"  "+sumreadlength);
				}

				// System.out.print(readfrombanktemp);
				String baotoulengthString;
				baotoulengthString = new String(readfrombanktemp, "gb2312");
				log.info("获得报文长度" + baotoulengthString);
				// System.out.println("length:"+baotoulengthString.length());
				// System.out.println(readfrombank);
				// System.out.println(baotoulengthString.replaceFirst("^0*",
				// ""));
				int baotoulengint = Integer.parseInt(baotoulengthString.trim());
				// System.out.println(readfrombank);
				// System.out.println(baotoulengint);
				readlength = 0;
				sumreadlength = 0;
				byte readfrombanktemp1[] = new byte[baotoulengint];
				while (sumreadlength != baotoulengint) {
					readlength = in.read(readfrombanktemp1, readlength,
							baotoulengint - sumreadlength);
					sumreadlength += readlength;
				}
				readfrombank = new String(readfrombanktemp1, "gb2312");
				// System.out.println("[" + readfrombank.length() + "]"
				// + readfrombank);
				log.info("ID[" + i + "][" + readfrombank.length() + "]["
						+ readfrombank + "]");
				FromGDianTCQuery.add(readfrombank);
			}

			log.info("函数正常退出！");
			return;
		} catch (Exception ex) {
			System.out.print(ex);
			GDPayService.log.error(ex);
		} finally {

		}
		// 读取字符串结束
		return;

	}

	// 将数据打包成广电支付平台需要的格式
	public String MakePackageForGDPayService(ArrayList<String> FromBankJFei,
			ArrayList<String> FromGDianUserQuery,
			ArrayList<String> FromGDianTCQuery) {
		log.info("函数开始执行");
		String urlString = "";
		String valueforSignature = "";
		String encryptString = "";
		valueforSignature += FromBankJFei.get(2).trim() + "GDYH" + "BIZ00801"
				+ FromGDianTCQuery.get(7).trim() + "";
		encryptString = SignatureUtil.hmacSign(valueforSignature, Configure.Key);
		urlString += IParam[0] + "=" + FromBankJFei.get(2).trim() + "&";
		urlString += IParam[1] + "=" + "GDYH" + "&";
		urlString += IParam[2] + "=" + "BIZ00801" + "&";
		urlString += IParam[3] + "=" + FromGDianTCQuery.get(7).trim() + "&";
		urlString += IParam[4] + "=" + "" + "&";
		urlString += IParam[5] + "=" + encryptString + "&";
		urlString += IParam[6] + "=" + FromGDianTCQuery.get(5).trim() + "&";
		System.out.println(diquyingshe.get(FromBankJFei.get(5)));
		urlString += IParam[7] + "=" + diquyingshe.get(FromBankJFei.get(5))
				+ "&";// 暂时为空
		System.out.println(FromBankJFei.get(10).trim());
		urlString += IParam[8] + "=" + "" + "&";// 暂时为空
		urlString += IParam[9] + "=" + "gb2312" + "&";

		urlString += PParam[0] + "=" + FromGDianTCQuery.get(5).trim() + "&";
		urlString += PParam[1] + "=" + FromGDianTCQuery.get(6).trim() + "&";
		urlString += PParam[2] + "=" + FromGDianTCQuery.get(7).trim() + "&";
		urlString += PParam[3] + "=" + "GDYH" + "&";
		urlString += PParam[4] + "=" + "BIZ00801" + "&";
		urlString += PParam[5] + "=" + "1" + "&";

		/*
		 * String busrequestidString = ""; Date now = new Date();
		 * SimpleDateFormat dateFormat = new
		 * SimpleDateFormat("yyyyMMddHHmmss");// 可以方便地修改日期格式 busrequestidString
		 * += dateFormat.format(now);
		 * 
		 * for (int i = 0; i < 4; i++) { int number = new Random().nextInt(10);
		 * busrequestidString += Integer.toString(number); }
		 */
		String busrequestidString = FromBankJFei.get(2).substring(0, 8)
				+ FromBankJFei.get(2).substring(14);

		urlString += BParam[0] + "=" + busrequestidString + "&";
		urlString += BParam[1] + "=" + "500005" + "&";
		urlString += BParam[2] + "=" + "1" + "&";
		urlString += BParam[3] + "=" + FromBankJFei.get(13).trim() + "&";
		urlString += BParam[4] + "=" + FromGDianTCQuery.get(5).trim() + "&";
		urlString += BParam[5] + "=" + "" + "&";
		urlString += BParam[6] + "=" + "" + "&";
		urlString += BParam[7] + "=" + "" + "&";
		urlString += BParam[8] + "=" + FromGDianUserQuery.get(4).trim() + "&";
		urlString += BParam[9] + "=" + "" + "&";
		urlString += BParam[10] + "=" + "" + "&";
		urlString += BParam[11] + "=" + "" + "&";
		urlString += BParam[12] + "=" + "" + "&";
		urlString += BParam[13] + "=" + "" + "&";
		urlString += BParam[14] + "=" + "" + "&";
		urlString += BParam[15] + "=" + FromGDianTCQuery.get(7).trim() + "&";
		urlString += BParam[16] + "=" + "GDYH" + "&";
		urlString += BParam[17] + "=" + "BIZ00801" + "&";
		urlString += BParam[18] + "=" + "1" + "&";
		urlString += BParam[19] + "=" + "";
		// System.out.println(urlString);
		log.info("打包结果：[" + urlString + "]");
		return urlString;
	}

	/*
	 * 
	 * 
	 * 正式运行时把该函数中的注释去掉
	 */
	public String CommWithGDPayService(String urlString) {
		HttpURLConnection urlConnection = null;
		String readfromgdpayString = null;
		try {
			/*
						String urlsend = "";
			urlsend += Configure.DTDT_DIRECTORY_URL + urlString;
			URL url = new URL(new String(urlsend.getBytes(), "GB2312"));
			log.info("url内容：[" + url + "]");
			urlConnection = (HttpURLConnection) url.openConnection();

			urlConnection.setConnectTimeout(TIME_OUT);
			urlConnection.setReadTimeout(TIME_OUT);

			urlConnection.setDoOutput(true);
			urlConnection.setDoInput(true);

			urlConnection.setRequestProperty("content-type",
					"application/x-www-form-urlencoded");
			urlConnection.setRequestMethod("GET");

			DataInputStream in = new DataInputStream(
					urlConnection.getInputStream());

			byte readfromgdpaybyte[] = new byte[4000];
			int readnumber;
			int sumreadnumber = 0;
			while ((readnumber = in.read(readfromgdpaybyte, sumreadnumber,
					4000 - sumreadnumber)) != -1) {
				sumreadnumber += readnumber;
				System.out.println(readnumber);
			}
			System.out.println(sumreadnumber);

			byte readfromgdpaybytenew[] = new byte[sumreadnumber];
			System.arraycopy(readfromgdpaybyte, 0, readfromgdpaybytenew, 0,
					sumreadnumber);

			readfromgdpayString = new String(readfromgdpaybytenew, "gb2312");
			*/

			readfromgdpayString ="{\"requestId\":\"20141203786500721160\",\"payId\":\"141203200000066285\",\"fiscalDate\":\"20141203\",\"busiOpenedResult\":\"14176053601635744332:20141203721160:0:021365:收视维护费(基本包)                                              2015020120150228    优享看吧                                                        2015020120150228\",\"description\":\"\",\"resultSignature\":\"b0118895eda59876e27e2b4b98ec2c91\"}";
			// "{requestId:2014071378650035,payId:140713200000026292,fiscalDate:20140713,busiOpenedResult:14053898819933018294:201407150955266391:0:019110:收视维护费(基本包)                                              2015071120150810    互动基本包                                                      2014081420140913    戏曲爱好                                                        2014071520140814,description:暂时为空,resultSignature:7c5bebe066b6e210a19ec5b677164b21}";
			// System.out.println(readfromgdpayString);
			log.info("广电支付平台返回结果：[" + readfromgdpayString + "]");
		} catch (Exception e) {
			GDPayService.log.error(e);
			if (e instanceof ConnectException) {
				GDPayService.log.error("连接超时！！");
				return null;
			} else if (e instanceof SocketTimeoutException) {
				GDPayService.log.error("读取数据超时！！");
				return null;
			} else {

			}

		} finally {
			if (urlConnection != null)
				urlConnection.disconnect();
		}
		return readfromgdpayString;
	}

	public void UnpackDataFromGDPayService(String readfromgdpay,
			ArrayList<String> FromGDPayServiceJieXi) throws JSONException {

		/*
		 * String readfromgdpaytemp = readfromgdpay.substring(1,
		 * readfromgdpay.length() - 2); String[] FromGDPayServiceJieXitemp =
		 * readfromgdpaytemp.split(","); // FromGDPayServiceJieXi =
		 * (ArrayList<String>) // Arrays.asList(FromGDPayServiceJieXitemp); for
		 * (int i = 0; i < FromGDPayServiceJieXitemp.length; i++) {
		 * log.info(FromGDPayServiceJieXitemp[i]); } for (int i = 0; i <
		 * OParam.length; i++) { for (int j = 0; j <
		 * FromGDPayServiceJieXitemp.length; j++) { String KeyValue[] =
		 * FromGDPayServiceJieXitemp[j].split(":"); if
		 * (OParam[i].equals(KeyValue[0])) {
		 * if(OParam[i].equals("description")){//透传信息特殊处理
		 * FromGDPayServiceJieXi.add(""); } else
		 * if(OParam[i].equals("busiOpenedResult")){//透传信息特殊处理 for (int k = 1; k
		 * < KeyValue.length; k++) { FromGDPayServiceJieXi.add(KeyValue[k]); }
		 * if( KeyValue.length < 6) { for (int k = KeyValue.length; k <
		 * KeyValue.length; k++) { FromGDPayServiceJieXi.add(""); } } } else{
		 * for (int k = 1; k < KeyValue.length; k++) {
		 * FromGDPayServiceJieXi.add(KeyValue[k]); } }
		 * 
		 * break; } } }
		 */
		// String json =
		// "{\"busiOpenedResult\":\"14102514289673970437:201409091619288434:0:019210:收视维护费(基本包)                                              2047011120480110    互动基本包                                                      2046021420470213    付费节目体验包                                                  2045082020460819    互动影院动作战争                                                2045082020460819    央视、卫视十套高清（体验）                                      2045082020460819    天华高清影院                                                    2045082020460819    天华高清体育                                                    2045082020460819    天华高清探索                                                    2045082020460819    优视e家包一                                                     2045082020460819    3D影视                                                          2045082020460819    幸福2012                                                        2045082020460819    互动高清                                                        2045082020460819\",\"description\":\"\",\"fiscalDate\":\"20140909\",\"payId\":\"140909200000101313\",\"requestId\":\"2014071878650024\",\"resultSignature\":\"56b05fbeef636fba52f095d37775cf84\"}";

		if (readfromgdpay == null) { // 未读取到广电数据
			for (int i = 0; i < SEND_C_FIELD_NO; i++) {
				FromGDPayServiceJieXi.add("1");
			}
			return;
		}
		JSONObject jo = new JSONObject(readfromgdpay);

		// System.out.println("\n将Json数据解析为Map：");
		try {
			if (jo.getString("ERROR_MSG") != null) {// 读取到的广电数据中含有ERROR_MSG字段
				for (int i = 0; i < SEND_C_FIELD_NO; i++) {
					FromGDPayServiceJieXi.add("1");
				}
				for (int i = 0; i < FromGDPayServiceJieXi.size(); i++) {
					log.info("广电支付平台数据解析结果:[ID" + i + "]["
							+ FromGDPayServiceJieXi.get(i) + "]");
				}
				return;
			}
		} catch (Exception e) {
			GDPayService.log.warn(e);
		}
		for (int i = 0; i < OParam.length; i++) {
			if (OParam[i].equals("busiOpenedResult")) {
				String KeyValue[] = jo.getString(OParam[i]).split(":");
				for (int j = 0; j < KeyValue.length; j++) {
					FromGDPayServiceJieXi.add(KeyValue[j]);
				}
			} else {
				FromGDPayServiceJieXi.add(jo.getString(OParam[i]));
			}
			System.out.println(jo.getString(OParam[i]));
		}
		for (int i = 0; i < FromGDPayServiceJieXi.size(); i++) {
			log.info("广电支付平台数据解析结果:[ID" + i + "]["
					+ FromGDPayServiceJieXi.get(i) + "]");
		}

	}

	// 向银行回复数据
	public void senddatatobank(ArrayList<String> FromGDPayServiceJieXi) {
		// log.info("函数开始运行");
		try {

			DataOutputStream out = new DataOutputStream(
					client.getOutputStream());
			// Mutil User but can't parallel

			// 读取字符串
			for (int i = 0; i < FromGDPayServiceJieXi.size(); i++) {

				byte sendtobank[] = FromGDPayServiceJieXi.get(i).getBytes();
				byte sendlengthtemp[] = Integer.toString(sendtobank.length)
						.getBytes();
				byte sendlength[] = new byte[4];
				if (sendlengthtemp.length == 1) {
					sendlength[0] = sendlengthtemp[0];
					sendlength[1] = ' ';
					sendlength[2] = ' ';
					sendlength[3] = ' ';
				} else if (sendlengthtemp.length == 2) {
					sendlength[0] = sendlengthtemp[0];
					sendlength[1] = sendlengthtemp[1];
					sendlength[2] = ' ';
					sendlength[3] = ' ';
				} else if (sendlengthtemp.length == 3) {
					sendlength[0] = sendlengthtemp[0];
					sendlength[1] = sendlengthtemp[1];
					sendlength[2] = sendlengthtemp[2];
					sendlength[3] = ' ';
				} else if (sendlengthtemp.length == 4) {
					sendlength[0] = sendlengthtemp[0];
					sendlength[1] = sendlengthtemp[1];
					sendlength[2] = sendlengthtemp[2];
					sendlength[3] = sendlengthtemp[3];
				}
				out.write(sendlength, 0, sendlength.length);
				out.flush();
				out.write(sendtobank, 0, sendtobank.length);
				out.flush();
			}
			return;
		} catch (Exception ex) {
			System.out.print(ex);
			GDPayService.log.error(ex);
		} finally {

		}
		// 读取字符串结束
		return;

	}

	public void WriteMysql(ArrayList<String> FromBankJFei,
			ArrayList<String> FromGDianUserQuery,
			ArrayList<String> FromGDianTCQuery,
			ArrayList<String> FromGDPayServiceJieXi) {
		// true---history表 false--not history

		Dbmanage dbmanage = new Dbmanage();
		Connection conn = null;
		Statement sta = null;

		try {
			conn = dbmanage.initDB();
			sta = conn.createStatement();

			String sql = "INSERT INTO gdbank_log VALUES('"
					+ FromBankJFei.get(2).trim() + "','" // 交易标识：流水号
					+ FromBankJFei.get(1).trim() + "','" // 交易码：缴费0001、撤销0002、卡表购电1001、卡表购电撤销1002
					+ "缴费" + "','" // 交易名称：缴费、撤销、卡表购电、卡表购电撤销
					+ FromBankJFei.get(13).trim() + "','" // 客户标识：缴费号、客户编号（设保留字段，填写关联信息）缴费交易，用于标识客户身份的信息
					+ "" + "','" // 客户辅助信息：（客户标识的关联信息等）(暂时为空)
					+ FromGDianUserQuery.get(6).trim() + "','" // 客户名称：
					+ FromBankJFei.get(3).trim() + "','" // 交易日期：
					+ FromBankJFei.get(4).trim() + "','" // 交易时间：
					+ FromBankJFei.get(16).trim() + "','"; // 交易金额：

			if (FromGDPayServiceJieXi.get(5).equals("0")) // 交易成功标识：0、1、2
															// 0成功，1失败，2是撤销
			{
				sql += "0" + "','" // 交易成功标识：0、1、2 0成功，1失败，2是撤销
						+ "成功" + "','" + "" + "','" // 原交易流水号：缴费交易时为空
						+ "0" + "','" // //交易状态：0、1 0成功，1失败
						+ "成功" + "','";
			} else {
				sql += "1" + "','" // 交易成功标识：0、1、2 0成功，1失败，2是撤销
						+ "失败" + "','" + "" + "','" // 原交易流水号：缴费交易时为空
						+ "1" + "','" // //交易状态：0、1 0成功，1失败
						+ "失败" + "','";
			}
			sql += "" + "','" // 返回信息：？
					+ FromBankJFei.get(5).trim()
					+ "','"// 区域编码：
					+ CHdiquyingshe.get(FromBankJFei.get(5))
					+ "','" // 区域名称：
					+ FromBankJFei.get(6).trim()
					+ "','" // 交易渠道编码
					+ CHjiaoyiqudaoyingshe.get(FromBankJFei.get(6).trim())
					+ "','" // 交易渠道名称:
					+ FromBankJFei.get(7).trim() + "','"// 交易机构：
					+ "" + "','" // 交易机构名称：?????
					+ FromBankJFei.get(8).trim() + "','" // 操作员号
					+ FromBankJFei.get(9).trim() + "','" // 收费单位类别编码：
					+ "广电缴费" + "','" // 收费单位类别名称：公共事业类缴费、物业类、学费类、其它类
					+ "096766" + "','" // 收费单位一级机构编码：一级机构编码
					+ "陕西广电网络" + "','"; // 收费单位一级机构名称：
			if (FromGDianUserQuery.get(5).equals("A")) {
				sql += "00" + "','" // 收费单位二级机构编码
						+ "陕西省公司（除宝鸡）用户" + "','"; // 收费单位二级机构名称
			} else if (FromGDianUserQuery.get(5).equals("B")) {
				sql += "01" + "','" // 收费单位二级机构编码
						+ "宝鸡子公司用户" + "','"; // 收费单位二级机构名称
			}
			sql += "0" + "','"// 删除标记 0--未删除 1--已删除 标识此记录是否删除
					+ "未删除" + "','"// 删除标记位详情 0--未删除 1--已删除 标识此记录是否删除
					+ "0" + "','"// 修改标记 数字0--未修改 1---修改一次 2--修改两次 3--修改三次
					+ "未修改" + "','"// 修改标记位详情 0--未修改 1---修改一次 2--修改两次 3--修改三次
					+ "0" + "','"// 对账结果 0--未对账1---对账成功2---记录有冲突3-对账文件不存在本记录
					+ "未对账" + "','"// 对账结果详情 0--未对账 1-对账成功 2---记录有冲突
									// 3-对账文件不存在本记录
					+ "" + "','"// 备注
					+ "" + "','"// 保留1
					+ "" + "','"// 保留2
					+ "" + "');";// 保留3

			// System.out.println(sql);
			log.info("sql语句：" + sql);
			int result = sta.executeUpdate(sql);
			log.info("受影响的行数" + result);
		} catch (SQLException e) {
			log.error(e);

		} finally {
			// 执行完关闭数据库
			dbmanage.closeDB(sta, conn);

		}
	}

	public void run() {
		ArrayList<String> FromBankJFei = new ArrayList<String>();
		ArrayList<String> FromGDianUserQuery = new ArrayList<String>();
		ArrayList<String> FromGDianTCQuery = new ArrayList<String>();
		ArrayList<String> FromGDPayServiceJieXi = new ArrayList<String>();
		String urlString;
		String readfromgdpay;
		try {
			log.info("线程开始运行");
			ReadDataFromBank(FromBankJFei, FromGDianUserQuery, FromGDianTCQuery);
			urlString = MakePackageForGDPayService(FromBankJFei,
					FromGDianUserQuery, FromGDianTCQuery);
			readfromgdpay = CommWithGDPayService(urlString);
			UnpackDataFromGDPayService(readfromgdpay, FromGDPayServiceJieXi);
			senddatatobank(FromGDPayServiceJieXi);
			WriteMysql(FromBankJFei, FromGDianUserQuery, FromGDianTCQuery,
					FromGDPayServiceJieXi);
			log.info("线程正常退出");

		} catch (Exception ex) {
			// System.out.print(ex);
			GDPayService.log.error(ex);
		} finally {
		}
	}

	public static void main(String[] args) throws IOException {
		PropertyConfigurator.configure(Configure.log4jPropertiesName);
		log.info("程序开始运行!");
		log.info(Configure.programVersion + "  " + Configure.DTDT_DIRECTORY_URL+ "  " + 
				Configure.PORT + "  " + Configure.Key + "  " + Configure.log4jPropertiesName + "  ");
		ServerSocket server = new ServerSocket(Configure.PORT);
		while (true) {
			// transfer location change Single User or Multi User

			Socket newsocket = server.accept();
			GDPayService mc = new GDPayService(newsocket);
			log.info(newsocket.getRemoteSocketAddress() + "发起连接，已接受!");
			mc.start();
		}

	}

}
