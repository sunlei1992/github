package gdftpserver;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.sql.Date;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.HashMap;
import java.util.Map;
import java.util.TimeZone;

import org.apache.commons.net.ftp.FTPClient;
import org.apache.commons.net.ftp.FTPFile;
import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;

public class MainServer extends Thread {
	public MainServer(Socket c) {
		this.client = c;
	}

	public static final Logger log = Logger.getLogger(MainServer.class);
	public static String ftpIP = "192.168.1.119";
	public static String ftpusernameString = "xidian";
	public static String ftppassword = "88203643";
	public static String localfilepath = "D:\\localftp\\";
	public static String remotefilepath = "D:\\remoteftp\\";
	
	private Socket client;

	/* duizhangjiaoyipeizhi */
	public static InterfaceConfigure duizhangConfigure[] = {
			new InterfaceConfigure(4, 0), new InterfaceConfigure(8, 4),
			new InterfaceConfigure(20, 12), new InterfaceConfigure(8, 32),
			new InterfaceConfigure(6, 40), new InterfaceConfigure(4, 46),
			new InterfaceConfigure(12, 50), new InterfaceConfigure(8, 62),
			new InterfaceConfigure(8, 70), new InterfaceConfigure(12, 78),
			new InterfaceConfigure(12, 90), new InterfaceConfigure(40, 102),
			new InterfaceConfigure(40, 142), new InterfaceConfigure(8, 182),
			new InterfaceConfigure(20, 190), new InterfaceConfigure(8, 210),
			new InterfaceConfigure(40, 218), new InterfaceConfigure(40, 258),
			new InterfaceConfigure(256, 298) };

	// 从银行读取数据
	public String ReadDataFromBank() {
		String readfrombank = null;
		try {
			log.info("函数开始执行");
			BufferedReader in = new BufferedReader(new InputStreamReader(
					client.getInputStream()));
			// Mutil User but can't parallel

			// 读取字符串
			char readfrombanktemp[] = new char[4];

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
			baotoulengthString = String.valueOf(readfrombanktemp);
			readfrombank = String.valueOf(readfrombanktemp);
			log.info("获得报文头" + readfrombank);
			// System.out.println("length:"+baotoulengthString.length());
			// System.out.println(readfrombank);
			// System.out.println(baotoulengthString.replaceFirst("^0*", ""));
			int baotoulengint = Integer.parseInt(baotoulengthString
					.replaceFirst("^0*", ""));
			// System.out.println(readfrombank);
			//System.out.println(baotoulengint);
			readlength = 0;
			sumreadlength = 0;
			char readfrombanktemp1[] = new char[baotoulengint];
			while (sumreadlength != baotoulengint) {
				readlength = in.read(readfrombanktemp1, readlength,
						baotoulengint - sumreadlength);
				sumreadlength += readlength;
			}
			readfrombank += String.valueOf(readfrombanktemp1);
		//	System.out.println("[" + readfrombank.length() + "]" + readfrombank);
			log.info("获得的整个报文" + "[" + readfrombank.length() + "]"
					+ readfrombank);
			log.info("函数正常退出！");
			return readfrombank;
		} catch (Exception ex) {
			System.out.print(ex);
			 MainServer.log.error(ex);
		} finally {

		}
		// 读取字符串结束
		return readfrombank;

	}

	// 对获得的数据进行解析
	public void DuiZhangDataJieXi(String readfrombank,Map<Integer, String> duizhangjiexi) {
			// 解析字符串
			log.info("函数开始运行");
			for (int i = 0; i < MainServer.duizhangConfigure.length; i++) {
				duizhangjiexi.put(i, readfrombank.substring(
						duizhangConfigure[i].offset, duizhangConfigure[i].offset
								+ duizhangConfigure[i].length));
			}
			for (int i = 0; i < duizhangjiexi.size(); i++) {
				//System.out.println(duizhangjiexi.get(i).length() + "  "
				//		+ duizhangjiexi.get(i));
				log.info("解析到的数据" + "[" + i + "][" + duizhangjiexi.get(i).length()
						+ "][" + duizhangjiexi.get(i)+"]");
			}// 解析字符串结束
			log.info("函数正常退出");
		}
		// 向银行回复数据
	public void SendDataToBank(String yingdama,Map<Integer, String> duizhangjiexi) {
		log.info("函数开始运行");
		try {
			PrintWriter out = new PrintWriter(client.getOutputStream());
			String sendtobank = "0224";
			sendtobank += duizhangjiexi.get(2);// 请求包流水号
			sendtobank += yingdama;
			/*
			 * if(yingdama.equals("00  ")){ sendtobank+="成功"; } else {
			 * sendtobank+="失败"; }
			 */
			for (int i = 0; i < 60; i++) {
				sendtobank += " ";
			}
			sendtobank += duizhangjiexi.get(5);// 地区代码
			for (int i = 0; i < 136; i++) {
				sendtobank += " ";
			}
			out.write(sendtobank, 0, sendtobank.length());
			out.flush();
			client.close();
			log.info("发送的数据为:[" + sendtobank.length() + "]" + sendtobank);
			log.info("函数正常退出");
		} catch (Exception ex) {
			System.out.print(ex);
			 MainServer.log.error(ex);
		} finally {

		}

	}

	public void DeleteOldData(Map<Integer, String> duizhangjiexi) {
		log.info("函数开始执行");
		TableManage tableManage = new TableManage();
		tableManage.LogTableReturn(duizhangjiexi.get(15));

		tableManage.DeleteOldData(duizhangjiexi.get(15));

		log.info("函数正常退出");
	}
	
	// 进行对账
	public void DuiZhangAction(ArrayList<LogVo> logrecordlist,
			ArrayList<BankStatementVo> bankStatementVos) {
		log.info("函数开始执行");
		int i, j;
		for (i = 0; i < bankStatementVos.size(); i++) {// 对账文件开始循环
			BankStatementVo bankStatementVo = new BankStatementVo();
			bankStatementVo = bankStatementVos.get(i); 
			for (j = 0; j < logrecordlist.size(); j++) {// 日志记录开始循环
				LogVo logVo = new LogVo();
				logVo = logrecordlist.get(j);
				//交易渠道暂时不做比对
				if (bankStatementVo.getJybs().equals(logVo.getJybs())) {// 日志中找到该记录
					if (bankStatementVo.getKhbs().equals(logVo.getKhbs()) // 客户标识
							&& bankStatementVo.getJym().equals(logVo.getJym())  //交易码
							&& bankStatementVo.getJyje() == logVo.getJyje() // 交易金额
							&& bankStatementVo.getQybm().equals(logVo.getQybm()) //区域编码
							&& bankStatementVo.getSfdwlbbm().equals(logVo.getSfdwlbbm())) { // 收费单位类别编码
						bankStatementVo.setDzjg(1);
						bankStatementVo.setDzjgxq("对账成功");
						logVo.setDzjg(1);
						logVo.setDzjgxq("对账成功");						
					} else {// 对账失败，记录有冲突
						bankStatementVo.setDzjg(2);
						bankStatementVo.setDzjgxq("记录有冲突");
						logVo.setDzjg(2);
						logVo.setDzjgxq("记录有冲突");
					}
					break;
				}
			}// 日志记录循环结束
			if (j == logrecordlist.size()) {
				bankStatementVo.setDzjg(3);
				bankStatementVo.setDzjgxq("日志中不存在本条对账记录");
			}
			log.info(bankStatementVo.getJybs()+bankStatementVo.getDzjgxq());
		}// 明细记录循环结束
		
		
		// 最后对log中没有对账的进行处理
		for (i = 0; i < logrecordlist.size(); i++) {
			if (logrecordlist.get(i).getDzjg() == 0) {
				logrecordlist.get(i).setDzjg(3);
				logrecordlist.get(i).setDzjgxq("对账文件中不存在本记录");
				log.info(logrecordlist.get(i).getJybs()+logrecordlist.get(i).getDzjgxq());
			}
			
		}

		log.info("函数正常退出");

	}
	
	// 将数据入库
	public void InsertDataIntoDatabase(ArrayList<LogVo> logrecordlist,
			ArrayList<BankStatementVo> bankStatementVos) {
		log.info("函数开始执行");
		TableManage tableManage = new TableManage();
		tableManage.LogTableUpdate(logrecordlist);
		tableManage.InsertBankStatementTable(bankStatementVos);
		log.info("函数正常退出");

	}

	

	public void run() {
		String TodayLocalPath;   //保存今天的对账文件保存路径，不包括文件名
		Map<Integer, String> DuiZhangJieXi = new HashMap<Integer, String>();  //保存对账数据报解析结果
		ArrayList<BankStatementVo> bankStatementVos = new ArrayList<BankStatementVo>();
		ArrayList<String> AllFileNameList = new ArrayList<String>();
		TableManage tableManage = new TableManage();
		
			
		StateGrid stateGrid = new StateGrid();  //国家电网
		SXBCTV	sxbctv = new SXBCTV();			//陕西广电网络
		try {
			log.info("线程开始运行");			
			String ReadFromBank = ReadDataFromBank(); //读取数据
			DuiZhangDataJieXi(ReadFromBank,DuiZhangJieXi);//解析数据
			TodayLocalPath = localfilepath + DuiZhangJieXi.get(3) + "\\";
			ArrayList<LogVo> logrecordlist = tableManage
					.LogTableSelectByDate(DuiZhangJieXi.get(15)); // 取得log表中交易日期为该银行账务日期的记录,且该交易的交易成功标示是成功（不是失败和撤销）
			stateGrid.GetDuiZhangFile(DuiZhangJieXi,TodayLocalPath,AllFileNameList); //获取电力对账文件，返回对账文件名
			sxbctv.GetDuiZhangFile(DuiZhangJieXi, TodayLocalPath, AllFileNameList);//获取广电对账文件，返回对账文件名
			// 向银行返回数据
			if (AllFileNameList.size() == 0) {
				SendDataToBank("99  ",DuiZhangJieXi);
				return;
			} else {
				SendDataToBank("00  ",DuiZhangJieXi);
			}		
			stateGrid.FileJieXi(bankStatementVos, AllFileNameList, TodayLocalPath,DuiZhangJieXi);  //电力对账文件解析
			sxbctv.FileJieXi(bankStatementVos, AllFileNameList, TodayLocalPath,DuiZhangJieXi);  //广电对账文件解析
			// 删除旧的数据
			DeleteOldData(DuiZhangJieXi);
			//进行对账
			DuiZhangAction(logrecordlist, bankStatementVos);
			//数据入库
			InsertDataIntoDatabase(logrecordlist, bankStatementVos);
			

			log.info("线程正常退出");

		} catch (Exception ex) {
			// System.out.print(ex);
			 MainServer.log.error(ex);
		} finally {
		}
	}

	public static void main(String[] args) throws IOException {
		PropertyConfigurator.configure("log4j.properties");
		log.info("程序开始运行!");
		ServerSocket server = new ServerSocket(5678);
		while (true) {
			// transfer location change Single User or Multi User
			Socket newsocket = server.accept();
			MainServer mc = new MainServer(newsocket);
			log.info(newsocket.getRemoteSocketAddress() + "发起连接，已接受!");
			mc.start();
		}
	}
}