package gdftpserver;

import java.util.HashMap;
import java.util.Map;

public class MapYingShe {
	public static Map<String, String> BankElecAreaMap = new HashMap<String, String>() {
		{
			put("0000000000000011", "029");
			put("0000000000000049", "0913");
			put("0000000000000048", "0910");
			put("0000000000000045", "0917");
			put("0000000000000046", "0916");
			put("0000000000000047", "0919");
			put("0000000000000044", "0915");
			put("0000000000000052", "0914");
			put("0000000000000050", "0911");
			put("0000000000000051", "0912");
		}
	};
	
	public static Map<String, String> ElecPayWayMap = new HashMap<String, String>() {
		{
			put("01", "��̨");
			put("02", "POS");
			put("03", "����");
			put("04", "�绰����");
		}
	};
	// ���н��ѷ�ʽ�ͽ�����������ӳ��
	public static Map<String, String> jiaoyiqudaoyingshe = new HashMap<String, String>() {
		{
			put("01", "BK010112");
			put("03", "BK010870");
			put("02", "BK010821");
		}
	};
	// �����ļ�����־�������ӳ��
	public static Map<String, String> BankElecTradeTypeMap = new HashMap<String, String>() {
		{
			put("0", "0001");
			put("1", "1001");
		}
	};
	public static Map<String, String>	BankAreaNoNameMap = new HashMap<String, String>() {
		{
			
			  
			put("029","����");
			put("0917","����");
			put("0910","����");
			put("0919","ͭ��");
			put("0913","μ��");
			put("0916","����");
			put("0915","����");
			put("0914","����");
			put("0912","����");
			put("0911","�Ӱ�");
		}
	};
	
	public static Map<String, String>	BCTVAreaNoNoMap = new HashMap<String, String>() {
		{
			
			  
			put("610100","029");  
			put("610200","0919"); 
			put("610300","0917");
			put("610400","0910");
			put("610500","0913");
			put("610600","0911");
			put("610700","0916");
			put("610800","0912");
			put("610900","0915");
			put("611000","0914");

		}
	};
}
