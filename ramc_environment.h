#pragma once
#include "ramvm_argument.h"

using ramvm::Argument;

namespace ramc {
	class Type;
	class TypeResult;

	class Environment {
		struct VarInfo { Type* type = nullptr; Argument* source = nullptr; };
		struct FuncInfo { std::string label; Type* type = nullptr; };

		static int varGPOffset;

		Environment* parent;
		std::unordered_map<std::string, FuncInfo> functions;
		std::unordered_map<std::string, VarInfo> variables;
		int varFPOffset;
	public:
		Environment(Environment* _parent);
		~Environment();

		TypeResult AddVariable(std::string _id, Type* _type, Argument* _src, Position _execPos);
		bool HasVariable(std::string _id, bool _localCheck);
		bool HasVariable(std::string _id, Type* _type, bool _localCheck);
		TypeResult GetVariableType(std::string _id, Position _execPos);
		Argument* GetVariableSource(std::string _id);

		TypeResult AddFunction(std::string _id, Type* _type, std::string& _label, Position _execPos);
		TypeResult GetFunctionRetType(std::string _id, Type* _paramsType, Position _execPos);

		int GetNextVarFPOffset(Type* _varType);

		static int GetNextVarGPOffset(Type* _varType);
		static std::string GenFuncLabel(std::string _name, Type* _paramsType);
	};
}