#pragma once
#include "ramc_token.h"
#include "ramc_typesystem.h"
#include "ramvm_instruction.h"
#include "ramvm_parser.h"

using ramvm::InstructionSet;

namespace ramc {
	enum class ASTNodeType { PROGAM, ASSIGNMENT, LITERAL, BINOP_EXPR, UNOP_EXPR, IDENTIFIER };

	enum class BinopType {
		ADD, SUB, MUL, DIV, MOD, POW,
		BIN_AND, BIN_OR, BIN_XOR, LSHIFT, RSHIFT,
		LT, GT, LT_EQ, GT_EQ, EQ_EQ, NEQ, LOG_AND, LOG_OR
	};

	enum class AssignmentType {
		EQ, ADD_EQ, SUB_EQ, MUL_EQ, DIV_EQ, MOD_EQ, POW_EQ,
		BIN_AND_EQ, BIN_OR_EQ, BIN_XOR_EQ, LSHIFT_EQ, RSHIFT_EQ,
	};

	enum class UnopType { NEG, LOG_NOT, BIN_NOT };
	enum class LiteralType { INT, FLOAT, STRING, BOOL, BYTE, DOUBLE, LONG };

	class ASTNode {
	private:
		ASTNodeType type;
		Position position;
		Type* typeSysType;
	protected:
		ASTNode(ASTNodeType _type, Position _pos);

		virtual TypeResult TypeCheck(Environment* _env) = 0;
	public:
		ASTNodeType GetType() { return type; }
		TypeResult GetTypeSysType(Environment* _env)
		{
			if (typeSysType) { return TypeResult::GenSuccess(typeSysType); }
			else
			{
				TypeResult typeRes = TypeCheck(_env);
				typeSysType = typeRes.GetValue();
				return typeRes;
			}
		}
		Position GetPosition() { return position; }

		virtual std::string ToString(int _indentLvl) = 0;
		virtual InstructionSet GenerateCode(std::map<std::string, std::string> _params) = 0;

		template<typename T> T* As() { return dynamic_cast<T*>(this); }
	};

	class ASTIdentifier;
	class ASTBinopEpxr;

#pragma region Program
	class ASTProgram : public ASTNode {
		std::string fileName;
		std::vector<ASTNode*> stmts;
	public:
		ASTProgram(std::string _fileName, std::vector<ASTNode*> _stmts);
		~ASTProgram();

		std::string ToString(int _indentLvl) override;
		TypeResult TypeCheck(Environment* _env) override;
		InstructionSet GenerateCode(std::map<std::string, std::string> _params) override;
	};
#pragma endregion

#pragma region VarDeclaration
	class ASTVarDecl : public ASTNode {
		ASTIdentifier* id;
		ASTNode* expr;
		bool isUnderscore;
	public:
		ASTVarDecl(ASTIdentifier*  _id, ASTNode* _expr, Position _pos);
		ASTVarDecl(ASTNode* _expr, Position _pos);
		~ASTVarDecl();

		std::string ToString(int _indentLvl) override;
		TypeResult TypeCheck(Environment* _env) override;
		InstructionSet GenerateCode(std::map<std::string, std::string> _params) override;
	};
#pragma endregion

#pragma region Assignment
	class ASTAssignment : public ASTNode {
		ASTIdentifier* id;
		ASTNode* expr;
		AssignmentType assignType;
	public:
		ASTAssignment(ASTIdentifier* _id, ASTNode* _expr, AssignmentType _assignType);
		~ASTAssignment();

		AssignmentType GetAssignType() { return assignType; }

		std::string ToString(int _indentLvl) override;
		TypeResult TypeCheck(Environment* _env) override;
		InstructionSet GenerateCode(std::map<std::string, std::string> _params) override;
	};
#pragma endregion

#pragma region BinopExpr
	class ASTBinopExpr : public ASTNode {
		ASTNode* left, * right;
		BinopType op;
	public:
		ASTBinopExpr(ASTNode* _left, ASTNode* _right, BinopType _op);
		~ASTBinopExpr();

		BinopType GetOp() { return op; }

		std::string ToString(int _indentLvl) override;
		TypeResult TypeCheck(Environment* _env) override;
		InstructionSet GenerateCode(std::map<std::string, std::string> _params) override;
	};
#pragma endregion

#pragma region UnopExpr
	class ASTUnopExpr : public ASTNode {
		ASTNode* expr;
		UnopType op;
	public:
		ASTUnopExpr(ASTNode* _expr, UnopType _op);
		~ASTUnopExpr();

		UnopType GetOp() { return op; }

		std::string ToString(int _indentLvl) override;
		TypeResult TypeCheck(Environment* _env) override;
		InstructionSet GenerateCode(std::map<std::string, std::string> _params) override;
	};
#pragma endregion

#pragma region Identifier
	class ASTIdentifier : public ASTNode {
		std::string id;
		Argument source;
	public:
		ASTIdentifier(std::string _id, Position _pos);

		std::string GetID() { return id; }
		Argument GetSource() { return source; }

		std::string ToString(int _indentLvl) override;
		TypeResult TypeCheck(Environment* _env) override;
		InstructionSet GenerateCode(std::map<std::string, std::string> _params) override;
	};
#pragma endregion

#pragma region Literal
	template<typename T> class ASTLiteral : public ASTNode {
		LiteralType litType;
		T value;
	protected:
		ASTLiteral(LiteralType _type, T _val, Position _pos)
			: ASTNode(ASTNodeType::LITERAL, _pos)
		{
			litType = _type;
			value = _val;
		}
	public:
		T GetValue() { return value; }
		LiteralType GetLitType() { return litType; }

		std::string ToString(int _indentLvl) override
		{
			std::stringstream ss;
			ss << CreateIndent(_indentLvl);

			switch (litType)
			{
				case LiteralType::INT: ss << "Integer Literal: "; break;
				case LiteralType::FLOAT: ss << "Float Literal: "; break;
				case LiteralType::STRING: ss << "String Literal: "; break;
				case LiteralType::BOOL: ss << "Bool Literal: "; break;
				case LiteralType::BYTE: ss << "Byte Literal: "; break;
				case LiteralType::DOUBLE: ss << "Double Literal: "; break;
				case LiteralType::LONG: ss << "Long Literal: "; break;
				default: ss << "Literal::ToString - LiteralType not handled!"; break;
			}

			ss << value;
			return ss.str();
		}

		TypeResult TypeCheck(Environment* _env) override
		{
			switch (litType)
			{
				case LiteralType::INT: return TypeResult::GenSuccess(Type::INT);
				case LiteralType::FLOAT: return TypeResult::GenSuccess(Type::FLOAT);
				case LiteralType::STRING: return TypeResult::GenSuccess(Type::STRING);
				case LiteralType::BOOL: return TypeResult::GenSuccess(Type::BOOL);
				case LiteralType::BYTE: return TypeResult::GenSuccess(Type::BYTE);
				case LiteralType::DOUBLE: return TypeResult::GenSuccess(Type::DOUBLE);
				case LiteralType::LONG: return TypeResult::GenSuccess(Type::LONG);
				default: return TypeResult::GenMismatch("Literal::Typecheck - LiteralType not handled!", GetPosition());
			}
		}

		InstructionSet GenerateCode(std::map<std::string, std::string> _params) override = 0;
	};

	struct ASTIntLit : public ASTLiteral<int> {
		ASTIntLit(int _val, Position _pos) : ASTLiteral(LiteralType::INT, _val, _pos) { }
		InstructionSet GenerateCode(std::map<std::string, std::string> _params) override;
	};

	struct ASTByteLit : public ASTLiteral<byte> {
		ASTByteLit(byte _val, Position _pos) : ASTLiteral(LiteralType::BYTE, _val, _pos) { }
		InstructionSet GenerateCode(std::map<std::string, std::string> _params) override;
	};

	struct ASTDoubleLit : public ASTLiteral<double> {
		ASTDoubleLit(double _val, Position _pos) : ASTLiteral(LiteralType::DOUBLE, _val, _pos) { }
		InstructionSet GenerateCode(std::map<std::string, std::string> _params) override;
	};

	struct ASTLongLit : public ASTLiteral<rLong> {
		ASTLongLit(rLong _val, Position _pos) : ASTLiteral(LiteralType::LONG, _val, _pos) { }
		InstructionSet GenerateCode(std::map<std::string, std::string> _params) override;
	};

	struct ASTFloatLit : public ASTLiteral<float> {
		ASTFloatLit(float _val, Position _pos) : ASTLiteral(LiteralType::FLOAT, _val, _pos) { }
		InstructionSet GenerateCode(std::map<std::string, std::string> _params) override;
	};

	struct ASTStringLit : public ASTLiteral<std::string> {
		ASTStringLit(std::string _val, Position _pos) : ASTLiteral(LiteralType::STRING, _val, _pos) {}
		InstructionSet GenerateCode(std::map<std::string, std::string> _params) override;
	};

	struct ASTBoolLit : public ASTLiteral<bool> {
		ASTBoolLit(bool _val, Position _pos) : ASTLiteral(LiteralType::BOOL, _val, _pos) {}
		InstructionSet GenerateCode(std::map<std::string, std::string> _params) override;
	};
#pragma endregion
}

