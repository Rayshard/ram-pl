#include "pch.h"
#include "ramc_ast.h"
#include "ramvm_parser.h"
#include "ramvm_instruction.h"

#pragma region RamVM Usings
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
using ramvm::InstrCJump;
using ramvm::InstrJump;
using ramvm::InstructionType;
#pragma endregion

namespace ramc {
	ASTNode::ASTNode(ASTNodeType _type, Position _pos)
	{
		type = _type;
		position = _pos;
		typeSysType = nullptr;
	}

	TypeResult ASTNode::TypeCheck(Environment* _env)
	{
		TypeResult result = _TypeCheck(_env);
		typeSysType = result.GetValue();
		return result;
	}

#pragma region Program
	ASTProgram::ASTProgram(std::string _fileName, std::vector<ASTStmt*> _stmts)
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

	std::string ASTProgram::ToString(int _indentLvl, std::string _prefix)
	{
		std::stringstream ss;

		ss << CreateIndent(_indentLvl) << _prefix << "Start:" << std::endl;
		ss << CreateIndent(_indentLvl + 1) << "Filename: " << fileName << std::endl;
		ss << CreateIndent(_indentLvl + 1) << "Statements: " << std::endl;

		for (auto it = stmts.begin(); it != stmts.end(); it++)
			ss << (*it)->ToString(_indentLvl + 2) << std::endl;

		return ss.str();
	}

	TypeResult ASTProgram::_TypeCheck(Environment* _env)
	{
		for (auto const& stmt : stmts)
		{
			TypeResult stmtTypeRes = stmt->TypeCheck(_env);
			if (!stmtTypeRes.IsSuccess())
				return stmtTypeRes;
		}

		return TypeResult::GenSuccess(Type::VOID);
	}

	InstructionSet ASTProgram::GenerateCode()
	{
		ProgramInfo info;
		InstructionSet instrs;

		for (int i = 0; i < (int)stmts.size(); i++)
		{
			auto stmtInstrs = stmts[i]->GenerateCode(info);
			instrs.insert(instrs.end(), stmtInstrs.begin(), stmtInstrs.end());
		}

		for (int i = 0; i < (int)info.offsetedCtrlInstrs.size(); i++)
		{
			auto instr = info.offsetedCtrlInstrs[i];
			int idx = std::find(instrs.begin(), instrs.end(), instr) - instrs.begin();

			if (instr->GetType() == InstructionType::JUMP) { ((InstrJump*)instr)->instrIdx += idx; }
			else if (instr->GetType() == InstructionType::CJUMP) { ((InstrCJump*)instr)->instrIdx += idx; }
		}

		return instrs;
	}
#pragma endregion

#pragma region VarDecl
	ASTVarDecl::ASTVarDecl(ASTIdentifier* _id, ASTExpr* _expr, Position _pos)
		: ASTStmt(ASTStmtType::VARDECL, _pos)
	{
		id = _id;
		expr = _expr;
		isUnderscore = false;
		restraint = nullptr;
	}

	ASTVarDecl::ASTVarDecl(ASTIdentifier* _id, TypePtr _restraint, ASTExpr* _expr, Position _pos)
		: ASTStmt(ASTStmtType::VARDECL, _pos)
	{
		id = _id;
		expr = _expr;
		isUnderscore = false;
		restraint = _restraint;
	}

	ASTVarDecl::ASTVarDecl(ASTExpr* _expr, Position _pos)
		: ASTStmt(ASTStmtType::VARDECL, _pos)
	{
		id = nullptr;
		expr = _expr;
		isUnderscore = true;
		restraint = nullptr;
	}

	ASTVarDecl::~ASTVarDecl()
	{
		delete id;
		delete expr;
	}

	std::string ASTVarDecl::ToString(int _indentLvl, std::string _prefix)
	{
		std::stringstream ss;

		ss << CreateIndent(_indentLvl) << _prefix << "Var Declaration:" << std::endl;
		ss << (isUnderscore ? CreateIndent(_indentLvl + 1) + "UNDERSCORE" : id->ToString(_indentLvl + 1)) << std::endl;
		ss << CreateIndent(_indentLvl + 1) << "Restraint: " << (restraint ? restraint->ToString(0) : "variable") << std::endl;
		ss << expr->ToString(_indentLvl + 1);

		return ss.str();
	}

	TypeResult ASTVarDecl::_TypeCheck(Environment* _env)
	{
		TypeResult exprTypeRes = expr->TypeCheck(_env);

		if (isUnderscore) { return TypeResult::GenSuccess(Type::VOID); }
		else if (_env->HasVariable(id->GetID(), true)) { return TypeResult::GenRedecalartion(id->GetID(), GetPosition()); }

		if (!exprTypeRes.IsSuccess()) { return exprTypeRes; }
		else if (restraint && !Type::Matches(exprTypeRes.GetValue(), restraint)) { return TypeResult::GenExpectation(restraint->ToString(0), exprTypeRes.GetValue()->ToString(0), GetPosition()); }
		else
		{
			_env->AddVariable(id->GetID(), exprTypeRes.GetValue());
			IGNORE(id->TypeCheck(_env)); //Set the info for the id for code generation later
			return TypeResult::GenSuccess(Type::VOID);
		}
	}

	InstructionSet ASTVarDecl::GenerateCode(ProgramInfo& _progInfo)
	{
		if (isUnderscore)
		{
			InstructionSet instrs = expr->GenerateCode(Argument::CreateStackTop(), _progInfo);
			instrs.push_back(new InstrPop(DataType::BYTE, Argument(ArgType::VALUE, expr->GetTypeSysType()->GetByteSize())));
			return instrs;
		}
		else { return expr->GenerateCode(id->GetSource(), _progInfo); }
	}
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

	ASTAssignment::ASTAssignment(ASTIdentifier* _id, ASTExpr* _expr, AssignmentType _assignType)
		: ASTStmt(ASTStmtType::ASSIGNMENT, _id->GetPosition())
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
			default: throw std::runtime_error("ASTAssignment() - AssignmentType not handled!");
		}
	}

	ASTAssignment::~ASTAssignment()
	{
		delete id;
		delete expr;
	}

	std::string ASTAssignment::ToString(int _indentLvl, std::string _prefix)
	{
		std::stringstream ss;

		ss << CreateIndent(_indentLvl) << _prefix << "Assignment:" << std::endl;
		ss << CreateIndent(_indentLvl + 1) << "Type: " << AssignmentTypeToString(assignType) << std::endl;
		ss << id->ToString(_indentLvl + 1) << std::endl;
		ss << expr->ToString(_indentLvl + 1);

		return ss.str();
	}

	TypeResult ASTAssignment::_TypeCheck(Environment* _env)
	{
		TypeResult exprTypeRes = expr->TypeCheck(_env);

		if (!exprTypeRes.IsSuccess())
			return exprTypeRes;

		TypeResult idTypeRes = id->TypeCheck(_env);

		if (!idTypeRes.IsSuccess())
			return idTypeRes;

		if (Type::Matches(idTypeRes.GetValue(), exprTypeRes.GetValue())) { return TypeResult::GenSuccess(Type::VOID); }
		else { return TypeResult::GenMismatch("Cannot assign " + exprTypeRes.GetValue()->ToString(false) + " to " + idTypeRes.GetValue()->ToString(false), GetPosition()); }
	}

	InstructionSet ASTAssignment::GenerateCode(ProgramInfo& _progInfo) { return expr->GenerateCode(id->GetSource(), _progInfo); }
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

	ASTBinopExpr::ASTBinopExpr(ASTExpr* _left, ASTExpr* _right, BinopType _op)
		: ASTExpr(ASTExprType::BINOP, _right->GetPosition())
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

	std::string ASTBinopExpr::ToString(int _indentLvl, std::string _prefix)
	{
		std::stringstream ss;

		ss << CreateIndent(_indentLvl) << _prefix << "Binop Expression:" << std::endl;
		ss << CreateIndent(_indentLvl + 1) << "OP: " << BinopTypeToString(op) << std::endl;
		ss << left->ToString(_indentLvl + 1, "Left: ") << std::endl;
		ss << right->ToString(_indentLvl + 1, "Right: ");

		return ss.str();
	}

	TypeResult ASTBinopExpr::_TypeCheck(Environment* _env)
	{
		TypeResult leftTypeRes = left->TypeCheck(_env);

		if (!leftTypeRes.IsSuccess())
			return leftTypeRes;

		TypeResult rightTypeRes = right->TypeCheck(_env);

		if (!rightTypeRes.IsSuccess())
			return rightTypeRes;

		TypePtr leftType = leftTypeRes.GetValue(), rightType = rightTypeRes.GetValue();

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

	InstructionSet ASTBinopExpr::GenerateCode(Argument _dest, ProgramInfo& _progInfo)
	{
		InstructionSet instrs;

		auto leftInstrs = left->GenerateCode(Argument::CreateStackTop(), _progInfo);
		auto rightInstrs = right->GenerateCode(Argument::CreateStackTop(), _progInfo);

		instrs.insert(instrs.end(), leftInstrs.begin(), leftInstrs.end());
		instrs.insert(instrs.end(), rightInstrs.begin(), rightInstrs.end());

		auto src1DataType = TypeSysTypeToDataType(left->GetTypeSysType()->GetType());
		auto src2DataType = TypeSysTypeToDataType(right->GetTypeSysType()->GetType());
		auto destDataType = TypeSysTypeToDataType(this->GetTypeSysType()->GetType());

		int src1ByteSize = left->GetTypeSysType()->GetByteSize();
		int src2ByteSize = right->GetTypeSysType()->GetByteSize();
		int destByteSize = this->GetTypeSysType()->GetByteSize();
		int sizeDiff = destByteSize - src1ByteSize - src2ByteSize;

		//Extend the stack to make room for the result if it takes up more space than the operands
		if (sizeDiff > 0)
			instrs.push_back(new InstrPush({ TypedArgument(DataType::BYTE, ArgType::VALUE, sizeDiff) }));

		auto src2 = TypedArgument(src2DataType, ArgType::SP_OFFSET, -1 * src2ByteSize + 1);
		auto src1 = TypedArgument(src1DataType, ArgType::SP_OFFSET, src2.value.i - src1ByteSize);
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
			default: throw std::runtime_error("BinopExpr::GenerateCode - BinopType not handled!");
		}

		//Create Pop to remove the bytes past the result
		if (sizeDiff < 0)
			instrs.push_back(new InstrPop(DataType::BYTE, Argument(ArgType::VALUE, -sizeDiff)));

		//Create Move to move result to destination
		if (!_dest.IsStackTop())
		{
			instrs.push_back(new InstrMove(destDataType, Argument(ArgType::SP_OFFSET, -1 * destByteSize + 1), _dest));
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

	ASTUnopExpr::ASTUnopExpr(ASTExpr* _expr, UnopType _op)
		: ASTExpr(ASTExprType::UNOP, _expr->GetPosition())
	{
		expr = _expr;
		op = _op;
	}

	ASTUnopExpr::~ASTUnopExpr() { delete expr; }

	std::string ASTUnopExpr::ToString(int _indentLvl, std::string _prefix)
	{
		std::stringstream ss;

		ss << CreateIndent(_indentLvl) << _prefix << "Unop Expression:" << std::endl;
		ss << CreateIndent(_indentLvl + 1) << "OP: " << UnopTypeToString(op) << std::endl;
		ss << expr->ToString(_indentLvl + 1);

		return ss.str();
	}

	TypeResult ASTUnopExpr::_TypeCheck(Environment* _env)
	{
		TypeResult typeRes = expr->TypeCheck(_env);

		if (!typeRes.IsSuccess())
			return typeRes;

		TypePtr exprType = typeRes.GetValue();

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

	InstructionSet ASTUnopExpr::GenerateCode(Argument _dest, ProgramInfo& _progInfo)
	{
		InstructionSet instrs = expr->GenerateCode(Argument::CreateStackTop(), _progInfo);

		auto srcDataType = TypeSysTypeToDataType(expr->GetTypeSysType()->GetType());
		auto destDataType = TypeSysTypeToDataType(this->GetTypeSysType()->GetType());

		int srcByteSize = expr->GetTypeSysType()->GetByteSize();
		int destByteSize = this->GetTypeSysType()->GetByteSize();
		int sizeDiff = destByteSize - srcByteSize;

		//Extend the stack to make room for the result if it takes up more space than the operand
		if (sizeDiff > 0)
			instrs.push_back(new InstrPush({ TypedArgument(DataType::BYTE, ArgType::VALUE, sizeDiff) }));

		auto src = TypedArgument(srcDataType, ArgType::SP_OFFSET, -1 * srcByteSize + 1);
		auto stackDest = TypedArgument(destDataType, ArgType::SP_OFFSET, src.value.i - fmax(0, sizeDiff));

		switch (op)
		{
			case UnopType::LOG_NOT: instrs.push_back(new InstrUnop(Unop::LOG_NOT, src, stackDest)); break;
			case UnopType::NEG: instrs.push_back(new InstrUnop(Unop::NEG, src, stackDest)); break;
			case UnopType::BIN_NOT: instrs.push_back(new InstrUnop(Unop::BIN_NOT, src, stackDest)); break;
			default: throw std::runtime_error("UnopExpr::GenerateCode - UnopType not handled!");
		}

		//Create Pop to remove the bytes past the result if any
		if (sizeDiff < 0)
			instrs.push_back(new InstrPop(DataType::BYTE, Argument(ArgType::VALUE, -sizeDiff)));

		//Create Move to move result to destination
		if (!_dest.IsStackTop())
		{
			instrs.push_back(new InstrMove(destDataType, Argument(ArgType::SP_OFFSET, -1 * destByteSize + 1), _dest));
			instrs.push_back(new InstrPop(destDataType, Argument(ArgType::VALUE, 1)));
		}

		return instrs;
	}
#pragma endregion

#pragma region Identifier
	ASTIdentifier::ASTIdentifier(std::string _id, Position _pos)
		: ASTExpr(ASTExprType::IDENTIFIER, _pos)
	{
		id = _id;
	}

	std::string ASTIdentifier::ToString(int _indentLvl, std::string _prefix) { return CreateIndent(_indentLvl) + _prefix + "Identifier: " + id; }

	TypeResult ASTIdentifier::_TypeCheck(Environment* _env)
	{
		source = _env->GetVarRegister(id);
		return _env->GetVariableType(id, GetPosition());
	}

	InstructionSet ASTIdentifier::GenerateCode(Argument _dest, ProgramInfo& _progInfo) { return { new InstrMove(TypeSysTypeToDataType(this->GetTypeSysType()->GetType()), GetSource(), _dest) }; }
#pragma endregion

#pragma region Literal
	InstructionSet ASTIntLit::GenerateCode(Argument _dest, ProgramInfo& _progInfo) { return { new InstrMove(DataType::INT, Argument(ArgType::VALUE, GetValue()), _dest) }; }

	InstructionSet ASTFloatLit::GenerateCode(Argument _dest, ProgramInfo& _progInfo) { return { new InstrMove(DataType::FLOAT, Argument(ArgType::VALUE, GetValue()), _dest) }; }

	InstructionSet ASTStringLit::GenerateCode(Argument _dest, ProgramInfo& _progInfo) { return { new InstrMove(DataType::INT, Argument(ArgType::VALUE, 0), _dest) }; }

	InstructionSet ASTBoolLit::GenerateCode(Argument _dest, ProgramInfo& _progInfo) { return { new InstrMove(DataType::BYTE, Argument(ArgType::VALUE, (byte)(GetValue() ? 1 : 0)), _dest) }; }

	InstructionSet ASTByteLit::GenerateCode(Argument _dest, ProgramInfo& _progInfo) { return { new InstrMove(DataType::BYTE, Argument(ArgType::VALUE, GetValue()), _dest) }; }

	InstructionSet ASTDoubleLit::GenerateCode(Argument _dest, ProgramInfo& _progInfo) { return { new InstrMove(DataType::DOUBLE, Argument(ArgType::VALUE, GetValue()), _dest) }; }

	InstructionSet ASTLongLit::GenerateCode(Argument _dest, ProgramInfo& _progInfo) { return { new InstrMove(DataType::LONG, Argument(ArgType::VALUE, GetValue()), _dest) }; }
#pragma endregion

#pragma region IfStmt
	ASTIfStmt::ASTIfStmt(ASTExpr* _condExpr, ASTStmt* _thenStmt, ASTStmt* _elseStmt, Position _pos)
		: ASTStmt(ASTStmtType::IF, _pos)
	{
		condExpr = _condExpr;
		thenStmt = _thenStmt;
		elseStmt = _elseStmt;
	}

	ASTIfStmt::~ASTIfStmt()
	{
		delete condExpr;
		delete thenStmt;

		if (elseStmt)
			delete elseStmt;
	}

	std::string ASTIfStmt::ToString(int _indentLvl, std::string _prefix)
	{
		std::stringstream ss;

		ss << CreateIndent(_indentLvl) << _prefix << "If Statement:" << std::endl;
		ss << condExpr->ToString(_indentLvl + 1, "Condition: ") << std::endl;
		ss << thenStmt->ToString(_indentLvl + 1, "Then: ") << std::endl;

		if (elseStmt)
			ss << elseStmt->ToString(_indentLvl + 1, "Else: ");

		return ss.str();
	}

	TypeResult ASTIfStmt::_TypeCheck(Environment* _env)
	{
		TypeResult condTypeRes = condExpr->TypeCheck(_env);
		if (!condTypeRes.IsSuccess()) { return condTypeRes; }
		else if (!condTypeRes.GetValue()->Matches(Type::BOOL)) { return TypeResult::GenExpectation(Type::BOOL->ToString(0), condTypeRes.GetValue()->ToString(0), condExpr->GetPosition()); }

		Environment thenSubEnv(_env, true);
		TypeResult thenTypeRes = thenStmt->TypeCheck(&thenSubEnv);
		if (!thenTypeRes.IsSuccess())
			return thenTypeRes;

		if (elseStmt)
		{
			Environment elseSubEnv(_env, true);
			TypeResult elseTypeRes = elseStmt->TypeCheck(&elseSubEnv);
			if (!elseTypeRes.IsSuccess())
				return elseTypeRes;
		}

		return TypeResult::GenSuccess(Type::VOID);
	}

	InstructionSet ASTIfStmt::GenerateCode(ProgramInfo& _progInfo)
	{
		int condByteSize = condExpr->GetTypeSysType()->GetByteSize();
		InstructionSet instrs = condExpr->GenerateCode(Argument::CreateStackTop(), _progInfo);
		InstructionSet thenInstrs = thenStmt->GenerateCode(_progInfo);

		if (elseStmt)
		{
			InstructionSet elseInstrs = elseStmt->GenerateCode(_progInfo);

			instrs.push_back(new InstrCJump(elseInstrs.size() + 3, Argument(ArgType::SP_OFFSET, 0), false));
			_progInfo.offsetedCtrlInstrs.push_back(instrs.back());

			//False
			instrs.push_back(new InstrPop(DataType::BYTE, Argument(ArgType::VALUE, condByteSize))); //Pop off condition
			instrs.insert(instrs.end(), elseInstrs.begin(), elseInstrs.end());
			instrs.push_back(new InstrJump(thenInstrs.size() + 2)); //Jump past true instructions
			_progInfo.offsetedCtrlInstrs.push_back(instrs.back());
		}
		else
		{
			instrs.push_back(new InstrCJump(thenInstrs.size() + 2, Argument(ArgType::SP_OFFSET, 0), true));
			_progInfo.offsetedCtrlInstrs.push_back(instrs.back());
		}

		instrs.push_back(new InstrPop(DataType::BYTE, Argument(ArgType::VALUE, condByteSize))); //Pop off condition
		instrs.insert(instrs.end(), thenInstrs.begin(), thenInstrs.end());
		return instrs;
	}
#pragma endregion

#pragma region IfExpr
	ASTIfExpr::ASTIfExpr(ASTExpr* _condExpr, ASTExpr* _thenExpr, ASTExpr* _elseExpr, Position _pos)
		: ASTExpr(ASTExprType::IF, _pos)
	{
		condExpr = _condExpr;
		thenExpr = _thenExpr;
		elseExpr = _elseExpr;
	}

	ASTIfExpr::~ASTIfExpr()
	{
		delete condExpr;
		delete thenExpr;
		delete elseExpr;
	}

	std::string ASTIfExpr::ToString(int _indentLvl, std::string _prefix)
	{
		std::stringstream ss;

		ss << CreateIndent(_indentLvl) << _prefix << "If Expression:" << std::endl;
		ss << condExpr->ToString(_indentLvl + 1, "Condition: ") << std::endl;
		ss << thenExpr->ToString(_indentLvl + 1, "Then: ") << std::endl;
		ss << elseExpr->ToString(_indentLvl + 1, "Else: ");

		return ss.str();
	}

	TypeResult ASTIfExpr::_TypeCheck(Environment* _env)
	{
		TypeResult condTypeRes = condExpr->TypeCheck(_env);
		if (!condTypeRes.IsSuccess()) { return condTypeRes; }
		else if (!condTypeRes.GetValue()->Matches(Type::BOOL)) { return TypeResult::GenExpectation(Type::BOOL->ToString(0), condTypeRes.GetValue()->ToString(0), condExpr->GetPosition()); }

		TypeResult thenTypeRes = thenExpr->TypeCheck(_env);
		if (!thenTypeRes.IsSuccess())
			return thenTypeRes;

		TypeResult elseTypeRes = elseExpr->TypeCheck(_env);
		if (!elseTypeRes.IsSuccess())
			return elseTypeRes;

		if (Type::Matches(thenTypeRes.GetValue(), elseTypeRes.GetValue())) { return thenTypeRes; }
		else { return TypeResult::GenExpectation(thenTypeRes.GetValue()->ToString(0), elseTypeRes.GetValue()->ToString(false), elseExpr->GetPosition()); }
	}

	InstructionSet ASTIfExpr::GenerateCode(Argument _dest, ProgramInfo& _progInfo)
	{
		int condByteSize = condExpr->GetTypeSysType()->GetByteSize();
		int resultByteSize = this->GetTypeSysType()->GetByteSize();

		InstructionSet thenInstrs = thenExpr->GenerateCode(Argument::CreateStackTop(), _progInfo);
		InstructionSet elseInstrs = elseExpr->GenerateCode(Argument::CreateStackTop(), _progInfo);
		InstructionSet instrs = condExpr->GenerateCode(Argument::CreateStackTop(), _progInfo);

		instrs.push_back(new InstrCJump(elseInstrs.size() + 3, Argument(ArgType::SP_OFFSET, 0), false));
		_progInfo.offsetedCtrlInstrs.push_back(instrs.back());

		//False
		instrs.push_back(new InstrPop(DataType::BYTE, Argument(ArgType::VALUE, condByteSize))); //Pop off condition
		instrs.insert(instrs.end(), elseInstrs.begin(), elseInstrs.end());
		instrs.push_back(new InstrJump(thenInstrs.size() + 2)); //Jump past true instructions
		_progInfo.offsetedCtrlInstrs.push_back(instrs.back());

		//True
		instrs.push_back(new InstrPop(DataType::BYTE, Argument(ArgType::VALUE, condByteSize))); //Pop off condition
		instrs.insert(instrs.end(), thenInstrs.begin(), thenInstrs.end());

		//Move value to destination
		if (!_dest.IsStackTop())
		{
			auto destDataType = TypeSysTypeToDataType(this->GetTypeSysType()->GetType());
			instrs.push_back(new InstrMove(destDataType, Argument(ArgType::SP_OFFSET, -resultByteSize + 1), _dest));
			instrs.push_back(new InstrPop(destDataType, Argument(ArgType::VALUE, 1)));
		}

		return instrs;
	}
#pragma endregion

#pragma region WhileStmt
	ASTWhileStmt::ASTWhileStmt(ASTExpr* _condExpr, ASTStmt* _body, Position _pos)
		: ASTStmt(ASTStmtType::WHILE, _pos)
	{
		condExpr = _condExpr;
		body = _body;
	}

	ASTWhileStmt::~ASTWhileStmt()
	{
		delete condExpr;
		delete body;
	}

	std::string ASTWhileStmt::ToString(int _indentLvl, std::string _prefix)
	{
		std::stringstream ss;

		ss << CreateIndent(_indentLvl) << _prefix << "While Statement:" << std::endl;
		ss << condExpr->ToString(_indentLvl + 1, "Condition: ") << std::endl;
		ss << body->ToString(_indentLvl + 1, "Body: ") << std::endl;

		return ss.str();
	}

	TypeResult ASTWhileStmt::_TypeCheck(Environment* _env)
	{
		TypeResult condTypeRes = condExpr->TypeCheck(_env);
		if (!condTypeRes.IsSuccess()) { return condTypeRes; }
		else if (!condTypeRes.GetValue()->Matches(Type::BOOL)) { return TypeResult::GenExpectation(Type::BOOL->ToString(0), condTypeRes.GetValue()->ToString(0), condExpr->GetPosition()); }

		Environment subEnv(_env, true);
		return body->TypeCheck(&subEnv);
	}

	InstructionSet ASTWhileStmt::GenerateCode(ProgramInfo& _progInfo)
	{
		int condByteSize = condExpr->GetTypeSysType()->GetByteSize();
		InstructionSet condInstrs = condExpr->GenerateCode(Argument::CreateStackTop(), _progInfo);;
		InstructionSet bodyInstrs = body->GenerateCode(_progInfo);
		InstructionSet instrs = condInstrs;

		instrs.push_back(new InstrCJump(bodyInstrs.size() + 3, Argument(ArgType::SP_OFFSET, 0), true));
		_progInfo.offsetedCtrlInstrs.push_back(instrs.back());

		instrs.push_back(new InstrPop(DataType::BYTE, Argument(ArgType::VALUE, condByteSize))); //Pop off condition if do
		instrs.insert(instrs.end(), bodyInstrs.begin(), bodyInstrs.end());

		instrs.push_back(new InstrJump(-int(bodyInstrs.size() + condInstrs.size() + 2))); //Jump back to condition instructions
		_progInfo.offsetedCtrlInstrs.push_back(instrs.back());

		instrs.push_back(new InstrPop(DataType::BYTE, Argument(ArgType::VALUE, condByteSize))); //Pop off condition if not do

		return instrs;
	}
#pragma endregion

#pragma region ForStmt
	ASTForStmt::ASTForStmt(ASTVarDecl* _initStmt, ASTExpr* _condExpr, ASTStmt* _body, ASTStmt* _thenStmt, Position _pos)
		: ASTStmt(ASTStmtType::FOR, _pos)
	{
		initStmt = _initStmt;
		condExpr = _condExpr;
		body = _body;
		thenStmt = _thenStmt;
		blockHolder = new ASTBlock({ initStmt, new ASTWhileStmt(condExpr, new ASTBlock({ body, thenStmt }, _pos), _pos) }, _pos);
	}

	ASTForStmt::~ASTForStmt() { delete blockHolder; }

	std::string ASTForStmt::ToString(int _indentLvl, std::string _prefix)
	{
		std::stringstream ss;

		ss << CreateIndent(_indentLvl) << _prefix << "For Statement:" << std::endl;
		ss << initStmt->ToString(_indentLvl + 1) << std::endl;
		ss << body->ToString(_indentLvl + 1, "Body: ") << std::endl;
		ss << thenStmt->ToString(_indentLvl + 1, "Then: ") << std::endl;

		return ss.str();
	}

	TypeResult ASTForStmt::_TypeCheck(Environment* _env)
	{
		Environment subEnv(_env, true);

		TypeResult initTypeRes = initStmt->TypeCheck(&subEnv);
		if (!initTypeRes.IsSuccess())
			return initTypeRes;

		TypeResult condTypeRes = condExpr->TypeCheck(&subEnv);
		if (!condTypeRes.IsSuccess()) { return condTypeRes; }
		else if (!condTypeRes.GetValue()->Matches(Type::BOOL)) { return TypeResult::GenExpectation(Type::BOOL->ToString(0), condTypeRes.GetValue()->ToString(0), condExpr->GetPosition()); }

		TypeResult bodyTypeRes = body->TypeCheck(&subEnv);
		if (!bodyTypeRes.IsSuccess())
			return bodyTypeRes;

		return thenStmt->TypeCheck(&subEnv);
	}

	InstructionSet ASTForStmt::GenerateCode(ProgramInfo& _progInfo) { return blockHolder->GenerateCode(_progInfo); }
#pragma endregion

#pragma region Block
	ASTBlock::ASTBlock(const std::vector<ASTStmt*>& _stmts, Position _pos)
		: ASTStmt(ASTStmtType::BLOCK, _pos), stmts(_stmts) { }

	ASTBlock::~ASTBlock()
	{
		for (auto it = stmts.begin(); it != stmts.end(); it++)
			delete (*it);
	}

	std::string ASTBlock::ToString(int _indentLvl, std::string _prefix)
	{
		std::stringstream ss;

		ss << CreateIndent(_indentLvl) << _prefix << "Block:" << std::endl;

		for (auto it = stmts.begin(); it != stmts.end(); it++)
			ss << (*it)->ToString(_indentLvl + 1) << std::endl;

		return ss.str();
	}

	TypeResult ASTBlock::_TypeCheck(Environment* _env)
	{
		for (auto it = stmts.begin(); it != stmts.end(); it++)
		{
			TypeResult typeRes = (*it)->TypeCheck(_env);
			if (!typeRes.IsSuccess())
				return typeRes;
		}

		return TypeResult::GenSuccess(Type::VOID);
	}

	InstructionSet ASTBlock::GenerateCode(ProgramInfo& _progInfo)
	{
		InstructionSet instrs;

		for (auto it = stmts.begin(); it != stmts.end(); it++)
		{
			InstructionSet stmtInstrs = (*it)->GenerateCode(_progInfo);
			instrs.insert(instrs.end(), stmtInstrs.begin(), stmtInstrs.end());
		}

		return instrs;
	}
#pragma endregion
}