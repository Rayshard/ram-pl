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

typedef unsigned char byte;

#define BYTE_SIZE 1
#define INT_SIZE 4

enum class DataType { UNKNOWN, BYTE, INT };

union DataValue
{
	byte b;
	int i = 0; //Set biggest one to 0
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
	DataVariant(const DataVariant& _org) : type(_org.type), value(_org.value) { }

	int GetSize()
	{
		switch (type)
		{
			case DataType::BYTE: return BYTE_SIZE;
			case DataType::INT: return INT_SIZE;
			default: return 0;
		}
	}

	DataType GetType() { return type; }
	DataValue GetValue() { return value; }
	byte AsByte() { return value.b; }
	int AsInt() { return value.i; }
};

template <typename T> std::string ToHexString(T _value) {
	static const char* digits = "0123456789ABCDEF";
	int hex_len = sizeof(T) << 1;
	std::string str(hex_len, '0');

	for (int i = 0, j = (hex_len - 1) * 4; i < hex_len; ++i, j -= 4)
		str[i] = digits[(_value >> j) & 0x0f];

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

#include "ramvm_util.h"

#endif
