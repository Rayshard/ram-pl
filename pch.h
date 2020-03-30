#ifndef PCH_H
#define PCH_H

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <set>
#include <unordered_map>
#include <regex>
#include <functional>
#include <iterator>

#define IGNORE(x) (void)(x)

struct Position
{
	int line, colomn;

	Position() = default;
	Position(int _line, int _col);

	std::string ToString();
};

typedef unsigned char byte;
typedef long long rLong;

#define BYTE_SIZE 1
#define INT_SIZE 4
#define FLOAT_SIZE 4
#define LONG_SIZE 8
#define DOUBLE_SIZE 8

enum class DataType : byte { UNKNOWN, BYTE, INT, FLOAT, DOUBLE, LONG };
typedef std::pair<DataType, DataType> DataTypeDouble;
typedef std::tuple<DataType, DataType, DataType> DataTypeTriple;

inline constexpr int GetDataTypeSize(DataType _type)
{
	switch (_type)
	{
		case DataType::BYTE: return BYTE_SIZE;
		case DataType::INT: return INT_SIZE;
		case DataType::FLOAT: return FLOAT_SIZE;
		case DataType::DOUBLE: return DOUBLE_SIZE;
		case DataType::LONG: return LONG_SIZE;
		default: return 0;
	}
}

union DataValue
{
	byte b;
	int i;
	float f;
	double d;
	rLong l;

	byte bytes[LONG_SIZE];

	DataValue() { l = 0; }
	DataValue(byte _val) { l = 0;  b = _val; }
	DataValue(int _val) { l = 0; i = _val; }
	DataValue(float _val) { l = 0; f = _val; }
	DataValue(double _val) { l = 0; d = _val; }
	DataValue(rLong _val) { l = 0; l = _val; }
	DataValue(byte* _bytes, int _length) { memcpy(bytes, _bytes, (int)fmin(LONG_SIZE, _length)); }
};

class DataVariant {
	DataType type;
	DataValue value;

public:
	DataVariant() : type(DataType::UNKNOWN) { }
	DataVariant(DataType _type) : type(_type) { }
	DataVariant(DataType _type, DataValue _val) : type(_type), value(_val) { }
	DataVariant(byte _value) : type(DataType::BYTE) { value.b = _value; }
	DataVariant(int _value) : type(DataType::INT) { value.i = _value; }
	DataVariant(float _value) : type(DataType::FLOAT) { value.f = _value; }
	DataVariant(double _value) : type(DataType::DOUBLE) { value.d = _value; }
	DataVariant(rLong _value) : type(DataType::LONG) { value.l = _value; }
	DataVariant(const DataVariant& _org) : type(_org.type), value(_org.value) { }

	int GetSize() { return GetDataTypeSize(type); }

	std::string ToString()
	{
		switch (type)
		{
			case DataType::BYTE: return std::to_string(value.b);
			case DataType::INT: return std::to_string(value.i);
			case DataType::FLOAT: return std::to_string(value.f);
			case DataType::DOUBLE: return std::to_string(value.d);
			case DataType::LONG: return std::to_string(value.l);
			default: return "UNKNOWN";
		}
	}

	DataType GetType() { return type; }
	DataValue GetValue() { return value; }
	byte B() { return value.b; }
	int I() { return value.i; }
	float F() { return value.f; }
	double D() { return value.d; }
	rLong L() { return value.l; }
	byte* Bytes() { return value.bytes; }
	template<typename T> T To()
	{
		switch (type)
		{
			case DataType::BYTE: return (T)value.b;
			case DataType::INT: return (T)value.i;
			case DataType::FLOAT: return (T)value.f;
			case DataType::DOUBLE: return (T)value.d;
			case DataType::LONG: return (T)value.l;
			default: return T();
		}
	}
	DataVariant To(DataType _type)
	{
		switch (_type)
		{
			case DataType::BYTE: return DataVariant(To<byte>());
			case DataType::INT: return DataVariant(To<int>());
			case DataType::FLOAT: return DataVariant(To<float>());
			case DataType::DOUBLE: return DataVariant(To<double>());
			case DataType::LONG: return DataVariant(To<rLong>());
			default: return DataVariant();
		}
	}
};

inline std::string DataTypeToString(DataType _type)
{
	switch (_type)
	{
		case DataType::BYTE: return "BYTE";
		case DataType::INT: return "INT";
		case DataType::FLOAT: return "FLOAT";
		case DataType::DOUBLE: return "DOUBLE";
		case DataType::LONG: return "LONG";
		default: return "UNKNOWN";
	}
}

inline std::string ToHexString(byte* _buffer, int _length)
{
	static const char* digits = "0123456789ABCDEF";
	std::string str;

	for (int i = _length - 1; i >= 0; i--)
	{
		byte b = _buffer[i];
		str.push_back(digits[(b & 0xF0) >> 4]);
		str.push_back(digits[(b & 0x0F) >> 0]);
	}

	return "0x" + str;
}

template <typename T> inline std::string ToHexString(T _value) { return ToHexString((byte*)&_value, sizeof(T)); }

template <typename T> inline std::string ToByteString(T _value) { return std::string((char*)&_value, (char*)&_value + sizeof(T)); }

inline std::string HexToByteStr(std::string& _str)
{
	std::string digits = (_str.length() % 2 == 1 ? "0" : "") + _str.substr(2);
	std::string result;

	for (int i = digits.length() - 1; i >= 0; i -= 2)
	{
		char bs[2] = { digits[i - 1], digits[i] };
		result.push_back((byte)strtol(bs, NULL, 16));
	}

	return result;
}

inline std::string TrimString(std::string& _str)
{
	const char* ws = " \t\n\r\f\v";

	std::string rTrim = _str.erase(_str.find_last_not_of(ws) + 1);
	return rTrim.erase(0, rTrim.find_first_not_of(ws));
}

inline std::string CreateIndent(int _indentLvl) { return std::string(_indentLvl * 5, ' '); }

inline constexpr int ConcatTriple(byte _fst, byte _snd, byte _thr) { return (int(_fst) << 16) | (int(_snd) << 8) | _thr; }
inline constexpr int ConcatDouble(byte _fst, byte _snd) { return (int(_fst) << 8) | _snd; }

inline bool IsInt(std::string _str) { try { IGNORE(std::stoi(_str)); return true; } catch (...) { return false; } }

inline std::string StrReplace(std::string _str, std::string _chars, char _c) {
	for (int i = 0; i < (int)_str.length(); ++i)
	{
		for (auto const& c : _chars)
		{
			if (_str[i] == c)
			{
				_str[i] = _c;
				break;
			}
		}
	}

	return _str;
}

#include "ramvm_util.h"

#endif
