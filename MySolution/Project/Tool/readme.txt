

//打开pmdb文件，解析数据库中，所有表

CDbConnect::Connect
	 CPMSQLBaseCtrl::ConnectSQLDataBase
		sqlite3_open16

CDbConnect::DisConnect

CDbConnect::ExecuteSQL		
	CPMSQLBaseCtrl::ExecuteSQL(
		sqlite3_prepare16_v2
		CPMSQLTableCtrl::Update
			sqlite3_step


//控制器转DataSet
pDataSet = CTableCtrl2DataSet::GetDataSet( pTableCtrl ) ;


//是否与pmdb,后缀无关