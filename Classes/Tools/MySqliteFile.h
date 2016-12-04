#pragma once
#include "Tools\MacroFile.h"


class MySqilteObjec
{
public:
	static MySqilteObjec* create();
	bool initInfo();
	void openMySqlite(string _fileName);
	void closeMySqlite();
	void createTable(string _tableName, const char* _args);
	void insetDataToTable(string _tableName, const char* _value);
	string searchStringDataFromTable(string _tableName, string _target, string _where, string _whereValue);
	void setStringDataValue(string _tableName, string _target, string _value, string _where, string _whereValue);
	void deleteTable(string _tableName);
	void deleteData(string _tableName ,string _where ,string _key);
	void addColumn(string _tableName, string _colName, string _type);
	CC_DEPRECATED_ATTRIBUTE void copyResToData();

	long searchIntDataFromTable(string _tableName, string _target, string _where = "", string _whrValue = "");               //根据table中的key获取value  atoi(  ().c_str()   )
	void setIntDataValue(string _tableName, string _target, int _value, string _where = "", string _whrValue = "");    //根据table中的key修改value
	float searchFloatDataFromTable(string _tableName, string _target, string _where = "", string _whrValue = "");               //根据table中的key获取value  atoi(  ().c_str()   )
	void setFloatDataValue(string _tableName, string _target, float _value, string _where = "", string _whrValue = "");    //根据table中的key修改value
	
private:

protected:
	int m_nResult;
	sqlite3* m_pSqliteDB;
	string m_strGetData;
};