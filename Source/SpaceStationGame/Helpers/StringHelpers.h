// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <locale.h>

class Helpers
{
public:
	static std::wstring widen(const std::string& str)
	{
		std::wostringstream wstm;
		const std::ctype<wchar_t>& ctfacet = std::use_facet <std::ctype<wchar_t> >(wstm.getloc());
		for (std::size_t i = 0; i < str.size(); ++i)
			wstm << ctfacet.widen(str[i]);
		return wstm.str();
	};

	static std::string narrow(const std::wstring& str)
	{
		std::ostringstream stm;
		const std::ctype<char>& ctfacet =
			std::use_facet< std::ctype<char> >(stm.getloc());
		for (size_t i = 0; i < str.size(); ++i)
			stm << ctfacet.narrow(str[i], 0);
		return stm.str();
	};

	static std::wstring ConvertToWString(FString str) 
	{
		return widen((TCHAR_TO_UTF8(*str)));
	};

	static std::string ConvertToString(FString str) 
	{
		return (TCHAR_TO_UTF8(*str));
	};

	static FString ConvertToFString(std::string str) 
	{
		return (FString(str.c_str()));
	};

	static FString ConvertToFString(std::wstring str) 
	{
		std::string o = narrow(str);
		return (FString(o.c_str()));
	};


	static const char* ConvertToCCP(FString str) 
	{
		std::string t = TCHAR_TO_UTF8(*str);
		char * returnvalue = (char *)malloc(sizeof(char)* (t.length() + 1));
		strncpy_s(returnvalue, t.length(), t.c_str(), t.length());
		return returnvalue;
	};

	static const char* ConvertToCCP(std::string str) 
	{
		char * returnvalue = (char *)malloc(sizeof(char)* (str.length() + 1));
		strncpy_s(returnvalue, str.length(), str.c_str(), str.length());
		return returnvalue;
	};

	static const char* ConvertToCCP(std::wstring str) 
	{
		std::string t = narrow(str);
		char * returnvalue = (char *)malloc(sizeof(char)* (t.length() + 1));
		strncpy_s(returnvalue, t.length(), t.c_str(), t.length());
		return returnvalue;
	};


	static std::string int64ToString(int64 number) 
	{
		std::stringstream stream;
		__int64 value(number);
		stream << value;
		std::string strValue(stream.str());
		return strValue;
	};

	static std::wstring int64ToWString(int64 number) 
	{
		std::stringstream stream;
		__int64 value(number);
		stream << value;
		std::string strValue(stream.str());
		return widen(strValue);
	};
};