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
using ramvm::InstrPush;

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

#pragma region VarDecl
	ASTVarDecl::ASTVarDecl(ASTIdentifier* _id, ASTNode* _expr, Position _pos)
		: ASTNode(ASTNodeType::ASSIGNMENT, _pos)
	{
		id = _id;
		expr = _expr;
		isUnderscore = false;
	}

	ASTVarDecl::ASTVarDecl(ASTNode* _expr, Position _pos)
		: ASTNode(ASTNodeType::ASSIGNMENT, _pos)
	{
		id = nullptr;
		expr = _expr;
		isUnderscore = true;
	}

	ASTVarDecl::~ASTVarDecl()
	{
		delete id;
		delete expr;
	}

	std::string ASTVarDecl::ToString(int _indentLvl)
	{
		std::stringstream ss;

		ss << CreateIndent(_indentLvl) << "Var Declaration:" << std::endl;
		ss << (isUnderscore ? CreateIndent(_indentLvl + 1) + "UNDERSCORE" : id->ToString(_indentLvl + 1)) << std::endl;
		ss << expr->ToString(_indentLvl + 1);

		return ss.str();
	}

	TypeResult ASTVarDecl::TypeCheck(Environment* _env)
	{
		if(isUnderscore)
			return TypeResult::GenSuccess(Type::VOID);

		if (_env->HasVariable(id->GetID()))
			return TypeResult::GenRedecalartion(id->GetID(), GetPosition());

		TypeResult exprTypeRes = expr->GetTypeSysType(_env);

		if (!exprTypeRes.IsSuccess())
			return exprTypeRes;

		_env->AddVariable(id->GetID(), exprTypeRes.GetValue());
		IGNORE(id->TypeCheck(_env)); //Set the info for the id for code generation later
		return TypeResult::GenSuccess(Type::VOID);
	}

	InstructionSet ASTVarDecl::GenerateCode(std::map<std::string, std::string> _params) { return expr->GenerateCode({ {"Dest", "R0" } }); }
#pragma endregion

#pragma region Assignment
	const std::string AssignmentTypeToString(AssignmentType _type)
	{
		const std::map<AssignmentType, std::string> strings{
			{ AssignmentType::EQ, "Equal" },
			{ AssignmentType::ADD_EQ, "Add Equal" },
			{ AssignmentType::SUB_EQ, "Subtract Equal" },
			{ AssignmentType::MUL_EQ, "Multiply Equal" },
			{ AssignmentType::DIV_EQ, "Divide Equal" },
			{ AssignmentType::MOD_EQ, "Mod Equal" },
			{ AssignmentType::POW_EQ, "Power Equal" },
			{ AssignmentType::BIN_AND_EQ, "Binary And Equal" },
			{ AssignmentType::BIN_OR_EQ, "Binary Or Equal" },
			{ AssignmentType::BIN_XOR_EQ, "Binary Xor Equal" },
			{ AssignmentType::LSHIFT_EQ, "Left Shift Equal" },
			{ AssignmentType::RSHIFT_EQ, "Right Shift Equal" }
		};

		auto search = strings.find(_type);
		return search == strings.end() ? "AssignmentTypeToString - AssignmentType not handled!" : search->second;
	}

	ASTAssignment::ASTAssignment(ASTIdentifier* _id, ASTNode* _expr, AssignmentType _assignType)
		: ASTNode(ASTNodeType::ASSIGNMENT, _id->GetPosition())
	{
		id = _id;
		expr = _expr;
		assignType = _assignType;

		switch (assignType)
		{
			case AssignmentType::EQ: expr = _expr; break;
			case AssignmentType::ADD_EQ: expr = new ASTBinopExpr(id, expr, BinopType::ADD); break;
			case AssignmentType::SUB_EQ: expr = new ASTBinopExpr(id, expr, BinopType::SUB); break;
			case AssignmentType::MUL_EQ: expr = new ASTBinopExpr(id, expr, BinopType::MUL); break;
			case AssignmentType::DIV_EQ: expr = new ASTBinopExpr(id, expr, BinopType::DIV); break;
			case AssignmentType::MOD_EQ: expr = new ASTBinopExpr(id, expr, BinopType::MOD); break;
			case AssignmentType::POW_EQ: expr = new ASTBinopExpr(id, expr, BinopType::POW); break;
			case AssignmentType::BIN_AND_EQ: expr = new ASTBinopExpr(id, expr, BinopType::BIN_AND); break;
			case AssignmentType::BIN_OR_EQ: expr = new ASTBinopExpr(id, expr, BinopType::BIN_OR); break;
			case AssignmentType::BIN_XOR_EQ: expr = new ASTBinopExpr(id, expr, BinopType::BIN_XOR); break;
			case AssignmentType::LSHIFT_EQ: expr = new ASTBinopExpr(id, expr, BinopType::LSHIFT); break;
			case AssignmentType::RSHIFT_EQ: expr = new ASTBinopExpr(id, expr, BinopType::RSHIFT); break;
			default: throw "ASTAssignment() - AssignmentType not handled!";
		}
	}

	ASTAssignment::~ASTAssignment()
	{
		delete id;
		delete expr;
	}

	std::string ASTAssignment::ToString(int _indentLvl)
	{
		std::stringstream ss;

		ss << CreateIndent(_indentLvl) << "Assignment:" << std::endl;
		ss << CreateIndent(_indentLvl + 1) << "Type: " << AssignmentTypeToString(assignType) << std::endl;
		ss << id->ToString(_indentLvl + 1) << std::endl;
		ss << expr->ToString(_indentLvl + 1);

		return ss.str();
	}

	TypeResult ASTAssignment::TypeCheck(Environment* _env)
	{
		TypeResult exprTypeRes = expr->GetTypeSysType(_env);

		if (!exprTypeRes.IsSuccess())
			return exprTypeRes;

		TypeResult idTypeRes = id->GetTypeSysType(_env);

		if (!idTypeRes.IsSuccess())
			return idTypeRes;

		if (idTypeRes.GetValue() == exprTypeRes.GetValue()) { return TypeResult::GenSuccess(Type::VOID); }
		else { return TypeResult::GenMismatch("Cannot assign " + exprTypeRes.GetValue()->ToString(false) + " to " + idTypeRes.GetValue()->ToString(false), GetPosition()); }
	}

	InstructionSet ASTAssignment::GenerateCode(std::map<std::string, std::string> _params) { return expr->GenerateCode({ {"Dest", id->GetSource().ToString() } }); }
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
			{ BinopType::LOG_OR, "Logical Or" }
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
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE);

			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE);

			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::LONG, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::DOUBLE);
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
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE);

			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE);

			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::LONG, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::DOUBLE);
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
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE);

			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE);

			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::LONG, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::DOUBLE);
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
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE);

			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE);

			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::LONG, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::DOUBLE);
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
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE);

			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE);

			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::LONG, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::DOUBLE);
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
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE);

			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE);

			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::LONG, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::LONG, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::LONG, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);
#pragma endregion

#pragma region BIN_AND
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);

			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);

			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::INT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::INT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::INT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::INT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::INT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::INT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);

			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE);

			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE);

			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::LONG, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::LONG, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::LONG, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);
#pragma endregion

#pragma region BIN_OR
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);

			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);

			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::INT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::INT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::INT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::INT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::INT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::INT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);

			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE);

			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE);

			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::LONG, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::LONG, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::LONG, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);
#pragma endregion

#pragma region BIN_XOR
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);

			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);

			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::INT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::INT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::INT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::INT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::INT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::INT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);

			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE);

			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE);

			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::LONG, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::LONG, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::LONG, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);
#pragma endregion

#pragma region LSHIFT
			case ConcatTriple((byte)BinopType::LSHIFT, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatTriple((byte)BinopType::LSHIFT, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatTriple((byte)BinopType::LSHIFT, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatTriple((byte)BinopType::LSHIFT, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BYTE);

			case ConcatTriple((byte)BinopType::LSHIFT, (byte)TypeSystemType::INT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::LSHIFT, (byte)TypeSystemType::INT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::LSHIFT, (byte)TypeSystemType::INT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::LSHIFT, (byte)TypeSystemType::INT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::INT);

			case ConcatTriple((byte)BinopType::LSHIFT, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::LSHIFT, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::LSHIFT, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::LSHIFT, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE);

			case ConcatTriple((byte)BinopType::LSHIFT, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::LSHIFT, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::LSHIFT, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::LSHIFT, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE);

			case ConcatTriple((byte)BinopType::LSHIFT, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::LSHIFT, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::LSHIFT, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::LSHIFT, (byte)TypeSystemType::LONG, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);
#pragma endregion

#pragma region RSHIFT
			case ConcatTriple((byte)BinopType::RSHIFT, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatTriple((byte)BinopType::RSHIFT, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatTriple((byte)BinopType::RSHIFT, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatTriple((byte)BinopType::RSHIFT, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BYTE);

			case ConcatTriple((byte)BinopType::RSHIFT, (byte)TypeSystemType::INT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::RSHIFT, (byte)TypeSystemType::INT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::RSHIFT, (byte)TypeSystemType::INT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT);
			case ConcatTriple((byte)BinopType::RSHIFT, (byte)TypeSystemType::INT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::INT);

			case ConcatTriple((byte)BinopType::RSHIFT, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::RSHIFT, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::RSHIFT, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatTriple((byte)BinopType::RSHIFT, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE);

			case ConcatTriple((byte)BinopType::RSHIFT, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::RSHIFT, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::RSHIFT, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatTriple((byte)BinopType::RSHIFT, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE);

			case ConcatTriple((byte)BinopType::RSHIFT, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::RSHIFT, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::RSHIFT, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::LONG);
			case ConcatTriple((byte)BinopType::RSHIFT, (byte)TypeSystemType::LONG, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);
#pragma endregion

#pragma region LT
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);

			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);

			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::INT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::INT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::INT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::INT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::INT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::INT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);

			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);

			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);

			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::LONG, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::LONG, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::LONG, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);
#pragma endregion

#pragma region GT
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);

			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);

			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::INT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::INT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::INT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::INT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::INT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::INT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);

			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);

			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);

			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::LONG, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::LONG, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::LONG, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);
#pragma endregion

#pragma region LT_EQ
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);

			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);

			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);

			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);

			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);

			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);
#pragma endregion

#pragma region GT_EQ
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);

			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);

			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);

			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);

			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);

			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);
#pragma endregion

#pragma region EQ_EQ
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);

			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);

			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);

			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);

			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);

			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);
#pragma endregion

#pragma region NEQ
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);

			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);

			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);

			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);

			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);

			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);
#pragma endregion

#pragma region LOG_AND
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);

			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);

			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::INT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::INT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::INT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::INT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::INT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::INT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);

			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);

			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);

			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::LONG, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::LONG, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::LONG, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);
#pragma endregion

#pragma region LOG_OR
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);

			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);

			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::INT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::INT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::INT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::INT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::INT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::INT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);

			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);

			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);

			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::LONG, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::LONG, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::LONG, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);
#pragma endregion
			default: return TypeResult::GenMismatch("Cannot perform operation on " + leftType->ToString(false) + " and " + rightType->ToString(false), GetPosition());
		}
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

		//Extend the stack to make room for the result if it takes up more space than the operands
		int sizeDiff = GetDataTypeSize(destDataType) - GetDataTypeSize(src1DataType) - GetDataTypeSize(src2DataType);
		if (sizeDiff > 0)
			instrs.push_back(new InstrPush({ TypedArgument(DataType::BYTE, ArgType::VALUE, sizeDiff) }));

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
			default: throw "BinopExpr::GenerateCode - BinopType not handled!";
		}

		//Create Pop to remove the bytes past the result
		if (sizeDiff < 0)
			instrs.push_back(new InstrPop(DataType::BYTE, Argument(ArgType::VALUE, -sizeDiff)));

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
			{ UnopType::BIN_NOT, "BINARY NOT" },
		};

		auto search = strings.find(_type);
		return search == strings.end() ? "UnopTypeToString - UnopType not handled!" : search->second;
	}

	ASTUnopExpr::ASTUnopExpr(ASTNode* _expr, UnopType _op)
		: ASTNode(ASTNodeType::UNOP_EXPR, _expr->GetPosition())
	{
		expr = _expr;
		op = _op;
	}

	ASTUnopExpr::~ASTUnopExpr() { delete expr; }

	std::string ASTUnopExpr::ToString(int _indentLvl)
	{
		std::stringstream ss;

		ss << CreateIndent(_indentLvl) << "Unop Expression:" << std::endl;
		ss << CreateIndent(_indentLvl + 1) << "OP: " << UnopTypeToString(op) << std::endl;
		ss << expr->ToString(_indentLvl + 1);

		return ss.str();
	}

	TypeResult ASTUnopExpr::TypeCheck(Environment* _env)
	{
		TypeResult typeRes = expr->GetTypeSysType(_env);

		if (!typeRes.IsSuccess())
			return typeRes;

		Type* exprType = typeRes.GetValue();

		switch (ConcatDouble((byte)op, (byte)exprType->GetType()))
		{
			case ConcatDouble((byte)UnopType::NEG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatDouble((byte)UnopType::NEG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatDouble((byte)UnopType::NEG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT);
			case ConcatDouble((byte)UnopType::NEG, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatDouble((byte)UnopType::NEG, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatDouble((byte)UnopType::NEG, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);

			case ConcatDouble((byte)UnopType::LOG_NOT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatDouble((byte)UnopType::LOG_NOT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatDouble((byte)UnopType::LOG_NOT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatDouble((byte)UnopType::LOG_NOT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatDouble((byte)UnopType::LOG_NOT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL);
			case ConcatDouble((byte)UnopType::LOG_NOT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL);

			case ConcatDouble((byte)UnopType::BIN_NOT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatDouble((byte)UnopType::BIN_NOT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE);
			case ConcatDouble((byte)UnopType::BIN_NOT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT);
			case ConcatDouble((byte)UnopType::BIN_NOT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT);
			case ConcatDouble((byte)UnopType::BIN_NOT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE);
			case ConcatDouble((byte)UnopType::BIN_NOT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG);

			default: return TypeResult::GenMismatch("Cannot perform operation on " + exprType->ToString(false), GetPosition());
		}
	}

	InstructionSet ASTUnopExpr::GenerateCode(std::map<std::string, std::string> _params)
	{
		InstructionSet instrs = expr->GenerateCode({ {"Dest", "[1]" } });

		auto srcDataType = TypeSysTypeToDataType(expr->GetTypeSysType(nullptr).GetValue()->GetType());
		auto destDataType = TypeSysTypeToDataType(this->GetTypeSysType(nullptr).GetValue()->GetType());

		//Extend the stack to make room for the result if it takes up more space than the operand
		int sizeDiff = GetDataTypeSize(destDataType) - GetDataTypeSize(srcDataType);
		if (sizeDiff > 0)
			instrs.push_back(new InstrPush({ TypedArgument(DataType::BYTE, ArgType::VALUE, sizeDiff) }));

		auto src = TypedArgument(srcDataType, ArgType::SP_OFFSET, -1 * GetDataTypeSize(srcDataType) + 1);
		auto stackDest = TypedArgument(destDataType, ArgType::SP_OFFSET, src.value.i - fmax(0, sizeDiff));

		switch (op)
		{
			case UnopType::LOG_NOT: instrs.push_back(new InstrUnop(Unop::LOG_NOT, src, stackDest)); break;
			case UnopType::NEG: instrs.push_back(new InstrUnop(Unop::NEG, src, stackDest)); break;
			case UnopType::BIN_NOT: instrs.push_back(new InstrUnop(Unop::BIN_NOT, src, stackDest)); break;
			default: throw "UnopExpr::GenerateCode - UnopType not handled!";
		}

		//Create Pop to remove the bytes past the result if any
		if (sizeDiff < 0)
			instrs.push_back(new InstrPop(DataType::BYTE, Argument(ArgType::VALUE, -sizeDiff)));

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
		source = _env->GetVarRegister(id);
		return _env->GetVariableType(id, GetPosition());
	}

	InstructionSet ASTIdentifier::GenerateCode(std::map<std::string, std::string> _params)
	{
		DataType dataType = TypeSysTypeToDataType(this->GetTypeSysType(nullptr).GetValue()->GetType());
		auto dest = Argument();
		IGNORE(ParseArgument(_params["Dest"], dest));

		return { new InstrMove(dataType, GetSource(), dest) };
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