package GDPayService;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

public class Dbmanage {
    public Connection initDB() {
        // ��ʼ�����ݿ����ӷ���
        Connection conn = null;
        // ����һ��Connection���
        try {
            Class.forName("com.mysql.jdbc.Driver");
            // �������ݿ�����
            String url = "jdbc:mysql://localhost:3306/test_gdbank_data?charaterEncoding=gb2312";
            // �������ݿ��ַurl�������ñ����ʽ
            conn = DriverManager.getConnection(url, "xidian", "88203643");
            // �õ���������
        } catch (ClassNotFoundException e) {

        	System.out.print("dddddddddddd");
            e.printStackTrace();
            GDPayService.log.error(e);
        } catch (SQLException e) {
        	System.out.print("aaaaaaaaaaaa");
            e.printStackTrace();
            GDPayService.log.error(e);
        }
        return conn;
        // �������ݿ�����
    }

    public void closeDB(Statement sta, Connection conn) {
        // �ر����ݿ����ӣ��޽������
        try {
            sta.close();
            conn.close();
        } catch (SQLException e) {

            e.printStackTrace();
            GDPayService.log.error(e);
        }

    }

    public void closeDB(ResultSet rs, Statement sta, Connection conn) {
        // �ر����ݿ����ӣ��н������
        try {
            rs.close();
            sta.close();
            conn.close();
        } catch (SQLException e) {

            e.printStackTrace();
            GDPayService.log.error(e);
        }

    }
}
