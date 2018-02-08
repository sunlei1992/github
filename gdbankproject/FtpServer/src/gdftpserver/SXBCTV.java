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

public class SXBCTV {
	 public static String changeY2F(String amount){    
	        String currency =  amount.replaceAll("\\$|\\��|\\,", "");  //�������, �� ����$�Ľ��    
	        int index = currency.indexOf(".");    
	        int length = currency.length();    
	        Long amLong = 0l;    
	        if(index == -1){    
	            amLong = Long.valueOf(currency+"00");    
	        }else if(length - index >= 3){    
	            amLong = Long.valueOf((currency.substring(0, index+3)).replace(".", ""));    
	        }else if(length - index == 2){    
	            amLong = Long.valueOf((currency.substring(0, index+2)).replace(".", "")+0);    
	        }else{    
	            amLong = Long.valueOf((currency.substring(0, index+1)).replace(".", "")+"00");    
	        }    
	        return amLong.toString();    
	    }
	public void FileJieXi(ArrayList<BankStatementVo> bankStatementVos,
			ArrayList<String> FileNameList, String TodayLocalPath,
			Map<Integer, String> duizhangjiexi) {

		for (int i = 0; i < FileNameList.size(); i++) {
			try {
				String FileName = FileNameList.get(i);
				MainServer.log.info("��ʼ�����ļ���" + TodayLocalPath + FileName);

				if (FileName.substring(0, 4).equals("GDYH")) { // �ж��ǲ��ǹ������ļ�
					MainServer.log.info("�ļ���" + FileName + "�ǹ��Ķ����ļ�����������");
					String filenamejiexi[] = FileName.split("_");
					File file = new File(TodayLocalPath + FileName);
					InputStreamReader read = new InputStreamReader(
							new FileInputStream(file), "utf-8");
					BufferedReader reader = new BufferedReader(read);
					String IParam;
					while((IParam = reader.readLine()) != null){//���һ�����׼�¼�ĵ�һ��
						String PParam = reader.readLine(); //���һ�����׼�¼�ĵڶ���
						String BParam = reader.readLine(); //���һ�����׼�¼�ĵ�����
						reader.readLine(); //����������׼�¼�ָ���
						MainServer.log.info("��õĵ�һ��Ϊ��" + IParam);
						MainServer.log.info("��õĵڶ���Ϊ��" + PParam);
						MainServer.log.info("��õĵ�����Ϊ��" + BParam);
						
						byte IParamBytes[] = IParam.getBytes();
						byte PParamBytes[] = PParam.getBytes();
						byte BParamBytes[] = BParam.getBytes();
						MainServer.log.info("��һ�г���Ϊ��" + IParamBytes.length);
						MainServer.log.info("�ڶ��г���Ϊ��" + PParamBytes.length);
						MainServer.log.info("�����г���Ϊ��" + BParamBytes.length);
						/*
						 * �� һ �У�
						 * 
						 * 			0--������ˮ���(40)
						 * 			1--�ⲿϵͳ��ʶ(20)
						 * 			2--ҵ������(50)
						 * 			3--�����ܼ۸�(9)
						 * 			4--��Ʒ����(20)
						 * 			5--�û���������(20)
						 * 			6--��������(3)
						 * 
						 */
						String[] IParamSplit = new String [7];
						IParamSplit[0] = new String(IParamBytes, 0, 40, "gb2312").trim();
						IParamSplit[1] = new String(IParamBytes,40,20, "gb2312").trim();
						IParamSplit[2] = new String(IParamBytes,60,50, "gb2312").trim();
						IParamSplit[3] = new String(IParamBytes,110,9, "gb2312").trim();
						IParamSplit[4] = new String(IParamBytes,119,20, "gb2312").trim();
						IParamSplit[5] = new String(IParamBytes,139,20, "gb2312").trim();
						IParamSplit[6] = new String(IParamBytes,159,3, "gb2312").trim();
						
						for(int j= 0; j < IParamSplit.length;j++){
							MainServer.log.info("�ָ��ĵ�һ�У�" + IParamSplit[j]);
						}
						
						
						/*
						 * �� �� �У�
						 * 			 
						 * 
						 * 			0--��Ʒ����(20)
						 * 			1--��Ʒ����(50)
						 * 			2--��Ʒ����(9)
						 * 			3--��Ӧ�̱���(20)
						 * 			4--��Ʒҵ������(50)
						 * 			5--��Ʒ��������(3)
						 * 
						 */
						
						
						String[] PParamSplit = new String [6];
						PParamSplit[0] = new String(PParamBytes,0,20, "gb2312").trim();
						PParamSplit[1] = new String(PParamBytes,20,50, "gb2312").trim();
						PParamSplit[2] = new String(PParamBytes,70,9, "gb2312").trim();
						PParamSplit[3] = new String(PParamBytes,79,20, "gb2312").trim();
						PParamSplit[4] = new String(PParamBytes,99,50, "gb2312").trim();
						PParamSplit[5] = new String(PParamBytes,149,3, "gb2312").trim();
						
						for(int j= 0; j < PParamSplit.length;j++){
							MainServer.log.info("�ָ��ĵڶ��У�" + PParamSplit[j]);
						}
						/*
						 * �� �� �У�
						 * 
						 * 
						 * 			0--ҵ��������ˮ���(20)
						 * 			1--ҵ���״���(6)
						 * 			2--����ҵ������(1)
						 * 			3--�ƷѶ���(16)
						 * 			4--ҵ���ײʹ���(6)
						 * 			5--�����б��(50)
						 * 			6--CA����(50)
						 * 			7--��ƷCAID(8)
						 * 			8--����ҵ������(20)
						 * 			9--ҵ���Ʒ����(20)
						 * 			10--ҵ���Ʒ����(50)
						 * 			11--ҵ���Ʒ����(9)
						 * 			12--ҵ���Ʒ��Ӧ�̱���(20)
						 * 			13--��Ʒҵ������(50)
						 * 			14--��Ʒ��������(3)
						 * 			15--��Ʒ��ͨ����(29)
						 * 
						 */
						
						String[] BParamSplit = new String [16];
						BParamSplit[0] = new String(BParamBytes,0,20, "gb2312").trim();
						BParamSplit[1] = new String(BParamBytes,20,6, "gb2312").trim();
						BParamSplit[2] = new String(BParamBytes,26,1, "gb2312").trim();
						BParamSplit[3] = new String(BParamBytes,27,16, "gb2312").trim();
						BParamSplit[4] = new String(BParamBytes,43,6, "gb2312").trim();
						BParamSplit[5] = new String(BParamBytes,49,50, "gb2312").trim();
						BParamSplit[6] = new String(BParamBytes,99,50, "gb2312").trim();
						BParamSplit[7] = new String(BParamBytes,149,8, "gb2312").trim();
						BParamSplit[8] = new String(BParamBytes,157,20, "gb2312").trim();
						BParamSplit[9] = new String(BParamBytes,177,20, "gb2312").trim();
						BParamSplit[10] = new String(BParamBytes,197,50, "gb2312").trim();
						BParamSplit[11] = new String(BParamBytes,247,9, "gb2312").trim();
						BParamSplit[12] = new String(BParamBytes,256,20, "gb2312").trim();
						BParamSplit[13] = new String(BParamBytes,276,50, "gb2312").trim();
						BParamSplit[14] = new String(BParamBytes,326,3, "gb2312").trim();
						BParamSplit[15] = new String(BParamBytes,329,29, "gb2312").trim();
						
						for(int j= 0; j < BParamSplit.length;j++){
							MainServer.log.info("�ָ��ĵ����У�" + BParamSplit[j]);
						}
						
						
						
						BankStatementVo bankStatementVo = new BankStatementVo();

						bankStatementVo.setJybs(IParamSplit[0]); // ���ױ�ʶ
						
						bankStatementVo.setJym("0001"); // ������
						bankStatementVo.setJymc("�ɷ�"); // ��������						
						bankStatementVo.setKhbs(BParamSplit[3]); // �ͻ���ʶ
						bankStatementVo.setKhfzxx(""); // �ͻ�������Ϣ
						bankStatementVo.setKhmc(""); // �ͻ�����
						bankStatementVo.setJyrq(filenamejiexi[1].substring(0, 8)); // ��������
						bankStatementVo.setJysj(""); // ����ʱ��
						bankStatementVo.setJyje(Integer
								.parseInt(changeY2F(IParamSplit[3]))); // ���׽��
						bankStatementVo.setJycgbs(0); // ���׳ɹ���ʶ
						bankStatementVo.setJycgbsxq("�ɹ�");// ���׳ɹ���ʶ����
						bankStatementVo.setYjylsh(""); // ԭ������ˮ��
						bankStatementVo.setJyzt(0); // ����״̬
						bankStatementVo.setJyztxq("�ɹ�"); // ����״̬����
						bankStatementVo.setFhxx(""); // ������Ϣ
						bankStatementVo.setQybm(MapYingShe.BCTVAreaNoNoMap
								.get(IParamSplit[5])); // �������
						bankStatementVo.setQymc(MapYingShe.BankAreaNoNameMap
								.get(MapYingShe.BCTVAreaNoNoMap
										.get(IParamSplit[5]))); // ��������

						// ����-------****************************************
						bankStatementVo.setJyqdbm(""); // ������������
						bankStatementVo.setJyqdmc(""); // ������������

						bankStatementVo.setJyjg(""); // ���׻���
						bankStatementVo.setJyjgmc(""); // ���׻�������
						bankStatementVo.setCzyh(""); // ����Ա��
						bankStatementVo.setSfdwlbbm("96766"); // �շѵ�λ������
						bankStatementVo.setSfdwlbmc("���ɷ�"); // �շѵ�λ�������
						bankStatementVo.setSfdwyjjgbm("096766"); // �շѵ�λһ����������
						bankStatementVo.setSfdwyjjgmc("�����������"); // �շѵ�λһ����������
						bankStatementVo.setSfdwejjgbm(""); // �շѵ�λ������������
						bankStatementVo.setSfdwejjgmc(""); // �շѵ�λ������������
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
						
					}
					MainServer.log.info("���������˳�");
					reader.close();
				} else {
					MainServer.log.warn("�ļ���" + FileName + "���ǹ��Ķ����ļ���ֹͣ����");
				}

			} catch (Exception e) {
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
				if (filenameString.substring(0, 4).equals("GDYH")) { // �ж��ǲ��ǹ������ļ�
					MainServer.log.info("�ļ���" + filenameString
							+ "�ǹ��Ķ����ļ�����������");
					String filenamejiexi[] = filenameString.split("_");
					MainServer.log.info("�ļ�" + filenameString + "�ļ������������");
					for (int j = 0; j < filenamejiexi.length; j++) {
						MainServer.log.info(filenamejiexi[j]);
					}
					if (duizhangjiexi.get(15).equals(filenamejiexi[1].substring(0, 8))) {// ȷ�������е������������ں��ļ����е�����һ��
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
					MainServer.log.warn("�ļ���" + filenameString + "���ǹ��Ķ����ļ���ֹͣ����");
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
