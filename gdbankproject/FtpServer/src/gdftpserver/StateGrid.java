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
 * @for 国家电网
 */
public class StateGrid {

	public void FileJieXi(ArrayList<BankStatementVo> bankStatementVos,
			ArrayList<String> FileNameList, String TodayLocalPath,
			Map<Integer, String> duizhangjiexi) {

		for (int i = 0; i < FileNameList.size(); i++) {
			try {
				String FileName = FileNameList.get(i);
				MainServer.log.info("开始解析文件：" + TodayLocalPath + FileName);

				if (FileName.substring(0, 6).equals("000000")) { // 判断是不是电力对账文件
					MainServer.log.info("文件：" + FileName + "是电力的对账文件，继续解析");
					File file = new File(TodayLocalPath + FileName);
					InputStreamReader read = new InputStreamReader(
							new FileInputStream(file), "gb2312");
					BufferedReader reader = new BufferedReader(read);

					String HuiZongLineData = reader.readLine(); // 获得汇总行
					MainServer.log.info("获得的汇总行为：" + HuiZongLineData);

					/*
					 * **********汇总行数据含义************ String HuiZongDataSplit[]
					 * 
					 * 
					 * 0----电力资金结算单位 1----资金结算银行 2----资金结算账号 3----电力资金编号
					 * 4----进账单编号 5----渠道商编号 6----银行账务日期 7----总笔数 8----总金额
					 */
					String HuiZongDataSplit[] = HuiZongLineData.split("\\|"); // 对汇总行进行分割
					for(int j= 0; j < HuiZongDataSplit.length;j++){
						MainServer.log.info("分割后的汇总行：" + HuiZongDataSplit[j]);
					}
					

					// 对明细行的处理

					String MingXiLineData = null; // 明细行
					MingXiLineData = reader.readLine();
					MainServer.log.info("获得的明细行为：" + MingXiLineData);
					while (MingXiLineData != null) {// 将所有的明细行分割同时保存
						/*
						 * **********明细行数据含义************ String
						 * MingXiLineDataSplit[]
						 * 
						 * 
						 * 0----交易流水号 1----用户编号 2----交易金额 3----交易时间 4----银行缴费方式
						 * 5----卡表标志 6----电能表标识 7----订单编号 8----记录数 9----数据集
						 */
						String MingXiLineDataSplit[] = MingXiLineData
								.split("\\|");
						for(int j= 0; j < MingXiLineDataSplit.length;j++){
							MainServer.log.info("分割后的明细行：" + MingXiLineDataSplit[j]);
						}
						BankStatementVo bankStatementVo = new BankStatementVo();

						bankStatementVo.setJybs(MingXiLineDataSplit[0]); // 交易标识
						if (MingXiLineDataSplit[5].equals("0")) {
							bankStatementVo
									.setJym(MapYingShe.BankElecTradeTypeMap
											.get("0")); // 交易码
							bankStatementVo.setJymc("缴费"); // 交易名称
						} else if (MingXiLineDataSplit[5].equals("1")) {
							bankStatementVo
									.setJym(MapYingShe.BankElecTradeTypeMap
											.get("1")); // 交易码
							bankStatementVo.setJymc("卡表购电"); // 交易名称
						} else {
							bankStatementVo.setJymc(""); // 交易名称
						}
						bankStatementVo.setKhbs(MingXiLineDataSplit[1]); // 客户标识
						bankStatementVo.setKhfzxx(""); // 客户辅助信息
						bankStatementVo.setKhmc(""); // 客户名称
						bankStatementVo.setJyrq(HuiZongDataSplit[6]); // 交易日期
						bankStatementVo.setJysj(""); // 交易时间
						bankStatementVo.setJyje(Integer
								.parseInt(MingXiLineDataSplit[2])); // 交易金额
						bankStatementVo.setJycgbs(0); // 交易成功标识
						bankStatementVo.setJycgbsxq("成功");// 交易成功标识详情
						bankStatementVo.setYjylsh(""); // 原交易流水号
						bankStatementVo.setJyzt(0); // 交易状态
						bankStatementVo.setJyztxq("成功"); // 交易状态详情
						bankStatementVo.setFhxx(""); // 返回信息
						bankStatementVo.setQybm(MapYingShe.BankElecAreaMap
								.get(HuiZongDataSplit[5])); // 区域编码
						bankStatementVo.setQymc(MapYingShe.BankAreaNoNameMap
								.get(MapYingShe.BankElecAreaMap
										.get(HuiZongDataSplit[5]))); // 区域名称

						// 待定-------****************************************
						bankStatementVo.setJyqdbm(MingXiLineDataSplit[4]); // 交易渠道编码
						bankStatementVo.setJyqdmc(MapYingShe.ElecPayWayMap
								.get(MingXiLineDataSplit[4])); // 交易渠道名称

						bankStatementVo.setJyjg(""); // 交易机构
						bankStatementVo.setJyjgmc(""); // 交易机构名称
						bankStatementVo.setCzyh(""); // 操作员号
						bankStatementVo.setSfdwlbbm("95598"); // 收费单位类别编码
						bankStatementVo.setSfdwlbmc("电费缴费"); // 收费单位类别名称
						bankStatementVo.setSfdwyjjgbm("095598"); // 收费单位一级机构编码
						bankStatementVo.setSfdwyjjgmc("电力"); // 收费单位一级技工名称
						bankStatementVo.setSfdwejjgbm("000"); // 收费单位二级机构编码
						bankStatementVo.setSfdwejjgmc("电力"); // 收费单位二级机构名称
						bankStatementVo.setDzwjm(FileName);// 对账文件名
						bankStatementVo.setSdrq(duizhangjiexi.get(3));// 收到日期
						bankStatementVo.setDzjg(0); // 对账结果
						bankStatementVo.setDzjgxq("未对账"); // 对账结果详情
						bankStatementVo.setBeizhu(""); // 备注
						bankStatementVo.setBaoliu1(""); // 保留1
						bankStatementVo.setBaoliu2(""); // 保留2
						bankStatementVo.setBaoliu3(""); // 保留3
						bankStatementVo.setScsj("");// history表特有

						MainServer.log.info("生成的对象：" + bankStatementVo);
						bankStatementVos.add(bankStatementVo);
						MingXiLineData = reader.readLine();

					}// 将所有的明细行分割保存结束
					MainServer.log.info("函数正常退出");
					reader.close();
				} else {
					MainServer.log.warn("文件：" + FileName + "不是电力的对账文件，停止解析");
				}

			} catch (IOException e) {
				// TODO 自动生成的 catch 块
				e.printStackTrace();
				MainServer.log.error(e);
			}

		}
		return;
	}

	// 取得对账文件 返回对账文件名
	public void GetDuiZhangFile(Map<Integer, String> duizhangjiexi,
			String todaylocalpath, ArrayList<String> allfileArrayList) {
		MainServer.log.info("函数开始执行");
		// System.out.println("localpath: " + todaylocalpath);
		MainServer.log.info("本地保存文件路径为" + todaylocalpath);
		try {
			// 创建今天的文件存储目录
			File fileDis = new File(todaylocalpath);
			boolean falg1 = fileDis.mkdir();
			MainServer.log
					.info((falg1 == true ? "文件夹不存在，创建成功" : "文件夹已存在，不需创建"));

			FtpApche fa = new FtpApche();

			FTPClient ftp = new FTPClient();
			ftp = fa.connecttoftp(MainServer.ftpIP, 21,
					MainServer.ftpusernameString, MainServer.ftppassword);
			ftp.enterLocalPassiveMode();
			// System.out.print(ftp);
			FTPFile fileFtpFiles[] = ftp.listFiles(); // 获得目录下所有文件的详细信息
			MainServer.log.info("远程目录下有" + fileFtpFiles.length + "个文件");
			// MainServer.log.info("银行账务日期为：" + duizhangjiexi.get(15));
			for (int i = 0; i < fileFtpFiles.length; i++) {
				String filenameString = fileFtpFiles[i].getName(); // 获得该文件的文件名
				if (filenameString.substring(0, 6).equals("000000")) { // 判断是不是电力对账文件
					MainServer.log.info("文件：" + filenameString
							+ "是电力的对账文件，继续解析");
					String filenamejiexi[] = filenameString.split("_");
					MainServer.log.info("文件" + filenameString + "文件名解析结果：");
					for (int j = 0; j < filenamejiexi.length; j++) {
						MainServer.log.info(filenamejiexi[j]);
					}
					if (duizhangjiexi.get(15).equals(filenamejiexi[3])) {// 确保报文中的银行帐务日期和文件名中的银行帐务日期一致
						MainServer.log.info("对账文件" + filenameString + "是"
								+ duizhangjiexi.get(15) + "的对账文件，进行下载");
						fa.downFile(MainServer.remotefilepath, filenameString,
								todaylocalpath, ftp); // 将每天收到的对账文件放到不同的目录下
						allfileArrayList.add(filenameString);
					} else {
						MainServer.log.warn("对账文件" + filenameString + "不是"
								+ duizhangjiexi.get(15) + "的对账文件，不进行下载");
					}
				}
				else{
					MainServer.log.warn("文件：" + filenameString + "不是电力的对账文件，停止解析");
				}
			}
			fa.loginout(ftp);// 退出ftp
			MainServer.log.info("函数正常退出");
			return;
		} catch (Exception ex) {
			System.out.print(ex);
			MainServer.log.error(ex);
		} finally {
		}
		return;
	}

}
