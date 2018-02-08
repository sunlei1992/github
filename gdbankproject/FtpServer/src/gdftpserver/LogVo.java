package gdftpserver;

public class LogVo {

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
		private int scbjw;	//删除标记位
		private String scbjwxq;	//删除标记位详情
		private int xgbjw;	//修改标记位
		private String xgbjwxq;	//修改标记位详情
		private int dzjg;	//对账结果
		private String dzjgxq;	//对账结果详情
		private String beizhu;	//备注
		private String baoliu1;	//保留1
		private String baoliu2;	//保留2
		private String baoliu3;	//保留3
		private String scsj;  //删除表特有
		private String xgsj;  //修改表特有
		
		
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

						//4. 客户标识：缴费号、客户编号（设保留字段，填写关联信息）缴费交易，用于标识客户身份的信息
		public String getKhbs() {
			return khbs;
		}
	 	public void setKhbs(String khbs) {
			this.khbs = khbs;
		}

				//5. 客户辅助信息：（客户标识的关联信息等）(暂时为空)
		public String getKhfzxx() {
			return khfzxx;
		}
	 	public void setKhfzxx(String khfzxx) {
			this.khfzxx = khfzxx;
		}
	 	
						//6. 客户名称：
		public String getKhmc() {
			return khmc;
		}
	 	public void setKhmc(String khmc) {
			this.khmc = khmc;
		}

		  		//7. 交易日期：
		public String getJyrq() {
			return jyrq;
		}
	 	public void setJyrq(String jyrq) {
			this.jyrq = jyrq;
		}

				//8. 交易时间：
		public String getJysj() {
			return jysj;
		}
	 	public void setJysj(String jysj) {
			this.jysj = jysj;
		}

					//9. 交易金额：
		public int getJyje() {
			return jyje;
		}
	 	public void setJyje(int jyje) {
			this.jyje = jyje;
		}
	 
				// 	10. 交易成功标识：0、1、2    0成功，1失败，2是撤销
		public int getJycgbs() {
			return jycgbs;
		}
	 	public void setJycgbs(int jycgbs) {
			this.jycgbs = jycgbs;
		}
	 	
	 			// 	10. 交易成功标识详情：0、1、2    0成功，1失败，2是撤销
		public String getJycgbsxq() {
			return jycgbsxq;
		}
	 	public void setJycgbsxq(String jycgbsxq) {
			this.jycgbsxq = jycgbsxq;
		}

				// 	11. 原交易流水号：
		public String getYjylsh() {
			return yjylsh;
		}
	 	public void setYjylsh(String yjylsh) {
			this.yjylsh = yjylsh;
		}

					//	12. 交易状态：？
		public int getJyzt() {
			return jyzt;
		}
	 	public void setJyzt(int jyzt) {
			this.jyzt = jyzt;
		}
	 	
	 				//	12. 交易状态详情：？
		public String getJyztxq() {
			return jyztxq;
		}
	 	public void setJyztxq(String jyztxq) {
			this.jyztxq = jyztxq;
		}
	 	 	
				//	13. 返回信息：？
		public String getFhxx() {
			return fhxx;
		}
	 	public void setFhxx(String fhxx) {
			this.fhxx = fhxx;
		}


				//14. 区域编码：
		public String getQybm() {
			return qybm;
		}
	 	public void setQybm(String qybm) {
			this.qybm = qybm;
		}

					//15. 区域名称：
		public String getQymc() {
			return qymc;
		}
	 	public void setQymc(String qymc) {
			this.qymc = qymc;
		}

				// 	16. 交易渠道编码：
		public String getJyqdbm() {
			return jyqdbm;
		}
	 	public void setJyqdbm(String jyqdbm) {
			this.jyqdbm = jyqdbm;
		}

				// 	17. 交易渠道名称：
		public String getJyqdmc() {
			return jyqdmc;
		}
	 	public void setJyqdmc(String jyqdmc) {
			this.jyqdmc = jyqdmc;
		}

					// 	18. 交易机构：
		public String getJyjg() {
			return jyjg;
		}
	 	public void setJyjg(String jyjg) {
			this.jyjg = jyjg;
		}

				// 	19. 交易机构名称：?????
		public String getJyjgmc() {
			return jyjgmc;
		}
	 	public void setJyjgmc(String jyjgmc) {
			this.jyjgmc = jyjgmc;
		}

					 	//20. 操作员号：
		public String getCzyh() {
			return czyh;
		}
	 	public void setCzyh(String czyh) {
			this.czyh = czyh;
		}

				// 	21. 收费单位类别编码：
		public String getSfdwlbbm() {
			return sfdwlbbm;
		}
	 	public void setSfdwlbbm(String sfdwlbbm) {
			this.sfdwlbbm = sfdwlbbm;
		}

				// 	22. 收费单位类别名称：公共事业类缴费、物业类、学费类、其它类
		public String getSfdwlbmc() {
			return sfdwlbmc;
		}
	 	public void setSfdwlbmc(String sfdwlbmc) {
			this.sfdwlbmc = sfdwlbmc;
		}

					// 	23. 收费单位一级机构编码：一级机构编码
		public String getSfdwyjjgbm() {
			return sfdwyjjgbm;
		}
	 	public void setSfdwyjjgbm(String sfdwyjjgbm) {
			this.sfdwyjjgbm = sfdwyjjgbm;
		}

					// 	24. 收费单位一级机构名称：
		public String getSfdwyjjgmc() {
			return sfdwyjjgmc;
		}
	 	public void setSfdwyjjgmc(String sfdwyjjgmc) {
			this.sfdwyjjgmc = sfdwyjjgmc;
		}

					// 	25. 收费单位二级机构编码：
		public String getSfdwejjgbm() {
			return sfdwejjgbm;
		}
	 	public void setSfdwejjgbm(String sfdwejjgbm) {
			this.sfdwejjgbm = sfdwejjgbm;
		}

						 	//26. 收费单位二级机构名称：
		public String getSfdwejjgmc() {
			return sfdwejjgmc;
		}
	 	public void setSfdwejjgmc(String sfdwejjgmc) {
			this.sfdwejjgmc = sfdwejjgmc;
		}
	 	
	 				//	12. 删除标记：？
		public int getScbjw() {
			return scbjw;
		}
	 	public void setScbjw(int scbjw) {
			this.scbjw = scbjw;
		}
	 	
	 				//	12. 删除标记详情？
		public String getScbjwxq() {
			return scbjwxq;
		}
	 	public void setScbjwxq(String scbjwxq) {
			this.scbjwxq = scbjwxq;
		}
	 	
	 				//	12. 修改标记：？
		public int getXgbjw() {
			return xgbjw;
		}
	 	public void setXgbjw(int xgbjw) {
			this.xgbjw = xgbjw;
		}
	 	
	 				//	12. 修改标记详情？
		public String getXgbjwxq() {
			return xgbjwxq;
		}
	 	public void setXgbjwxq(String xgbjwxq) {
			this.xgbjwxq = xgbjwxq;
		}
	 		 	
	 				//	12. 修改标记：？
		public int getDzjg() {
			return dzjg;
		}
	 	public void setDzjg(int dzjg) {
			this.dzjg = dzjg;
		}
	 	
	 				//	12. 修改标记详情？
		public String getDzjgxq() {
			return dzjgxq;
		}
	 	public void setDzjgxq(String dzjgxq) {
			this.dzjgxq = dzjgxq;
		}
	 	
						// 	27.备注
		public String getBeizhu() {
			return beizhu;
		}
	 	public void setBeizhu(String beizhu) {
			this.beizhu = beizhu;
		}

					//保留1
		public String getBaoliu1() {
			return baoliu1;
		}
	 	public void setBaoliu1(String baoliu1) {
			this.baoliu1 = baoliu1;
		}

		//保留2
		public String getBaoliu2() {
			return baoliu2;
		}
	 	public void setBaoliu2(String baoliu2) {
			this.baoliu2 = baoliu2;
		}

		//保留3
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
	 	
	 	
	 	public String getXgsj() {
			return xgsj;
		}
	 	public void setXgsj(String xgsj) {
			this.xgsj = xgsj;
	 	}
		@Override
		public String toString() {
			return "LogVo [jybs=" + jybs + ", jym=" + jym + ", jymc=" + jymc
					+ ", khbs=" + khbs + ", khfzxx=" + khfzxx + ", khmc="
					+ khmc + ", jyrq=" + jyrq + ", jysj=" + jysj + ", jyje="
					+ jyje + ", jycgbs=" + jycgbs + ", jycgbsxq=" + jycgbsxq
					+ ", yjylsh=" + yjylsh + ", jyzt=" + jyzt + ", jyztxq="
					+ jyztxq + ", fhxx=" + fhxx + ", qybm=" + qybm + ", qymc="
					+ qymc + ", jyqdbm=" + jyqdbm + ", jyqdmc=" + jyqdmc
					+ ", jyjg=" + jyjg + ", jyjgmc=" + jyjgmc + ", czyh="
					+ czyh + ", sfdwlbbm=" + sfdwlbbm + ", sfdwlbmc="
					+ sfdwlbmc + ", sfdwyjjgbm=" + sfdwyjjgbm + ", sfdwyjjgmc="
					+ sfdwyjjgmc + ", sfdwejjgbm=" + sfdwejjgbm
					+ ", sfdwejjgmc=" + sfdwejjgmc + ", scbjw=" + scbjw
					+ ", scbjwxq=" + scbjwxq + ", xgbjw=" + xgbjw
					+ ", xgbjwxq=" + xgbjwxq + ", dzjg=" + dzjg + ", dzjgxq="
					+ dzjgxq + ", beizhu=" + beizhu + ", baoliu1=" + baoliu1
					+ ", baoliu2=" + baoliu2 + ", baoliu3=" + baoliu3
					+ ", scsj=" + scsj + ", xgsj=" + xgsj + "]";
		}



	 	
	

}
