package gdftpserver;

public class BankStatementVo {
	private String jybs;  //交易标识
 	private String jym;	//交易码
 	private String jymc;	//交易名称
	private String khbs;	//客户标识
	private String khfzxx; 	//客户辅助信息
	private String khmc; 	//客户名称
	private String jyrq;	//交易日期
	private String jysj; 	//交易时间
	private int jyje;	//交易金额
	private int jycgbs; 	//交易成功标识
	private String jycgbsxq;//交易成功标识详情	
	private String yjylsh;	//原交易流水号
	private int jyzt;	//交易状态
	private String jyztxq;	//交易状态详情
	private String fhxx;	//返回信息
	private String qybm;	//区域编码
	private String qymc;	//区域名称
	private String jyqdbm;	//交易渠道编码
	private String jyqdmc; 	//交易渠道名称
	private String jyjg;	//交易机构
	private String jyjgmc;	//交易机构名称
	private String czyh;	//操作员号
	private String sfdwlbbm;	//收费单位类别编码
	private String sfdwlbmc;	//收费单位类别名称
	private String sfdwyjjgbm;	//收费单位一级机构编码
	private String sfdwyjjgmc;	//收费单位一级技工名称
	private String sfdwejjgbm;	//收费单位二级机构编码
	private String sfdwejjgmc;	//收费单位二级机构名称
	private String dzwjm;		//对账文件名
	private String sdrq;		//收到日期
	private int dzjg;	//对账结果
	private String dzjgxq;	//对账结果详情
	private String beizhu;	//备注
	private String baoliu1;	//保留1
	private String baoliu2;	//保留2
	private String baoliu3;	//保留3
	private String scsj;  //history表特有
	public String getJybs() {
		return jybs;
	}
	public void setJybs(String jybs) {
		this.jybs = jybs;
	}
	public String getJym() {
		return jym;
	}
	public void setJym(String jym) {
		this.jym = jym;
	}
	public String getJymc() {
		return jymc;
	}
	public void setJymc(String jymc) {
		this.jymc = jymc;
	}
	public String getKhbs() {
		return khbs;
	}
	public void setKhbs(String khbs) {
		this.khbs = khbs;
	}
	public String getKhfzxx() {
		return khfzxx;
	}
	public void setKhfzxx(String khfzxx) {
		this.khfzxx = khfzxx;
	}
	public String getKhmc() {
		return khmc;
	}
	public void setKhmc(String khmc) {
		this.khmc = khmc;
	}
	public String getJyrq() {
		return jyrq;
	}
	public void setJyrq(String jyrq) {
		this.jyrq = jyrq;
	}
	public String getJysj() {
		return jysj;
	}
	public void setJysj(String jysj) {
		this.jysj = jysj;
	}
	public int getJyje() {
		return jyje;
	}
	public void setJyje(int jyje) {
		this.jyje = jyje;
	}
	public int getJycgbs() {
		return jycgbs;
	}
	public void setJycgbs(int jycgbs) {
		this.jycgbs = jycgbs;
	}
	public String getJycgbsxq() {
		return jycgbsxq;
	}
	public void setJycgbsxq(String jycgbsxq) {
		this.jycgbsxq = jycgbsxq;
	}
	public String getYjylsh() {
		return yjylsh;
	}
	public void setYjylsh(String yjylsh) {
		this.yjylsh = yjylsh;
	}
	public int getJyzt() {
		return jyzt;
	}
	public void setJyzt(int jyzt) {
		this.jyzt = jyzt;
	}
	public String getJyztxq() {
		return jyztxq;
	}
	public void setJyztxq(String jyztxq) {
		this.jyztxq = jyztxq;
	}
	public String getFhxx() {
		return fhxx;
	}
	public void setFhxx(String fhxx) {
		this.fhxx = fhxx;
	}
	public String getQybm() {
		return qybm;
	}
	public void setQybm(String qybm) {
		this.qybm = qybm;
	}
	public String getQymc() {
		return qymc;
	}
	public void setQymc(String qymc) {
		this.qymc = qymc;
	}
	public String getJyqdbm() {
		return jyqdbm;
	}
	public void setJyqdbm(String jyqdbm) {
		this.jyqdbm = jyqdbm;
	}
	public String getJyqdmc() {
		return jyqdmc;
	}
	public void setJyqdmc(String jyqdmc) {
		this.jyqdmc = jyqdmc;
	}
	public String getJyjg() {
		return jyjg;
	}
	public void setJyjg(String jyjg) {
		this.jyjg = jyjg;
	}
	public String getJyjgmc() {
		return jyjgmc;
	}
	public void setJyjgmc(String jyjgmc) {
		this.jyjgmc = jyjgmc;
	}
	public String getCzyh() {
		return czyh;
	}
	public void setCzyh(String czyh) {
		this.czyh = czyh;
	}
	public String getSfdwlbbm() {
		return sfdwlbbm;
	}
	public void setSfdwlbbm(String sfdwlbbm) {
		this.sfdwlbbm = sfdwlbbm;
	}
	public String getSfdwlbmc() {
		return sfdwlbmc;
	}
	public void setSfdwlbmc(String sfdwlbmc) {
		this.sfdwlbmc = sfdwlbmc;
	}
	public String getSfdwyjjgbm() {
		return sfdwyjjgbm;
	}
	public void setSfdwyjjgbm(String sfdwyjjgbm) {
		this.sfdwyjjgbm = sfdwyjjgbm;
	}
	public String getSfdwyjjgmc() {
		return sfdwyjjgmc;
	}
	public void setSfdwyjjgmc(String sfdwyjjgmc) {
		this.sfdwyjjgmc = sfdwyjjgmc;
	}
	public String getSfdwejjgbm() {
		return sfdwejjgbm;
	}
	public void setSfdwejjgbm(String sfdwejjgbm) {
		this.sfdwejjgbm = sfdwejjgbm;
	}
	public String getSfdwejjgmc() {
		return sfdwejjgmc;
	}
	public void setSfdwejjgmc(String sfdwejjgmc) {
		this.sfdwejjgmc = sfdwejjgmc;
	}
	public String getDzwjm() {
		return dzwjm;
	}
	public void setDzwjm(String dzwjm) {
		this.dzwjm = dzwjm;
	}
	public String getSdrq() {
		return sdrq;
	}
	public void setSdrq(String sdrq) {
		this.sdrq = sdrq;
	}
	public int getDzjg() {
		return dzjg;
	}
	public void setDzjg(int dzjg) {
		this.dzjg = dzjg;
	}
	public String getDzjgxq() {
		return dzjgxq;
	}
	public void setDzjgxq(String dzjgxq) {
		this.dzjgxq = dzjgxq;
	}
	public String getBeizhu() {
		return beizhu;
	}
	public void setBeizhu(String beizhu) {
		this.beizhu = beizhu;
	}
	public String getBaoliu1() {
		return baoliu1;
	}
	public void setBaoliu1(String baoliu1) {
		this.baoliu1 = baoliu1;
	}
	public String getBaoliu2() {
		return baoliu2;
	}
	public void setBaoliu2(String baoliu2) {
		this.baoliu2 = baoliu2;
	}
	public String getBaoliu3() {
		return baoliu3;
	}
	public void setBaoliu3(String baoliu3) {
		this.baoliu3 = baoliu3;
	}
	public String getScsj() {
		return scsj;
	}
	public void setScsj(String scsj) {
		this.scsj = scsj;
	}
	@Override
	public String toString() {
		return "BankStateVo [jybs=" + jybs + ", jym=" + jym + ", jymc=" + jymc
				+ ", khbs=" + khbs + ", khfzxx=" + khfzxx + ", khmc=" + khmc
				+ ", jyrq=" + jyrq + ", jysj=" + jysj + ", jyje=" + jyje
				+ ", jycgbs=" + jycgbs + ", jycgbsxq=" + jycgbsxq + ", yjylsh="
				+ yjylsh + ", jyzt=" + jyzt + ", jyztxq=" + jyztxq + ", fhxx="
				+ fhxx + ", qybm=" + qybm + ", qymc=" + qymc + ", jyqdbm="
				+ jyqdbm + ", jyqdmc=" + jyqdmc + ", jyjg=" + jyjg
				+ ", jyjgmc=" + jyjgmc + ", czyh=" + czyh + ", sfdwlbbm="
				+ sfdwlbbm + ", sfdwlbmc=" + sfdwlbmc + ", sfdwyjjgbm="
				+ sfdwyjjgbm + ", sfdwyjjgmc=" + sfdwyjjgmc + ", sfdwejjgbm="
				+ sfdwejjgbm + ", sfdwejjgmc=" + sfdwejjgmc + ", dzwjm="
				+ dzwjm + ", sdrq=" + sdrq + ", dzjg=" + dzjg + ", dzjgxq="
				+ dzjgxq + ", beizhu=" + beizhu + ", baoliu1=" + baoliu1
				+ ", baoliu2=" + baoliu2 + ", baoliu3=" + baoliu3 + ", scsj="
				+ scsj + "]";
	}
	
	
	
}
