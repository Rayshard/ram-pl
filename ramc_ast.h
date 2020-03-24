#pragma once
#include "ramc_token.h"
#include "ramc_typesystem.h"
#include "ramvm_instruction.h"
#include "ramvm_parser.h"

namespace ramc {
	enum class ASTNodeType { PROGAM, LITERAL, BINOP_EXPR, UNOP_EXPR, IDENTIFIER };

	enum class BinopType {
		ADD, SUB, MUL, DIV, MOD, POW,
		BIN_AND, BIN_OR, BIN_XOR, LSHIFT, RSHIFT,
		LT, GT, LT_EQ, GT_EQ, EQ_EQ, NEQ, LOG_AND, LOG_OR,
		EQ, ADD_EQ, SUB_EQ, MUL_EQ, DIV_EQ, MOD_EQ, POW_EQ,
		BIN_AND_EQ, BIN_OR_EQ, BIN_XOR_EQ, LSHIFT_EQ, RSHIFT_EQ,
	};

	enum class UnopType { NEG, LOG_NOT, PRE_INC, POST_INC, PRE_DEC, POST_DEC };
	enum class LiteralType { INT, FLOAT, STRING, BOOL };

	class ASTNode {
	private:
		ASTNodeType type;
		Position position;
	protected:
		ASTNode(ASTNodeType _type, Position _pos);
	public:
		ASTNodeType GetType() { return type; }
		Position GetPosition() { return position; }

		virtual std::string ToString(int _indentLvl) = 0;
		virtual TypeResult TypeCheck(Environment& _env) = 0;
		virtual ramvm::InstructionSet GenerateCode(std::map<std::string, std::string> _params) = 0;

		template<typename T> T* As() { return dynamic_cast<T*>(this); }
	};

#pragma region Program
	class ASTProgram : public ASTNode {
		std::string fileName;
		std::vector<ASTNode*> stmts;
	public:
		ASTProgram(std::string _fileName, std::vector<ASTNode*> _stmts);
		~ASTProgram();

		std::string ToString(int _indentLvl) override;
		TypeResult TypeCheck(Environment& _env) override;
		ramvm::InstructionSet GenerateCode(std::map<std::string, std::string> _params) override;
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
		TypeResult TypeCheck(Environment& _env) override;
		ramvm::InstructionSet GenerateCode(std::map<std::string, std::string> _params) override;
	};
#pragma endregion

#pragma region UnopExpr
	class ASTUnopExpr : public ASTNode {
		ASTNode* right;
		UnopType op;
	public:
		ASTUnopExpr(ASTNode* _right, UnopType _op);
		~ASTUnopExpr();

		UnopType GetOp() { return op; }

		std::string ToString(int _indentLvl) override;
		TypeResult TypeCheck(Environment& _env) override;
		ramvm::InstructionSet GenerateCode(std::map<std::string, std::string> _params) override;
	};
#pragma endregion

#pragma region Identifier
	class ASTIdentifier : public ASTNode {
		std::string id;
	public:
		ASTIdentifier(std::string _id, Position _pos);

		std::string GetID() { return id; }

		std::string ToString(int _indentLvl) override;
		TypeResult TypeCheck(Environment& _env) override;
		ramvm::InstructionSet GenerateCode(std::map<std::string, std::string> _params) override;
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
				default: ss << "Literal::ToString - LiteralType not handled!"; break;
			}

			ss << value;
			return ss.str();
		}

		TypeResult TypeCheck(Environment& _env) override
		{
			switch (litType)
			{
				case LiteralType::INT: return TypeResult::GenSuccess(PrimitiveType::INT);
				case LiteralType::FLOAT: return TypeResult::GenSuccess(PrimitiveType::FLOAT);
				case LiteralType::STRING: return TypeResult::GenSuccess(PrimitiveType::STRING);
				case LiteralType::BOOL: return TypeResult::GenSuccess(PrimitiveType::BOOL);
				default: return TypeResult::GenMismatch("Literal::Typecheck - LiteralType not handled!", GetPosition());
			}
		}

		ramvm::InstructionSet GenerateCode(std::map<std::string, std::string> _params) override = 0;
	};

	struct ASTIntLit : public ASTLiteral<int> {
		ASTIntLit(int _val, Position _pos) : ASTLiteral(LiteralType::INT, _val, _pos) { }
		ramvm::InstructionSet GenerateCode(std::map<std::string, std::string> _params) override;
	};

	struct ASTFloatLit : public ASTLiteral<float> {
		ASTFloatLit(float _val, Position _pos) : ASTLiteral(LiteralType::FLOAT, _val, _pos) { }
		ramvm::InstructionSet GenerateCode(std::map<std::string, std::string> _params) override;
	};

	struct ASTStringLit : public ASTLiteral<std::string> {
		ASTStringLit(std::string _val, Position _pos) : ASTLiteral(LiteralType::STRING, _val, _pos) {}
		ramvm::InstructionSet GenerateCode(std::map<std::string, std::string> _params) override;
	};

	struct ASTBoolLit : public ASTLiteral<bool> {
		ASTBoolLit(bool _val, Position _pos) : ASTLiteral(LiteralType::BOOL, _val, _pos) {}
		ramvm::InstructionSet GenerateCode(std::map<std::string, std::string> _params) override;
	};
#pragma endregion
}

