package gdftpserver;

import java.sql.Connection;
import java.sql.Date;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.sql.Time;
import java.util.*;

public class TableManage {

		public ArrayList<LogVo> LogTableSelectByDate(String date) {

		Dbmanage dbmanage = new Dbmanage();
		Connection conn = null;
		Statement sta = null;
		ResultSet rs = null;

		ArrayList<LogVo> logrecordlist = new ArrayList<LogVo>();
		try {
			conn = dbmanage.initDB();
			sta = conn.createStatement();

			String sql = "SELECT * FROM gdbank_log where jyrq ='" + date + "' and jycgbs ='0'" ;
			MainServer.log.info("sql语句：" + sql);
			rs = sta.executeQuery(sql);
			while (rs.next()) {
				LogVo logrecord = new LogVo();
				logrecord.setJybs(rs.getString(1)); // 交易标识
				logrecord.setJym(rs.getString(2)); // 交易码
				logrecord.setJymc(rs.getString(3)); // 交易名称
				logrecord.setKhbs(rs.getString(4)); // 客户标识
				logrecord.setKhfzxx(rs.getString(5)); // 客户辅助信息
				logrecord.setKhmc(rs.getString(6)); // 客户名称
				logrecord.setJyrq(rs.getString(7)); // 交易日期
				logrecord.setJysj(rs.getString(8)); // 交易时间
				logrecord.setJyje(rs.getInt(9)); // 交易金额
				logrecord.setJycgbs(rs.getInt(10)); // 交易成功标识
				logrecord.setJycgbsxq(rs.getString(11));// 交易成功标识详情

				logrecord.setYjylsh(rs.getString(12)); // 原交易流水号
				logrecord.setJyzt(rs.getInt(13)); // 交易状态
				logrecord.setJyztxq(rs.getString(14)); // 交易状态详情
				logrecord.setFhxx(rs.getString(15)); // 返回信息
				logrecord.setQybm(rs.getString(16)); // 区域编码
				logrecord.setQymc(rs.getString(17)); // 区域名称
				logrecord.setJyqdbm(rs.getString(18)); // 交易渠道编码
				logrecord.setJyqdmc(rs.getString(19)); // 交易渠道名称
				logrecord.setJyjg(rs.getString(20)); // 交易机构
				logrecord.setJyjgmc(rs.getString(21)); // 交易机构名称
				logrecord.setCzyh(rs.getString(22)); // 操作员号
				logrecord.setSfdwlbbm(rs.getString(23)); // 收费单位类别编码
				logrecord.setSfdwlbmc(rs.getString(24)); // 收费单位类别名称
				logrecord.setSfdwyjjgbm(rs.getString(25)); // 收费单位一级机构编码
				logrecord.setSfdwyjjgmc(rs.getString(26)); // 收费单位一级技工名称
				logrecord.setSfdwejjgbm(rs.getString(27)); // 收费单位二级机构编码
				logrecord.setSfdwejjgmc(rs.getString(28)); // 收费单位二级机构名称
				logrecord.setScbjw(rs.getInt(29)); // 删除标记位
				logrecord.setScbjwxq(rs.getString(30)); // 删除标记位详情
				logrecord.setXgbjw(rs.getInt(31)); // 修改标记位
				logrecord.setXgbjwxq(rs.getString(32)); // 修改标记位详情
				logrecord.setDzjg(rs.getInt(33)); // 对账结果
				logrecord.setDzjgxq(rs.getString(34)); // 对账结果详情
				logrecord.setBeizhu(rs.getString(35)); // 备注
				logrecord.setBaoliu1(rs.getString(36)); // 保留1
				logrecord.setBaoliu2(rs.getString(37)); // 保留2
				logrecord.setBaoliu3(rs.getString(38)); // 保留3

				logrecordlist.add(logrecord);

				MainServer.log.info("获得的log表对象" + logrecord);
			}

		} catch (Exception ex) {
			ex.printStackTrace();
			MainServer.log.error(ex);
		} finally {

			dbmanage.closeDB(rs, sta, conn);
		}
		return logrecordlist;

	}

	public void LogTableReturn(String date) {
		Dbmanage dbmanage = new Dbmanage();
		Connection conn = null;
		Statement sta = null;
		try {
			conn = dbmanage.initDB();
			sta = conn.createStatement();
			String sql = "UPDATE  gdbank_log SET dzjg ='0',dzjgxq='未对账' WHERE jyrq='"
					+ date + "'";
			MainServer.log.info("sql语句：" + sql);
			int result = sta.executeUpdate(sql);
			MainServer.log.info("受影响的行数" + result);

		} catch (SQLException e) {

			e.printStackTrace();
			MainServer.log.error(e);
		} finally {

			dbmanage.closeDB(sta, conn);
		}
	}

	public void LogTableUpdate(ArrayList<LogVo> logrecordlist) {
		Dbmanage dbmanage = new Dbmanage();
		Connection conn = null;
		Statement sta = null;
		try {
			conn = dbmanage.initDB();
			sta = conn.createStatement();
			for (int i = 0; i < logrecordlist.size(); i++) {
				String sql = "UPDATE  gdbank_log SET dzjg ='"
						+ logrecordlist.get(i).getDzjg() + "',dzjgxq='"
						+ logrecordlist.get(i).getDzjgxq() + "' WHERE jybs='"
						+ logrecordlist.get(i).getJybs() + "'";
				MainServer.log.info("sql语句：" + sql);
				int result = sta.executeUpdate(sql);
				MainServer.log.info("受影响的行数" + result);
			}

		} catch (SQLException e) {

			e.printStackTrace();
			MainServer.log.error(e);
		} finally {

			dbmanage.closeDB(sta, conn);
		}
	}

	
	public void InsertBankStatementTable(ArrayList<BankStatementVo> bankStatementVos) {
		// 用户注册方法
		Dbmanage dbmanage = new Dbmanage();
		Connection conn = null;
		Statement sta = null;

		try {
			conn = dbmanage.initDB();
			sta = conn.createStatement();
		
				for (int i = 0; i < bankStatementVos.size(); i++) {
					BankStatementVo bankStatementVo = new BankStatementVo();
					bankStatementVo = bankStatementVos.get(i);
					String sql = "INSERT INTO gdbank_statement VALUES('"

						+ bankStatementVo.getJybs() // 交易标识
						+ "','" + bankStatementVo.getJym() // 交易码
						+ "','" + bankStatementVo.getJymc() // 交易名称
						+ "','" + bankStatementVo.getKhbs() // 客户标识
						+ "','" + bankStatementVo.getKhfzxx() // 客户辅助信息
						+ "','" + bankStatementVo.getKhmc() // 客户名称
						+ "','" + bankStatementVo.getJyrq() // 交易日期
						+ "','" + bankStatementVo.getJysj() // 交易时间
						+ "','" + bankStatementVo.getJyje() // 交易金额
						+ "','" + bankStatementVo.getJycgbs() // 交易成功标识
						+ "','" + bankStatementVo.getJycgbsxq()// 交易成功标识详情
						+ "','" + bankStatementVo.getYjylsh() // 原交易流水号
						+ "','" + bankStatementVo.getJyzt() // 交易状态
						+ "','" + bankStatementVo.getJyztxq() // 交易状态详情
						+ "','" + bankStatementVo.getFhxx() // 返回信息
						+ "','" + bankStatementVo.getQybm() // 区域编码
						+ "','" + bankStatementVo.getQymc() // 区域名称
						+ "','" + bankStatementVo.getJyqdbm() // 交易渠道编码
						+ "','" + bankStatementVo.getJyqdmc() // 交易渠道名称
						+ "','" + bankStatementVo.getJyjg() // 交易机构
						+ "','" + bankStatementVo.getJyjgmc() // 交易机构名称
						+ "','" + bankStatementVo.getCzyh() // 操作员号
						+ "','" + bankStatementVo.getSfdwlbbm() // 收费单位类别编码
						+ "','" + bankStatementVo.getSfdwlbmc() // 收费单位类别名称
						+ "','" + bankStatementVo.getSfdwyjjgbm() // 收费单位一级机构编码
						+ "','" + bankStatementVo.getSfdwyjjgmc() // 收费单位一级技工名称
						+ "','" + bankStatementVo.getSfdwejjgbm() // 收费单位二级机构编码
						+ "','" + bankStatementVo.getSfdwejjgmc() // 收费单位二级机构名称
						+ "','" + bankStatementVo.getDzwjm() // 对账文件名
						+ "','" + bankStatementVo.getSdrq() // 收到日期
						+ "','" + bankStatementVo.getDzjg() // 对账结果
						+ "','" + bankStatementVo.getDzjgxq() // 对账结果详情
						+ "','" + bankStatementVo.getBeizhu() // 备注
						+ "','" + bankStatementVo.getBaoliu1() // 保留1
						+ "','" + bankStatementVo.getBaoliu2() // 保留2
						+ "','" + bankStatementVo.getBaoliu3() // 保留3
						+ "')";
					MainServer.log.info("sql语句：" + sql);
					int result = sta.executeUpdate(sql);
					MainServer.log.info("受影响的行数" + result);
				}
					
				

		} catch (SQLException e) {
			e.printStackTrace();
			MainServer.log.error(e);
		} finally {
			// 执行完关闭数据库
			dbmanage.closeDB(sta, conn);

		}
	}

	public void DeleteOldData(String zwdate) {
		Dbmanage dbmanage = new Dbmanage();
		Connection conn = null;
		Statement sta = null;
		ResultSet rs = null;

		ArrayList<BankStatementVo> bankStatementVos = new ArrayList<BankStatementVo>();
		try {
			conn = dbmanage.initDB();
			sta = conn.createStatement();

			// 先做转储，再做更新
			String sql = "SELECT * FROM gdbank_statement where jyrq='" + zwdate
					+ "'";
			MainServer.log.info("sql语句：" + sql);
			rs = sta.executeQuery(sql);
			while (rs.next()) {
				BankStatementVo bankStatementVo = new BankStatementVo();
				bankStatementVo.setJybs(rs.getString(1)); // 交易标识
				bankStatementVo.setJym(rs.getString(2)); // 交易码
				bankStatementVo.setJymc(rs.getString(3)); // 交易名称
				bankStatementVo.setKhbs(rs.getString(4)); // 客户标识
				bankStatementVo.setKhfzxx(rs.getString(5)); // 客户辅助信息
				bankStatementVo.setKhmc(rs.getString(6)); // 客户名称
				bankStatementVo.setJyrq(rs.getString(7)); // 交易日期
				bankStatementVo.setJysj(rs.getString(8)); // 交易时间
				bankStatementVo.setJyje(rs.getInt(9)); // 交易金额
				bankStatementVo.setJycgbs(rs.getInt(10)); // 交易成功标识
				bankStatementVo.setJycgbsxq(rs.getString(11));// 交易成功标识详情
				bankStatementVo.setYjylsh(rs.getString(12)); // 原交易流水号
				bankStatementVo.setJyzt(rs.getInt(13)); // 交易状态
				bankStatementVo.setJyztxq(rs.getString(14)); // 交易状态详情
				bankStatementVo.setFhxx(rs.getString(15)); // 返回信息
				bankStatementVo.setQybm(rs.getString(16)); // 区域编码
				bankStatementVo.setQymc(rs.getString(17)); // 区域名称
				bankStatementVo.setJyqdbm(rs.getString(18)); // 交易渠道编码
				bankStatementVo.setJyqdmc(rs.getString(19)); // 交易渠道名称
				bankStatementVo.setJyjg(rs.getString(20)); // 交易机构
				bankStatementVo.setJyjgmc(rs.getString(21)); // 交易机构名称
				bankStatementVo.setCzyh(rs.getString(22)); // 操作员号
				bankStatementVo.setSfdwlbbm(rs.getString(23)); // 收费单位类别编码
				bankStatementVo.setSfdwlbmc(rs.getString(24)); // 收费单位类别名称
				bankStatementVo.setSfdwyjjgbm(rs.getString(25)); // 收费单位一级机构编码
				bankStatementVo.setSfdwyjjgmc(rs.getString(26)); // 收费单位一级技工名称
				bankStatementVo.setSfdwejjgbm(rs.getString(27)); // 收费单位二级机构编码
				bankStatementVo.setSfdwejjgmc(rs.getString(28)); // 收费单位二级机构名称
				bankStatementVo.setDzwjm(rs.getString(29)); // 对账文件名
				bankStatementVo.setSdrq(rs.getString(30)); // 收到日期
				bankStatementVo.setDzjg(rs.getInt(31)); // 对账结果
				bankStatementVo.setDzjgxq(rs.getString(32)); // 对账结果详情
				bankStatementVo.setBeizhu(rs.getString(33)); // 备注
				bankStatementVo.setBaoliu1(rs.getString(34)); // 保留1
				bankStatementVo.setBaoliu2(rs.getString(35)); // 保留2
				bankStatementVo.setBaoliu3(rs.getString(36)); // 保留3

				MainServer.log.info("获得的对象:" + bankStatementVo);
				bankStatementVos.add(bankStatementVo);

			}
			// 转储
			for (int i = 0; i < bankStatementVos.size(); i++) {
				BankStatementVo bankStatementVo = new BankStatementVo();
				bankStatementVo = bankStatementVos.get(i);
				Date currentday = new java.sql.Date(System.currentTimeMillis());
				Time currenttime = new java.sql.Time(System.currentTimeMillis());
				bankStatementVo.setScsj(currentday.toString() + " "
						+ currenttime.toString());
				sql = "INSERT INTO gdbank_statement_history VALUES('"

				+ bankStatementVo.getJybs() // 交易标识
						+ "','" + bankStatementVo.getJym() // 交易码
						+ "','" + bankStatementVo.getJymc() // 交易名称
						+ "','" + bankStatementVo.getKhbs() // 客户标识
						+ "','" + bankStatementVo.getKhfzxx() // 客户辅助信息
						+ "','" + bankStatementVo.getKhmc() // 客户名称
						+ "','" + bankStatementVo.getJyrq() // 交易日期
						+ "','" + bankStatementVo.getJysj() // 交易时间
						+ "','" + bankStatementVo.getJyje() // 交易金额
						+ "','" + bankStatementVo.getJycgbs() // 交易成功标识
						+ "','" + bankStatementVo.getJycgbsxq()// 交易成功标识详情
						+ "','" + bankStatementVo.getYjylsh() // 原交易流水号
						+ "','" + bankStatementVo.getJyzt() // 交易状态
						+ "','" + bankStatementVo.getJyztxq() // 交易状态详情
						+ "','" + bankStatementVo.getFhxx() // 返回信息
						+ "','" + bankStatementVo.getQybm() // 区域编码
						+ "','" + bankStatementVo.getQymc() // 区域名称
						+ "','" + bankStatementVo.getJyqdbm() // 交易渠道编码
						+ "','" + bankStatementVo.getJyqdmc() // 交易渠道名称
						+ "','" + bankStatementVo.getJyjg() // 交易机构
						+ "','" + bankStatementVo.getJyjgmc() // 交易机构名称
						+ "','" + bankStatementVo.getCzyh() // 操作员号
						+ "','" + bankStatementVo.getSfdwlbbm() // 收费单位类别编码
						+ "','" + bankStatementVo.getSfdwlbmc() // 收费单位类别名称
						+ "','" + bankStatementVo.getSfdwyjjgbm() // 收费单位一级机构编码
						+ "','" + bankStatementVo.getSfdwyjjgmc() // 收费单位一级技工名称
						+ "','" + bankStatementVo.getSfdwejjgbm() // 收费单位二级机构编码
						+ "','" + bankStatementVo.getSfdwejjgmc() // 收费单位二级机构名称
						+ "','" + bankStatementVo.getDzwjm() // 对账文件名
						+ "','" + bankStatementVo.getSdrq() // 收到日期
						+ "','" + bankStatementVo.getDzjg() // 对账结果
						+ "','" + bankStatementVo.getDzjgxq() // 对账结果详情
						+ "','" + bankStatementVo.getBeizhu() // 备注
						+ "','" + bankStatementVo.getBaoliu1() // 保留1
						+ "','" + bankStatementVo.getBaoliu2() // 保留2
						+ "','" + bankStatementVo.getBaoliu3() // 保留3
						+ "','" + bankStatementVo.getScsj() // 删除时间
						+ "')";
				MainServer.log.info("sql语句：" + sql);
			sta.executeUpdate(sql);

			}
			// 更新
			sql = "DELETE FROM  gdbank_statement WHERE jyrq=" + "'"
					+ zwdate + "'";
			MainServer.log.info("sql语句：" + sql);
			int result = sta.executeUpdate(sql);
			MainServer.log.info("受影响的行数" + result);
			return;
		}

		catch (SQLException e) {

			e.printStackTrace();
		} finally {
			// 执行完关闭数据库
			dbmanage.closeDB(rs, sta, conn);
		}
		return;
	}
    
}
