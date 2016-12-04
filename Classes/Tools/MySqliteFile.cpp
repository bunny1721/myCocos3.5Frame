#include "MySqliteFile.h"
#include "CommonFile.h"
MySqilteObjec* MySqilteObjec::create()
{
	MySqilteObjec* _mso = new MySqilteObjec();
	if (_mso &&_mso->initInfo())
	{

	}
	else
	{
		CC_SAFE_DELETE(_mso);
	}
	return _mso;
}
bool MySqilteObjec::initInfo()
{
	bool _isInit = UserDefault::getInstance()->getBoolForKey("initSqlite");
	if (!_isInit)
	{
		string _playerName = "";


		this->openMySqlite(SQLITE_FILE_NAME);
		
		this->createTable(SQLITE_TABLE_PLAYER , "id text , name text , status text ,price text");

		char _id[8];
		for (int i = 0; i < PLAYER_NUMBER; i++)
		{
			_playerName = __String::createWithFormat("player%d", i)->getCString();
			int _posId = 0;
			string _valueStr;
			_valueStr = "'','" + _playerName + "','lock','0'";
			if (i == 0)
			{
				_valueStr = "'','" + _playerName + "','equip','0'";
			}
			sprintf(_id, "%d", i);
			_posId = _valueStr.find('\'') + 1;
			_valueStr.insert(_posId, _id);
			this->insetDataToTable(SQLITE_TABLE_PLAYER, _valueStr.c_str());
		}

		this->createTable(SQLITE_TABLE_PASSDATA , "passNum text , status text,starNum text");
		for (int i = 0; i < SELECTPASS_CHAPTER_NUMBER; i++)
		{
			for (int j = 0; j < 19; j++)
			{
				sprintf(_id, "'%d'", j + i * 19);
				string _passNum = _id;
				if (j + i * 19==0)
					_passNum += ", 'unlock' ,'0'";
				else
					_passNum += ", 'lock' ,'0'";
				this->insetDataToTable(SQLITE_TABLE_PASSDATA, _passNum.c_str());
			}
		}

		this->createTable(SQLITE_TABLE_KEYVALUE, "key text , value text");
		string _keyValue;
		_keyValue = "'coin' ,'0'";
		this->insetDataToTable(SQLITE_TABLE_KEYVALUE , _keyValue.c_str());
		_keyValue = "'LastTimeForBody' ,'0'";
		this->insetDataToTable(SQLITE_TABLE_KEYVALUE, _keyValue.c_str());
		_keyValue = "'continueLoginDayNum' ,'0'";
		this->insetDataToTable(SQLITE_TABLE_KEYVALUE, _keyValue.c_str());
		_keyValue = "'lastLoginDay' ,'0'";
		this->insetDataToTable(SQLITE_TABLE_KEYVALUE, _keyValue.c_str());
		_keyValue = "'bodyPowerNum' ,'30'";
		this->insetDataToTable(SQLITE_TABLE_KEYVALUE, _keyValue.c_str());
		_keyValue = "'myStarNum' ,'0'";
		this->insetDataToTable(SQLITE_TABLE_KEYVALUE, _keyValue.c_str());

		_keyValue = "'unlockChapter' ,'1'";
		this->insetDataToTable(SQLITE_TABLE_KEYVALUE, _keyValue.c_str());

		_keyValue = "'unlimitTargetTime' ,'0'";
		this->insetDataToTable(SQLITE_TABLE_KEYVALUE, _keyValue.c_str());

		_keyValue = "'isUnlimitBodyPower' ,'0'";
		this->insetDataToTable(SQLITE_TABLE_KEYVALUE, _keyValue.c_str());

		_keyValue = "'lastLimitTime' ,'0'";
		this->insetDataToTable(SQLITE_TABLE_KEYVALUE, _keyValue.c_str());

		_keyValue = "'limitRestartCount' ,'2'";
		this->insetDataToTable(SQLITE_TABLE_KEYVALUE, _keyValue.c_str());
		

		this->closeMySqlite();
		UserDefault::getInstance()->setBoolForKey("initSqlite" , true);
	}

	return true;
}
void MySqilteObjec::openMySqlite(string _fileName)
{
	string _path = FileUtils::getInstance()->getWritablePath()+_fileName;
	m_nResult = sqlite3_open(_path.c_str(), &m_pSqliteDB);
	if (m_nResult != SQLITE_OK)
	{
		log("open sql failed");
		const char* erromsg = sqlite3_errmsg(m_pSqliteDB);
		log("sqlite open error: %s", erromsg);
	}
}
void MySqilteObjec::closeMySqlite()
{
	sqlite3_close(m_pSqliteDB);
}

void MySqilteObjec::createTable(string _tableName ,const char* _args)
{
	string _createTableStr("create table " + _tableName + "()");
	int _insertId = _createTableStr.find('(')+1;
	_createTableStr.insert(_insertId, _args);
	m_nResult = sqlite3_exec(m_pSqliteDB, _createTableStr.c_str() , nullptr , nullptr , nullptr);
	if (m_nResult != SQLITE_OK)
	{
		log("create tanle failed");
		const char* errormsg = sqlite3_errmsg(m_pSqliteDB);
		log("sqlite exec error: %s", errormsg);
	}
}
void MySqilteObjec::insetDataToTable(string _tableName, const char* _value)
{
	string _insetStr("insert into " + _tableName + " values()");
	int _insertId = _insetStr.find('(') + 1;
	_insetStr.insert(_insertId, _value);
	m_nResult = sqlite3_exec(m_pSqliteDB , _insetStr.c_str() , nullptr , nullptr ,nullptr);
	if (m_nResult != SQLITE_OK)
	{
		log("insert into table failed");
		const char* errormsg = sqlite3_errmsg(m_pSqliteDB);
		log("sqlite exec into error : %s", errormsg);
	}

}
string MySqilteObjec::searchStringDataFromTable(string _tableName, string _target, string _where, string _whereValue)
{
	string _getData;
	char** re;
	int r, c;
	string _searchStr;
	_searchStr = ("select " + _target + " from " + _tableName + " where " + _where + " like '" + _whereValue + "'");
	m_nResult = sqlite3_get_table(m_pSqliteDB , _searchStr.c_str() ,&re , &r , &c , nullptr);
	if (m_nResult != SQLITE_OK)
	{
		log("search table failed");
		const char* errormsg = sqlite3_errmsg(m_pSqliteDB);
		log("sqlite exec search error : %s", errormsg);
		sqlite3_free_table(re);
	}
	else
	{
		_getData = re[(r - 1) * 1 + c];
		sqlite3_free_table(re);
	}
	return _getData;
}
void MySqilteObjec::setStringDataValue(string _tableName, string _target, string _value, string _where, string _whereValue)
{
	string _setStr;
	if (_where.size() != 0)
		_setStr = ("update " + _tableName + " set " + _target + " = '" + _value + "' where " + _where + " = '" + _whereValue + "'");
	else
		_setStr = ("update " + _tableName + " set value = '" + _value + "' where key = '" + _target + "'");
	m_nResult = sqlite3_exec(m_pSqliteDB, _setStr.c_str() , nullptr , nullptr , nullptr);
	if (m_nResult != SQLITE_OK)
	{
		log("search update failed");
		const char* errormsg = sqlite3_errmsg(m_pSqliteDB);
		log("sqlite exec update error : %s", errormsg);
	}
}
void MySqilteObjec::deleteTable(string _tableName)
{
	string _deleteTableStr("drop table " + _tableName);
	m_nResult = sqlite3_exec(m_pSqliteDB, _deleteTableStr.c_str() , nullptr , nullptr , nullptr);
	if (m_nResult != SQLITE_OK)
	{
		log("search update failed");
		const char* errormsg = sqlite3_errmsg(m_pSqliteDB);
		log("sqlite exec update error : %s", errormsg);
	}
}
void MySqilteObjec::deleteData(string _tableName, string _where, string _key)
{
	string _deleteData("delete from " + _tableName + " where " + _where + " = " + _key);
	m_nResult = sqlite3_exec(m_pSqliteDB, _deleteData.c_str() , nullptr, nullptr, nullptr);
	if (m_nResult != SQLITE_OK)
	{
		log("search delete failed");
		const char* errormsg = sqlite3_errmsg(m_pSqliteDB);
		log("sqlite exec delete error : %s", errormsg);
	}
}
void MySqilteObjec::addColumn(string _tableName, string _colName,string _type)
{
	string _addColimn("alter table " + _tableName + " add column " + _colName+" " + _type);
	m_nResult = sqlite3_exec(m_pSqliteDB, _addColimn.c_str(), nullptr, nullptr, nullptr);
	if (m_nResult != SQLITE_OK)
	{
		log("search delete failed");
		const char* errormsg = sqlite3_errmsg(m_pSqliteDB);
		log("sqlite exec delete error : %s", errormsg);
	}
}
void MySqilteObjec::copyResToData()
{
	string _path = FileUtils::getInstance()->fullPathForFilename("sqlite/jump.zyl");
	string _writePath = FileUtils::getInstance()->getWritablePath()+"jump.zyl";
	
	unsigned const char* _data = nullptr;
	ssize_t _len;
	_data = FileUtils::getInstance()->getFileData(_path, "r", &_len);
	FILE* _pfile = fopen(_writePath.c_str() , "r");
	if (!_pfile)
	{
		FILE* _pfile1 = fopen(_writePath.c_str() , "w+");
		fwrite(_data, _len, 1, _pfile1);
		fclose(_pfile1);
	}
	else
	{
		fclose(_pfile);
	}
}

long MySqilteObjec::searchIntDataFromTable(string _tableName, string _target , string _where , string _whrValue)
{
	string _valueStr = searchStringDataFromTable(_tableName, _target, _where, _whrValue);
	return atol(_valueStr.c_str());
}
void MySqilteObjec::setIntDataValue(string _tableName, string _target, int _value, string _where, string _whrValue)
{
	char _valueInt[50];
	sprintf(_valueInt, "%d", _value);
	this->setStringDataValue(_tableName, _target, _valueInt, _where, _whrValue);
}
float MySqilteObjec::searchFloatDataFromTable(string _tableName, string _target, string _where, string _whrValue)
{
	string _valueStr = searchStringDataFromTable(_tableName, _target, _where, _whrValue);
	return atof(_valueStr.c_str());
}
void MySqilteObjec::setFloatDataValue(string _tableName, string _target, float _value, string _where , string _whrValue )
{
	char _valueInt[50];
	sprintf(_valueInt, "%f", _value);
	this->setStringDataValue(_tableName, _target, _valueInt, _where, _whrValue);
}