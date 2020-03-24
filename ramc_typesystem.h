#pragma once
#include "ramc_position.h"

namespace ramc {
	enum class PrimitiveType {
		INT, FLOAT, STRING, BOOL, VOID
	};

	const std::string PrimTypeToString(PrimitiveType _type);

	enum class TypeResultType {
		SUCCESS,
		ID_NOT_FOUND,
		MISMATCH
	};

	class TypeResult {
		TypeResultType resType;
		PrimitiveType type;

		std::string errString;
		Position errPosition;
	public:
		bool IsSuccess() { return resType == TypeResultType::SUCCESS; }
		PrimitiveType GetType() { return type; }

		std::string ToString();

		static TypeResult GenSuccess(PrimitiveType _type);
		static TypeResult GenIDNotFound(std::string _id, Position _pos);
		static TypeResult GenMismatch(std::string _mismatch, Position _pos);
	};

	class Environment {
		Environment* parent;
		std::unordered_map<std::string, PrimitiveType> variables;

	public:
		Environment(Environment* _parent);

		bool AddVariable(std::string _id, PrimitiveType _type);
		bool HasVariable(std::string _id);
		bool HasVariable(std::string _id, PrimitiveType _type);
		TypeResult GetVariableType(std::string _id, Position _execPos);
	};
}