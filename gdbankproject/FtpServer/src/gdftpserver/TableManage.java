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
			MainServer.log.info("sql��䣺" + sql);
			rs = sta.executeQuery(sql);
			while (rs.next()) {
				LogVo logrecord = new LogVo();
				logrecord.setJybs(rs.getString(1)); // ���ױ�ʶ
				logrecord.setJym(rs.getString(2)); // ������
				logrecord.setJymc(rs.getString(3)); // ��������
				logrecord.setKhbs(rs.getString(4)); // �ͻ���ʶ
				logrecord.setKhfzxx(rs.getString(5)); // �ͻ�������Ϣ
				logrecord.setKhmc(rs.getString(6)); // �ͻ�����
				logrecord.setJyrq(rs.getString(7)); // ��������
				logrecord.setJysj(rs.getString(8)); // ����ʱ��
				logrecord.setJyje(rs.getInt(9)); // ���׽��
				logrecord.setJycgbs(rs.getInt(10)); // ���׳ɹ���ʶ
				logrecord.setJycgbsxq(rs.getString(11));// ���׳ɹ���ʶ����

				logrecord.setYjylsh(rs.getString(12)); // ԭ������ˮ��
				logrecord.setJyzt(rs.getInt(13)); // ����״̬
				logrecord.setJyztxq(rs.getString(14)); // ����״̬����
				logrecord.setFhxx(rs.getString(15)); // ������Ϣ
				logrecord.setQybm(rs.getString(16)); // �������
				logrecord.setQymc(rs.getString(17)); // ��������
				logrecord.setJyqdbm(rs.getString(18)); // ������������
				logrecord.setJyqdmc(rs.getString(19)); // ������������
				logrecord.setJyjg(rs.getString(20)); // ���׻���
				logrecord.setJyjgmc(rs.getString(21)); // ���׻�������
				logrecord.setCzyh(rs.getString(22)); // ����Ա��
				logrecord.setSfdwlbbm(rs.getString(23)); // �շѵ�λ������
				logrecord.setSfdwlbmc(rs.getString(24)); // �շѵ�λ�������
				logrecord.setSfdwyjjgbm(rs.getString(25)); // �շѵ�λһ����������
				logrecord.setSfdwyjjgmc(rs.getString(26)); // �շѵ�λһ����������
				logrecord.setSfdwejjgbm(rs.getString(27)); // �շѵ�λ������������
				logrecord.setSfdwejjgmc(rs.getString(28)); // �շѵ�λ������������
				logrecord.setScbjw(rs.getInt(29)); // ɾ�����λ
				logrecord.setScbjwxq(rs.getString(30)); // ɾ�����λ����
				logrecord.setXgbjw(rs.getInt(31)); // �޸ı��λ
				logrecord.setXgbjwxq(rs.getString(32)); // �޸ı��λ����
				logrecord.setDzjg(rs.getInt(33)); // ���˽��
				logrecord.setDzjgxq(rs.getString(34)); // ���˽������
				logrecord.setBeizhu(rs.getString(35)); // ��ע
				logrecord.setBaoliu1(rs.getString(36)); // ����1
				logrecord.setBaoliu2(rs.getString(37)); // ����2
				logrecord.setBaoliu3(rs.getString(38)); // ����3

				logrecordlist.add(logrecord);

				MainServer.log.info("��õ�log�����" + logrecord);
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
			String sql = "UPDATE  gdbank_log SET dzjg ='0',dzjgxq='δ����' WHERE jyrq='"
					+ date + "'";
			MainServer.log.info("sql��䣺" + sql);
			int result = sta.executeUpdate(sql);
			MainServer.log.info("��Ӱ�������" + result);

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
				MainServer.log.info("sql��䣺" + sql);
				int result = sta.executeUpdate(sql);
				MainServer.log.info("��Ӱ�������" + result);
			}

		} catch (SQLException e) {

			e.printStackTrace();
			MainServer.log.error(e);
		} finally {

			dbmanage.closeDB(sta, conn);
		}
	}

	
	public void InsertBankStatementTable(ArrayList<BankStatementVo> bankStatementVos) {
		// �û�ע�᷽��
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

						+ bankStatementVo.getJybs() // ���ױ�ʶ
						+ "','" + bankStatementVo.getJym() // ������
						+ "','" + bankStatementVo.getJymc() // ��������
						+ "','" + bankStatementVo.getKhbs() // �ͻ���ʶ
						+ "','" + bankStatementVo.getKhfzxx() // �ͻ�������Ϣ
						+ "','" + bankStatementVo.getKhmc() // �ͻ�����
						+ "','" + bankStatementVo.getJyrq() // ��������
						+ "','" + bankStatementVo.getJysj() // ����ʱ��
						+ "','" + bankStatementVo.getJyje() // ���׽��
						+ "','" + bankStatementVo.getJycgbs() // ���׳ɹ���ʶ
						+ "','" + bankStatementVo.getJycgbsxq()// ���׳ɹ���ʶ����
						+ "','" + bankStatementVo.getYjylsh() // ԭ������ˮ��
						+ "','" + bankStatementVo.getJyzt() // ����״̬
						+ "','" + bankStatementVo.getJyztxq() // ����״̬����
						+ "','" + bankStatementVo.getFhxx() // ������Ϣ
						+ "','" + bankStatementVo.getQybm() // �������
						+ "','" + bankStatementVo.getQymc() // ��������
						+ "','" + bankStatementVo.getJyqdbm() // ������������
						+ "','" + bankStatementVo.getJyqdmc() // ������������
						+ "','" + bankStatementVo.getJyjg() // ���׻���
						+ "','" + bankStatementVo.getJyjgmc() // ���׻�������
						+ "','" + bankStatementVo.getCzyh() // ����Ա��
						+ "','" + bankStatementVo.getSfdwlbbm() // �շѵ�λ������
						+ "','" + bankStatementVo.getSfdwlbmc() // �շѵ�λ�������
						+ "','" + bankStatementVo.getSfdwyjjgbm() // �շѵ�λһ����������
						+ "','" + bankStatementVo.getSfdwyjjgmc() // �շѵ�λһ����������
						+ "','" + bankStatementVo.getSfdwejjgbm() // �շѵ�λ������������
						+ "','" + bankStatementVo.getSfdwejjgmc() // �շѵ�λ������������
						+ "','" + bankStatementVo.getDzwjm() // �����ļ���
						+ "','" + bankStatementVo.getSdrq() // �յ�����
						+ "','" + bankStatementVo.getDzjg() // ���˽��
						+ "','" + bankStatementVo.getDzjgxq() // ���˽������
						+ "','" + bankStatementVo.getBeizhu() // ��ע
						+ "','" + bankStatementVo.getBaoliu1() // ����1
						+ "','" + bankStatementVo.getBaoliu2() // ����2
						+ "','" + bankStatementVo.getBaoliu3() // ����3
						+ "')";
					MainServer.log.info("sql��䣺" + sql);
					int result = sta.executeUpdate(sql);
					MainServer.log.info("��Ӱ�������" + result);
				}
					
				

		} catch (SQLException e) {
			e.printStackTrace();
			MainServer.log.error(e);
		} finally {
			// ִ����ر����ݿ�
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

			// ����ת������������
			String sql = "SELECT * FROM gdbank_statement where jyrq='" + zwdate
					+ "'";
			MainServer.log.info("sql��䣺" + sql);
			rs = sta.executeQuery(sql);
			while (rs.next()) {
				BankStatementVo bankStatementVo = new BankStatementVo();
				bankStatementVo.setJybs(rs.getString(1)); // ���ױ�ʶ
				bankStatementVo.setJym(rs.getString(2)); // ������
				bankStatementVo.setJymc(rs.getString(3)); // ��������
				bankStatementVo.setKhbs(rs.getString(4)); // �ͻ���ʶ
				bankStatementVo.setKhfzxx(rs.getString(5)); // �ͻ�������Ϣ
				bankStatementVo.setKhmc(rs.getString(6)); // �ͻ�����
				bankStatementVo.setJyrq(rs.getString(7)); // ��������
				bankStatementVo.setJysj(rs.getString(8)); // ����ʱ��
				bankStatementVo.setJyje(rs.getInt(9)); // ���׽��
				bankStatementVo.setJycgbs(rs.getInt(10)); // ���׳ɹ���ʶ
				bankStatementVo.setJycgbsxq(rs.getString(11));// ���׳ɹ���ʶ����
				bankStatementVo.setYjylsh(rs.getString(12)); // ԭ������ˮ��
				bankStatementVo.setJyzt(rs.getInt(13)); // ����״̬
				bankStatementVo.setJyztxq(rs.getString(14)); // ����״̬����
				bankStatementVo.setFhxx(rs.getString(15)); // ������Ϣ
				bankStatementVo.setQybm(rs.getString(16)); // �������
				bankStatementVo.setQymc(rs.getString(17)); // ��������
				bankStatementVo.setJyqdbm(rs.getString(18)); // ������������
				bankStatementVo.setJyqdmc(rs.getString(19)); // ������������
				bankStatementVo.setJyjg(rs.getString(20)); // ���׻���
				bankStatementVo.setJyjgmc(rs.getString(21)); // ���׻�������
				bankStatementVo.setCzyh(rs.getString(22)); // ����Ա��
				bankStatementVo.setSfdwlbbm(rs.getString(23)); // �շѵ�λ������
				bankStatementVo.setSfdwlbmc(rs.getString(24)); // �շѵ�λ�������
				bankStatementVo.setSfdwyjjgbm(rs.getString(25)); // �շѵ�λһ����������
				bankStatementVo.setSfdwyjjgmc(rs.getString(26)); // �շѵ�λһ����������
				bankStatementVo.setSfdwejjgbm(rs.getString(27)); // �շѵ�λ������������
				bankStatementVo.setSfdwejjgmc(rs.getString(28)); // �շѵ�λ������������
				bankStatementVo.setDzwjm(rs.getString(29)); // �����ļ���
				bankStatementVo.setSdrq(rs.getString(30)); // �յ�����
				bankStatementVo.setDzjg(rs.getInt(31)); // ���˽��
				bankStatementVo.setDzjgxq(rs.getString(32)); // ���˽������
				bankStatementVo.setBeizhu(rs.getString(33)); // ��ע
				bankStatementVo.setBaoliu1(rs.getString(34)); // ����1
				bankStatementVo.setBaoliu2(rs.getString(35)); // ����2
				bankStatementVo.setBaoliu3(rs.getString(36)); // ����3

				MainServer.log.info("��õĶ���:" + bankStatementVo);
				bankStatementVos.add(bankStatementVo);

			}
			// ת��
			for (int i = 0; i < bankStatementVos.size(); i++) {
				BankStatementVo bankStatementVo = new BankStatementVo();
				bankStatementVo = bankStatementVos.get(i);
				Date currentday = new java.sql.Date(System.currentTimeMillis());
				Time currenttime = new java.sql.Time(System.currentTimeMillis());
				bankStatementVo.setScsj(currentday.toString() + " "
						+ currenttime.toString());
				sql = "INSERT INTO gdbank_statement_history VALUES('"

				+ bankStatementVo.getJybs() // ���ױ�ʶ
						+ "','" + bankStatementVo.getJym() // ������
						+ "','" + bankStatementVo.getJymc() // ��������
						+ "','" + bankStatementVo.getKhbs() // �ͻ���ʶ
						+ "','" + bankStatementVo.getKhfzxx() // �ͻ�������Ϣ
						+ "','" + bankStatementVo.getKhmc() // �ͻ�����
						+ "','" + bankStatementVo.getJyrq() // ��������
						+ "','" + bankStatementVo.getJysj() // ����ʱ��
						+ "','" + bankStatementVo.getJyje() // ���׽��
						+ "','" + bankStatementVo.getJycgbs() // ���׳ɹ���ʶ
						+ "','" + bankStatementVo.getJycgbsxq()// ���׳ɹ���ʶ����
						+ "','" + bankStatementVo.getYjylsh() // ԭ������ˮ��
						+ "','" + bankStatementVo.getJyzt() // ����״̬
						+ "','" + bankStatementVo.getJyztxq() // ����״̬����
						+ "','" + bankStatementVo.getFhxx() // ������Ϣ
						+ "','" + bankStatementVo.getQybm() // �������
						+ "','" + bankStatementVo.getQymc() // ��������
						+ "','" + bankStatementVo.getJyqdbm() // ������������
						+ "','" + bankStatementVo.getJyqdmc() // ������������
						+ "','" + bankStatementVo.getJyjg() // ���׻���
						+ "','" + bankStatementVo.getJyjgmc() // ���׻�������
						+ "','" + bankStatementVo.getCzyh() // ����Ա��
						+ "','" + bankStatementVo.getSfdwlbbm() // �շѵ�λ������
						+ "','" + bankStatementVo.getSfdwlbmc() // �շѵ�λ�������
						+ "','" + bankStatementVo.getSfdwyjjgbm() // �շѵ�λһ����������
						+ "','" + bankStatementVo.getSfdwyjjgmc() // �շѵ�λһ����������
						+ "','" + bankStatementVo.getSfdwejjgbm() // �շѵ�λ������������
						+ "','" + bankStatementVo.getSfdwejjgmc() // �շѵ�λ������������
						+ "','" + bankStatementVo.getDzwjm() // �����ļ���
						+ "','" + bankStatementVo.getSdrq() // �յ�����
						+ "','" + bankStatementVo.getDzjg() // ���˽��
						+ "','" + bankStatementVo.getDzjgxq() // ���˽������
						+ "','" + bankStatementVo.getBeizhu() // ��ע
						+ "','" + bankStatementVo.getBaoliu1() // ����1
						+ "','" + bankStatementVo.getBaoliu2() // ����2
						+ "','" + bankStatementVo.getBaoliu3() // ����3
						+ "','" + bankStatementVo.getScsj() // ɾ��ʱ��
						+ "')";
				MainServer.log.info("sql��䣺" + sql);
			sta.executeUpdate(sql);

			}
			// ����
			sql = "DELETE FROM  gdbank_statement WHERE jyrq=" + "'"
					+ zwdate + "'";
			MainServer.log.info("sql��䣺" + sql);
			int result = sta.executeUpdate(sql);
			MainServer.log.info("��Ӱ�������" + result);
			return;
		}

		catch (SQLException e) {

			e.printStackTrace();
		} finally {
			// ִ����ر����ݿ�
			dbmanage.closeDB(rs, sta, conn);
		}
		return;
	}
    
}
