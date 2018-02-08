/**
 * 
 */
package gdftpserver;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Map;

import org.apache.commons.net.ftp.FTPClient;
import org.apache.commons.net.ftp.FTPFile;

/**
 * @author skyfly
 * 
 * @for ���ҵ���
 */
public class StateGrid {

	public void FileJieXi(ArrayList<BankStatementVo> bankStatementVos,
			ArrayList<String> FileNameList, String TodayLocalPath,
			Map<Integer, String> duizhangjiexi) {

		for (int i = 0; i < FileNameList.size(); i++) {
			try {
				String FileName = FileNameList.get(i);
				MainServer.log.info("��ʼ�����ļ���" + TodayLocalPath + FileName);

				if (FileName.substring(0, 6).equals("000000")) { // �ж��ǲ��ǵ��������ļ�
					MainServer.log.info("�ļ���" + FileName + "�ǵ����Ķ����ļ�����������");
					File file = new File(TodayLocalPath + FileName);
					InputStreamReader read = new InputStreamReader(
							new FileInputStream(file), "gb2312");
					BufferedReader reader = new BufferedReader(read);

					String HuiZongLineData = reader.readLine(); // ��û�����
					MainServer.log.info("��õĻ�����Ϊ��" + HuiZongLineData);

					/*
					 * **********���������ݺ���************ String HuiZongDataSplit[]
					 * 
					 * 
					 * 0----�����ʽ���㵥λ 1----�ʽ�������� 2----�ʽ�����˺� 3----�����ʽ���
					 * 4----���˵���� 5----�����̱�� 6----������������ 7----�ܱ��� 8----�ܽ��
					 */
					String HuiZongDataSplit[] = HuiZongLineData.split("\\|"); // �Ի����н��зָ�
					for(int j= 0; j < HuiZongDataSplit.length;j++){
						MainServer.log.info("�ָ��Ļ����У�" + HuiZongDataSplit[j]);
					}
					

					// ����ϸ�еĴ���

					String MingXiLineData = null; // ��ϸ��
					MingXiLineData = reader.readLine();
					MainServer.log.info("��õ���ϸ��Ϊ��" + MingXiLineData);
					while (MingXiLineData != null) {// �����е���ϸ�зָ�ͬʱ����
						/*
						 * **********��ϸ�����ݺ���************ String
						 * MingXiLineDataSplit[]
						 * 
						 * 
						 * 0----������ˮ�� 1----�û���� 2----���׽�� 3----����ʱ�� 4----���нɷѷ�ʽ
						 * 5----�����־ 6----���ܱ��ʶ 7----������� 8----��¼�� 9----���ݼ�
						 */
						String MingXiLineDataSplit[] = MingXiLineData
								.split("\\|");
						for(int j= 0; j < MingXiLineDataSplit.length;j++){
							MainServer.log.info("�ָ�����ϸ�У�" + MingXiLineDataSplit[j]);
						}
						BankStatementVo bankStatementVo = new BankStatementVo();

						bankStatementVo.setJybs(MingXiLineDataSplit[0]); // ���ױ�ʶ
						if (MingXiLineDataSplit[5].equals("0")) {
							bankStatementVo
									.setJym(MapYingShe.BankElecTradeTypeMap
											.get("0")); // ������
							bankStatementVo.setJymc("�ɷ�"); // ��������
						} else if (MingXiLineDataSplit[5].equals("1")) {
							bankStatementVo
									.setJym(MapYingShe.BankElecTradeTypeMap
											.get("1")); // ������
							bankStatementVo.setJymc("������"); // ��������
						} else {
							bankStatementVo.setJymc(""); // ��������
						}
						bankStatementVo.setKhbs(MingXiLineDataSplit[1]); // �ͻ���ʶ
						bankStatementVo.setKhfzxx(""); // �ͻ�������Ϣ
						bankStatementVo.setKhmc(""); // �ͻ�����
						bankStatementVo.setJyrq(HuiZongDataSplit[6]); // ��������
						bankStatementVo.setJysj(""); // ����ʱ��
						bankStatementVo.setJyje(Integer
								.parseInt(MingXiLineDataSplit[2])); // ���׽��
						bankStatementVo.setJycgbs(0); // ���׳ɹ���ʶ
						bankStatementVo.setJycgbsxq("�ɹ�");// ���׳ɹ���ʶ����
						bankStatementVo.setYjylsh(""); // ԭ������ˮ��
						bankStatementVo.setJyzt(0); // ����״̬
						bankStatementVo.setJyztxq("�ɹ�"); // ����״̬����
						bankStatementVo.setFhxx(""); // ������Ϣ
						bankStatementVo.setQybm(MapYingShe.BankElecAreaMap
								.get(HuiZongDataSplit[5])); // �������
						bankStatementVo.setQymc(MapYingShe.BankAreaNoNameMap
								.get(MapYingShe.BankElecAreaMap
										.get(HuiZongDataSplit[5]))); // ��������

						// ����-------****************************************
						bankStatementVo.setJyqdbm(MingXiLineDataSplit[4]); // ������������
						bankStatementVo.setJyqdmc(MapYingShe.ElecPayWayMap
								.get(MingXiLineDataSplit[4])); // ������������

						bankStatementVo.setJyjg(""); // ���׻���
						bankStatementVo.setJyjgmc(""); // ���׻�������
						bankStatementVo.setCzyh(""); // ����Ա��
						bankStatementVo.setSfdwlbbm("95598"); // �շѵ�λ������
						bankStatementVo.setSfdwlbmc("��ѽɷ�"); // �շѵ�λ�������
						bankStatementVo.setSfdwyjjgbm("095598"); // �շѵ�λһ����������
						bankStatementVo.setSfdwyjjgmc("����"); // �շѵ�λһ����������
						bankStatementVo.setSfdwejjgbm("000"); // �շѵ�λ������������
						bankStatementVo.setSfdwejjgmc("����"); // �շѵ�λ������������
						bankStatementVo.setDzwjm(FileName);// �����ļ���
						bankStatementVo.setSdrq(duizhangjiexi.get(3));// �յ�����
						bankStatementVo.setDzjg(0); // ���˽��
						bankStatementVo.setDzjgxq("δ����"); // ���˽������
						bankStatementVo.setBeizhu(""); // ��ע
						bankStatementVo.setBaoliu1(""); // ����1
						bankStatementVo.setBaoliu2(""); // ����2
						bankStatementVo.setBaoliu3(""); // ����3
						bankStatementVo.setScsj("");// history������

						MainServer.log.info("���ɵĶ���" + bankStatementVo);
						bankStatementVos.add(bankStatementVo);
						MingXiLineData = reader.readLine();

					}// �����е���ϸ�зָ�����
					MainServer.log.info("���������˳�");
					reader.close();
				} else {
					MainServer.log.warn("�ļ���" + FileName + "���ǵ����Ķ����ļ���ֹͣ����");
				}

			} catch (IOException e) {
				// TODO �Զ����ɵ� catch ��
				e.printStackTrace();
				MainServer.log.error(e);
			}

		}
		return;
	}

	// ȡ�ö����ļ� ���ض����ļ���
	public void GetDuiZhangFile(Map<Integer, String> duizhangjiexi,
			String todaylocalpath, ArrayList<String> allfileArrayList) {
		MainServer.log.info("������ʼִ��");
		// System.out.println("localpath: " + todaylocalpath);
		MainServer.log.info("���ر����ļ�·��Ϊ" + todaylocalpath);
		try {
			// ����������ļ��洢Ŀ¼
			File fileDis = new File(todaylocalpath);
			boolean falg1 = fileDis.mkdir();
			MainServer.log
					.info((falg1 == true ? "�ļ��в����ڣ������ɹ�" : "�ļ����Ѵ��ڣ����贴��"));

			FtpApche fa = new FtpApche();

			FTPClient ftp = new FTPClient();
			ftp = fa.connecttoftp(MainServer.ftpIP, 21,
					MainServer.ftpusernameString, MainServer.ftppassword);
			ftp.enterLocalPassiveMode();
			// System.out.print(ftp);
			FTPFile fileFtpFiles[] = ftp.listFiles(); // ���Ŀ¼�������ļ�����ϸ��Ϣ
			MainServer.log.info("Զ��Ŀ¼����" + fileFtpFiles.length + "���ļ�");
			// MainServer.log.info("������������Ϊ��" + duizhangjiexi.get(15));
			for (int i = 0; i < fileFtpFiles.length; i++) {
				String filenameString = fileFtpFiles[i].getName(); // ��ø��ļ����ļ���
				if (filenameString.substring(0, 6).equals("000000")) { // �ж��ǲ��ǵ��������ļ�
					MainServer.log.info("�ļ���" + filenameString
							+ "�ǵ����Ķ����ļ�����������");
					String filenamejiexi[] = filenameString.split("_");
					MainServer.log.info("�ļ�" + filenameString + "�ļ������������");
					for (int j = 0; j < filenamejiexi.length; j++) {
						MainServer.log.info(filenamejiexi[j]);
					}
					if (duizhangjiexi.get(15).equals(filenamejiexi[3])) {// ȷ�������е������������ں��ļ����е�������������һ��
						MainServer.log.info("�����ļ�" + filenameString + "��"
								+ duizhangjiexi.get(15) + "�Ķ����ļ�����������");
						fa.downFile(MainServer.remotefilepath, filenameString,
								todaylocalpath, ftp); // ��ÿ���յ��Ķ����ļ��ŵ���ͬ��Ŀ¼��
						allfileArrayList.add(filenameString);
					} else {
						MainServer.log.warn("�����ļ�" + filenameString + "����"
								+ duizhangjiexi.get(15) + "�Ķ����ļ�������������");
					}
				}
				else{
					MainServer.log.warn("�ļ���" + filenameString + "���ǵ����Ķ����ļ���ֹͣ����");
				}
			}
			fa.loginout(ftp);// �˳�ftp
			MainServer.log.info("���������˳�");
			return;
		} catch (Exception ex) {
			System.out.print(ex);
			MainServer.log.error(ex);
		} finally {
		}
		return;
	}

}
