#pragma once
#include <mysql/mysql.h>
#include "charconv.h"
#include <string>

class MysqlDb;
void logsqlstr(const char *sql, int ret, MysqlDb *db);
class MysqlDb
{
public:
	MysqlDb()
	{
		_handle=mysql_init(NULL);
	}
	~MysqlDb()
	{
		close();
	}
	class ConnOpt
	{
	public:
		std::string _db;
		std::string _server;
		std::string _user;
		std::string _password;
		unsigned int _port;
		unsigned int _clientflags;
	};
	bool connect_emb(const char *dbname)
	{
		return (mysql_real_connect(_handle, NULL,NULL,NULL, dbname, 0,NULL,0)!=NULL);
	}
	bool connect(const char* db, const char* server = 0, const char* user = 0,const char* password = 0, unsigned int port = 0,unsigned int client_flag=0,const char *socket_name=NULL)
	{
		return(mysql_real_connect(_handle, server, user, password, db,port, socket_name, client_flag)!=NULL);
	}
	bool connect(const ConnOpt &opt)
	{
		return connect(opt._db.c_str(),opt._server.c_str(),opt._user.c_str(),opt._password.c_str(),opt._port,opt._clientflags,NULL);
	}
	bool close()
	{
		if(_handle!=NULL)
		{
			mysql_close(_handle);
			_handle=NULL;
		}
		return true;
	}
	int exec(const char *sql,int sqllen=0)
	{
		if(sqllen<=0)
			sqllen=(int)strlen(sql);
		
		int ret=mysql_real_query(_handle,sql,sqllen);
		logsqlstr(sql,ret,this);
		return ret;
	}

	int exec2(const char *pszFormat, ...)
	{
		char tempsqlbuf[4096]={0};
		va_list argptr;
		va_start(argptr, pszFormat);
		int n = _vsnprintf(tempsqlbuf, sizeof(tempsqlbuf)-1, pszFormat, argptr);
		va_end(argptr);
		if(n<=0)
			return -1;
		return exec(tempsqlbuf);
	}

	int begin_transaction()
	{
		return exec("START TRANSACTION");
	}
	int begin_transaction_readonly()
	{
		return exec("START TRANSACTION READ ONLY");
	}
	int commit_transaction()
	{
		return exec("COMMIT");
	}
	int rollback_transaction()
	{
		return exec("ROLLBACK");
	}
	bool ping()
	{
		return (mysql_ping(_handle)==0);
	}
	bool options(enum mysql_option optionvalue, const char *arg)
	{
		return (mysql_options(_handle,optionvalue,arg)==0);
	};
	int errnum(){return mysql_errno(_handle);}
	const char *errstr(){return mysql_error(_handle);}

	uint64_t insert_id()
	{
		return mysql_insert_id(_handle);
	}

	MYSQL *_handle;

	std::string escape(const std::string &buf)
	{
		return escape(buf.c_str());
	}

	std::string escape(const char *buf)
	{
		if(buf==NULL)
		{
			return std::string("");
		}
		std::string result;
		int fromlen=(int)strlen(buf);
		if(fromlen<=0)
			return std::string("");
		result.resize(fromlen*2+1);
		unsigned resultlen=mysql_real_escape_string(_handle,(char *)result.c_str(),buf,fromlen);
		if(resultlen==0)
			return std::string("");
		result.resize(resultlen);
		return result;
	}


};

class MysqlTransaction
{
public:
	MysqlTransaction(MysqlDb &db,bool readonly=false)
		:_db(db)
	{
		if(readonly)
		{
			db.begin_transaction_readonly();
		}
		else
		{
			db.begin_transaction();
		}
		_handled=false;
	}
	~MysqlTransaction()
	{
		rollback();
	}
	int rollback()
	{
		if(_handled)
			return 0;
		int nret=_db.rollback_transaction();
		_handled=true;
		return nret;
	}
	int commit()
	{
		if(_handled)
			return 0;
		int nret=_db.commit_transaction();
		_handled=true;
		return nret;
	}

	int save_point(int id)
	{
		char buf[64];
		sprintf(buf,"SAVEPOINT %d",id);
		return _db.exec(buf);
	}
	int roll_point(int id)
	{
		char buf[64];
		sprintf(buf,"ROLLBACK TO %d",id);
		return _db.exec(buf);
	}

	int save_point(const char *id)
	{
		char buf[64];
		sprintf(buf,"SAVEPOINT %s",id);
		return _db.exec(buf);
	}
	int roll_point(const char * id)
	{
		char buf[64];
		sprintf(buf,"ROLLBACK TO %s",id);
		return _db.exec(buf);
	}

	int save_point(const std::string &id)
	{
		char buf[64];
		sprintf(buf,"SAVEPOINT %s",id.c_str());
		return _db.exec(buf);
	}
	int roll_point(const std::string &id)
	{
		char buf[64];
		sprintf(buf,"ROLLBACK TO %s",id.c_str());
		return _db.exec(buf);
	}

	MysqlDb &_db;
	bool _handled;
};

class MysqlQuery
{
public:
	MysqlQuery(MysqlDb &db)
		:_db(db)
	{
		_result=NULL;
		_rows=0;
		_cur_row=NULL;
		_cur_lengths=NULL;
		_num_fields=0;
	}
	~MysqlQuery()
	{
		freeresult();
	}
	bool exec(const std::string &sql)
	{
		return exec(sql.c_str(),(int)sql.size());
	}
	bool exec2(const char *pszFormat, ...)
	{
		char tempsqlbuf[4096]={0};
		va_list argptr;
		va_start(argptr, pszFormat);
		int n = _vsnprintf(tempsqlbuf, sizeof(tempsqlbuf)-1, pszFormat, argptr);
		va_end(argptr);
		if(n<=0)
			return false;
		return exec(tempsqlbuf);
	}
	bool exec_no_result(const char *sql,int sqllen=0)
	{
		if(sqllen==0)
			sqllen=(int)strlen(sql);
		if(sqllen==0)
			return false;
		int queryret=_db.exec(sql,sqllen);
		if(queryret!=0)
			return false;
		return true;
	}
	bool exec_no_result(const std::string &sql)
	{
		return exec_no_result(sql.c_str(), sql.length());
	}
	bool exec(const char *sql,int sqllen=0)
	{
		_rows=0;
		_num_fields=0;
		int queryret=_db.exec(sql,sqllen);
		if(queryret!=0)
			return false;
		_result = mysql_store_result(_db._handle);
		if(_result!=NULL)
		{
			_num_fields=mysql_num_fields(_result);
			_rows=mysql_num_rows(_result);
		}
		return true;
	}
	bool exec_and_fetch_row(const char *sql,int sqllen=0)
	{
		if(!exec(sql,sqllen))
			return false;
		return fetch_row();
	}
	bool exec_and_fetch_row(const std::string &sql)
	{
		return exec_and_fetch_row(sql.c_str(), sql.length());
	}
	void freeresult()
	{
		if(_result!=NULL)
		{
			mysql_free_result(_result);

			while(!mysql_next_result(_db._handle))
			{
				_result = mysql_store_result(_db._handle);
				if(_result)
				{
					mysql_free_result(_result);
				}
			}
			_result=NULL;
		}
	}
	int affected_rows()
	{
		return (int)mysql_affected_rows(_db._handle);
	}
public:
	unsigned long long num_rows()
	{
		return _rows;
	}
	unsigned long num_of_fields()
	{
		return _num_fields;
	}
	bool fetch_row()
	{
		_cur_row = mysql_fetch_row(_result);
		_cur_lengths=mysql_fetch_lengths(_result);
		return _cur_row!=NULL;
	}

	const std::string get_utf8_str(int index)
	{
		const char *orgvalue=getstr(index);
		return gbk_2_utf(orgvalue);
	}

	const std::string get_utf8_str(int index,const char *defaultstr)
	{
		const char *orgvalue=getstr(index,defaultstr);
		return gbk_2_utf(orgvalue);
	}

	const char *getstr(int index)
	{
		if(_cur_lengths[index]==0)
			return "";
		if(_cur_row[index]==0)
			return "";
		return _cur_row[index];
	}
	const char *getstr(int index,const char *defaultstr)
	{
		if(_cur_lengths[index]==0)
			return defaultstr;
		if(_cur_row[index]==0)
			return defaultstr;
		return _cur_row[index];
	}
	int getint(int index,int defaultint=-1)
	{
		const char *str=getstr(index);
		if(str==NULL)
			return defaultint;
		return atoi(str);
	}
	unsigned int getuint(int index,unsigned int defaultuint=0)
	{
		const char *str=getstr(index);
		if(str==NULL)
			return defaultuint;
		return strtoul(str,NULL,10);
	}
	unsigned int gettime_t(int index,unsigned int defaultint=-1)
	{
		const char *str=getstr(index);
		if(str==NULL)
			return defaultint;

		char num[5];
		num[0] = *str++;
		num[1] = *str++;
		num[2] = *str++;
		num[3] = *str++;
		num[4] = 0;
		int year = strtol(num, 0, 10);
		if (*str == '-') str++;
		num[0] = *str++;
		num[1] = *str++;
		num[2] = 0;
		int month = (strtol(num, 0, 10));
		if (*str == '-') str++;
		num[0] = *str++;
		num[1] = *str++;
		num[2] = 0;
		int day = (strtol(num, 0, 10));

		if (*str == ' ') ++str;
		num[0] = *str++;
		num[1] = *str++;
		num[2] = 0;
		int hour = (strtol(num,0,10));
		if (*str == ':') str++;
		num[0] = *str++;
		num[1] = *str++;
		num[2] = 0;
		int minute = (strtol(num,0,10));
		if (*str == ':') str++;
		num[0] = *str++;
		num[1] = *str++;
		num[2] = 0;
		int second = (strtol(num,0,10));

		struct tm tm;
		tm.tm_sec = second;
		tm.tm_min = minute;
		tm.tm_hour = hour;
		tm.tm_mday = day;
		tm.tm_mon = month - 1;
		tm.tm_year = year - 1900;
		tm.tm_isdst = -1;
		return (unsigned int)_mktime32(&tm);
	}
	double getdouble(int index,double defaultdouble=0.0f)
	{
		const char *str=getstr(index);
		if(str==NULL)
			return defaultdouble;
		return atof(str);
	}
	long long getint64(int index,long long defaultint=-1)
	{
		const char *str=getstr(index);
		if(str==NULL)
			return defaultint;
		return _atoi64(str);
	}
	unsigned long long getuint64(int index,unsigned long long defaultint=0)
	{
		const char *str=getstr(index);
		if(str==NULL)
			return defaultint;
		return static_cast<unsigned __int64>(_atoi64(str));
	}
	const unsigned char *getblob(int index,int &nlen)
	{
		if(_cur_lengths==NULL)
			return NULL;
		nlen=_cur_lengths[index];
		return (const unsigned char *)_cur_row[index];
	}
	//mysql_fetch_field_direct
	MYSQL_FIELD *fetch_field(int index)
	{
		return mysql_fetch_field_direct(_result,index);
	}

	const char* errormsg()
	{
		return _db.errstr();
	}

private:
	MysqlDb &_db;
	MYSQL_RES *_result;
	unsigned long long _rows;
	MYSQL_ROW _cur_row;
	unsigned int _num_fields;
	unsigned long *_cur_lengths;
};


class MysqlStatement
{
public:
	MysqlStatement(MysqlDb &db)
		:_db(db)
	{
		_stmt=mysql_stmt_init(_db._handle);
	}
	~MysqlStatement()
	{
		close();
	}
	void close()
	{
		if(_stmt!=NULL)
		{
			mysql_stmt_close(_stmt);
			_stmt=NULL;
		}
	}
	bool prepare_param(const char *sqlstr,int sqllen=0)
	{
		if(sqllen==0)
			sqllen=(int)strlen(sqlstr);
		if(sqllen==0)
			return false;
		int prepareret=mysql_stmt_prepare(_stmt,sqlstr,sqllen);
		if(prepareret!=0)
			return false;
		int paramcount=mysql_stmt_param_count(_stmt);
		if(paramcount<=0)
			return false;
		_binds.resize(paramcount);
		for(int i=0;i<paramcount;i++)
		{
			memset(&_binds[i],0,sizeof(MYSQL_BIND));
		}
		return true;
	}
	bool prepare_result(const char *sqlstr,int sqllen=0)
	{
		if(sqllen==0)
			sqllen=(int)strlen(sqlstr);
		if(sqllen==0)
			return false;
		int prepareret=mysql_stmt_prepare(_stmt,sqlstr,sqllen);
		if(prepareret!=0)
			return false;
		MYSQL_RES *mtdata= mysql_stmt_result_metadata(_stmt);
		if(mtdata==NULL)
			return false;

		int resultlen=mysql_num_fields(mtdata);
		mysql_free_result(mtdata);
		_binds.resize(resultlen);
		for(int i=0;i<resultlen;i++)
		{
			memset(&_binds[i],0,sizeof(MYSQL_BIND));
		}
		return true;
	}
	bool prepare_param(const std::string &sqlstr){return prepare_param(sqlstr.c_str(),(int)sqlstr.size());}
	bool prepare_result(const std::string &sqlstr){return prepare_result(sqlstr.c_str(),(int)sqlstr.size());}
	const char *errstr(){return mysql_stmt_error(_stmt);}
public:
	void bind(int index,enum_field_types paramtype,void *paramdata,unsigned int paramdatalen,my_bool *pisnullind=NULL,unsigned long *pparamdatalen=NULL)
	{
		if(index<0)return;
		if(index>=(int)_binds.size())return;
		MYSQL_BIND &curbind=_binds[index];

		curbind.buffer_type= paramtype;
		curbind.buffer= paramdata;
		curbind.buffer_length= paramdatalen;
		//out�Ĳ���������
		curbind.is_null= pisnullind;
		curbind.length= pparamdatalen;
	}
	bool bind_param()
	{
		return (mysql_stmt_bind_param(_stmt,&_binds[0])==0);
	}
	bool bind_result()
	{
		return (mysql_stmt_bind_result(_stmt,&_binds[0])==0);
	}
	int execute_no_result()
	{
		int exeresult=mysql_stmt_execute(_stmt);
		if(exeresult!=0)
			return -1;
		return (int)mysql_stmt_affected_rows(_stmt);
	}

	bool execute_with_result()
	{
		int exeresult=mysql_stmt_execute(_stmt);
		if(exeresult!=0)
			return false;
		if(!bind_result())
			return false;
		if(mysql_stmt_store_result(_stmt)!=0)
			return false;
		return true;
	}
	bool fetch()
	{
		return (mysql_stmt_fetch(_stmt)==0);
	}
public:
	MysqlDb &_db;
	MYSQL_STMT *_stmt;
	std::vector<MYSQL_BIND> _binds;
};