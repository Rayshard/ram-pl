#ifndef PCH_H
#define PCH_H

#include <iostream>
#include <vector>
#include <map>
#include <functional>
#include <memory>
#include <string>
#include <array>
#include <algorithm> 
#include <fstream>
#include <sstream>
#include <cctype>
#include <locale>
#include <iterator>
#include <filesystem>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include "Result.h"
#include "Lexer.h"
#include "Grammar.h"
#include "Ram.h"

namespace Windows
{
#define WINDOWS_LEAN_AND_MEAN
#include <Windows.h>
};

class IValue;
class Value;
class IStatement;
class IExpression;
class Environment;

typedef std::string TypeName, Signature;
typedef std::map<TypeName, Signature> SignatureMap;
typedef std::vector<IExpression*> ArgumentList;
typedef std::map<std::string, IExpression*> AssignmentMap;
typedef std::pair<std::string, std::string> Definition; // Either Identifier->TypeDef or Identifier->Signature
typedef std::vector<Definition> DefinitionList;
typedef std::map<std::string, TypeName> DefinitionMap;

typedef std::shared_ptr<Value> SharedValue;
typedef std::weak_ptr<Value> ObservedValue;
typedef std::function<SharedValue(Environment*, Position)> builtInFunc;

#define SHARE(v) SharedValue(new Value(v))
#define SHARE_VOID(pos) SHARE(new VoidValue(pos))
#define SHARE_COPY(val) SHARE(val->GetCopy())
#define SYS_SEP std::string(1, std::experimental::filesystem::path::preferred_separator)

struct RamVoid : RamValue
{
	RamVoid() {}

	int AsCInt() { throw std::runtime_error("Value is void."); }
	float AsCFloat() { throw std::runtime_error("Value is void."); }
	const char *AsCString() { throw std::runtime_error("Value is void."); }
	bool AsCBool() { throw std::runtime_error("Value is void."); }
};

struct RamInt : RamValue
{
	int value;

	RamInt(int _val) { value = _val; }

	int AsCInt() { return value; }
	float AsCFloat() { throw std::runtime_error("Value is an int."); }
	const char *AsCString() { throw std::runtime_error("Value is an int."); }
	bool AsCBool() { throw std::runtime_error("Value is an int."); }
};

struct RamFloat : RamValue
{
	float value;

	RamFloat(float _val) { value = _val; }

	float AsCFloat() { return value; }
	int AsCInt() { throw std::runtime_error("Value is a float."); }
	const char *AsCString() { throw std::runtime_error("Value is a float."); }
	bool AsCBool() { throw std::runtime_error("Value is a float."); }
};

struct RamBool : RamValue
{
	bool value;

	RamBool(bool _val) { value = _val; }

	bool AsCBool() { return value; }
	int AsCInt() { throw std::runtime_error("Value is a bool."); }
	float AsCFloat() { throw std::runtime_error("Value is a bool."); }
	const char *AsCString() { throw std::runtime_error("Value is a bool."); }
};

struct RamString : RamValue
{
	std::string value;

	RamString(std::string _val) { value = _val; }

	const char *AsCString() { return value.c_str(); }
	bool AsCBool() { throw std::runtime_error("Value is a string."); }
	int AsCInt() { throw std::runtime_error("Value is a string."); }
	float AsCFloat() { throw std::runtime_error("Value is a string."); }
};

inline std::string GetFileName(std::string _filePath, bool _includeExt)
{
	std::string result = _filePath;

	const size_t lastSlash = _filePath.find_last_of("\\/");
	if(std::string::npos != lastSlash)
		result = _filePath.erase(0, lastSlash + 1);

	if(!_includeExt)
	{
		const size_t extStart = _filePath.rfind('.');
		if(std::string::npos != extStart)
			result = _filePath.erase(extStart);
	}

	return result;
};

inline std::string GetFileDir(std::string _filePath)
{
	std::string result = _filePath;

	const size_t lastSlash = _filePath.find_last_of("\\/");
	if(std::string::npos != lastSlash)
		result = _filePath.substr(0, lastSlash + 1);

	return result;
};

inline bool FileExists(const char* _path)
{
	std::ifstream f(_path);
	return f.is_open();
}

#endif

