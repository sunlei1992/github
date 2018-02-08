新版本

1. 修改配置文件configuration.xml

		修改 <ProgramVersion>标签内容


2. debug版本注意事项
	debug版本是本地模拟广电BOSS和支付平台，BOSS的模拟开启模拟程序即可，支付平台的模拟如下所述。
	
	 GDPayService.java源文件中在这两部分之间进行注释切换
	
	public String CommWithGDPayService(String urlString) {
		HttpURLConnection urlConnection = null;
		String readfromgdpayString = null;
		try {
//第一部分开始
			String urlsend = "";
			urlsend += DTDT_DIRECTORY_URL + urlString;
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
//第一部分结束


			
//第二部分开始			
			readfromgdpayString =
			"{\"requestId\":\"20141203786500721160\",\"payId\":\"141203200000066285\",\"fiscalDate\":\"20141203\",\"busiOpenedResult\":\"14176053601635744332:20141203721160:0:021365:收视维护费(基本包)                                              2015020120150228    优享看吧                                                        2015020120150228\",\"description\":\"\",\"resultSignature\":\"b0118895eda59876e27e2b4b98ec2c91\"}";
			"{requestId:2014071378650035,payId:140713200000026292,fiscalDate:20140713,busiOpenedResult:14053898819933018294:201407150955266391:0:019110:收视维护费(基本包)                                              2015071120150810    互动基本包                                                      2014081420140913    戏曲爱好                                                        2014071520140814,description:暂时为空,resultSignature:7c5bebe066b6e210a19ec5b677164b21}";
			System.out.println(readfromgdpayString);			
//第二部分结束
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
	
































老版本

测试版本，正式版本，debug版本之间切换注意事项


1. GDPayService.java源文件中的配置参数进行切换

// 正式环境
	private static String DTDT_DIRECTORY_URL = "http://10.27.109.138:8080/notify/notify_start?"; // 正式环境
	private static int PORT = 9340;// 正式环境
	private static String Key = "z19EReMdHJwGsw2p";// 正式环境
	
	
// 测试环境	
	//private static String DTDT_DIRECTORY_URL = "http://10.27.109.140:8080/notify/notify_start?"; // 测试环境
	//private static int PORT = 9330;// 测试环境
	//private static String Key = "f0mcn6r0ZLYg4EZZ"; // 测试环境

	
// debug环境	
	/*private static String DTDT_DIRECTORY_URL ="http://localhost:8080/notify/notify_start?"; //debug环境
	private static int PORT = 9320;// debug环境
	private static String Key = "f0mcn6r0ZLYg4EZZ"; // debug环境
	*/	


2. 修改log4j.properties中关于日志文件路径的参数。
	#record parh
	log4j.appender.ServerDailyRollingFile.File=./formalgdpayservicelog/formalgdpayservice.log
	#log4j.appender.ServerDailyRollingFile.File=./testgdpayservicelog/testgdpayservice.log
	#log4j.appender.ServerDailyRollingFile.File=./debuggdpayservicelog/debuggdpayservice.log

3. debug版本注意事项
	debug版本是本地模拟广电BOSS和支付平台，BOSS的模拟开启模拟程序即可，支付平台的模拟如下所述。
	
	 GDPayService.java源文件中在这两部分之间进行注释切换
	
	public String CommWithGDPayService(String urlString) {
		HttpURLConnection urlConnection = null;
		String readfromgdpayString = null;
		try {
//第一部分开始
			String urlsend = "";
			urlsend += DTDT_DIRECTORY_URL + urlString;
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
//第一部分结束


			
//第二部分开始			
			readfromgdpayString =
			"{\"requestId\":\"20141203786500721160\",\"payId\":\"141203200000066285\",\"fiscalDate\":\"20141203\",\"busiOpenedResult\":\"14176053601635744332:20141203721160:0:021365:收视维护费(基本包)                                              2015020120150228    优享看吧                                                        2015020120150228\",\"description\":\"\",\"resultSignature\":\"b0118895eda59876e27e2b4b98ec2c91\"}";
			"{requestId:2014071378650035,payId:140713200000026292,fiscalDate:20140713,busiOpenedResult:14053898819933018294:201407150955266391:0:019110:收视维护费(基本包)                                              2015071120150810    互动基本包                                                      2014081420140913    戏曲爱好                                                        2014071520140814,description:暂时为空,resultSignature:7c5bebe066b6e210a19ec5b677164b21}";
			System.out.println(readfromgdpayString);			
//第二部分结束
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
	