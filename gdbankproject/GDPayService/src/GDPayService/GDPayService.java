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

	//private static String DTDT_DIRECTORY_URL = "http://10.27.109.138:8080/notify/notify_start?"; // ��ʽ����
	//private static int PORT = 9340;// ��ʽ����
	//private static String Key = "z19EReMdHJwGsw2p";// ��ʽ����
	
	
	
	//private static String DTDT_DIRECTORY_URL = "http://10.27.109.140:8080/notify/notify_start?"; // ���Ի���
	//private static int PORT = 9330;// ���Ի���
	//private static String Key = "f0mcn6r0ZLYg4EZZ"; // ���Ի���
	
	
	/*private static String DTDT_DIRECTORY_URL ="http://localhost:8080/notify/notify_start?"; //debug����
	private static int PORT = 9320;// debug����
	private static String Key = "f0mcn6r0ZLYg4EZZ"; // debug����
*/	
	
	public static Map<String, String> CHdiquyingshe = new HashMap<String, String>() {
		{
			put("029 ", "����");
			put("0913", "μ��");
			put("0910", "����");
			put("0917", "����");
			put("0916", "����");
			put("0919", "ͭ��");
			put("0915", "����");
			put("0914", "����");
			put("0911", "�Ӱ�");
			put("0912", "����");
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
	// ���н��������͹�綩������ӳ��
	/*
	 * public static Map<String, String> jiaoyiqudaoyingshe = new
	 * HashMap<String, String>() { { put("BK010112", "1"); put("BK010870", "3");
	 * put("BK010821", "3"); } };
	 */
	public static Map<String, String> CHjiaoyiqudaoyingshe = new HashMap<String, String>() {
		{
			put("BK010112", "����̨");
			put("BK010821", "����������ֻ�����");
			put("BK010001", "֧����");
			put("BK010003", "�Ƹ�ͨ");
			put("BK010020", "��������");
			put("BK010002", "��������");
			put("BK010004", "�й��ƶ�");
			put("BK010005", "����");
			put("BK010007", "��������");
			put("BK010008", "��Ѹ");
			put("BK010011", "�㷢����");
			put("BK010010", "������");
			put("BK010012", "����");
			put("BK010014", "ʢ��ͨ");
			put("BK010015", "�ױ�֧��");
		}
	};
	private static String IParam[] = { // ���֧��ƽ̨�������
	"requestId", "tradeProcess", "totalBizType", "totalPrice", "description",
			"mersignature", "goodsDesc", "area", "orderChannel", "encode", };

	private static String PParam[] = { // ���֧��ƽ̨��Ʒ����
	"productId", "productName", "fund", "merAcct", "bizType", "productNumber", };

	private static String BParam[] = { // ���֧��ƽ̨ҵ�����
	"busiRequestId", "busiCommandId", "busiPermark", "busiCardObject",
			"busiPackageCode", "busiBoxNo", "busiCaCard", "busiCaId",
			"busiOrderArea", "busiReceiveName", "busiAddress",
			"busiPostalcode", "busiPhone", "busiProductId", "busiProductName",
			"busiFund", "busiMerAcct", "busiBizType", "busiProductNumber",
			"busiProductDateRange", };

	private static String OParam[] = { // ���֧��ƽ̨�������
	"requestId", "payId", "fiscalDate", "busiOpenedResult", "description",
			"resultSignature", };
	private static int FromBankJFeiFieldCount = 22;// �������нɷѰ��ֶθ���
	private static int FromGDianUserQueryFieldCount = 8;// ���Թ��boos�û���ѯ���ذ��ֶθ���
	private static int FromGDianTCQueryFieldCount = 8;// ���Թ��boos�ײ�Ŀ¼��ѯ���ذ��ֶθ���
	
	private static int TIME_OUT = 30000; // ��ʱʱ��
	private static int SEND_C_FIELD_NO = 10;// ���͸�����C������ֶ�����
	public static final Logger log = Logger.getLogger(GDPayService.class); // ��¼��־
	private Socket client;

	// �����ж�ȡ����
	public void ReadDataFromBank(ArrayList<String> FromBankJFei,
			ArrayList<String> FromGDianUserQuery,
			ArrayList<String> FromGDianTCQuery) {

		try {
			log.info("������ʼִ��");

			DataInputStream in = new DataInputStream(client.getInputStream());
			// Mutil User but can't parallel

			// ��ȡ�ַ���
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
				log.info("��ñ��ĳ���" + baotoulengthString);
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
				log.info("��ñ��ĳ���" + baotoulengthString);
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
				log.info("��ñ��ĳ���" + baotoulengthString);
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

			log.info("���������˳���");
			return;
		} catch (Exception ex) {
			System.out.print(ex);
			GDPayService.log.error(ex);
		} finally {

		}
		// ��ȡ�ַ�������
		return;

	}

	// �����ݴ���ɹ��֧��ƽ̨��Ҫ�ĸ�ʽ
	public String MakePackageForGDPayService(ArrayList<String> FromBankJFei,
			ArrayList<String> FromGDianUserQuery,
			ArrayList<String> FromGDianTCQuery) {
		log.info("������ʼִ��");
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
				+ "&";// ��ʱΪ��
		System.out.println(FromBankJFei.get(10).trim());
		urlString += IParam[8] + "=" + "" + "&";// ��ʱΪ��
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
		 * SimpleDateFormat("yyyyMMddHHmmss");// ���Է�����޸����ڸ�ʽ busrequestidString
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
		log.info("��������[" + urlString + "]");
		return urlString;
	}

	/*
	 * 
	 * 
	 * ��ʽ����ʱ�Ѹú����е�ע��ȥ��
	 */
	public String CommWithGDPayService(String urlString) {
		HttpURLConnection urlConnection = null;
		String readfromgdpayString = null;
		try {
			/*
						String urlsend = "";
			urlsend += Configure.DTDT_DIRECTORY_URL + urlString;
			URL url = new URL(new String(urlsend.getBytes(), "GB2312"));
			log.info("url���ݣ�[" + url + "]");
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

			readfromgdpayString ="{\"requestId\":\"20141203786500721160\",\"payId\":\"141203200000066285\",\"fiscalDate\":\"20141203\",\"busiOpenedResult\":\"14176053601635744332:20141203721160:0:021365:����ά����(������)                                              2015020120150228    ������                                                        2015020120150228\",\"description\":\"\",\"resultSignature\":\"b0118895eda59876e27e2b4b98ec2c91\"}";
			// "{requestId:2014071378650035,payId:140713200000026292,fiscalDate:20140713,busiOpenedResult:14053898819933018294:201407150955266391:0:019110:����ά����(������)                                              2015071120150810    ����������                                                      2014081420140913    Ϸ������                                                        2014071520140814,description:��ʱΪ��,resultSignature:7c5bebe066b6e210a19ec5b677164b21}";
			// System.out.println(readfromgdpayString);
			log.info("���֧��ƽ̨���ؽ����[" + readfromgdpayString + "]");
		} catch (Exception e) {
			GDPayService.log.error(e);
			if (e instanceof ConnectException) {
				GDPayService.log.error("���ӳ�ʱ����");
				return null;
			} else if (e instanceof SocketTimeoutException) {
				GDPayService.log.error("��ȡ���ݳ�ʱ����");
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
		 * if(OParam[i].equals("description")){//͸����Ϣ���⴦��
		 * FromGDPayServiceJieXi.add(""); } else
		 * if(OParam[i].equals("busiOpenedResult")){//͸����Ϣ���⴦�� for (int k = 1; k
		 * < KeyValue.length; k++) { FromGDPayServiceJieXi.add(KeyValue[k]); }
		 * if( KeyValue.length < 6) { for (int k = KeyValue.length; k <
		 * KeyValue.length; k++) { FromGDPayServiceJieXi.add(""); } } } else{
		 * for (int k = 1; k < KeyValue.length; k++) {
		 * FromGDPayServiceJieXi.add(KeyValue[k]); } }
		 * 
		 * break; } } }
		 */
		// String json =
		// "{\"busiOpenedResult\":\"14102514289673970437:201409091619288434:0:019210:����ά����(������)                                              2047011120480110    ����������                                                      2046021420470213    ���ѽ�Ŀ�����                                                  2045082020460819    ����ӰԺ����ս��                                                2045082020460819    ���ӡ�����ʮ�׸��壨���飩                                      2045082020460819    �컪����ӰԺ                                                    2045082020460819    �컪��������                                                    2045082020460819    �컪����̽��                                                    2045082020460819    ����e�Ұ�һ                                                     2045082020460819    3DӰ��                                                          2045082020460819    �Ҹ�2012                                                        2045082020460819    ��������                                                        2045082020460819\",\"description\":\"\",\"fiscalDate\":\"20140909\",\"payId\":\"140909200000101313\",\"requestId\":\"2014071878650024\",\"resultSignature\":\"56b05fbeef636fba52f095d37775cf84\"}";

		if (readfromgdpay == null) { // δ��ȡ���������
			for (int i = 0; i < SEND_C_FIELD_NO; i++) {
				FromGDPayServiceJieXi.add("1");
			}
			return;
		}
		JSONObject jo = new JSONObject(readfromgdpay);

		// System.out.println("\n��Json���ݽ���ΪMap��");
		try {
			if (jo.getString("ERROR_MSG") != null) {// ��ȡ���Ĺ�������к���ERROR_MSG�ֶ�
				for (int i = 0; i < SEND_C_FIELD_NO; i++) {
					FromGDPayServiceJieXi.add("1");
				}
				for (int i = 0; i < FromGDPayServiceJieXi.size(); i++) {
					log.info("���֧��ƽ̨���ݽ������:[ID" + i + "]["
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
			log.info("���֧��ƽ̨���ݽ������:[ID" + i + "]["
					+ FromGDPayServiceJieXi.get(i) + "]");
		}

	}

	// �����лظ�����
	public void senddatatobank(ArrayList<String> FromGDPayServiceJieXi) {
		// log.info("������ʼ����");
		try {

			DataOutputStream out = new DataOutputStream(
					client.getOutputStream());
			// Mutil User but can't parallel

			// ��ȡ�ַ���
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
		// ��ȡ�ַ�������
		return;

	}

	public void WriteMysql(ArrayList<String> FromBankJFei,
			ArrayList<String> FromGDianUserQuery,
			ArrayList<String> FromGDianTCQuery,
			ArrayList<String> FromGDPayServiceJieXi) {
		// true---history�� false--not history

		Dbmanage dbmanage = new Dbmanage();
		Connection conn = null;
		Statement sta = null;

		try {
			conn = dbmanage.initDB();
			sta = conn.createStatement();

			String sql = "INSERT INTO gdbank_log VALUES('"
					+ FromBankJFei.get(2).trim() + "','" // ���ױ�ʶ����ˮ��
					+ FromBankJFei.get(1).trim() + "','" // �����룺�ɷ�0001������0002��������1001�������糷��1002
					+ "�ɷ�" + "','" // �������ƣ��ɷѡ������������硢�����糷��
					+ FromBankJFei.get(13).trim() + "','" // �ͻ���ʶ���ɷѺš��ͻ���ţ��豣���ֶΣ���д������Ϣ���ɷѽ��ף����ڱ�ʶ�ͻ���ݵ���Ϣ
					+ "" + "','" // �ͻ�������Ϣ�����ͻ���ʶ�Ĺ�����Ϣ�ȣ�(��ʱΪ��)
					+ FromGDianUserQuery.get(6).trim() + "','" // �ͻ����ƣ�
					+ FromBankJFei.get(3).trim() + "','" // �������ڣ�
					+ FromBankJFei.get(4).trim() + "','" // ����ʱ�䣺
					+ FromBankJFei.get(16).trim() + "','"; // ���׽�

			if (FromGDPayServiceJieXi.get(5).equals("0")) // ���׳ɹ���ʶ��0��1��2
															// 0�ɹ���1ʧ�ܣ�2�ǳ���
			{
				sql += "0" + "','" // ���׳ɹ���ʶ��0��1��2 0�ɹ���1ʧ�ܣ�2�ǳ���
						+ "�ɹ�" + "','" + "" + "','" // ԭ������ˮ�ţ��ɷѽ���ʱΪ��
						+ "0" + "','" // //����״̬��0��1 0�ɹ���1ʧ��
						+ "�ɹ�" + "','";
			} else {
				sql += "1" + "','" // ���׳ɹ���ʶ��0��1��2 0�ɹ���1ʧ�ܣ�2�ǳ���
						+ "ʧ��" + "','" + "" + "','" // ԭ������ˮ�ţ��ɷѽ���ʱΪ��
						+ "1" + "','" // //����״̬��0��1 0�ɹ���1ʧ��
						+ "ʧ��" + "','";
			}
			sql += "" + "','" // ������Ϣ����
					+ FromBankJFei.get(5).trim()
					+ "','"// ������룺
					+ CHdiquyingshe.get(FromBankJFei.get(5))
					+ "','" // �������ƣ�
					+ FromBankJFei.get(6).trim()
					+ "','" // ������������
					+ CHjiaoyiqudaoyingshe.get(FromBankJFei.get(6).trim())
					+ "','" // ������������:
					+ FromBankJFei.get(7).trim() + "','"// ���׻�����
					+ "" + "','" // ���׻������ƣ�?????
					+ FromBankJFei.get(8).trim() + "','" // ����Ա��
					+ FromBankJFei.get(9).trim() + "','" // �շѵ�λ�����룺
					+ "���ɷ�" + "','" // �շѵ�λ������ƣ�������ҵ��ɷѡ���ҵ�ࡢѧ���ࡢ������
					+ "096766" + "','" // �շѵ�λһ���������룺һ����������
					+ "�����������" + "','"; // �շѵ�λһ���������ƣ�
			if (FromGDianUserQuery.get(5).equals("A")) {
				sql += "00" + "','" // �շѵ�λ������������
						+ "����ʡ��˾�����������û�" + "','"; // �շѵ�λ������������
			} else if (FromGDianUserQuery.get(5).equals("B")) {
				sql += "01" + "','" // �շѵ�λ������������
						+ "�����ӹ�˾�û�" + "','"; // �շѵ�λ������������
			}
			sql += "0" + "','"// ɾ����� 0--δɾ�� 1--��ɾ�� ��ʶ�˼�¼�Ƿ�ɾ��
					+ "δɾ��" + "','"// ɾ�����λ���� 0--δɾ�� 1--��ɾ�� ��ʶ�˼�¼�Ƿ�ɾ��
					+ "0" + "','"// �޸ı�� ����0--δ�޸� 1---�޸�һ�� 2--�޸����� 3--�޸�����
					+ "δ�޸�" + "','"// �޸ı��λ���� 0--δ�޸� 1---�޸�һ�� 2--�޸����� 3--�޸�����
					+ "0" + "','"// ���˽�� 0--δ����1---���˳ɹ�2---��¼�г�ͻ3-�����ļ������ڱ���¼
					+ "δ����" + "','"// ���˽������ 0--δ���� 1-���˳ɹ� 2---��¼�г�ͻ
									// 3-�����ļ������ڱ���¼
					+ "" + "','"// ��ע
					+ "" + "','"// ����1
					+ "" + "','"// ����2
					+ "" + "');";// ����3

			// System.out.println(sql);
			log.info("sql��䣺" + sql);
			int result = sta.executeUpdate(sql);
			log.info("��Ӱ�������" + result);
		} catch (SQLException e) {
			log.error(e);

		} finally {
			// ִ����ر����ݿ�
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
			log.info("�߳̿�ʼ����");
			ReadDataFromBank(FromBankJFei, FromGDianUserQuery, FromGDianTCQuery);
			urlString = MakePackageForGDPayService(FromBankJFei,
					FromGDianUserQuery, FromGDianTCQuery);
			readfromgdpay = CommWithGDPayService(urlString);
			UnpackDataFromGDPayService(readfromgdpay, FromGDPayServiceJieXi);
			senddatatobank(FromGDPayServiceJieXi);
			WriteMysql(FromBankJFei, FromGDianUserQuery, FromGDianTCQuery,
					FromGDPayServiceJieXi);
			log.info("�߳������˳�");

		} catch (Exception ex) {
			// System.out.print(ex);
			GDPayService.log.error(ex);
		} finally {
		}
	}

	public static void main(String[] args) throws IOException {
		PropertyConfigurator.configure(Configure.log4jPropertiesName);
		log.info("����ʼ����!");
		log.info(Configure.programVersion + "  " + Configure.DTDT_DIRECTORY_URL+ "  " + 
				Configure.PORT + "  " + Configure.Key + "  " + Configure.log4jPropertiesName + "  ");
		ServerSocket server = new ServerSocket(Configure.PORT);
		while (true) {
			// transfer location change Single User or Multi User

			Socket newsocket = server.accept();
			GDPayService mc = new GDPayService(newsocket);
			log.info(newsocket.getRemoteSocketAddress() + "�������ӣ��ѽ���!");
			mc.start();
		}

	}

}
