// hiddentest.cpp : Defines the entry point for the console application.
// This file has code to read a xml file and create a structure out of it.
//

#include "stdafx.h"
#include <iostream>
#include "tinyxml.h"
#include "hidden_points.h"

#define ERROR_PARSE 1

PointManager g_points_table;

#pragma comment (lib, "C:\\Users\\jkhurana\\Documents\\visual studio 2015\\Projects\\tinyxml_2_6_2\\tinyxml\\Debugtinyxml\\tinyxml.lib")

int helper_load_settings(TiXmlNode* pParent);

int PointManager::load_settings(char* file_name)
{
	TiXmlDocument doc(file_name);
	int error = 0;
	bool load_okay = doc.LoadFile();
	TiXmlNode* pParent = &doc;
	if (load_okay)
	{
		error = helper_load_settings(pParent);
	}
	else
		error = 1;
	return error;
}

void PointManager::insert(char* function, Point* pt)
{
		std::pair <map<char*, Point*>::iterator, bool> ret;
		
		char* key = _strdup(function);
		
		if (!function) return;
		/* Insert into the map*/
		ret = point_table.insert(pair<char*, Point*>(key, pt));
		//Ignore any error

		assert (ret.second == true);

		return;
	
}
int helper_load_settings(TiXmlNode* pParent)
{
	/* Start reading one item at a time and populate the */
	if (!pParent) return 1;

	TiXmlNode* pChild;
	TiXmlText* pText;
	int t = pParent->Type();
	switch (t)
	{
	case TiXmlNode::TINYXML_DOCUMENT:
		break;

	case TiXmlNode::TINYXML_ELEMENT:
		//	We need to populate the attributes
		// EIther Function Point or FunctionPoint1 will work
		if (strlen(pParent->Value()) >= strlen("FunctionPoint") &&
			_strnicmp(pParent->Value(), "FunctionPoint", strlen("FunctionPoint")) == 0)
		{
			TiXmlAttribute* pAttrib = pParent->ToElement()->FirstAttribute();
			int order = 0;
			char* func_name = NULL, *arg_list = NULL;
			while (pAttrib)
			{
				/*
				Each function point needs to be populkated in the map.
				*/
				if (_stricmp(pAttrib->Name(), "function_name") == 0)
				{

					//populate the Point function name.
					func_name = const_cast <char*> (pAttrib->Value());
					//printf("Point %s, Function name is %s\n ", pParent->Value(), pAttrib->Value());
					order++;
				}
				else if (_stricmp(pAttrib->Name(), "arg_list") == 0)
				{
					arg_list = const_cast <char *>(pAttrib->Value());
					//printf("Point %s arg list is %s\n ", pParent->Value(), pAttrib->Value());
					order++;
				}
				pAttrib = pAttrib->Next();
			}
			// All attributes of structure found in the correct order.
			assert(order == 2);
			if (order == 2)
			{
				/*
				* We have a point.
				*/
				Point *point = new Point(func_name, arg_list, 0, 0);
				g_points_table.insert(func_name, point);
			}
			else
			{
				printf("Could not validate the xml entry : skipping it");
			}
		}
		break;

	case TiXmlNode::TINYXML_COMMENT:
		break;

	case TiXmlNode::TINYXML_UNKNOWN:
		break;

	case TiXmlNode::TINYXML_TEXT:
		pText = pParent->ToText();
		break;

	case TiXmlNode::TINYXML_DECLARATION:
		break;
	default:
		break;
	}
	for (pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling())
	{
		int error = helper_load_settings(pChild);
		if (error != 0)
			return error;
	}
	return 0;
}

void PointManager::print()
{
	std::map<char*, Point*>::iterator it;

	cout << "The function points from the xml are::\n";
	for (it = g_points_table.point_table.begin(); it != g_points_table.point_table.end(); it++)
	{
		char* fn = it->first;
		Point * second = it->second;

		cout << "Function name::" << fn << *(dynamic_cast <Point*>(it->second));
	}
}
int main(int argc, char*argv[])
{
	if (argc != 2)
		printf("Syntax error :Error please specify filename %d", argc);
	else
	{
		g_points_table.load_settings(argv[1]);
	}
	g_points_table.print();

	return 0;
}
