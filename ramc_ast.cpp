#include "pch.h"
#include "ramc_ast.h"
#include "ramvm_parser.h"
#include "ramvm_instruction.h"

using ramvm::Argument;
using ramvm::TypedArgument;
using ramvm::InstrBinop;

namespace ramc {
	ASTNode::ASTNode(ASTNodeType _type, Position _pos)
	{
		type = _type;
		position = _pos;
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

	TypeResult ASTProgram::TypeCheck(Environment& _env)
	{
		for (auto const& stmt : stmts)
		{
			TypeResult stmtTypeRes = stmt->TypeCheck(_env);
			if (!stmtTypeRes.IsSuccess())
				return stmtTypeRes;
		}

		return TypeResult::GenSuccess(PrimitiveType::VOID);
	}

	ramvm::InstructionSet ASTProgram::GenerateCode(std::map<std::string, std::string> _params)
	{
		ramvm::InstructionSet instrs;

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

	TypeResult ASTBinopExpr::TypeCheck(Environment& _env)
	{
		TypeResult leftTypeRes = left->TypeCheck(_env);

		if (!leftTypeRes.IsSuccess())
			return leftTypeRes;

		TypeResult rightTypeRes = right->TypeCheck(_env);

		if (!rightTypeRes.IsSuccess())
			return rightTypeRes;

		PrimitiveType leftType = leftTypeRes.GetType(), rightType = rightTypeRes.GetType();

		switch (op)
		{
			case ramc::BinopType::ADD: {
				if (leftType == PrimitiveType::INT && rightType == PrimitiveType::INT) { return TypeResult::GenSuccess(PrimitiveType::INT); }
				else if (leftType == PrimitiveType::INT && rightType == PrimitiveType::FLOAT) { return TypeResult::GenSuccess(PrimitiveType::FLOAT); }
				else if (leftType == PrimitiveType::FLOAT && rightType == PrimitiveType::INT) { return TypeResult::GenSuccess(PrimitiveType::FLOAT); }
				else if (leftType == PrimitiveType::FLOAT && rightType == PrimitiveType::FLOAT) { return TypeResult::GenSuccess(PrimitiveType::FLOAT); }
				else if (leftType == PrimitiveType::STRING && rightType == PrimitiveType::INT) { return TypeResult::GenSuccess(PrimitiveType::STRING); }
				else if (leftType == PrimitiveType::STRING && rightType == PrimitiveType::FLOAT) { return TypeResult::GenSuccess(PrimitiveType::STRING); }
				else if (leftType == PrimitiveType::STRING && rightType == PrimitiveType::STRING) { return TypeResult::GenSuccess(PrimitiveType::STRING); }
				else if (leftType == PrimitiveType::INT && rightType == PrimitiveType::STRING) { return TypeResult::GenSuccess(PrimitiveType::STRING); }
				else if (leftType == PrimitiveType::FLOAT && rightType == PrimitiveType::STRING) { return TypeResult::GenSuccess(PrimitiveType::STRING); }
				else { return TypeResult::GenMismatch("Cannot perform addition on " + PrimTypeToString(leftType) + " and " + PrimTypeToString(rightType), GetPosition()); }
			}
			case ramc::BinopType::SUB: {
				if (leftType == PrimitiveType::INT && rightType == PrimitiveType::INT) { return TypeResult::GenSuccess(PrimitiveType::INT); }
				else if (leftType == PrimitiveType::INT && rightType == PrimitiveType::FLOAT) { return TypeResult::GenSuccess(PrimitiveType::FLOAT); }
				else if (leftType == PrimitiveType::FLOAT && rightType == PrimitiveType::INT) { return TypeResult::GenSuccess(PrimitiveType::FLOAT); }
				else if (leftType == PrimitiveType::FLOAT && rightType == PrimitiveType::FLOAT) { return TypeResult::GenSuccess(PrimitiveType::FLOAT); }
				else { return TypeResult::GenMismatch("Cannot perform subtraction on " + PrimTypeToString(leftType) + " and " + PrimTypeToString(rightType), GetPosition()); }
			}
			case ramc::BinopType::MUL: {
				if (leftType == PrimitiveType::INT && rightType == PrimitiveType::INT) { return TypeResult::GenSuccess(PrimitiveType::INT); }
				else if (leftType == PrimitiveType::INT && rightType == PrimitiveType::FLOAT) { return TypeResult::GenSuccess(PrimitiveType::FLOAT); }
				else if (leftType == PrimitiveType::FLOAT && rightType == PrimitiveType::INT) { return TypeResult::GenSuccess(PrimitiveType::FLOAT); }
				else if (leftType == PrimitiveType::FLOAT && rightType == PrimitiveType::FLOAT) { return TypeResult::GenSuccess(PrimitiveType::FLOAT); }
				else if (leftType == PrimitiveType::STRING && rightType == PrimitiveType::INT) { return TypeResult::GenSuccess(PrimitiveType::STRING); }
				else if (leftType == PrimitiveType::INT && rightType == PrimitiveType::STRING) { return TypeResult::GenSuccess(PrimitiveType::STRING); }
				else { return TypeResult::GenMismatch("Cannot perform multiplication on " + PrimTypeToString(leftType) + " and " + PrimTypeToString(rightType), GetPosition()); }
			}
			case ramc::BinopType::DIV: {
				if (leftType == PrimitiveType::INT && rightType == PrimitiveType::INT) { return TypeResult::GenSuccess(PrimitiveType::INT); }
				else if (leftType == PrimitiveType::INT && rightType == PrimitiveType::FLOAT) { return TypeResult::GenSuccess(PrimitiveType::FLOAT); }
				else if (leftType == PrimitiveType::FLOAT && rightType == PrimitiveType::INT) { return TypeResult::GenSuccess(PrimitiveType::FLOAT); }
				else if (leftType == PrimitiveType::FLOAT && rightType == PrimitiveType::FLOAT) { return TypeResult::GenSuccess(PrimitiveType::FLOAT); }
				else { return TypeResult::GenMismatch("Cannot perform diviiosn on " + PrimTypeToString(leftType) + " and " + PrimTypeToString(rightType), GetPosition()); }
			}
			case ramc::BinopType::MOD: {
				if (leftType == PrimitiveType::INT && rightType == PrimitiveType::INT) { return TypeResult::GenSuccess(PrimitiveType::INT); }
				else if (leftType == PrimitiveType::INT && rightType == PrimitiveType::FLOAT) { return TypeResult::GenSuccess(PrimitiveType::FLOAT); }
				else if (leftType == PrimitiveType::FLOAT && rightType == PrimitiveType::INT) { return TypeResult::GenSuccess(PrimitiveType::FLOAT); }
				else if (leftType == PrimitiveType::FLOAT && rightType == PrimitiveType::FLOAT) { return TypeResult::GenSuccess(PrimitiveType::FLOAT); }
				else { return TypeResult::GenMismatch("Cannot perform modulo on " + PrimTypeToString(leftType) + " and " + PrimTypeToString(rightType), GetPosition()); }
			}
			case ramc::BinopType::POW: {
				if (leftType == PrimitiveType::INT && rightType == PrimitiveType::INT) { return TypeResult::GenSuccess(PrimitiveType::INT); }
				else if (leftType == PrimitiveType::INT && rightType == PrimitiveType::FLOAT) { return TypeResult::GenSuccess(PrimitiveType::FLOAT); }
				else if (leftType == PrimitiveType::FLOAT && rightType == PrimitiveType::INT) { return TypeResult::GenSuccess(PrimitiveType::FLOAT); }
				else if (leftType == PrimitiveType::FLOAT && rightType == PrimitiveType::FLOAT) { return TypeResult::GenSuccess(PrimitiveType::FLOAT); }
				else { return TypeResult::GenMismatch("Cannot perform exponentiation on " + PrimTypeToString(leftType) + " and " + PrimTypeToString(rightType), GetPosition()); }
			}
			case ramc::BinopType::BIN_AND: {
				if (leftType == PrimitiveType::INT && rightType == PrimitiveType::INT) { return TypeResult::GenSuccess(PrimitiveType::INT); }
				else if (leftType == PrimitiveType::INT && rightType == PrimitiveType::FLOAT) { return TypeResult::GenSuccess(PrimitiveType::FLOAT); }
				else if (leftType == PrimitiveType::FLOAT && rightType == PrimitiveType::INT) { return TypeResult::GenSuccess(PrimitiveType::FLOAT); }
				else if (leftType == PrimitiveType::FLOAT && rightType == PrimitiveType::FLOAT) { return TypeResult::GenSuccess(PrimitiveType::FLOAT); }
				else { return TypeResult::GenMismatch("Cannot perform binary and on " + PrimTypeToString(leftType) + " and " + PrimTypeToString(rightType), GetPosition()); }
			}
			case ramc::BinopType::BIN_OR: {
				if (leftType == PrimitiveType::INT && rightType == PrimitiveType::INT) { return TypeResult::GenSuccess(PrimitiveType::INT); }
				else if (leftType == PrimitiveType::INT && rightType == PrimitiveType::FLOAT) { return TypeResult::GenSuccess(PrimitiveType::FLOAT); }
				else if (leftType == PrimitiveType::FLOAT && rightType == PrimitiveType::INT) { return TypeResult::GenSuccess(PrimitiveType::FLOAT); }
				else if (leftType == PrimitiveType::FLOAT && rightType == PrimitiveType::FLOAT) { return TypeResult::GenSuccess(PrimitiveType::FLOAT); }
				else { return TypeResult::GenMismatch("Cannot perform binary or on " + PrimTypeToString(leftType) + " and " + PrimTypeToString(rightType), GetPosition()); }
			}
			case ramc::BinopType::BIN_XOR: {
				if (leftType == PrimitiveType::INT && rightType == PrimitiveType::INT) { return TypeResult::GenSuccess(PrimitiveType::INT); }
				else if (leftType == PrimitiveType::INT && rightType == PrimitiveType::FLOAT) { return TypeResult::GenSuccess(PrimitiveType::FLOAT); }
				else if (leftType == PrimitiveType::FLOAT && rightType == PrimitiveType::INT) { return TypeResult::GenSuccess(PrimitiveType::FLOAT); }
				else if (leftType == PrimitiveType::FLOAT && rightType == PrimitiveType::FLOAT) { return TypeResult::GenSuccess(PrimitiveType::FLOAT); }
				else { return TypeResult::GenMismatch("Cannot perform binary xor on " + PrimTypeToString(leftType) + " and " + PrimTypeToString(rightType), GetPosition()); }
			}
			case ramc::BinopType::LSHIFT: {
				if (leftType == PrimitiveType::INT && rightType == PrimitiveType::INT) { return TypeResult::GenSuccess(PrimitiveType::INT); }
				else if (leftType == PrimitiveType::INT && rightType == PrimitiveType::FLOAT) { return TypeResult::GenSuccess(PrimitiveType::FLOAT); }
				else if (leftType == PrimitiveType::FLOAT && rightType == PrimitiveType::INT) { return TypeResult::GenSuccess(PrimitiveType::FLOAT); }
				else if (leftType == PrimitiveType::FLOAT && rightType == PrimitiveType::FLOAT) { return TypeResult::GenSuccess(PrimitiveType::FLOAT); }
				else { return TypeResult::GenMismatch("Cannot perform left shift on " + PrimTypeToString(leftType) + " and " + PrimTypeToString(rightType), GetPosition()); }
			}
			case ramc::BinopType::RSHIFT: {
				if (leftType == PrimitiveType::INT && rightType == PrimitiveType::INT) { return TypeResult::GenSuccess(PrimitiveType::INT); }
				else if (leftType == PrimitiveType::INT && rightType == PrimitiveType::FLOAT) { return TypeResult::GenSuccess(PrimitiveType::FLOAT); }
				else if (leftType == PrimitiveType::FLOAT && rightType == PrimitiveType::INT) { return TypeResult::GenSuccess(PrimitiveType::FLOAT); }
				else if (leftType == PrimitiveType::FLOAT && rightType == PrimitiveType::FLOAT) { return TypeResult::GenSuccess(PrimitiveType::FLOAT); }
				else { return TypeResult::GenMismatch("Cannot perform right shift on " + PrimTypeToString(leftType) + " and " + PrimTypeToString(rightType), GetPosition()); }
			}
			case ramc::BinopType::LT: {
				if (leftType == PrimitiveType::INT && rightType == PrimitiveType::INT) { return TypeResult::GenSuccess(PrimitiveType::BOOL); }
				else if (leftType == PrimitiveType::INT && rightType == PrimitiveType::FLOAT) { return TypeResult::GenSuccess(PrimitiveType::BOOL); }
				else if (leftType == PrimitiveType::FLOAT && rightType == PrimitiveType::INT) { return TypeResult::GenSuccess(PrimitiveType::BOOL); }
				else if (leftType == PrimitiveType::FLOAT && rightType == PrimitiveType::FLOAT) { return TypeResult::GenSuccess(PrimitiveType::BOOL); }
				else { return TypeResult::GenMismatch("Cannot compare " + PrimTypeToString(leftType) + " and " + PrimTypeToString(rightType), GetPosition()); }
			}
			case ramc::BinopType::GT: {
				if (leftType == PrimitiveType::INT && rightType == PrimitiveType::INT) { return TypeResult::GenSuccess(PrimitiveType::BOOL); }
				else if (leftType == PrimitiveType::INT && rightType == PrimitiveType::FLOAT) { return TypeResult::GenSuccess(PrimitiveType::BOOL); }
				else if (leftType == PrimitiveType::FLOAT && rightType == PrimitiveType::INT) { return TypeResult::GenSuccess(PrimitiveType::BOOL); }
				else if (leftType == PrimitiveType::FLOAT && rightType == PrimitiveType::FLOAT) { return TypeResult::GenSuccess(PrimitiveType::BOOL); }
				else { return TypeResult::GenMismatch("Cannot compare " + PrimTypeToString(leftType) + " and " + PrimTypeToString(rightType), GetPosition()); }
			}
			case ramc::BinopType::LT_EQ: {
				if (leftType == PrimitiveType::INT && rightType == PrimitiveType::INT) { return TypeResult::GenSuccess(PrimitiveType::BOOL); }
				else if (leftType == PrimitiveType::INT && rightType == PrimitiveType::FLOAT) { return TypeResult::GenSuccess(PrimitiveType::BOOL); }
				else if (leftType == PrimitiveType::FLOAT && rightType == PrimitiveType::INT) { return TypeResult::GenSuccess(PrimitiveType::BOOL); }
				else if (leftType == PrimitiveType::FLOAT && rightType == PrimitiveType::FLOAT) { return TypeResult::GenSuccess(PrimitiveType::BOOL); }
				else { return TypeResult::GenMismatch("Cannot compare " + PrimTypeToString(leftType) + " and " + PrimTypeToString(rightType), GetPosition()); }
			}
			case ramc::BinopType::GT_EQ: {
				if (leftType == PrimitiveType::INT && rightType == PrimitiveType::INT) { return TypeResult::GenSuccess(PrimitiveType::BOOL); }
				else if (leftType == PrimitiveType::INT && rightType == PrimitiveType::FLOAT) { return TypeResult::GenSuccess(PrimitiveType::BOOL); }
				else if (leftType == PrimitiveType::FLOAT && rightType == PrimitiveType::INT) { return TypeResult::GenSuccess(PrimitiveType::BOOL); }
				else if (leftType == PrimitiveType::FLOAT && rightType == PrimitiveType::FLOAT) { return TypeResult::GenSuccess(PrimitiveType::BOOL); }
				else { return TypeResult::GenMismatch("Cannot compare " + PrimTypeToString(leftType) + " and " + PrimTypeToString(rightType), GetPosition()); }
			}
			case ramc::BinopType::EQ_EQ: return TypeResult::GenSuccess(PrimitiveType::BOOL);
			case ramc::BinopType::NEQ: return TypeResult::GenSuccess(PrimitiveType::BOOL);
			case ramc::BinopType::LOG_AND: {
				if (leftType == PrimitiveType::BOOL && rightType == PrimitiveType::BOOL) { return TypeResult::GenSuccess(PrimitiveType::BOOL); }
				else { return TypeResult::GenMismatch("Cannot perform logical and on " + PrimTypeToString(leftType) + " and " + PrimTypeToString(rightType), GetPosition()); }
			}
			case ramc::BinopType::LOG_OR: {
				if (leftType == PrimitiveType::BOOL && rightType == PrimitiveType::BOOL) { return TypeResult::GenSuccess(PrimitiveType::BOOL); }
				else { return TypeResult::GenMismatch("Cannot perform logical and on " + PrimTypeToString(leftType) + " and " + PrimTypeToString(rightType), GetPosition()); }
			}
			case ramc::BinopType::EQ: return TypeResult::GenMismatch("ATSUnopExpr::TypeCheck - Operation not handled!", GetPosition());
			case ramc::BinopType::ADD_EQ: return TypeResult::GenMismatch("ATSUnopExpr::TypeCheck - Operation not handled!", GetPosition());
			case ramc::BinopType::SUB_EQ: return TypeResult::GenMismatch("ATSUnopExpr::TypeCheck - Operation not handled!", GetPosition());
			case ramc::BinopType::MUL_EQ: return TypeResult::GenMismatch("ATSUnopExpr::TypeCheck - Operation not handled!", GetPosition());
			case ramc::BinopType::DIV_EQ: return TypeResult::GenMismatch("ATSUnopExpr::TypeCheck - Operation not handled!", GetPosition());
			case ramc::BinopType::MOD_EQ: return TypeResult::GenMismatch("ATSUnopExpr::TypeCheck - Operation not handled!", GetPosition());
			case ramc::BinopType::POW_EQ: return TypeResult::GenMismatch("ATSUnopExpr::TypeCheck - Operation not handled!", GetPosition());
			case ramc::BinopType::BIN_AND_EQ: return TypeResult::GenMismatch("ATSUnopExpr::TypeCheck - Operation not handled!", GetPosition());
			case ramc::BinopType::BIN_OR_EQ: return TypeResult::GenMismatch("ATSUnopExpr::TypeCheck - Operation not handled!", GetPosition());
			case ramc::BinopType::BIN_XOR_EQ: return TypeResult::GenMismatch("ATSUnopExpr::TypeCheck - Operation not handled!", GetPosition());
			case ramc::BinopType::LSHIFT_EQ: return TypeResult::GenMismatch("ATSUnopExpr::TypeCheck - Operation not handled!", GetPosition());
			case ramc::BinopType::RSHIFT_EQ: return TypeResult::GenMismatch("ATSUnopExpr::TypeCheck - Operation not handled!", GetPosition());
			default: return TypeResult::GenMismatch("ATSUnopExpr::TypeCheck - Operation not handled!", GetPosition());
		}
	}

	InstructionSet ASTBinopExpr::GenerateCode(std::map<std::string, std::string> _params)
	{
		ramvm::InstructionSet instrs;

		auto leftInstrs = left->GenerateCode({ {"Dest", "[1]"} });
		auto rightInstrs = right->GenerateCode({ {"Dest", "[1]" } });

		instrs.insert(instrs.end(), leftInstrs.begin(), leftInstrs.end());
		instrs.insert(instrs.end(), rightInstrs.begin(), rightInstrs.end());

		auto src1 =  TypedArgument(DataType::INT, Argument(ramvm::ArgType::SP_OFFSET, -1));
		auto src2 = TypedArgument(DataType::INT, Argument(ramvm::ArgType::SP_OFFSET, 0));

		switch (op)
		{
			case ramc::BinopType::ADD: instrs.push_back(new ramvm::InstrBinop(ramvm::Binop::ADD, src1, src2, src1)); break;
			case ramc::BinopType::SUB: instrs.push_back(new ramvm::InstrBinop(ramvm::Binop::SUB, src1, src2, src1)); break;
			case ramc::BinopType::MUL: instrs.push_back(new ramvm::InstrBinop(ramvm::Binop::MUL, src1, src2, src1)); break;
			case ramc::BinopType::DIV: instrs.push_back(new ramvm::InstrBinop(ramvm::Binop::DIV, src1, src2, src1)); break;
			case ramc::BinopType::MOD: instrs.push_back(new ramvm::InstrBinop(ramvm::Binop::MOD, src1, src2, src1)); break;
			case ramc::BinopType::POW: throw "BinopExpr::GenerateCode - BinopType not handled!";
			case ramc::BinopType::BIN_AND: instrs.push_back(new ramvm::InstrBinop(ramvm::Binop::BIT_AND, src1, src2, src1)); break;
			case ramc::BinopType::BIN_OR: instrs.push_back(new ramvm::InstrBinop(ramvm::Binop::BIT_OR, src1, src2, src1)); break;
			case ramc::BinopType::BIN_XOR: instrs.push_back(new ramvm::InstrBinop(ramvm::Binop::BIT_XOR, src1, src2, src1)); break;
			case ramc::BinopType::LSHIFT: instrs.push_back(new ramvm::InstrBinop(ramvm::Binop::LSHIFT, src1, src2, src1)); break;
			case ramc::BinopType::RSHIFT: instrs.push_back(new ramvm::InstrBinop(ramvm::Binop::RSHIFT, src1, src2, src1)); break;
			case ramc::BinopType::LT: instrs.push_back(new ramvm::InstrBinop(ramvm::Binop::LT, src1, src2, src1)); break;
			case ramc::BinopType::GT: instrs.push_back(new ramvm::InstrBinop(ramvm::Binop::GT, src1, src2, src1)); break;
			case ramc::BinopType::LT_EQ: instrs.push_back(new ramvm::InstrBinop(ramvm::Binop::LTEQ, src1, src2, src1)); break;
			case ramc::BinopType::GT_EQ: instrs.push_back(new ramvm::InstrBinop(ramvm::Binop::GTEQ, src1, src2, src1)); break;
			case ramc::BinopType::EQ_EQ: instrs.push_back(new ramvm::InstrBinop(ramvm::Binop::EQ, src1, src2, src1)); break;
			case ramc::BinopType::NEQ: instrs.push_back(new ramvm::InstrBinop(ramvm::Binop::NEQ, src1, src2, src1)); break;
			case ramc::BinopType::LOG_AND: instrs.push_back(new ramvm::InstrBinop(ramvm::Binop::LOG_AND, src1, src2, src1)); break;
			case ramc::BinopType::LOG_OR: instrs.push_back(new ramvm::InstrBinop(ramvm::Binop::LOG_OR, src1, src2, src1)); break;
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

		//TODO gotta change the pop scale to be based on the return type of this binop
		//This will require you to store the type during typechecking
		instrs.push_back(new ramvm::InstrPop(ramvm::Argument(ramvm::ArgType::VALUE, 1), INT_SIZE));

		auto dest = ramvm::Argument();
		IGNORE(ramvm::ParseArgument(_params["Dest"], dest));

		if (!dest.IsStackTop())
		{
			instrs.push_back(new ramvm::InstrMove(TypedArgument(DataType::INT, ramvm::Argument(ramvm::ArgType::SP_OFFSET, 0)), dest));
			instrs.push_back(new ramvm::InstrPop(ramvm::Argument(ramvm::ArgType::VALUE, 1), INT_SIZE));
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

	TypeResult ASTUnopExpr::TypeCheck(Environment& _env)
	{
		TypeResult typeRes = right->TypeCheck(_env);

		if (!typeRes.IsSuccess())
			return typeRes;

		PrimitiveType primType = typeRes.GetType();

		switch (op)
		{
			case UnopType::NEG: {
				if (primType == PrimitiveType::INT || primType == PrimitiveType::FLOAT) { return typeRes; }
				else { return TypeResult::GenMismatch("Expected an int or float on the right of arithmetic negation but found " + PrimTypeToString(primType), GetPosition()); }
			}
			case UnopType::LOG_NOT: {
				if (primType == PrimitiveType::BOOL) { return typeRes; }
				else { return TypeResult::GenMismatch("Expected an bool on the right of logical negation but found " + PrimTypeToString(primType), GetPosition()); }
			}
			case UnopType::PRE_INC: {
				if (primType == PrimitiveType::INT || primType == PrimitiveType::FLOAT) { return TypeResult::GenSuccess(PrimitiveType::VOID); }
				else { return TypeResult::GenMismatch("Expected an int or float on the right of increment but found " + PrimTypeToString(primType), GetPosition()); }
			}
			case UnopType::POST_INC: {
				if (primType == PrimitiveType::INT || primType == PrimitiveType::FLOAT) { return TypeResult::GenSuccess(PrimitiveType::VOID); }
				else { return TypeResult::GenMismatch("Expected an int or float on the left of increment but found " + PrimTypeToString(primType), GetPosition()); }
			}
			case UnopType::PRE_DEC: {
				if (primType == PrimitiveType::INT || primType == PrimitiveType::FLOAT) { return TypeResult::GenSuccess(PrimitiveType::VOID); }
				else { return TypeResult::GenMismatch("Expected an int or float on the right of decrement but found " + PrimTypeToString(primType), GetPosition()); }
			}
			case UnopType::POST_DEC: {
				if (primType == PrimitiveType::INT || primType == PrimitiveType::FLOAT) { return TypeResult::GenSuccess(PrimitiveType::VOID); }
				else { return TypeResult::GenMismatch("Expected an int or float on the left of decrement but found " + PrimTypeToString(primType), GetPosition()); }
			}
			default: return TypeResult::GenMismatch("ATSUnopExpr::TypeCheck - Operation not handled!", GetPosition());
		}
	}

	ramvm::InstructionSet ASTUnopExpr::GenerateCode(std::map<std::string, std::string> _params)
	{
		ramvm::InstructionSet instrs = right->GenerateCode({ {"Dest", "[1]" } });
		auto src = TypedArgument(DataType::INT, Argument(ramvm::ArgType::SP_OFFSET, 0));
		
		switch (op)
		{
			case ramc::UnopType::LOG_NOT: instrs.push_back(new ramvm::InstrUnop(ramvm::Unop::LOG_NOT, src, src)); break;
			case ramc::UnopType::NEG: instrs.push_back(new ramvm::InstrUnop(ramvm::Unop::NEG, src, src)); break;
			case ramc::UnopType::PRE_INC: throw "UnopExpr::GenerateCode - UnopType not handled!";
			case ramc::UnopType::PRE_DEC: throw "UnopExpr::GenerateCode - UnopType not handled!";
			case ramc::UnopType::POST_INC: throw "UnopExpr::GenerateCode - UnopType not handled!";
			case ramc::UnopType::POST_DEC: throw "UnopExpr::GenerateCode - UnopType not handled!";
			default: throw "UnopExpr::GenerateCode - UnopType not handled!";
		}

		
		auto dest = ramvm::Argument();
		IGNORE(ramvm::ParseArgument(_params["Dest"], dest));

		if (!dest.IsStackTop())
		{
			instrs.push_back(new ramvm::InstrMove(TypedArgument(DataType::INT, ramvm::Argument(ramvm::ArgType::SP_OFFSET, 0)), dest));

			//TODO gotta change the pop scale to be based on the return type of this unop
			//This will require you to store the type during typechecking
			instrs.push_back(new ramvm::InstrPop(ramvm::Argument(ramvm::ArgType::VALUE, 1), INT_SIZE));
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

	TypeResult ASTIdentifier::TypeCheck(Environment& _env)
	{
		return _env.GetVariableType(id, GetPosition());
	}

	ramvm::InstructionSet ASTIdentifier::GenerateCode(std::map<std::string, std::string> _params)
	{
		ramvm::InstructionSet instrs;

		return instrs;
	}
#pragma endregion

#pragma region Literal
	ramvm::InstructionSet ASTIntLit::GenerateCode(std::map<std::string, std::string> _params)
	{
		auto dest = ramvm::Argument();
		IGNORE(ramvm::ParseArgument(_params["Dest"], dest));

		return { new ramvm::InstrMove(TypedArgument(DataType::INT, ramvm::Argument(ramvm::ArgType::VALUE, GetValue())), dest) };
	}

	ramvm::InstructionSet ASTFloatLit::GenerateCode(std::map<std::string, std::string> _params)
	{
		auto dest = ramvm::Argument();
		IGNORE(ramvm::ParseArgument(_params["Dest"], dest));

		return { new ramvm::InstrMove(TypedArgument(DataType::FLOAT, ramvm::Argument(ramvm::ArgType::VALUE, GetValue())), dest) };
	}

	ramvm::InstructionSet ASTStringLit::GenerateCode(std::map<std::string, std::string> _params)
	{
		auto dest = ramvm::Argument();
		IGNORE(ramvm::ParseArgument(_params["Dest"], dest));

		return { new ramvm::InstrMove(TypedArgument(DataType::INT, ramvm::Argument(ramvm::ArgType::VALUE, 0)), dest) };
	}

	ramvm::InstructionSet ASTBoolLit::GenerateCode(std::map<std::string, std::string> _params)
	{
		auto dest = ramvm::Argument();
		IGNORE(ramvm::ParseArgument(_params["Dest"], dest));

		return { new ramvm::InstrMove(TypedArgument(DataType::BYTE, ramvm::Argument(ramvm::ArgType::VALUE, (byte)(GetValue() ? 1 : 0))), dest) };
	}
#pragma endregion
}