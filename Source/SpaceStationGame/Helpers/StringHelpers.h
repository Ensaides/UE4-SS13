// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SpaceStationGame.h"

#include <iostream>
#include <sstream>
#include <string.h>
#include <locale.h>

namespace Helpers
{
	std::wstring widen(const std::string& str) {
		std::wostringstream wstm;
		const std::ctype<wchar_t>& ctfacet = std::use_facet <std::ctype<wchar_t> >(wstm.getloc());
		for (std::size_t i = 0; i < str.size(); ++i)
			wstm << ctfacet.widen(str[i]);
		return wstm.str();
	};

	std::string narrow(const std::wstring& str) {
		std::ostringstream stm;
		const std::ctype<char>& ctfacet =
			std::use_facet< std::ctype<char> >(stm.getloc());
		for (size_t i = 0; i < str.size(); ++i)
			stm << ctfacet.narrow(str[i], 0);
		return stm.str();
	};

	std::wstring ConvertToWString(FString str) 
	{
		return widen((TCHAR_TO_UTF8(*str)));
	};

	std::string ConvertToString(FString str) 
	{
		return (TCHAR_TO_UTF8(*str));
	};

	FString ConvertToFString(std::string str) 
	{
		return (FString(str.c_str()));
	};

	FString ConvertToFString(std::wstring str) 
	{
		std::string o = narrow(str);
		return (FString(o.c_str()));
	};


	const char* ConvertToCCP(FString str) 
	{
		std::string t = TCHAR_TO_UTF8(*str);
		char * returnvalue = (char *)malloc(sizeof(char)* (t.length() + 1));
		strncpy_s(returnvalue, t.length(), t.c_str(), t.length());
		return returnvalue;
	};

	const char* ConvertToCCP(std::string str) 
	{
		char * returnvalue = (char *)malloc(sizeof(char)* (str.length() + 1));
		strncpy_s(returnvalue, str.length(), str.c_str(), str.length());
		return returnvalue;
	};

	const char* ConvertToCCP(std::wstring str) 
	{
		std::string t = narrow(str);
		char * returnvalue = (char *)malloc(sizeof(char)* (t.length() + 1));
		strncpy_s(returnvalue, t.length(), t.c_str(), t.length());
		return returnvalue;
	};


	std::string int64ToString(int64 number) 
	{
		std::stringstream stream;
		__int64 value(number);
		stream << value;
		std::string strValue(stream.str());
		return strValue;
	};

	std::wstring int64ToWString(int64 number) 
	{
		std::stringstream stream;
		__int64 value(number);
		stream << value;
		std::string strValue(stream.str());
		return widen(strValue);
	};
}