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

	// �����ж�ȡ����
	public String ReadDataFromBank() {
		String readfrombank = null;
		try {
			log.info("������ʼִ��");
			BufferedReader in = new BufferedReader(new InputStreamReader(
					client.getInputStream()));
			// Mutil User but can't parallel

			// ��ȡ�ַ���
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
			log.info("��ñ���ͷ" + readfrombank);
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
			log.info("��õ���������" + "[" + readfrombank.length() + "]"
					+ readfrombank);
			log.info("���������˳���");
			return readfrombank;
		} catch (Exception ex) {
			System.out.print(ex);
			 MainServer.log.error(ex);
		} finally {

		}
		// ��ȡ�ַ�������
		return readfrombank;

	}

	// �Ի�õ����ݽ��н���
	public void DuiZhangDataJieXi(String readfrombank,Map<Integer, String> duizhangjiexi) {
			// �����ַ���
			log.info("������ʼ����");
			for (int i = 0; i < MainServer.duizhangConfigure.length; i++) {
				duizhangjiexi.put(i, readfrombank.substring(
						duizhangConfigure[i].offset, duizhangConfigure[i].offset
								+ duizhangConfigure[i].length));
			}
			for (int i = 0; i < duizhangjiexi.size(); i++) {
				//System.out.println(duizhangjiexi.get(i).length() + "  "
				//		+ duizhangjiexi.get(i));
				log.info("������������" + "[" + i + "][" + duizhangjiexi.get(i).length()
						+ "][" + duizhangjiexi.get(i)+"]");
			}// �����ַ�������
			log.info("���������˳�");
		}
		// �����лظ�����
	public void SendDataToBank(String yingdama,Map<Integer, String> duizhangjiexi) {
		log.info("������ʼ����");
		try {
			PrintWriter out = new PrintWriter(client.getOutputStream());
			String sendtobank = "0224";
			sendtobank += duizhangjiexi.get(2);// �������ˮ��
			sendtobank += yingdama;
			/*
			 * if(yingdama.equals("00  ")){ sendtobank+="�ɹ�"; } else {
			 * sendtobank+="ʧ��"; }
			 */
			for (int i = 0; i < 60; i++) {
				sendtobank += " ";
			}
			sendtobank += duizhangjiexi.get(5);// ��������
			for (int i = 0; i < 136; i++) {
				sendtobank += " ";
			}
			out.write(sendtobank, 0, sendtobank.length());
			out.flush();
			client.close();
			log.info("���͵�����Ϊ:[" + sendtobank.length() + "]" + sendtobank);
			log.info("���������˳�");
		} catch (Exception ex) {
			System.out.print(ex);
			 MainServer.log.error(ex);
		} finally {

		}

	}

	public void DeleteOldData(Map<Integer, String> duizhangjiexi) {
		log.info("������ʼִ��");
		TableManage tableManage = new TableManage();
		tableManage.LogTableReturn(duizhangjiexi.get(15));

		tableManage.DeleteOldData(duizhangjiexi.get(15));

		log.info("���������˳�");
	}
	
	// ���ж���
	public void DuiZhangAction(ArrayList<LogVo> logrecordlist,
			ArrayList<BankStatementVo> bankStatementVos) {
		log.info("������ʼִ��");
		int i, j;
		for (i = 0; i < bankStatementVos.size(); i++) {// �����ļ���ʼѭ��
			BankStatementVo bankStatementVo = new BankStatementVo();
			bankStatementVo = bankStatementVos.get(i); 
			for (j = 0; j < logrecordlist.size(); j++) {// ��־��¼��ʼѭ��
				LogVo logVo = new LogVo();
				logVo = logrecordlist.get(j);
				//����������ʱ�����ȶ�
				if (bankStatementVo.getJybs().equals(logVo.getJybs())) {// ��־���ҵ��ü�¼
					if (bankStatementVo.getKhbs().equals(logVo.getKhbs()) // �ͻ���ʶ
							&& bankStatementVo.getJym().equals(logVo.getJym())  //������
							&& bankStatementVo.getJyje() == logVo.getJyje() // ���׽��
							&& bankStatementVo.getQybm().equals(logVo.getQybm()) //�������
							&& bankStatementVo.getSfdwlbbm().equals(logVo.getSfdwlbbm())) { // �շѵ�λ������
						bankStatementVo.setDzjg(1);
						bankStatementVo.setDzjgxq("���˳ɹ�");
						logVo.setDzjg(1);
						logVo.setDzjgxq("���˳ɹ�");						
					} else {// ����ʧ�ܣ���¼�г�ͻ
						bankStatementVo.setDzjg(2);
						bankStatementVo.setDzjgxq("��¼�г�ͻ");
						logVo.setDzjg(2);
						logVo.setDzjgxq("��¼�г�ͻ");
					}
					break;
				}
			}// ��־��¼ѭ������
			if (j == logrecordlist.size()) {
				bankStatementVo.setDzjg(3);
				bankStatementVo.setDzjgxq("��־�в����ڱ������˼�¼");
			}
			log.info(bankStatementVo.getJybs()+bankStatementVo.getDzjgxq());
		}// ��ϸ��¼ѭ������
		
		
		// ����log��û�ж��˵Ľ��д���
		for (i = 0; i < logrecordlist.size(); i++) {
			if (logrecordlist.get(i).getDzjg() == 0) {
				logrecordlist.get(i).setDzjg(3);
				logrecordlist.get(i).setDzjgxq("�����ļ��в����ڱ���¼");
				log.info(logrecordlist.get(i).getJybs()+logrecordlist.get(i).getDzjgxq());
			}
			
		}

		log.info("���������˳�");

	}
	
	// ���������
	public void InsertDataIntoDatabase(ArrayList<LogVo> logrecordlist,
			ArrayList<BankStatementVo> bankStatementVos) {
		log.info("������ʼִ��");
		TableManage tableManage = new TableManage();
		tableManage.LogTableUpdate(logrecordlist);
		tableManage.InsertBankStatementTable(bankStatementVos);
		log.info("���������˳�");

	}

	

	public void run() {
		String TodayLocalPath;   //�������Ķ����ļ�����·�����������ļ���
		Map<Integer, String> DuiZhangJieXi = new HashMap<Integer, String>();  //����������ݱ��������
		ArrayList<BankStatementVo> bankStatementVos = new ArrayList<BankStatementVo>();
		ArrayList<String> AllFileNameList = new ArrayList<String>();
		TableManage tableManage = new TableManage();
		
			
		StateGrid stateGrid = new StateGrid();  //���ҵ���
		SXBCTV	sxbctv = new SXBCTV();			//�����������
		try {
			log.info("�߳̿�ʼ����");			
			String ReadFromBank = ReadDataFromBank(); //��ȡ����
			DuiZhangDataJieXi(ReadFromBank,DuiZhangJieXi);//��������
			TodayLocalPath = localfilepath + DuiZhangJieXi.get(3) + "\\";
			ArrayList<LogVo> logrecordlist = tableManage
					.LogTableSelectByDate(DuiZhangJieXi.get(15)); // ȡ��log���н�������Ϊ�������������ڵļ�¼,�Ҹý��׵Ľ��׳ɹ���ʾ�ǳɹ�������ʧ�ܺͳ�����
			stateGrid.GetDuiZhangFile(DuiZhangJieXi,TodayLocalPath,AllFileNameList); //��ȡ���������ļ������ض����ļ���
			sxbctv.GetDuiZhangFile(DuiZhangJieXi, TodayLocalPath, AllFileNameList);//��ȡ�������ļ������ض����ļ���
			// �����з�������
			if (AllFileNameList.size() == 0) {
				SendDataToBank("99  ",DuiZhangJieXi);
				return;
			} else {
				SendDataToBank("00  ",DuiZhangJieXi);
			}		
			stateGrid.FileJieXi(bankStatementVos, AllFileNameList, TodayLocalPath,DuiZhangJieXi);  //���������ļ�����
			sxbctv.FileJieXi(bankStatementVos, AllFileNameList, TodayLocalPath,DuiZhangJieXi);  //�������ļ�����
			// ɾ���ɵ�����
			DeleteOldData(DuiZhangJieXi);
			//���ж���
			DuiZhangAction(logrecordlist, bankStatementVos);
			//�������
			InsertDataIntoDatabase(logrecordlist, bankStatementVos);
			

			log.info("�߳������˳�");

		} catch (Exception ex) {
			// System.out.print(ex);
			 MainServer.log.error(ex);
		} finally {
		}
	}

	public static void main(String[] args) throws IOException {
		PropertyConfigurator.configure("log4j.properties");
		log.info("����ʼ����!");
		ServerSocket server = new ServerSocket(5678);
		while (true) {
			// transfer location change Single User or Multi User
			Socket newsocket = server.accept();
			MainServer mc = new MainServer(newsocket);
			log.info(newsocket.getRemoteSocketAddress() + "�������ӣ��ѽ���!");
			mc.start();
		}
	}
}