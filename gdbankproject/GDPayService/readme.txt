�°汾

1. �޸������ļ�configuration.xml

		�޸� <ProgramVersion>��ǩ����


2. debug�汾ע������
	debug�汾�Ǳ���ģ����BOSS��֧��ƽ̨��BOSS��ģ�⿪��ģ����򼴿ɣ�֧��ƽ̨��ģ������������
	
	 GDPayService.javaԴ�ļ�������������֮�����ע���л�
	
	public String CommWithGDPayService(String urlString) {
		HttpURLConnection urlConnection = null;
		String readfromgdpayString = null;
		try {
//��һ���ֿ�ʼ
			String urlsend = "";
			urlsend += DTDT_DIRECTORY_URL + urlString;
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
//��һ���ֽ���


			
//�ڶ����ֿ�ʼ			
			readfromgdpayString =
			"{\"requestId\":\"20141203786500721160\",\"payId\":\"141203200000066285\",\"fiscalDate\":\"20141203\",\"busiOpenedResult\":\"14176053601635744332:20141203721160:0:021365:����ά����(������)                                              2015020120150228    ������                                                        2015020120150228\",\"description\":\"\",\"resultSignature\":\"b0118895eda59876e27e2b4b98ec2c91\"}";
			"{requestId:2014071378650035,payId:140713200000026292,fiscalDate:20140713,busiOpenedResult:14053898819933018294:201407150955266391:0:019110:����ά����(������)                                              2015071120150810    ����������                                                      2014081420140913    Ϸ������                                                        2014071520140814,description:��ʱΪ��,resultSignature:7c5bebe066b6e210a19ec5b677164b21}";
			System.out.println(readfromgdpayString);			
//�ڶ����ֽ���
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
	
































�ϰ汾

���԰汾����ʽ�汾��debug�汾֮���л�ע������


1. GDPayService.javaԴ�ļ��е����ò��������л�

// ��ʽ����
	private static String DTDT_DIRECTORY_URL = "http://10.27.109.138:8080/notify/notify_start?"; // ��ʽ����
	private static int PORT = 9340;// ��ʽ����
	private static String Key = "z19EReMdHJwGsw2p";// ��ʽ����
	
	
// ���Ի���	
	//private static String DTDT_DIRECTORY_URL = "http://10.27.109.140:8080/notify/notify_start?"; // ���Ի���
	//private static int PORT = 9330;// ���Ի���
	//private static String Key = "f0mcn6r0ZLYg4EZZ"; // ���Ի���

	
// debug����	
	/*private static String DTDT_DIRECTORY_URL ="http://localhost:8080/notify/notify_start?"; //debug����
	private static int PORT = 9320;// debug����
	private static String Key = "f0mcn6r0ZLYg4EZZ"; // debug����
	*/	


2. �޸�log4j.properties�й�����־�ļ�·���Ĳ�����
	#record parh
	log4j.appender.ServerDailyRollingFile.File=./formalgdpayservicelog/formalgdpayservice.log
	#log4j.appender.ServerDailyRollingFile.File=./testgdpayservicelog/testgdpayservice.log
	#log4j.appender.ServerDailyRollingFile.File=./debuggdpayservicelog/debuggdpayservice.log

3. debug�汾ע������
	debug�汾�Ǳ���ģ����BOSS��֧��ƽ̨��BOSS��ģ�⿪��ģ����򼴿ɣ�֧��ƽ̨��ģ������������
	
	 GDPayService.javaԴ�ļ�������������֮�����ע���л�
	
	public String CommWithGDPayService(String urlString) {
		HttpURLConnection urlConnection = null;
		String readfromgdpayString = null;
		try {
//��һ���ֿ�ʼ
			String urlsend = "";
			urlsend += DTDT_DIRECTORY_URL + urlString;
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
//��һ���ֽ���


			
//�ڶ����ֿ�ʼ			
			readfromgdpayString =
			"{\"requestId\":\"20141203786500721160\",\"payId\":\"141203200000066285\",\"fiscalDate\":\"20141203\",\"busiOpenedResult\":\"14176053601635744332:20141203721160:0:021365:����ά����(������)                                              2015020120150228    ������                                                        2015020120150228\",\"description\":\"\",\"resultSignature\":\"b0118895eda59876e27e2b4b98ec2c91\"}";
			"{requestId:2014071378650035,payId:140713200000026292,fiscalDate:20140713,busiOpenedResult:14053898819933018294:201407150955266391:0:019110:����ά����(������)                                              2015071120150810    ����������                                                      2014081420140913    Ϸ������                                                        2014071520140814,description:��ʱΪ��,resultSignature:7c5bebe066b6e210a19ec5b677164b21}";
			System.out.println(readfromgdpayString);			
//�ڶ����ֽ���
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
	