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

struct RamInt : RamArgValue
{
	int value;

	RamInt(int _val) { value = _val; }

	int AsCInt() { return value; }
	float AsCFloat() { return 0; }
	bool AsCBool() { return false; }
	const char* AsCString() { return ""; }
};

struct RamFloat : RamArgValue
{
	float value;

	RamFloat(float _val) { value = _val; }

	int AsCInt() { return 0; }
	float AsCFloat() { return value; }
	bool AsCBool() { return false; }
	const char* AsCString() { return ""; }
};

struct RamBool : RamArgValue
{
	bool value;

	RamBool(bool _val) { value = _val; }

	int AsCInt() { return 0; }
	float AsCFloat() { return 0; }
	bool AsCBool() { return value; }
	const char* AsCString() { return ""; }
};

struct RamString : RamArgValue
{
	std::string value;

	RamString(std::string _val) { value = _val; }

	int AsCInt() { return 0; }
	float AsCFloat() { return 0; }
	bool AsCBool() { return false; }
	const char* AsCString() { return value.c_str(); }
};

struct RamAny : RamReturnValue
{
	enum Type { RVOID, RINT, RFLOAT, RBOOL, RSTRING, REXCEPTION };

	Type type = RVOID;

	int _int;
	float _float;
	bool _bool;
	std::string _string;
	std::pair<std::string, std::string> _exception;

	void SetAsInt(int _i) { _int = _i; type = RINT; }
	void SetAsFloat(float _f) { _float = _f; type = RFLOAT; }
	void SetAsBool(bool _b) { _bool = _b; type = RBOOL; }
	void SetAsString(const char* _s) { _string = _s; type = RSTRING; }
	void ThrowException(const char* _name, const char* _msg) { _exception = std::make_pair(_name, _msg); type = REXCEPTION; }
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

