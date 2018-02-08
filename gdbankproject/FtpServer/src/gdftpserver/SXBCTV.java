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
	        String currency =  amount.replaceAll("\\$|\\￥|\\,", "");  //处理包含, ￥ 或者$的金额    
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
				MainServer.log.info("开始解析文件：" + TodayLocalPath + FileName);

				if (FileName.substring(0, 4).equals("GDYH")) { // 判断是不是广电对账文件
					MainServer.log.info("文件：" + FileName + "是广电的对账文件，继续解析");
					String filenamejiexi[] = FileName.split("_");
					File file = new File(TodayLocalPath + FileName);
					InputStreamReader read = new InputStreamReader(
							new FileInputStream(file), "utf-8");
					BufferedReader reader = new BufferedReader(read);
					String IParam;
					while((IParam = reader.readLine()) != null){//获得一条交易记录的第一行
						String PParam = reader.readLine(); //获得一条交易记录的第二行
						String BParam = reader.readLine(); //获得一条交易记录的第三行
						reader.readLine(); //获得两条交易记录分割行
						MainServer.log.info("获得的第一行为：" + IParam);
						MainServer.log.info("获得的第二行为：" + PParam);
						MainServer.log.info("获得的第三行为：" + BParam);
						
						byte IParamBytes[] = IParam.getBytes();
						byte PParamBytes[] = PParam.getBytes();
						byte BParamBytes[] = BParam.getBytes();
						MainServer.log.info("第一行长度为：" + IParamBytes.length);
						MainServer.log.info("第二行长度为：" + PParamBytes.length);
						MainServer.log.info("第三行长度为：" + BParamBytes.length);
						/*
						 * 第 一 行：
						 * 
						 * 			0--请求流水编号(40)
						 * 			1--外部系统标识(20)
						 * 			2--业务类型(50)
						 * 			3--交易总价格(9)
						 * 			4--商品描述(20)
						 * 			5--用户所属区域(20)
						 * 			6--订单渠道(3)
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
							MainServer.log.info("分割后的第一行：" + IParamSplit[j]);
						}
						
						
						/*
						 * 第 二 行：
						 * 			 
						 * 
						 * 			0--产品编码(20)
						 * 			1--产品名称(50)
						 * 			2--产品定价(9)
						 * 			3--供应商编码(20)
						 * 			4--产品业务类型(50)
						 * 			5--产品订购数量(3)
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
							MainServer.log.info("分割后的第二行：" + PParamSplit[j]);
						}
						/*
						 * 第 三 行：
						 * 
						 * 
						 * 			0--业务请求流水编号(20)
						 * 			1--业务交易代码(6)
						 * 			2--自有业务类型(1)
						 * 			3--计费对象(16)
						 * 			4--业务套餐代码(6)
						 * 			5--机顶盒编号(50)
						 * 			6--CA卡号(50)
						 * 			7--产品CAID(8)
						 * 			8--订单业务区域(20)
						 * 			9--业务产品代码(20)
						 * 			10--业务产品名称(50)
						 * 			11--业务产品定价(9)
						 * 			12--业务产品供应商编码(20)
						 * 			13--产品业务类型(50)
						 * 			14--产品订购数量(3)
						 * 			15--产品开通周期(29)
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
							MainServer.log.info("分割后的第三行：" + BParamSplit[j]);
						}
						
						
						
						BankStatementVo bankStatementVo = new BankStatementVo();

						bankStatementVo.setJybs(IParamSplit[0]); // 交易标识
						
						bankStatementVo.setJym("0001"); // 交易码
						bankStatementVo.setJymc("缴费"); // 交易名称						
						bankStatementVo.setKhbs(BParamSplit[3]); // 客户标识
						bankStatementVo.setKhfzxx(""); // 客户辅助信息
						bankStatementVo.setKhmc(""); // 客户名称
						bankStatementVo.setJyrq(filenamejiexi[1].substring(0, 8)); // 交易日期
						bankStatementVo.setJysj(""); // 交易时间
						bankStatementVo.setJyje(Integer
								.parseInt(changeY2F(IParamSplit[3]))); // 交易金额
						bankStatementVo.setJycgbs(0); // 交易成功标识
						bankStatementVo.setJycgbsxq("成功");// 交易成功标识详情
						bankStatementVo.setYjylsh(""); // 原交易流水号
						bankStatementVo.setJyzt(0); // 交易状态
						bankStatementVo.setJyztxq("成功"); // 交易状态详情
						bankStatementVo.setFhxx(""); // 返回信息
						bankStatementVo.setQybm(MapYingShe.BCTVAreaNoNoMap
								.get(IParamSplit[5])); // 区域编码
						bankStatementVo.setQymc(MapYingShe.BankAreaNoNameMap
								.get(MapYingShe.BCTVAreaNoNoMap
										.get(IParamSplit[5]))); // 区域名称

						// 待定-------****************************************
						bankStatementVo.setJyqdbm(""); // 交易渠道编码
						bankStatementVo.setJyqdmc(""); // 交易渠道名称

						bankStatementVo.setJyjg(""); // 交易机构
						bankStatementVo.setJyjgmc(""); // 交易机构名称
						bankStatementVo.setCzyh(""); // 操作员号
						bankStatementVo.setSfdwlbbm("96766"); // 收费单位类别编码
						bankStatementVo.setSfdwlbmc("广电缴费"); // 收费单位类别名称
						bankStatementVo.setSfdwyjjgbm("096766"); // 收费单位一级机构编码
						bankStatementVo.setSfdwyjjgmc("陕西广电网络"); // 收费单位一级技工名称
						bankStatementVo.setSfdwejjgbm(""); // 收费单位二级机构编码
						bankStatementVo.setSfdwejjgmc(""); // 收费单位二级机构名称
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
						
					}
					MainServer.log.info("函数正常退出");
					reader.close();
				} else {
					MainServer.log.warn("文件：" + FileName + "不是广电的对账文件，停止解析");
				}

			} catch (Exception e) {
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
				if (filenameString.substring(0, 4).equals("GDYH")) { // 判断是不是广电对账文件
					MainServer.log.info("文件：" + filenameString
							+ "是广电的对账文件，继续解析");
					String filenamejiexi[] = filenameString.split("_");
					MainServer.log.info("文件" + filenameString + "文件名解析结果：");
					for (int j = 0; j < filenamejiexi.length; j++) {
						MainServer.log.info(filenamejiexi[j]);
					}
					if (duizhangjiexi.get(15).equals(filenamejiexi[1].substring(0, 8))) {// 确保报文中的银行帐务日期和文件名中的日期一致
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
					MainServer.log.warn("文件：" + filenameString + "不是广电的对账文件，停止解析");
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
