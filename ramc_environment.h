#pragma once
#include "ramvm_instruction.h"

using ramvm::Argument;

namespace ramc {
	class Type;
	class TypeResult;

	class Environment {
		struct VarInfo { Type* type = nullptr; Argument* source = nullptr; };
		struct FuncInfo { std::string label; Type* type = nullptr; int regCnt = 0; };

		Environment* parent;
		std::unordered_map<std::string, FuncInfo> functions;
		std::unordered_map<std::string, VarInfo> variables;
		std::unordered_map<Environment*, bool> subEnvs;
		int nextRegIdx, numRegNeeded, nextGlobalStackPos;

		void SetMaxNumVarRegNeeded(int _val);
	public:
		Environment(Environment* _parent, bool _incrParentRegCnt);
		~Environment();

		TypeResult AddVariable(std::string _id, Type* _type, Argument* _source, Position _execPos);
		bool HasVariable(std::string _id, bool _localCheck);
		bool HasVariable(std::string _id, Type* _type, bool _localCheck);
		TypeResult GetVariableType(std::string _id, Position _execPos);
		Argument* GetVarSource(std::string _id);

		TypeResult AddFunction(std::string _id, Type* _type, std::string& _label, Position _execPos);
		TypeResult GetFunctionRetType(std::string _id, Type* _paramsType, Position _execPos);

		int GetNumRegNeeded() { return numRegNeeded; }
		int GetNextRegIdx() { return nextRegIdx++; }
		int GetNextGlobalStackPos(int _byteLen) { nextGlobalStackPos += _byteLen;  return nextGlobalStackPos - _byteLen; }

		static std::string GenFuncLabel(std::string _name, Type* _paramsType);
	};
}