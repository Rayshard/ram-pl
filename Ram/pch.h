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
#include "Result.h"
#include "Lexer.h"
#include "Grammar.h"
#include <chrono>

class IValue;
class Value;
class IStatement;
class IExpression;
class Environment;

typedef std::string TypeName, TypeSig;
typedef std::map<TypeName, TypeSig> TypeSigMap;
typedef std::vector<IExpression*> ArgumentList;
typedef std::map<std::string, IExpression*> AssignmentMap;
typedef std::pair<std::string, std::string> Definition; // Either Identifier->TypeDef or Identifier->TypeSig
typedef std::vector<Definition> DefinitionList;
typedef std::map<std::string, TypeName> DefinitionMap;
typedef std::shared_ptr<Value> SharedValue;

#define SHARE(v) SharedValue(new Value(v))
#define SHARE_VOID(pos) SHARE(new VoidValue(pos))
#define SHARE_COPY(val) SHARE(val->GetCopy())
#define SYS_SEP std::string(1, std::experimental::filesystem::path::preferred_separator)

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

