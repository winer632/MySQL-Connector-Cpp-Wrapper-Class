 /*
 * =====================================================================================
 *
 *       Filename:  mysqlconn_wrapper.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016/8/15 11:08:21
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wangxinxin, winer632@qq.com
 *   Organization:  lj
 *
 * =====================================================================================
 * ============================== CHANGE REPORT HISTORY ================================
 * |   VERSION   |   UPDATE DATE   |   UPDATED BY   |   DESCRIPTION OF CHANGE   |*
 
 * =========================== END OF CHANGE REPORT HISTORY ============================
 */
 /*
preinstall:
yum install -y boost boost-devel boost-doc
 */

#include "mysql_connection.h"
	
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include "mysqlconn_wrapper.h"

using namespace std;

MySQLConnWrapper::~MySQLConnWrapper()
{
	delete res;
	delete prep_stmt;
	delete stmt;
	delete con;
}


void MySQLConnWrapper::init(string h, string u, string p)
{
	host=h;
	user=u;
	password=p;
}

string MySQLConnWrapper::getHost()
{
	return host;
}

string MySQLConnWrapper::getUser()
{
	return user;
}

string MySQLConnWrapper::getPassword()
{
	return password;
}

void MySQLConnWrapper::manageException(sql::SQLException& e)
{
	if (e.getErrorCode() != 0){
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
}

void MySQLConnWrapper::connect()
{
	try{
		driver = get_driver_instance();
		con = driver->connect(host, user, password);
	} catch (sql::SQLException &e){
		manageException(e);
	}
}

void MySQLConnWrapper::switchDb(const string& db_name)
{
	try{
		con->setSchema(db_name);
		stmt = con->createStatement();
	} catch (sql::SQLException &e) {
		manageException(e);
	}
}

void MySQLConnWrapper::prepare(const string& query)
{
	try{
		prep_stmt = con->prepareStatement(query);
	} catch (sql::SQLException &e){
		manageException(e);
	}
}

void MySQLConnWrapper::setInt(const int& num, const int& data)
{
	prep_stmt->setInt(num, data);
}

void MySQLConnWrapper::setString(const int& num, const string& data)
{
	prep_stmt->setString(num, data);
}

void MySQLConnWrapper::execute(const string& query)
{
	try {
		if (query != "") {
			res = stmt->executeQuery(query);
		} else {
			res = prep_stmt->executeQuery();
		}
	} catch (sql::SQLException &e) {
		manageException(e);
	}
}

void MySQLConnWrapper::closeCon()
{
	con->close();
}


bool MySQLConnWrapper::fetch()
{
	return res->next();
}

string MySQLConnWrapper::getString(const string& field)
{
	return res->getString(field);
}

string MySQLConnWrapper::getString(const int& index)
{
	return res->getString(index);
}

int MySQLConnWrapper::getInt(const string& field)
{
	return res->getInt(field);
}

int MySQLConnWrapper::getInt(const int& index)
{
	return res->getInt(index);
}


sql::ResultSet* MySQLConnWrapper::getRes()
{
	return res;
}

