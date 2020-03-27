#include "pch.h"
#include "ramc_ast.h"
#include "ramvm_parser.h"
#include "ramvm_instruction.h"

using ramvm::Argument;
using ramvm::TypedArgument;
using ramvm::ArgType;
using ramvm::InstrBinop;
using ramvm::InstrUnop;
using ramvm::Binop;
using ramvm::Unop;
using ramvm::InstrMove;
using ramvm::InstrPop;

namespace ramc {
	ASTNode::ASTNode(ASTNodeType _type, Position _pos)
	{
		type = _type;
		position = _pos;
		typeSysType = nullptr;
	}

#pragma region Program
	ASTProgram::ASTProgram(std::string _fileName, std::vector<ASTNode*> _stmts)
		: ASTNode(ASTNodeType::PROGAM, Position(0, 0))
	{
		fileName = _fileName;
		stmts = _stmts;
	}

	ASTProgram::~ASTProgram()
	{
		for (auto it = stmts.begin(); it != stmts.end(); it++)
			delete (*it);
	}

	std::string ASTProgram::ToString(int _indentLvl)
	{
		std::stringstream ss;

		ss << CreateIndent(_indentLvl) << "Start:" << std::endl;
		ss << CreateIndent(_indentLvl + 1) << "Filename: " << fileName << std::endl;
		ss << CreateIndent(_indentLvl + 1) << "Statements: " << std::endl;

		for (auto it = stmts.begin(); it != stmts.end(); it++)
			ss << (*it)->ToString(_indentLvl + 2) << std::endl;

		return ss.str();
	}

	TypeResult ASTProgram::TypeCheck(Environment* _env)
	{
		for (auto const& stmt : stmts)
		{
			TypeResult stmtTypeRes = stmt->GetTypeSysType(_env);
			if (!stmtTypeRes.IsSuccess())
				return stmtTypeRes;
		}

		return TypeResult::GenSuccess(Type::VOID);
	}

	InstructionSet ASTProgram::GenerateCode(std::map<std::string, std::string> _params)
	{
		InstructionSet instrs;

		for (int i = 0; i < (int)stmts.size(); i++)
		{
			auto stmtInstrs = stmts[i]->GenerateCode({ { "Dest", "R0" } });
			instrs.insert(instrs.end(), stmtInstrs.begin(), stmtInstrs.end());
		}

		return instrs;
	}
#pragma endregion

#pragma region BinopExpr
	const std::string BinopTypeToString(BinopType _type)
	{
		const std::map<BinopType, std::string> strings{
			{ BinopType::ADD, "Add" },
			{ BinopType::SUB, "Subtract" },
			{ BinopType::MUL, "Multiply" },
			{ BinopType::DIV, "Divide" },
			{ BinopType::MOD, "Mod" },
			{ BinopType::POW, "Power" },

			{ BinopType::BIN_AND, "Binary And" },
			{ BinopType::BIN_OR, "Binary Or" },
			{ BinopType::BIN_XOR, "Binary Xor" },
			{ BinopType::LSHIFT, "Left Shift" },
			{ BinopType::RSHIFT, "Right Shift" },

			{ BinopType::LT, "Less Than" },
			{ BinopType::GT, "Greater Than" },
			{ BinopType::LT_EQ, "Less Than Equal" },
			{ BinopType::GT_EQ, "Greater Than Equal" },
			{ BinopType::EQ_EQ, "Equal Eqaul" },
			{ BinopType::NEQ, "Not Equal" },
			{ BinopType::LOG_AND, "Logical And" },
			{ BinopType::LOG_OR, "Logical Or" },

			{ BinopType::EQ, "Equal" },
			{ BinopType::ADD_EQ, "Add Equal" },
			{ BinopType::SUB_EQ, "Subtract Equal" },
			{ BinopType::MUL_EQ, "Multiply Equal" },
			{ BinopType::DIV_EQ, "Divide Equal" },
			{ BinopType::MOD_EQ, "Mod Equal" },
			{ BinopType::POW_EQ, "Power Equal" },
			{ BinopType::BIN_AND_EQ, "Binary And Equal" },
			{ BinopType::BIN_OR_EQ, "Binary Or Equal" },
			{ BinopType::BIN_XOR_EQ, "Binary Xor Equal" },
			{ BinopType::LSHIFT_EQ, "Left Shift Equal" },
			{ BinopType::RSHIFT_EQ, "Right Shift Equal" },
		};

		auto search = strings.find(_type);
		return search == strings.end() ? "BinopTypeToString - BinopType not handled!" : search->second;
	}

	ASTBinopExpr::ASTBinopExpr(ASTNode* _left, ASTNode* _right, BinopType _op)
		: ASTNode(ASTNodeType::BINOP_EXPR, _right->GetPosition())
	{
		left = _left;
		right = _right;
		op = _op;
	}

	ASTBinopExpr::~ASTBinopExpr()
	{
		delete left;
		delete right;
	}

	std::string ASTBinopExpr::ToString(int _indentLvl)
	{
		std::stringstream ss;

		ss << CreateIndent(_indentLvl) << "Binop Expression:" << std::endl;
		ss << CreateIndent(_indentLvl + 1) << "OP: " << BinopTypeToString(op) << std::endl;
		ss << left->ToString(_indentLvl + 1) << std::endl;
		ss << right->ToString(_indentLvl + 1);

		return ss.str();
	}

	TypeResult ASTBinopExpr::TypeCheck(Environment* _env)
	{
		TypeResult leftTypeRes = left->GetTypeSysType(_env);

		if (!leftTypeRes.IsSuccess())
			return leftTypeRes;

		TypeResult rightTypeRes = right->GetTypeSysType(_env);

		if (!rightTypeRes.IsSuccess())
			return rightTypeRes;

		Type* leftType = leftTypeRes.GetValue(), * rightType = rightTypeRes.GetValue();
		
		switch (ConcatTriple((byte)op, (byte)leftType->GetType(), (byte)rightType->GetType()))
		{
#pragma region ADD
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);

			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);

			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::INT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::INT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::INT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::INT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::INT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::INT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);

			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);

			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE);

			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::LONG, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::LONG, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::LONG, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);
#pragma endregion

#pragma region SUB
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);

			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);

			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::INT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::INT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::INT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::INT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::INT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::INT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);

			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);

			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE);

			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::LONG, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::LONG, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::LONG, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);
#pragma endregion

#pragma region MUL
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);

			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);

			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::INT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::INT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::INT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::INT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::INT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::INT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);

			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);

			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE);

			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::LONG, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::LONG, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::LONG, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);
#pragma endregion

#pragma region DIV
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);

			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);

			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::INT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::INT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::INT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::INT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::INT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::INT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);

			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);

			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE);

			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::LONG, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::LONG, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::LONG, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);
#pragma endregion

#pragma region MOD
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);

			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);

			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::INT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::INT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::INT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::INT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::INT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::INT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);

			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);

			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE);

			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::LONG, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::LONG, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::LONG, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);
#pragma endregion

#pragma region POW
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);

			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);

			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::INT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::INT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::INT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::INT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::INT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::INT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);

			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);

			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE);

			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::LONG, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::LONG, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::LONG, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);
#pragma endregion
			
			default: return TypeResult::GenMismatch("Cannot perform addition on " + leftType->ToString(false) + " and " + rightType->ToString(false), GetPosition());
		}

		/*
		switch (op)
		{
			case BinopType::ADD: {
				if (leftType == Type::INT && rightType == Type::INT) { return TypeResult::GenSuccess(Type::INT); }
				else if (leftType == Type::INT && rightType == Type::FLOAT) { return TypeResult::GenSuccess(Type::FLOAT); }
				else if (leftType == Type::FLOAT && rightType == Type::INT) { return TypeResult::GenSuccess(Type::FLOAT); }
				else if (leftType == Type::FLOAT && rightType == Type::FLOAT) { return TypeResult::GenSuccess(Type::FLOAT); }
				else if (leftType == Type::STRING && rightType == Type::INT) { return TypeResult::GenSuccess(Type::STRING); }
				else if (leftType == Type::STRING && rightType == Type::FLOAT) { return TypeResult::GenSuccess(Type::STRING); }
				else if (leftType == Type::STRING && rightType == Type::STRING) { return TypeResult::GenSuccess(Type::STRING); }
				else if (leftType == Type::INT && rightType == Type::STRING) { return TypeResult::GenSuccess(Type::STRING); }
				else if (leftType == Type::FLOAT && rightType == Type::STRING) { return TypeResult::GenSuccess(Type::STRING); }
				else { return TypeResult::GenMismatch("Cannot perform addition on " + leftType->ToString(false) + " and " + rightType->ToString(false), GetPosition()); }
			}
			case BinopType::SUB: {
				if (leftType == Type::INT && rightType == Type::INT) { return TypeResult::GenSuccess(Type::INT); }
				else if (leftType == Type::INT && rightType == Type::FLOAT) { return TypeResult::GenSuccess(Type::FLOAT); }
				else if (leftType == Type::FLOAT && rightType == Type::INT) { return TypeResult::GenSuccess(Type::FLOAT); }
				else if (leftType == Type::FLOAT && rightType == Type::FLOAT) { return TypeResult::GenSuccess(Type::FLOAT); }
				else { return TypeResult::GenMismatch("Cannot perform subtraction on " + leftType->ToString(false) + " and " + rightType->ToString(false), GetPosition()); }
			}
			case BinopType::MUL: {
				if (leftType == Type::INT && rightType == Type::INT) { return TypeResult::GenSuccess(Type::INT); }
				else if (leftType == Type::INT && rightType == Type::FLOAT) { return TypeResult::GenSuccess(Type::FLOAT); }
				else if (leftType == Type::FLOAT && rightType == Type::INT) { return TypeResult::GenSuccess(Type::FLOAT); }
				else if (leftType == Type::FLOAT && rightType == Type::FLOAT) { return TypeResult::GenSuccess(Type::FLOAT); }
				else if (leftType == Type::STRING && rightType == Type::INT) { return TypeResult::GenSuccess(Type::STRING); }
				else if (leftType == Type::INT && rightType == Type::STRING) { return TypeResult::GenSuccess(Type::STRING); }
				else { return TypeResult::GenMismatch("Cannot perform multiplication on " + leftType->ToString(false) + " and " + rightType->ToString(false), GetPosition()); }
			}
			case BinopType::DIV: {
				if (leftType == Type::INT && rightType == Type::INT) { return TypeResult::GenSuccess(Type::INT); }
				else if (leftType == Type::INT && rightType == Type::FLOAT) { return TypeResult::GenSuccess(Type::FLOAT); }
				else if (leftType == Type::FLOAT && rightType == Type::INT) { return TypeResult::GenSuccess(Type::FLOAT); }
				else if (leftType == Type::FLOAT && rightType == Type::FLOAT) { return TypeResult::GenSuccess(Type::FLOAT); }
				else { return TypeResult::GenMismatch("Cannot perform diviiosn on " + leftType->ToString(false) + " and " + rightType->ToString(false), GetPosition()); }
			}
			case BinopType::MOD: {
				if (leftType == Type::INT && rightType == Type::INT) { return TypeResult::GenSuccess(Type::INT); }
				else if (leftType == Type::INT && rightType == Type::FLOAT) { return TypeResult::GenSuccess(Type::FLOAT); }
				else if (leftType == Type::FLOAT && rightType == Type::INT) { return TypeResult::GenSuccess(Type::FLOAT); }
				else if (leftType == Type::FLOAT && rightType == Type::FLOAT) { return TypeResult::GenSuccess(Type::FLOAT); }
				else { return TypeResult::GenMismatch("Cannot perform modulo on " + leftType->ToString(false) + " and " + rightType->ToString(false), GetPosition()); }
			}
			case BinopType::POW: {
				if (leftType == Type::INT && rightType == Type::INT) { return TypeResult::GenSuccess(Type::INT); }
				else if (leftType == Type::INT && rightType == Type::FLOAT) { return TypeResult::GenSuccess(Type::FLOAT); }
				else if (leftType == Type::FLOAT && rightType == Type::INT) { return TypeResult::GenSuccess(Type::FLOAT); }
				else if (leftType == Type::FLOAT && rightType == Type::FLOAT) { return TypeResult::GenSuccess(Type::FLOAT); }
				else { return TypeResult::GenMismatch("Cannot perform exponentiation on " + leftType->ToString(false) + " and " + rightType->ToString(false), GetPosition()); }
			}
			case BinopType::BIN_AND: {
				if (leftType == Type::INT && rightType == Type::INT) { return TypeResult::GenSuccess(Type::INT); }
				else if (leftType == Type::INT && rightType == Type::FLOAT) { return TypeResult::GenSuccess(Type::FLOAT); }
				else if (leftType == Type::FLOAT && rightType == Type::INT) { return TypeResult::GenSuccess(Type::FLOAT); }
				else if (leftType == Type::FLOAT && rightType == Type::FLOAT) { return TypeResult::GenSuccess(Type::FLOAT); }
				else { return TypeResult::GenMismatch("Cannot perform binary and on " + leftType->ToString(false) + " and " + rightType->ToString(false), GetPosition()); }
			}
			case BinopType::BIN_OR: {
				if (leftType == Type::INT && rightType == Type::INT) { return TypeResult::GenSuccess(Type::INT); }
				else if (leftType == Type::INT && rightType == Type::FLOAT) { return TypeResult::GenSuccess(Type::FLOAT); }
				else if (leftType == Type::FLOAT && rightType == Type::INT) { return TypeResult::GenSuccess(Type::FLOAT); }
				else if (leftType == Type::FLOAT && rightType == Type::FLOAT) { return TypeResult::GenSuccess(Type::FLOAT); }
				else { return TypeResult::GenMismatch("Cannot perform binary or on " + leftType->ToString(false) + " and " + rightType->ToString(false), GetPosition()); }
			}
			case BinopType::BIN_XOR: {
				if (leftType == Type::INT && rightType == Type::INT) { return TypeResult::GenSuccess(Type::INT); }
				else if (leftType == Type::INT && rightType == Type::FLOAT) { return TypeResult::GenSuccess(Type::FLOAT); }
				else if (leftType == Type::FLOAT && rightType == Type::INT) { return TypeResult::GenSuccess(Type::FLOAT); }
				else if (leftType == Type::FLOAT && rightType == Type::FLOAT) { return TypeResult::GenSuccess(Type::FLOAT); }
				else { return TypeResult::GenMismatch("Cannot perform binary xor on " + leftType->ToString(false) + " and " + rightType->ToString(false), GetPosition()); }
			}
			case BinopType::LSHIFT: {
				if (leftType == Type::INT && rightType == Type::INT) { return TypeResult::GenSuccess(Type::INT); }
				else if (leftType == Type::INT && rightType == Type::FLOAT) { return TypeResult::GenSuccess(Type::FLOAT); }
				else if (leftType == Type::FLOAT && rightType == Type::INT) { return TypeResult::GenSuccess(Type::FLOAT); }
				else if (leftType == Type::FLOAT && rightType == Type::FLOAT) { return TypeResult::GenSuccess(Type::FLOAT); }
				else { return TypeResult::GenMismatch("Cannot perform left shift on " + leftType->ToString(false) + " and " + rightType->ToString(false), GetPosition()); }
			}
			case BinopType::RSHIFT: {
				if (leftType == Type::INT && rightType == Type::INT) { return TypeResult::GenSuccess(Type::INT); }
				else if (leftType == Type::INT && rightType == Type::FLOAT) { return TypeResult::GenSuccess(Type::FLOAT); }
				else if (leftType == Type::FLOAT && rightType == Type::INT) { return TypeResult::GenSuccess(Type::FLOAT); }
				else if (leftType == Type::FLOAT && rightType == Type::FLOAT) { return TypeResult::GenSuccess(Type::FLOAT); }
				else { return TypeResult::GenMismatch("Cannot perform right shift on " + leftType->ToString(false) + " and " + rightType->ToString(false), GetPosition()); }
			}
			case BinopType::LT: {
				if (leftType == Type::INT && rightType == Type::INT) { return TypeResult::GenSuccess(Type::BOOL); }
				else if (leftType == Type::INT && rightType == Type::FLOAT) { return TypeResult::GenSuccess(Type::BOOL); }
				else if (leftType == Type::FLOAT && rightType == Type::INT) { return TypeResult::GenSuccess(Type::BOOL); }
				else if (leftType == Type::FLOAT && rightType == Type::FLOAT) { return TypeResult::GenSuccess(Type::BOOL); }
				else { return TypeResult::GenMismatch("Cannot compare " + leftType->ToString(false) + " and " + rightType->ToString(false), GetPosition()); }
			}
			case BinopType::GT: {
				if (leftType == Type::INT && rightType == Type::INT) { return TypeResult::GenSuccess(Type::BOOL); }
				else if (leftType == Type::INT && rightType == Type::FLOAT) { return TypeResult::GenSuccess(Type::BOOL); }
				else if (leftType == Type::FLOAT && rightType == Type::INT) { return TypeResult::GenSuccess(Type::BOOL); }
				else if (leftType == Type::FLOAT && rightType == Type::FLOAT) { return TypeResult::GenSuccess(Type::BOOL); }
				else { return TypeResult::GenMismatch("Cannot compare " + leftType->ToString(false) + " and " + rightType->ToString(false), GetPosition()); }
			}
			case BinopType::LT_EQ: {
				if (leftType == Type::INT && rightType == Type::INT) { return TypeResult::GenSuccess(Type::BOOL); }
				else if (leftType == Type::INT && rightType == Type::FLOAT) { return TypeResult::GenSuccess(Type::BOOL); }
				else if (leftType == Type::FLOAT && rightType == Type::INT) { return TypeResult::GenSuccess(Type::BOOL); }
				else if (leftType == Type::FLOAT && rightType == Type::FLOAT) { return TypeResult::GenSuccess(Type::BOOL); }
				else { return TypeResult::GenMismatch("Cannot compare " + leftType->ToString(false) + " and " + rightType->ToString(false), GetPosition()); }
			}
			case BinopType::GT_EQ: {
				if (leftType == Type::INT && rightType == Type::INT) { return TypeResult::GenSuccess(Type::BOOL); }
				else if (leftType == Type::INT && rightType == Type::FLOAT) { return TypeResult::GenSuccess(Type::BOOL); }
				else if (leftType == Type::FLOAT && rightType == Type::INT) { return TypeResult::GenSuccess(Type::BOOL); }
				else if (leftType == Type::FLOAT && rightType == Type::FLOAT) { return TypeResult::GenSuccess(Type::BOOL); }
				else { return TypeResult::GenMismatch("Cannot compare " + leftType->ToString(false) + " and " + rightType->ToString(false), GetPosition()); }
			}
			case BinopType::EQ_EQ: return TypeResult::GenSuccess(Type::BOOL);
			case BinopType::NEQ: return TypeResult::GenSuccess(Type::BOOL);
			case BinopType::LOG_AND: {
				if (leftType == Type::BOOL && rightType == Type::BOOL) { return TypeResult::GenSuccess(Type::BOOL); }
				else { return TypeResult::GenMismatch("Cannot perform logical and on " + leftType->ToString(false) + " and " + rightType->ToString(false), GetPosition()); }
			}
			case BinopType::LOG_OR: {
				if (leftType == Type::BOOL && rightType == Type::BOOL) { return TypeResult::GenSuccess(Type::BOOL); }
				else { return TypeResult::GenMismatch("Cannot perform logical and on " + leftType->ToString(false) + " and " + rightType->ToString(false), GetPosition()); }
			}
			case BinopType::EQ: return TypeResult::GenMismatch("ATSUnopExpr::TypeCheck - Operation not handled!", GetPosition());
			case BinopType::ADD_EQ: return TypeResult::GenMismatch("ATSUnopExpr::TypeCheck - Operation not handled!", GetPosition());
			case BinopType::SUB_EQ: return TypeResult::GenMismatch("ATSUnopExpr::TypeCheck - Operation not handled!", GetPosition());
			case BinopType::MUL_EQ: return TypeResult::GenMismatch("ATSUnopExpr::TypeCheck - Operation not handled!", GetPosition());
			case BinopType::DIV_EQ: return TypeResult::GenMismatch("ATSUnopExpr::TypeCheck - Operation not handled!", GetPosition());
			case BinopType::MOD_EQ: return TypeResult::GenMismatch("ATSUnopExpr::TypeCheck - Operation not handled!", GetPosition());
			case BinopType::POW_EQ: return TypeResult::GenMismatch("ATSUnopExpr::TypeCheck - Operation not handled!", GetPosition());
			case BinopType::BIN_AND_EQ: return TypeResult::GenMismatch("ATSUnopExpr::TypeCheck - Operation not handled!", GetPosition());
			case BinopType::BIN_OR_EQ: return TypeResult::GenMismatch("ATSUnopExpr::TypeCheck - Operation not handled!", GetPosition());
			case BinopType::BIN_XOR_EQ: return TypeResult::GenMismatch("ATSUnopExpr::TypeCheck - Operation not handled!", GetPosition());
			case BinopType::LSHIFT_EQ: return TypeResult::GenMismatch("ATSUnopExpr::TypeCheck - Operation not handled!", GetPosition());
			case BinopType::RSHIFT_EQ: return TypeResult::GenMismatch("ATSUnopExpr::TypeCheck - Operation not handled!", GetPosition());
			default: return TypeResult::GenMismatch("ATSBinopExpr::TypeCheck - Operation not handled!", GetPosition());
		}*/
	}

	InstructionSet ASTBinopExpr::GenerateCode(std::map<std::string, std::string> _params)
	{
		InstructionSet instrs;

		auto leftInstrs = left->GenerateCode({ {"Dest", "[1]"} });
		auto rightInstrs = right->GenerateCode({ {"Dest", "[1]" } });

		instrs.insert(instrs.end(), leftInstrs.begin(), leftInstrs.end());
		instrs.insert(instrs.end(), rightInstrs.begin(), rightInstrs.end());

		auto src1DataType = TypeSysTypeToDataType(left->GetTypeSysType(nullptr).GetValue()->GetType());
		auto src2DataType = TypeSysTypeToDataType(right->GetTypeSysType(nullptr).GetValue()->GetType());
		auto destDataType = TypeSysTypeToDataType(this->GetTypeSysType(nullptr).GetValue()->GetType());

		auto src2 = TypedArgument(src2DataType, ArgType::SP_OFFSET, -1 * GetDataTypeSize(src2DataType) + 1);
		auto src1 = TypedArgument(src1DataType, ArgType::SP_OFFSET, src2.value.i - GetDataTypeSize(src1DataType));
		auto stackDest = TypedArgument(destDataType, ArgType::SP_OFFSET, src1.value);

		switch (op)
		{
			case BinopType::ADD: instrs.push_back(new InstrBinop(Binop::ADD, src1, src2, stackDest)); break;
			case BinopType::SUB: instrs.push_back(new InstrBinop(Binop::SUB, src1, src2, stackDest)); break;
			case BinopType::MUL: instrs.push_back(new InstrBinop(Binop::MUL, src1, src2, stackDest)); break;
			case BinopType::DIV: instrs.push_back(new InstrBinop(Binop::DIV, src1, src2, stackDest)); break;
			case BinopType::MOD: instrs.push_back(new InstrBinop(Binop::MOD, src1, src2, stackDest)); break;
			case BinopType::POW: instrs.push_back(new InstrBinop(Binop::POW, src1, src2, stackDest)); break;
			case BinopType::BIN_AND: instrs.push_back(new InstrBinop(Binop::BIT_AND, src1, src2, stackDest)); break;
			case BinopType::BIN_OR: instrs.push_back(new InstrBinop(Binop::BIT_OR, src1, src2, stackDest)); break;
			case BinopType::BIN_XOR: instrs.push_back(new InstrBinop(Binop::BIT_XOR, src1, src2, stackDest)); break;
			case BinopType::LSHIFT: instrs.push_back(new InstrBinop(Binop::LSHIFT, src1, src2, stackDest)); break;
			case BinopType::RSHIFT: instrs.push_back(new InstrBinop(Binop::RSHIFT, src1, src2, stackDest)); break;
			case BinopType::LT: instrs.push_back(new InstrBinop(Binop::LT, src1, src2, stackDest)); break;
			case BinopType::GT: instrs.push_back(new InstrBinop(Binop::GT, src1, src2, stackDest)); break;
			case BinopType::LT_EQ: instrs.push_back(new InstrBinop(Binop::LTEQ, src1, src2, stackDest)); break;
			case BinopType::GT_EQ: instrs.push_back(new InstrBinop(Binop::GTEQ, src1, src2, stackDest)); break;
			case BinopType::EQ_EQ: instrs.push_back(new InstrBinop(Binop::EQ, src1, src2, stackDest)); break;
			case BinopType::NEQ: instrs.push_back(new InstrBinop(Binop::NEQ, src1, src2, stackDest)); break;
			case BinopType::LOG_AND: instrs.push_back(new InstrBinop(Binop::LOG_AND, src1, src2, stackDest)); break;
			case BinopType::LOG_OR: instrs.push_back(new InstrBinop(Binop::LOG_OR, src1, src2, stackDest)); break;
			case BinopType::EQ: throw "BinopExpr::GenerateCode - BinopType not handled!";
			case BinopType::ADD_EQ: throw "BinopExpr::GenerateCode - BinopType not handled!";
			case BinopType::SUB_EQ: throw "BinopExpr::GenerateCode - BinopType not handled!";
			case BinopType::MUL_EQ: throw "BinopExpr::GenerateCode - BinopType not handled!";
			case BinopType::DIV_EQ: throw "BinopExpr::GenerateCode - BinopType not handled!";
			case BinopType::MOD_EQ: throw "BinopExpr::GenerateCode - BinopType not handled!";
			case BinopType::POW_EQ: throw "BinopExpr::GenerateCode - BinopType not handled!";
			case BinopType::BIN_AND_EQ: throw "BinopExpr::GenerateCode - BinopType not handled!";
			case BinopType::BIN_OR_EQ: throw "BinopExpr::GenerateCode - BinopType not handled!";
			case BinopType::BIN_XOR_EQ: throw "BinopExpr::GenerateCode - BinopType not handled!";
			case BinopType::LSHIFT_EQ: throw "BinopExpr::GenerateCode - BinopType not handled!";
			case BinopType::RSHIFT_EQ: throw "BinopExpr::GenerateCode - BinopType not handled!";
			default: throw "BinopExpr::GenerateCode - BinopType not handled!";
		}

		//Create Pop to remove the bytes past the result
		instrs.push_back(new InstrPop(DataType::BYTE, Argument(ArgType::VALUE, -stackDest.value.i - GetDataTypeSize(stackDest.dataType) + 1)));

		//Create Move to move result to destination
		auto dest = Argument();
		IGNORE(ParseArgument(_params["Dest"], dest));

		if (!dest.IsStackTop())
		{
			instrs.push_back(new InstrMove(destDataType, Argument(ArgType::SP_OFFSET, -1 * GetDataTypeSize(destDataType) + 1), dest));
			instrs.push_back(new InstrPop(destDataType, Argument(ArgType::VALUE, 1)));
		}

		return instrs;
	}
#pragma endregion

#pragma region UnopExpr
	const std::string UnopTypeToString(UnopType _type)
	{
		const std::map<UnopType, std::string> strings{
			{ UnopType::NEG, "NEGATE" },
			{ UnopType::LOG_NOT, "LOGICAL NOT" },
			{ UnopType::PRE_INC, "PRE_INCREMENT" },
			{ UnopType::POST_INC, "POST_INCREMENT" },
			{ UnopType::PRE_DEC, "PRE-DECREMENT" },
			{ UnopType::POST_DEC, "POST-DECREMENT" }
		};

		auto search = strings.find(_type);
		return search == strings.end() ? "UnopTypeToString - UnopType not handled!" : search->second;
	}

	ASTUnopExpr::ASTUnopExpr(ASTNode* _right, UnopType _op)
		: ASTNode(ASTNodeType::UNOP_EXPR, _right->GetPosition())
	{
		right = _right;
		op = _op;
	}

	ASTUnopExpr::~ASTUnopExpr() { delete right; }

	std::string ASTUnopExpr::ToString(int _indentLvl)
	{
		std::stringstream ss;

		ss << CreateIndent(_indentLvl) << "Unop Expression:" << std::endl;
		ss << CreateIndent(_indentLvl + 1) << "OP: " << UnopTypeToString(op) << std::endl;
		ss << right->ToString(_indentLvl + 1);

		return ss.str();
	}

	TypeResult ASTUnopExpr::TypeCheck(Environment* _env)
	{
		TypeResult typeRes = right->GetTypeSysType(_env);

		if (!typeRes.IsSuccess())
			return typeRes;

		Type* primType = typeRes.GetValue();

		switch (op)
		{
			case UnopType::NEG: {
				if (primType == Type::INT || primType == Type::FLOAT) { return typeRes; }
				else { return TypeResult::GenMismatch("Expected an int or float on the right of arithmetic negation but found " + primType->ToString(false), GetPosition()); }
			}
			case UnopType::LOG_NOT: {
				if (primType == Type::BOOL) { return typeRes; }
				else { return TypeResult::GenMismatch("Expected an bool on the right of logical negation but found " + primType->ToString(false), GetPosition()); }
			}
			case UnopType::PRE_INC: {
				if (primType == Type::INT || primType == Type::FLOAT) { return TypeResult::GenSuccess(Type::VOID); }
				else { return TypeResult::GenMismatch("Expected an int or float on the right of increment but found " + primType->ToString(false), GetPosition()); }
			}
			case UnopType::POST_INC: {
				if (primType == Type::INT || primType == Type::FLOAT) { return TypeResult::GenSuccess(Type::VOID); }
				else { return TypeResult::GenMismatch("Expected an int or float on the left of increment but found " + primType->ToString(false), GetPosition()); }
			}
			case UnopType::PRE_DEC: {
				if (primType == Type::INT || primType == Type::FLOAT) { return TypeResult::GenSuccess(Type::VOID); }
				else { return TypeResult::GenMismatch("Expected an int or float on the right of decrement but found " + primType->ToString(false), GetPosition()); }
			}
			case UnopType::POST_DEC: {
				if (primType == Type::INT || primType == Type::FLOAT) { return TypeResult::GenSuccess(Type::VOID); }
				else { return TypeResult::GenMismatch("Expected an int or float on the left of decrement but found " + primType->ToString(false), GetPosition()); }
			}
			default: return TypeResult::GenMismatch("ATSUnopExpr::TypeCheck - Operation not handled!", GetPosition());
		}
	}

	InstructionSet ASTUnopExpr::GenerateCode(std::map<std::string, std::string> _params)
	{
		InstructionSet instrs = right->GenerateCode({ {"Dest", "[1]" } });
		auto src = TypedArgument(DataType::INT, ArgType::SP_OFFSET, -1 * INT_SIZE + 1);

		switch (op)
		{
			case UnopType::LOG_NOT: instrs.push_back(new InstrUnop(Unop::LOG_NOT, src, src)); break;
			case UnopType::NEG: instrs.push_back(new InstrUnop(Unop::NEG, src, src)); break;
			case UnopType::PRE_INC: throw "UnopExpr::GenerateCode - UnopType not handled!";
			case UnopType::PRE_DEC: throw "UnopExpr::GenerateCode - UnopType not handled!";
			case UnopType::POST_INC: throw "UnopExpr::GenerateCode - UnopType not handled!";
			case UnopType::POST_DEC: throw "UnopExpr::GenerateCode - UnopType not handled!";
			default: throw "UnopExpr::GenerateCode - UnopType not handled!";
		}


		auto dest = ::Argument();
		IGNORE(ParseArgument(_params["Dest"], dest));

		if (!dest.IsStackTop())
		{
			instrs.push_back(new ::InstrMove(DataType::INT, Argument(ArgType::SP_OFFSET, -1 * INT_SIZE + 1), dest));

			//TODO gotta change the pop scale to be based on the return type of this unop
			//This will require you to store the type during typechecking
			instrs.push_back(new ::InstrPop(DataType::INT, Argument(ArgType::VALUE, 1)));
		}

		return instrs;
	}
#pragma endregion

#pragma region Identifier
	ASTIdentifier::ASTIdentifier(std::string _id, Position _pos)
		: ASTNode(ASTNodeType::IDENTIFIER, _pos)
	{
		id = _id;
	}

	std::string ASTIdentifier::ToString(int _indentLvl)
	{
		return CreateIndent(_indentLvl) + "Identifier: " + id;
	}

	TypeResult ASTIdentifier::TypeCheck(Environment* _env)
	{
		return _env->GetVariableType(id, GetPosition());
	}

	InstructionSet ASTIdentifier::GenerateCode(std::map<std::string, std::string> _params)
	{
		InstructionSet instrs;

		return instrs;
	}
#pragma endregion

#pragma region Literal
	InstructionSet ASTIntLit::GenerateCode(std::map<std::string, std::string> _params)
	{
		auto dest = Argument();
		IGNORE(ParseArgument(_params["Dest"], dest));

		return { new InstrMove(DataType::INT, Argument(ArgType::VALUE, GetValue()), dest) };
	}

	InstructionSet ASTFloatLit::GenerateCode(std::map<std::string, std::string> _params)
	{
		auto dest = Argument();
		IGNORE(ParseArgument(_params["Dest"], dest));

		return { new InstrMove(DataType::FLOAT, Argument(ArgType::VALUE, GetValue()), dest) };
	}

	InstructionSet ASTStringLit::GenerateCode(std::map<std::string, std::string> _params)
	{
		auto dest = Argument();
		IGNORE(ParseArgument(_params["Dest"], dest));

		return { new InstrMove(DataType::INT, Argument(ArgType::VALUE, 0), dest) };
	}

	InstructionSet ASTBoolLit::GenerateCode(std::map<std::string, std::string> _params)
	{
		auto dest = Argument();
		IGNORE(ParseArgument(_params["Dest"], dest));

		return { new InstrMove(DataType::BYTE, Argument(ArgType::VALUE, (byte)(GetValue() ? 1 : 0)), dest) };
	}

	InstructionSet ASTByteLit::GenerateCode(std::map<std::string, std::string> _params)
	{
		auto dest = Argument();
		IGNORE(ParseArgument(_params["Dest"], dest));

		return { new InstrMove(DataType::BYTE, Argument(ArgType::VALUE, GetValue()), dest) };
	}

	InstructionSet ASTDoubleLit::GenerateCode(std::map<std::string, std::string> _params)
	{
		auto dest = Argument();
		IGNORE(ParseArgument(_params["Dest"], dest));

		return { new InstrMove(DataType::DOUBLE, Argument(ArgType::VALUE, GetValue()), dest) };
	}

	InstructionSet ASTLongLit::GenerateCode(std::map<std::string, std::string> _params)
	{
		auto dest = Argument();
		IGNORE(ParseArgument(_params["Dest"], dest));

		return { new InstrMove(DataType::LONG, Argument(ArgType::VALUE, GetValue()), dest) };
	}
#pragma endregion
}