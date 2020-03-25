#ifndef PCH_H
#define PCH_H

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
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

#define BYTE_SIZE 1
#define INT_SIZE 4
#define FLOAT_SIZE 4
#define LONG_SIZE 8
#define DOUBLE_SIZE 8

enum class DataType { UNKNOWN, BYTE, INT, FLOAT, DOUBLE, LONG };

union DataValue
{
	byte b;
	int i;
	float f;
	double d;
	long l = 0;
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
	DataVariant(long _value) : type(DataType::LONG) { value.l = _value; }
	DataVariant(const DataVariant& _org) : type(_org.type), value(_org.value) { }

	int GetSize()
	{
		switch (type)
		{
			case DataType::BYTE: return BYTE_SIZE;
			case DataType::INT: return INT_SIZE;
			case DataType::FLOAT: return FLOAT_SIZE;
			case DataType::DOUBLE: return DOUBLE_SIZE;
			case DataType::LONG: return LONG_SIZE;
			default: return 0;
		}
	}

	std::string ToString()
	{
		switch (type)
		{
			case DataType::BYTE: return std::to_string(value.b);
			case DataType::INT: return std::to_string(value.i);
			case DataType::FLOAT: return std::to_string(value.f);
			case DataType::DOUBLE: return std::to_string(value.d);
			case DataType::LONG: return std::to_string(value.l);
			case DataType::UNKNOWN:
			default: return "UNKNOWN";
		}
	}

	DataType GetType() { return type; }
	DataValue GetValue() { return value; }
	byte AsByte() { return value.b; }
	int AsInt() { return value.i; }
	float AsFloat() { return value.f; }
	double AsDouble() { return value.d; }
	long AsLong() { return value.l; }
};

template <typename T> std::string ToHexString(T _value)
{
	static const char* digits = "0123456789ABCDEF";
	byte* bytes = (byte*)&_value;
	std::string str;
	
	for (int i = 0; i < sizeof(T); i++)
	{
		byte b = bytes[i];
		str.push_back(digits[(b & 0xF0) >> 4]);
		str.push_back(digits[(b & 0x0F) >> 0]);
	}
	
	return "0x" + str;
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

inline bool IsByte(std::string _str) { try { return std::stoul(_str) <= 255; } catch (...) { return false; } }
inline bool IsInt(std::string _str) { try { IGNORE(std::stoi(_str)); return true; } catch (...) { return false; } }
inline bool IsLong(std::string _str) { try { IGNORE(std::stol(_str)); return true; } catch (...) { return false; } }
inline bool IsFloat(std::string _str) { try { IGNORE(std::stof(_str)); return true; } catch (...) { return false; } }
inline bool IsDouble(std::string _str) { try { IGNORE(std::stod(_str)); return true; } catch (...) { return false; } }

#include "ramvm_util.h"

#endif
