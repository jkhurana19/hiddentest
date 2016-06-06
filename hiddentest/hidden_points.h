#pragma once
#include <map>
using namespace std;

typedef struct context_
{
	string function_name;
	string arg_list;
	//char** argv;
	int flags;
	int refcount_unused;
	struct context_(char*func, char* arglist, int flg, int refcnt)
	{
		/* TBD handle allocation failures*/
		function_name = _strdup (func);
		flags = flg;
		refcount_unused = refcnt;
		arg_list = _strdup (arglist);
	}
} Context;

class Point
{
private:
	static const int enabled = 1;
	//static const int disabled = 0;
	Context point_ctx;
public:
	Point(char*function_in, char* arg_in, int flags, int refcount_unused) : point_ctx(function_in,arg_in, 0 ,0)
	{
	//	cout << "Function" << function_in << "Argument" << arg_in;
	}
	
	friend ostream & operator <<(ostream &os, Point& point)
	{
		return os << "arg list" << point.point_ctx.arg_list.c_str ();
	}
	
};

class PointManager {
private:
	map <char*, Point *> point_table;
public:
	// Load the the settings from a file into this map. 
	int load_settings(char* file_name);
	void insert(char* file_name, Point *pt);
	void print();
};
