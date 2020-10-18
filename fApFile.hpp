#pragma once

#include <iostream>
#include <fstream>
#include <tchar.h>
#include <string>
#include <stack>
#include <codecvt>
#include <windows.h>
#include <locale>
#include <sstream>

using namespace std;

enum FILE_TYPE
{
	TYPE_UTF8_BOM,
	TYPE_UTF8,
	TYPE_GBK,
	TYPE_UNICODE,
	TYPE_UNICODE_BIG,
	TYPE_ANSI
};

class fApFile
{
public:
	//��ʼ��
	fApFile();
	//�ж��Ƿ�����BOM��utf-8����
	BOOL IsUTF8(const void* pBuffer, long size);
	//��ȡ�ļ���ʽ
	INT getFileType(string path);

	//utf-8תGBK
	string utf2gbk(const string& res);
	//GBKתutf-8
	string gbk2utf(const string& res);
	//utf-8תunicode
	string uncode2utf(const string& res);
	//unicodeתutf-8
	string utf2unicode(const string& res);

	//wstringתstring
	string ws2s(const wstring& src);
	//stringתwstring
	wstring s2ws(const string& src);
	//wstringתutf-8
	string ws2utf8(const wstring& src);
	//utf-8תwstring
	wstring utf8_2_ws(const string& src);
	//��ȡANSI�����ļ�
	BOOL readANSI();
	//��ȡ��BOM utf-8�����ļ�
	BOOL readUTF8();
	//��ȡ��BOM utf-8�����ļ�
	BOOL readUTF8BOM();
	//��ȡUnicode�����ļ�
	BOOL readUnicode();
	//��ȡ�ļ��������ļ����ݵ��ڴ�
	BOOL readFileAndsave(string path);
	//ȡ���������ڴ��е��ļ���Ϣ
	string getContent();
	//д���ļ�
	BOOL write2Txt(const string& content, const string& name = "out");


	//�ļ���--д��
	fstream forWrite;
	//д���ļ���·��
	string writePath;
	//�Ѷ�ȡ���ļ��ı����ʽ
	INT readType;
private:
	//�ļ���--��ȡ
	fstream forRead;
	
	//�ļ���--��ȡ��д��
	fstream forAll;
	//�����ļ�����
	string fileStr;
	//д���ļ�������
	//string writeStr;
	//��ȡ�ļ���·��
	string readPath;
	//rwFile��·��
	string rwPath;
};

/*-----------------------------fApFile-----------------------------*/

//��ʼ��
fApFile::fApFile()
{
	fileStr = "";
	//writeStr = "";
	readType = -1;
	readPath = "";
	writePath = "";
	rwPath = "";
}

inline BOOL fApFile::IsUTF8(const void* pBuffer, long size)
{
	bool IsUTF8 = true;
	unsigned char* start = (unsigned char*)pBuffer;
	unsigned char* end = (unsigned char*)pBuffer + size;
	while (start < end)
	{
		if (*start < 0x80) // (10000000): ֵС��0x80��ΪASCII�ַ�   
		{
			start++;
		}
		else if (*start < (0xC0)) // (11000000): ֵ����0x80��0xC0֮���Ϊ��ЧUTF-8�ַ�   
		{
			IsUTF8 = false;
			break;
		}
		else if (*start < (0xE0)) // (11100000): �˷�Χ��Ϊ2�ֽ�UTF-8�ַ�   
		{
			if (start >= end - 1)
				break;
			if ((start[1] & (0xC0)) != 0x80)
			{
				IsUTF8 = false;
				break;
			}
			start += 2;
		}
		else if (*start < (0xF0)) // (11110000): �˷�Χ��Ϊ3�ֽ�UTF-8�ַ�   
		{
			if (start >= end - 2)
				break;
			if ((start[1] & (0xC0)) != 0x80 || (start[2] & (0xC0)) != 0x80)
			{
				IsUTF8 = false;
				break;
			}
			start += 3;
		}
		else
		{
			IsUTF8 = false;
			break;
		}
	}
	return IsUTF8;
}

//��ȡ�ļ���ʽ
inline INT fApFile::getFileType(string path)
{
	readPath.assign(path);
	forRead.open(readPath, ios::in | ios::binary);

	if (!forRead.is_open()) return -1;

	char* buffer = new char[20];
	char* buf = new char[1];

	forRead.read(buf, 1);

	WORD type = *buf;
	type = type << 8;

	forRead.read(buf, 1);
	WORD temp = *buf;
	temp &= 0xff;
	type |= temp;

	switch (type)
	{
	case 0xfffe:
		readType = TYPE_UNICODE;
		return TYPE_UNICODE;
	case 0xfeff:
		readType = TYPE_UNICODE_BIG;
		return TYPE_UNICODE_BIG;
	case 0xefbb:
		readType = TYPE_UTF8_BOM;
		return TYPE_UTF8_BOM;
	default:
		forRead.seekg(0, SEEK_SET);

		forRead.read(buffer, 20);

		if (IsUTF8((void*)buffer, 20))
		{
			readType = TYPE_UTF8;
			return TYPE_UTF8;
		}

		readType = TYPE_ANSI;
		return TYPE_ANSI;
	}

	delete[] buffer;
	delete[] buf;
	forRead.close();

	return -1;
}

//utf-8תGBK
inline string fApFile::utf2gbk(const string& res)
{
	int nwLen = MultiByteToWideChar(CP_UTF8, 0, res.c_str(), -1, NULL, 0);

	wchar_t* pwBuf = new wchar_t[nwLen + 1];//һ��Ҫ��1����Ȼ�����β��
	memset(pwBuf, 0, nwLen * 2 + 2);

	MultiByteToWideChar(CP_UTF8, 0, res.c_str(), res.length(), pwBuf, nwLen);

	int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char* pBuf = new char[nLen + 1];
	memset(pBuf, 0, nLen + 1);

	WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr = pBuf;

	delete[]pBuf;
	delete[]pwBuf;

	pBuf = NULL;
	pwBuf = NULL;

	return retStr;
}

//GBKתutf-8
inline string fApFile::gbk2utf(const string& res)
{
	
}

inline string fApFile::uncode2utf(const string& res)
{
	
}

inline string fApFile::utf2unicode(const string& res)
{
	
}

inline string fApFile::ws2s(const wstring& src)
{
	locale sys_locale("");

	const wchar_t* data_from = src.c_str();
	const wchar_t* data_from_end = src.c_str() + src.size();
	const wchar_t* data_from_next = 0;

	int wchar_size = 4;
	char* data_to = new char[(src.size() + 1) * wchar_size];
	char* data_to_end = data_to + (src.size() + 1) * wchar_size;
	char* data_to_next = 0;

	memset(data_to, 0, (src.size() + 1) * wchar_size);

	typedef codecvt<wchar_t, char, mbstate_t> convert_facet;
	mbstate_t out_state;
	auto result = use_facet<convert_facet>(sys_locale).out(
		out_state, data_from, data_from_end, data_from_next,
		data_to, data_to_end, data_to_next);
	if (result == convert_facet::ok)
	{
		string dst = data_to;
		delete[] data_to;
		return dst;
	}
	else
	{
		printf("convert error!\n");
		delete[] data_to;
		return string("");
	}
}

inline wstring fApFile::s2ws(const string& src)
{
	locale sys_locale("");

	const char* data_from = src.c_str();
	const char* data_from_end = src.c_str() + src.size();
	const char* data_from_next = 0;

	wchar_t* data_to = new wchar_t[src.size() + 1];
	wchar_t* data_to_end = data_to + src.size() + 1;
	wchar_t* data_to_next = 0;

	wmemset(data_to, 0, src.size() + 1);

	typedef codecvt<wchar_t, char, mbstate_t> convert_facet;
	mbstate_t in_state;
	auto result = use_facet<convert_facet>(sys_locale).in(
		in_state, data_from, data_from_end, data_from_next,
		data_to, data_to_end, data_to_next);
	if (result == convert_facet::ok)
	{
		wstring dst = data_to;
		delete[] data_to;
		return dst;
	}
	else
	{
		printf("convert error!\n");
		delete[] data_to;
		return wstring(L"");
	}
}


inline string fApFile::ws2utf8(const wstring& src)
{
	wstring_convert<codecvt_utf8<wchar_t>> conv;
	return conv.to_bytes(src);
}

inline wstring fApFile::utf8_2_ws(const string& src)
{
	wstring_convert<codecvt_utf8<wchar_t> > conv;
	return conv.from_bytes(src);
}

inline BOOL fApFile::readANSI()
{
	if (!forRead.is_open() && readPath == "") return FALSE;

	if (!forRead.is_open() && readPath != "")
	{
		forRead.open(readPath, ios::in | ios::binary);
		if (!forRead.is_open()) return FALSE;
	}

	forRead.seekg(0, SEEK_SET);
	stringstream temp;
	temp << forRead.rdbuf();

	fileStr = temp.str();
	//cout << fileStr << endl;

	return TRUE;
}

inline BOOL fApFile::readUTF8()
{
	if (!forRead.is_open() && readPath == "") return FALSE;

	if (!forRead.is_open() && readPath != "")
	{
		forRead.open(readPath, ios::in | ios::binary);
		if (!forRead.is_open()) return FALSE;
	}

	forRead.seekg(0, SEEK_SET);
	stringstream temp;
	temp << forRead.rdbuf();

	string utf8 = temp.str();
	wstring transfer = utf8_2_ws(utf8);
	fileStr = ws2s(transfer);
	//fileStr = utf8;

	//cout << fileStr << endl;

	return TRUE;
}

inline BOOL fApFile::readUTF8BOM()
{
	if (!forRead.is_open() && readPath == "") return FALSE;

	if (!forRead.is_open() && readPath != "")
	{
		forRead.open(readPath, ios::in | ios::binary);
		if (!forRead.is_open()) return FALSE;
	}

	forRead.seekg(3, ios::beg);
	stringstream temp;
	temp << forRead.rdbuf();

	string utf8 = temp.str();
	wstring transfer = utf8_2_ws(utf8);
	fileStr = ws2s(transfer);

	//cout << fileStr << endl;

	return TRUE;
}

inline BOOL fApFile::readUnicode()
{
	if (!forRead.is_open() && readPath == "") return FALSE;

	if (!forRead.is_open() && readPath != "")
	{
		forRead.open(readPath, ios::in | ios::binary);
		if (!forRead.is_open()) return FALSE;
	}

	forRead.seekg(0, SEEK_SET);
	wstringstream temp;
	temp << forRead.rdbuf();

	wstring utf8 = temp.str();
	fileStr = ws2s(utf8);

	//cout << fileStr << endl;

	return TRUE;
}

inline BOOL fApFile::readFileAndsave(string path)
{
	int type = getFileType(path);
	BOOL isSucceed = FALSE;

	switch (type)
	{
	case TYPE_UTF8_BOM:
		isSucceed = readUTF8BOM();
		break;
	case TYPE_UTF8:
		isSucceed = readUTF8();
		break;
	case TYPE_UNICODE:
	case TYPE_UNICODE_BIG:
		isSucceed = readUnicode();
		break;
	case TYPE_ANSI:
		isSucceed = readANSI();
		break;
	default:
		cout << "�����ʽδʶ��..." << endl;
		break;
	}

	return isSucceed;
}

//ȡ���������ڴ��е��ļ���Ϣ
inline string fApFile::getContent()
{
	return fileStr;
}

inline BOOL fApFile::write2Txt(const string& content, const string& name)
{
	writePath.assign("C:\\Users\\Administrator\\Desktop\\");
	writePath.append(name);
	writePath.append(".txt");

	if (!forWrite.is_open())
	{
		forWrite.open(writePath, ios::out | ios::ate);
		if (!forWrite.is_open()) return FALSE;
	}

	forWrite << content;

	forWrite.close();

	return TRUE;
}
