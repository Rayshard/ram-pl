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
#include "Result.h"
#include "Lexer.h"
#include "Grammar.h"
#include<chrono>

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
#endif

