#pragma once
#include <mysql/mysql.h>
#include <string.h>
#include <string>
#include <iostream>
#include <vector>
using namespace std;

#define SQL_HOST "localhost"
#define SQL_USER "ik"
#define SQL_PASSWORD "123456"
#define SQL_DATEBASE "bigdata"

class DateBase
{
protected:
	DateBase()
	{
		connect_ = mysql_init(nullptr);
		if (mysql_real_connect(connect_, SQL_HOST, SQL_USER, SQL_PASSWORD, SQL_DATEBASE, 0, nullptr, 0) == nullptr)
		{
			exit(0);
		}
	}

	~DateBase()
	{
		mysql_close(connect_);
	}

	//��table��������value_vec����Ϣ
	void sql_add(string table,vector<string>& value_vec)
	{
		string add_com = "INSERT INTO ";
		add_com += table;
		add_com += " VALUES (";

		for (int i=0;i<value_vec.size();i++)
		{
			add_com = add_com + value_vec[i].c_str();
			if(i!=value_vec.size()-1)
			{
				add_com += ",";
			}
		}
		add_com = add_com + ");";


		if (general_operation(add_com.c_str()) == 0)
		{
			cout << "sql add error!" << endl;
		}
	}

	//��table���в�ѯ����cond��������Ϣ
	MYSQL_RES* sql_select(string table, string cond = "")
	{
		string sel_com = "SELECT * FROM ";
		sel_com += table;
		if (cond.empty())
		{
			sel_com += " ;";
		}
		else
		{
			sel_com = sel_com + " WHERE " + cond + " ;";
		}
		//cout << sel_com << endl;
		MYSQL_RES* res = select_operation(sel_com.c_str());
		if (res == nullptr)
		{
			cout << "sql select error!" << endl;
		}

		return res;
	}

	//�õ�table���е�����
	int sql_get_rownumber(string table)
	{
		string sql = "SELECT COUNT(*) FROM ";
		sql += table;
		sql += ";";
		MYSQL_RES* res = select_operation(sql.c_str());
		if (res == nullptr)
		{
			cout << "get row number error!" << endl;
		}
		
		MYSQL_ROW row = mysql_fetch_row(res);
		string ret = row[0];

		return atoi(ret.c_str());
	}

	//�õ�table��������conditions����������
	int sql_get_rownumber(string table,string conditions)
	{
		string sql = "SELECT COUNT(*) FROM ";
		sql += table;
		if (!conditions.empty())
		{
			sql = sql + " WHERE " + conditions + ";";
		}
		//cout << sql << endl;
		MYSQL_RES* res = select_operation(sql.c_str());
		if (res == nullptr)
		{
			cout << "get row number error!" << endl;
		}
		
		MYSQL_ROW row = mysql_fetch_row(res);
		string ret = row[0];

		return atoi(ret.c_str());
	}

	//�õ�table������ɸѡ������conditions��������Ϣ���ٸ���group����������ͳ��
	vector<pair<string,int>> sql_get_rownumber_bygroup(string table,string group, string conditions = "")
	{
		string sql = "SELECT ";
		sql += group;
		sql+=" ,COUNT(*) FROM ";
		sql += table;
		if (!conditions.empty())
		{
			sql = sql + " WHERE " + conditions;
		}
		
		sql += " GROUP BY ";
		sql = sql + group + ";";
		
		MYSQL_RES* res = select_operation(sql.c_str());
		if (res == nullptr)
		{
			cout << "get row number by group error!" << endl;
		}
		
		vector<pair<string,int>> vec;
		while (MYSQL_ROW row = mysql_fetch_row(res))
		{
			string first = row[0];
			string second = row[1];
			vec.push_back(pair<string, int>(first, atoi(second.c_str())));
		}

		return vec;
	}

	//ɾ��table��������cond��������Ϣ
	void sql_delete(string table, string cond = "")
	{
		string del_com = "DELETE FROM ";
		del_com = del_com + table;
		if (cond.empty())
		{
			del_com = del_com + " WHERE " + cond + ";";
		}
		else
		{
			return;
		}

		if (general_operation(del_com.c_str()) == 0)
		{
			cout << "sql delete error!" << endl;
		}
	}

	//������cond��������table���и���str�������Ϣ
	void sql_update(string table, string str, string cond = "")
	{
		string update_com = "UPDATE ";
		update_com = update_com + table + " SET " + str;
		if (cond.empty())
		{
			update_com += " ;";
		}
		else
		{
			update_com = update_com + " WHERE " + cond + ";";
		}
		
		if (general_operation(update_com.c_str()) == 0)
		{
			cout << "sql update error!" << endl;
		}
	}

public:
	//��ӡ��ѯ����������
	void print_res(MYSQL_RES* res)
	{
		while (MYSQL_ROW row = mysql_fetch_row(res))
		{
			for (int i = 0; i < mysql_num_fields(res); i++) {
				cout << row[i] << "  ";
			}
			cout << endl;
		}
	}

	//�ͷŲ�ѯ�����ݽṹ
	void free_res(MYSQL_RES* res)
	{
		mysql_free_result(res);
	}
private:
	//select ѡ�����
	MYSQL_RES* select_operation(const char* str)
	{
		if (mysql_real_query(connect_, str, strlen(str)) != 0)
		{
			cout << "select operation error!" << endl;
			return nullptr;
		}
		return mysql_store_result(connect_);
	}

	//ͨ�ò���
	bool general_operation(const char* str)
	{
		bool flag = true;
		if (mysql_real_query(connect_, str, strlen(str)) != 0)
		{
			flag = false;
		}
		return flag;
	}

private:
	MYSQL* connect_;
};