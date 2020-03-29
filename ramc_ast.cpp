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
using ramvm::InstrNoOp;
using ramvm::InstructionType;
using ramvm::InstrHalt;
using ramvm::InstrReturn;
using ramvm::InstrCall;
#pragma endregion

namespace ramc {
	std::string ASTStmtTypeToString(ASTStmtType _type)
	{
		switch (_type)
		{
			case ASTStmtType::ASSIGNMENT: return "ASSIGNMENT";
			case ASTStmtType::VARDECL: return "VARDECL";
			case ASTStmtType::BLOCK: return "BLOCK";
			case ASTStmtType::IF: return "IF";
			case ASTStmtType::WHILE: return "WHILE";
			case ASTStmtType::FORLOOP: return "FORLOOP";
			case ASTStmtType::BREAK: return "BREAK";
			case ASTStmtType::CONTINUE: return "CONTINUE";
			case ASTStmtType::RETURN: return "RETURN";
			default: return "ASTStmtTypeToString() - ASTStmtType not handled!";
		}
	}

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
	ASTProgram::ASTProgram(std::string _fileName, std::vector<ASTVarDecl*> _varDecls, std::vector<ASTFuncDecl*> _funcDecls)
		: ASTNode(ASTNodeType::PROGRAM, Position(0, 0))
	{
		fileName = _fileName;
		varDecls = _varDecls;
		funcDecls = _funcDecls;
	}

	ASTProgram::~ASTProgram()
	{
		for (auto const& varDecl : varDecls)
			delete varDecl;

		for (auto const& funcDecl : funcDecls)
			delete funcDecl;
	}

	std::string ASTProgram::ToString(int _indentLvl, std::string _prefix)
	{
		std::stringstream ss;

		ss << CreateIndent(_indentLvl) << _prefix << "Start:" << std::endl;
		ss << CreateIndent(_indentLvl + 1) << "Filename: " << fileName << std::endl;

		ss << CreateIndent(_indentLvl + 1) << "Top Level Variables: " << std::endl;
		for (auto const& varDecl : varDecls)
			ss << varDecl->ToString(_indentLvl + 2) << std::endl;

		ss << CreateIndent(_indentLvl + 1) << "Function Declarations: " << std::endl;
		for (auto const& funcDecl : funcDecls)
			ss << funcDecl->ToString(_indentLvl + 2) << std::endl;

		return ss.str();
	}

	TypeResult ASTProgram::_TypeCheck(Environment* _env)
	{
		for (auto const& varDecl : varDecls)
		{
			TypeResult declTypeRes = varDecl->TypeCheck(_env);
			if (!declTypeRes.IsSuccess())
				return declTypeRes;
		}

		for (auto const& funcDecl : funcDecls)
		{
			TypeResult declTypeRes = funcDecl->TypeCheck(_env);
			if (!declTypeRes.IsSuccess())
				return declTypeRes;
		}

		return TypeResult::GenSuccess(Type::UNIT);
	}

	InstructionSet ASTProgram::GenerateCode(Environment* _env)
	{
		InstructionSet instrs;

		for (auto const& varDecl : varDecls)
		{
			auto declInstrs = varDecl->GenerateCode(info);
			instrs.insert(instrs.end(), declInstrs.begin(), declInstrs.end());
		}

		instrs.push_back(new InstrHalt());

		for (auto const& funcDecl : funcDecls)
		{
			auto declInstrs = funcDecl->GenerateCode(_env, info);
			instrs.insert(instrs.end(), declInstrs.begin(), declInstrs.end());
		}

		//Convert Labels
		for (int i = 0; i < (int)info.offsetedCtrlInstrs.size(); i++)
		{
			auto instr = info.offsetedCtrlInstrs[i];
			int idx = std::find(instrs.begin(), instrs.end(), instr) - instrs.begin();

			if (instr->GetType() == InstructionType::JUMP) { ((InstrJump*)instr)->instrIdx += idx; }
			else if (instr->GetType() == InstructionType::CJUMP) { ((InstrCJump*)instr)->instrIdx += idx; }
			else if (instr->GetType() == InstructionType::CALL) { ((InstrCall*)instr)->instrIdx += idx; }
		}

		for (auto it = info.labeledCtrlInstrs.begin(); it != info.labeledCtrlInstrs.end(); it++)
		{
			int labelInstrIdx = std::find(instrs.begin(), instrs.end(), info.labels[it->first]) - instrs.begin();

			for (auto instr : it->second)
			{
				if (instr->GetType() == InstructionType::JUMP) { ((InstrJump*)instr)->instrIdx = labelInstrIdx; }
				else if (instr->GetType() == InstructionType::CJUMP) { ((InstrCJump*)instr)->instrIdx = labelInstrIdx; }
				else if (instr->GetType() == InstructionType::CALL) { ((InstrCall*)instr)->instrIdx = labelInstrIdx; }
			}
		}

		return instrs;
	}
#pragma endregion

#pragma region FuncDecl
	ASTFuncDecl::ASTFuncDecl(std::string _name, ParamList _params, TypeList _retTypes, ASTStmt* _body, Position _pos)
		:ASTNode(ASTNodeType::FUNCDECL, _pos)
	{
		name = _name;
		params = _params;

		TypeList paramTypes;
		for (auto const& it : params)
			paramTypes.push_back(it.type);

		TypePtr retType;
		if (_retTypes.size() == 0) { retType = Type::UNIT; }
		else if (_retTypes.size() == 1) { retType = _retTypes[0]; }
		else { retType = TypePtr(new TupleType(_retTypes)); }

		funcType = new FuncType(TypePtr(new TupleType(paramTypes)), retType);
		body = _body;
	}

	ASTFuncDecl::~ASTFuncDecl() { delete body; }

	std::string ASTFuncDecl::ToString(int _indentLvl, std::string _prefix)
	{
		std::stringstream ss;

		ss << CreateIndent(_indentLvl) << _prefix << "Func Declaration:" << std::endl;
		ss << CreateIndent(_indentLvl + 1) << "Type: " << funcType->ToString(0) << std::endl;
		ss << CreateIndent(_indentLvl + 1) << "Body: " << std::endl << body->ToString(_indentLvl + 2) << std::endl;

		return ss.str();
	}

	TypeResult ASTFuncDecl::_TypeCheck(Environment* _env)
	{
		TypePtr typePtr = TypePtr(funcType);

		//Add Function to environment
		TypeResult declTypeRes = _env->AddFunction(name, typePtr, label, GetPosition());
		if (!declTypeRes.IsSuccess())
			return declTypeRes;

		Environment subEnv(_env, false); //Local function environment

		//Type check parameters in local environment
		for (auto const& it : params)
		{
			TypeResult paramTypeRes = subEnv.AddVariable(it.name, it.type, ArgType::STACK_REG, it.position);
			if (!paramTypeRes.IsSuccess())
				return paramTypeRes;
		}

		//Type check body in local environment
		TypeResult bodyTypeRes = body->TypeCheck(&subEnv);
		if (!bodyTypeRes.IsSuccess())
			return bodyTypeRes;

		std::set<ASTStmt*> terminalStmts = body->GetCodePath()->GetTerminalStmts();
		bool needsReturn = !funcType->IsProcedure();

		for (auto const& stmt : terminalStmts)
		{
			if (needsReturn && stmt->GetStmtType() != ASTStmtType::RETURN) { return TypeResult::GenCodePathLacksReturn(name, GetPosition()); }
			else if (!Type::Matches(funcType->GetRetType(), stmt->GetTypeSysType())) { return TypeResult::GenExpectation(funcType->GetRetType()->ToString(0), stmt->GetTypeSysType()->ToString(0), stmt->GetPosition()); }
		}

		regCnt = subEnv.GetNumRegNeeded();
		return TypeResult::GenSuccess(typePtr);
	}

	InstructionSet ASTFuncDecl::GenerateCode(Environment* _env, ProgramInfo& _progInfo)
	{
		InstructionSet instrs;

		//Set the needed number of registers 
		_progInfo.funcRegCnts.insert_or_assign(label, regCnt);

		int paramsTotalByteSize = 0;
		for (int i = 0, offset = 0; i < params.size(); i++)
		{
			Argument offsetArg = Argument(ArgType::VALUE, offset);
			Argument destReg = Argument(ArgType::REGISTER, i);
			instrs.push_back(new InstrMove(DataType::INT, offsetArg, destReg));

			int byteSize = params[i].type->GetByteSize();
			offset -= byteSize;
			paramsTotalByteSize += byteSize;
		}

		InstructionSet bodyInstrs = body->GenerateCode(_progInfo);
		instrs.insert(instrs.end(), bodyInstrs.begin(), bodyInstrs.end());

		if (funcType->IsProcedure())
			instrs.push_back(new InstrReturn(Argument(ArgType::VALUE, 0)));

		_progInfo.AddFuncDecl(Environment::GenFuncLabel(name, funcType->GetParamsType()), instrs.front());
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

		if (!exprTypeRes.IsSuccess()) { return exprTypeRes; }
		else if (isUnderscore) { return TypeResult::GenSuccess(Type::UNIT); }
		else if (restraint && !Type::Matches(exprTypeRes.GetValue(), restraint)) { return TypeResult::GenExpectation(restraint->ToString(0), exprTypeRes.GetValue()->ToString(0), GetPosition()); }
		else
		{
			TypeResult idRes = _env->AddVariable(id->GetID(), exprTypeRes.GetValue(), ArgType::REGISTER, GetPosition());
			if (!idRes.IsSuccess()) 
				return idRes;

			IGNORE(id->TypeCheck(_env)); //Set the info for the id for code generation later
			return TypeResult::GenSuccess(Type::UNIT);
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

	CodePathNode* ASTVarDecl::GetCodePath() { return new CodePathNode(nullptr, nullptr, this, true); }
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

		if (Type::Matches(idTypeRes.GetValue(), exprTypeRes.GetValue())) { return TypeResult::GenSuccess(Type::UNIT); }
		else { return TypeResult::GenMismatch("Cannot assign " + exprTypeRes.GetValue()->ToString(false) + " to " + idTypeRes.GetValue()->ToString(false), GetPosition()); }
	}

	InstructionSet ASTAssignment::GenerateCode(ProgramInfo& _progInfo) { return expr->GenerateCode(id->GetSource(), _progInfo); }
	CodePathNode* ASTAssignment::GetCodePath() { return new CodePathNode(nullptr, nullptr, this, true); }
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
		source = _env->GetVarSource(id);
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

#pragma region FuncCall Expression
	ASTFuncCallExpr::ASTFuncCallExpr(std::string _funcName, const ExprList& _args, Position _pos)
		: ASTExpr(ASTExprType::FUNC_CALL, _pos)
	{
		funcName = _funcName;
		args = _args;
	}

	ASTFuncCallExpr::~ASTFuncCallExpr()
	{
		for (auto const& arg : args)
			delete arg;
	}

	std::string ASTFuncCallExpr::ToString(int _indentLvl, std::string _prefix)
	{
		std::stringstream ss;

		ss << CreateIndent(_indentLvl) << _prefix << "FuncCall Expression: " << funcName;

		for (int i = 0; i < (int)args.size(); i++)
			ss << std::endl << CreateIndent(_indentLvl + 1) << "ARG" << i << " = " << args[i]->ToString(0);

		return ss.str();
	}

	TypeResult ASTFuncCallExpr::_TypeCheck(Environment* _env)
	{
		TypeList argTypes;
		for (auto const& arg : args)
		{
			TypeResult argTypeRes = arg->TypeCheck(_env);
			if (!argTypeRes.IsSuccess())
				return argTypeRes;

			argTypes.push_back(argTypeRes.GetValue());
		}

		TypePtr paramsType = TypePtr(new TupleType(argTypes));
		TypeResult funcTypeRes = _env->GetFunctionRetType(funcName, paramsType, GetPosition());
		if (!funcTypeRes.IsSuccess())
			return funcTypeRes;

		funcLabel = Environment::GenFuncLabel(funcName, paramsType);
		return TypeResult::GenSuccess(((FuncType*)funcTypeRes.GetValue().get())->GetRetType());
	}

	InstructionSet ASTFuncCallExpr::GenerateCode(Argument _dest, ProgramInfo& _progInfo)
	{
		InstructionSet instrs;
		int argsByteLen = 0;

		//Push onto stack in reverse order to first arg is on top
		//when the callee starts reading them
		for (int i = args.size() - 1; i >= 0; i--)
		{
			ASTExpr* arg = args[i];
			auto argInstrs = arg->GenerateCode(Argument::CreateStackTop(), _progInfo);

			instrs.insert(instrs.end(), argInstrs.begin(), argInstrs.end());
			argsByteLen += arg->GetTypeSysType()->GetByteSize();
		}

		instrs.push_back(new InstrCall(-1, funcRegCnt, Argument(ArgType::VALUE, argsByteLen)));
		_progInfo.labeledCtrlInstrs[funcLabel].push_back(instrs.back());

		return instrs;
	}
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

		return TypeResult::GenSuccess(Type::UNIT);
	}

	InstructionSet ASTIfStmt::GenerateCode(ProgramInfo& _progInfo)
	{
		auto ifLabels = _progInfo.GenIfLabels(elseStmt); //Gen Labels

		int condByteSize = condExpr->GetTypeSysType()->GetByteSize();
		InstructionSet instrs = condExpr->GenerateCode(Argument::CreateStackTop(), _progInfo);
		InstructionSet thenInstrs = thenStmt->GenerateCode(_progInfo);

		_progInfo.SetLabelInstr(ifLabels.begin, instrs.front()); //Set If Begin

		if (elseStmt)
		{
			InstructionSet elseInstrs = elseStmt->GenerateCode(_progInfo);

			instrs.push_back(new InstrCJump(-1, Argument(ArgType::SP_OFFSET, 0), false)); //Jump to then clause if cond = true
			_progInfo.labeledCtrlInstrs[ifLabels.thenClause].push_back(instrs.back());

			//False
			instrs.push_back(new InstrPop(DataType::BYTE, Argument(ArgType::VALUE, condByteSize))); //Pop off condition
			_progInfo.SetLabelInstr(ifLabels.elseClause, instrs.back()); //Set Else Begin
			instrs.insert(instrs.end(), elseInstrs.begin(), elseInstrs.end());
			instrs.push_back(new InstrJump(-1)); //Jump to If End
			_progInfo.labeledCtrlInstrs[ifLabels.end].push_back(instrs.back());
		}
		else
		{
			instrs.push_back(new InstrCJump(-1, Argument(ArgType::SP_OFFSET, 0), true)); //Jump to end if cond = false
			_progInfo.labeledCtrlInstrs[ifLabels.end].push_back(instrs.back());
		}

		instrs.push_back(new InstrPop(DataType::BYTE, Argument(ArgType::VALUE, condByteSize))); //Pop off condition
		_progInfo.SetLabelInstr(ifLabels.thenClause, instrs.back()); //Set Then Begin
		instrs.insert(instrs.end(), thenInstrs.begin(), thenInstrs.end());

		//To Set the End Label to the right spot
		if (elseStmt) { instrs.push_back(new InstrNoOp()); }
		else { instrs.push_back(new InstrPop(DataType::BYTE, Argument(ArgType::VALUE, condByteSize))); } //Pop off condition

		_progInfo.SetLabelInstr(ifLabels.end, instrs.back()); //Set If End
		return instrs;
	}

	CodePathNode* ASTIfStmt::GetCodePath() { return new CodePathNode(thenStmt->GetCodePath(), elseStmt ? elseStmt->GetCodePath() : nullptr, this, true); }
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
		auto ifLabels = _progInfo.GenIfLabels(true); //Gen Labels

		int condByteSize = condExpr->GetTypeSysType()->GetByteSize();
		int resultByteSize = this->GetTypeSysType()->GetByteSize();

		InstructionSet thenInstrs = thenExpr->GenerateCode(Argument::CreateStackTop(), _progInfo);
		InstructionSet elseInstrs = elseExpr->GenerateCode(Argument::CreateStackTop(), _progInfo);
		InstructionSet instrs = condExpr->GenerateCode(Argument::CreateStackTop(), _progInfo);

		_progInfo.SetLabelInstr(ifLabels.begin, instrs.front()); //Set If Begin

		instrs.push_back(new InstrCJump(-1, Argument(ArgType::SP_OFFSET, 0), false)); //Jump to then clause if cond = true
		_progInfo.labeledCtrlInstrs[ifLabels.thenClause].push_back(instrs.back());

		//False
		instrs.push_back(new InstrPop(DataType::BYTE, Argument(ArgType::VALUE, condByteSize))); //Pop off condition
		_progInfo.SetLabelInstr(ifLabels.elseClause, instrs.back()); //Set Else Begin
		instrs.insert(instrs.end(), elseInstrs.begin(), elseInstrs.end());
		instrs.push_back(new InstrJump(-1)); //Jump to If End
		_progInfo.labeledCtrlInstrs[ifLabels.end].push_back(instrs.back());

		//True
		instrs.push_back(new InstrPop(DataType::BYTE, Argument(ArgType::VALUE, condByteSize))); //Pop off condition
		_progInfo.SetLabelInstr(ifLabels.thenClause, instrs.back()); //Set Then Begin
		instrs.insert(instrs.end(), thenInstrs.begin(), thenInstrs.end());
		instrs.push_back(new InstrNoOp());
		_progInfo.SetLabelInstr(ifLabels.end, instrs.back()); //Set If End

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
		auto loopLabels = _progInfo.GenWhileLoopLabels(); //Gen Labels
		_progInfo.curLoopLabels.push_back((ProgramInfo::LabeledLoop*) & loopLabels); //Begin Loop Section

		InstructionSet instrs = condExpr->GenerateCode(Argument::CreateStackTop(), _progInfo);
		InstructionSet bodyInstrs = body->GenerateCode(_progInfo);

		_progInfo.SetLabelInstr(loopLabels.begin, instrs.front()); //Set Loop Begin

		instrs.push_back(new InstrCJump(-1, Argument(ArgType::SP_OFFSET, 0), true)); //JUMPF to Loop End
		_progInfo.labeledCtrlInstrs[loopLabels.pop].push_back(instrs.back());

		instrs.push_back(new InstrPop(DataType::BYTE, Argument(ArgType::VALUE, BYTE_SIZE))); //Pop off condition if do
		instrs.insert(instrs.end(), bodyInstrs.begin(), bodyInstrs.end());

		instrs.push_back(new InstrJump(-1)); //Jump back to condition instructions
		_progInfo.labeledCtrlInstrs[loopLabels.begin].push_back(instrs.back());

		instrs.push_back(new InstrPop(DataType::BYTE, Argument(ArgType::VALUE, BYTE_SIZE))); //Pop off condition if not do
		_progInfo.SetLabelInstr(loopLabels.pop, instrs.back()); //Set Loop Pop

		instrs.push_back(new InstrNoOp());
		_progInfo.SetLabelInstr(loopLabels.end, instrs.back()); //Set Loop End

		_progInfo.curLoopLabels.pop_back(); //End Loop Section 
		return instrs;
	}

	CodePathNode* ASTWhileStmt::GetCodePath()
	{
		CodePathNode* codePath = new CodePathNode(body->GetCodePath(), nullptr, this, true);
		codePath->SetAppendable({ ASTStmtType::BREAK, ASTStmtType::CONTINUE });
		return codePath;
	}
#pragma endregion

#pragma region ForStmt
	ASTForStmt::ASTForStmt(ASTVarDecl* _initStmt, ASTExpr* _condExpr, ASTStmt* _body, ASTStmt* _thenStmt, Position _pos)
		: ASTStmt(ASTStmtType::FORLOOP, _pos)
	{
		initStmt = _initStmt;
		condExpr = _condExpr;
		body = _body;
		thenStmt = _thenStmt;
	}

	ASTForStmt::~ASTForStmt()
	{
		delete initStmt;
		delete condExpr;
		delete body;
		delete thenStmt;
	}

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

	InstructionSet ASTForStmt::GenerateCode(ProgramInfo& _progInfo)
	{
		auto loopLabels = _progInfo.GenForLoopLabels(); //Gen Labels
		_progInfo.curLoopLabels.push_back((ProgramInfo::LabeledLoop*) & loopLabels); //Begin Loop Section

		InstructionSet instrs = initStmt->GenerateCode(_progInfo);
		InstructionSet condInstrs = condExpr->GenerateCode(Argument::CreateStackTop(), _progInfo);
		InstructionSet bodyInstrs = body->GenerateCode(_progInfo);
		InstructionSet thenInstrs = thenStmt->GenerateCode(_progInfo);

		instrs.insert(instrs.end(), condInstrs.begin(), condInstrs.end());
		_progInfo.SetLabelInstr(loopLabels.begin, condInstrs.front()); //Set Loop Begin

		instrs.push_back(new InstrCJump(-1, Argument(ArgType::SP_OFFSET, 0), true)); //JUMPF to Loop End
		_progInfo.labeledCtrlInstrs[loopLabels.pop].push_back(instrs.back());

		instrs.push_back(new InstrPop(DataType::BYTE, Argument(ArgType::VALUE, BYTE_SIZE))); //Pop off condition if do
		instrs.insert(instrs.end(), bodyInstrs.begin(), bodyInstrs.end());

		instrs.insert(instrs.end(), thenInstrs.begin(), thenInstrs.end());
		_progInfo.SetLabelInstr(loopLabels.then, thenInstrs.front()); //Set Loop Then

		instrs.push_back(new InstrJump(-1)); //Jump back to condition instructions
		_progInfo.labeledCtrlInstrs[loopLabels.begin].push_back(instrs.back());

		instrs.push_back(new InstrPop(DataType::BYTE, Argument(ArgType::VALUE, BYTE_SIZE))); //Pop off condition if not do
		_progInfo.SetLabelInstr(loopLabels.pop, instrs.back()); //Set Loop Pop

		instrs.push_back(new InstrNoOp());
		_progInfo.SetLabelInstr(loopLabels.end, instrs.back()); //Set Loop End

		_progInfo.curLoopLabels.pop_back(); //End Loop Section 
		return instrs;
	}

	CodePathNode* ASTForStmt::GetCodePath()
	{
		CodePathNode* codePath = new CodePathNode(body->GetCodePath(), nullptr, this, true);
		codePath->Append(thenStmt, { ASTStmtType::CONTINUE });
		codePath->SetAppendable({ ASTStmtType::BREAK });
		return codePath;
	}
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

		return TypeResult::GenSuccess(Type::UNIT);
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

	CodePathNode* ASTBlock::GetCodePath()
	{
		CodePathNode* codePath = new CodePathNode(nullptr, nullptr, this, true);

		for (auto const& stmt : stmts)
		{
			auto terminalNodes = codePath->GetTerminalNodes();
			for (auto const& terminal : terminalNodes)
				terminal->Append(stmt, { });

			ASTStmtType stmtASTType = stmt->GetStmtType();
			if (stmtASTType == ASTStmtType::RETURN || stmtASTType == ASTStmtType::BREAK || stmtASTType == ASTStmtType::CONTINUE)
				break;
		}

		return codePath;
	}
#pragma endregion

#pragma region Break/Continue
	ASTBreakContinueStmt::ASTBreakContinueStmt(bool _isBreak, Position _pos)
		: ASTStmt(_isBreak ? ASTStmtType::BREAK : ASTStmtType::CONTINUE, _pos) { }

	std::string ASTBreakContinueStmt::ToString(int _indentLvl, std::string _prefix) { return CreateIndent(_indentLvl) + _prefix + (IsBreak() ? "Break" : "Continue"); }

	TypeResult ASTBreakContinueStmt::_TypeCheck(Environment* _env) { return TypeResult::GenSuccess(Type::UNIT); }

	InstructionSet ASTBreakContinueStmt::GenerateCode(ProgramInfo& _progInfo)
	{
		ProgramInfo::LabeledLoop* curLoopLabel = _progInfo.curLoopLabels.back();
		InstructionSet instrs = { new InstrJump(-1) };

		std::string label;
		if (curLoopLabel->isForLoop) { label = IsBreak() ? ((ProgramInfo::LabeledForLoop*)curLoopLabel)->end : ((ProgramInfo::LabeledForLoop*)curLoopLabel)->then; }
		else { label = IsBreak() ? ((ProgramInfo::LabeledWhileLoop*)curLoopLabel)->end : ((ProgramInfo::LabeledWhileLoop*)curLoopLabel)->begin; }

		_progInfo.labeledCtrlInstrs[label].push_back(instrs.back());

		return instrs;
	}

	CodePathNode* ASTBreakContinueStmt::GetCodePath() { return new CodePathNode(nullptr, nullptr, this, false); }
#pragma endregion

#pragma region ReturnStmt
	ASTReturnStmt::ASTReturnStmt(std::vector<ASTExpr*> _exprs, Position _pos)
		: ASTStmt(ASTStmtType::RETURN, _pos), exprs(_exprs) { }

	ASTReturnStmt::~ASTReturnStmt()
	{
		for (auto const& it : exprs)
			delete it;
	}

	std::string ASTReturnStmt::ToString(int _indentLvl, std::string _prefix)
	{
		std::stringstream ss;
		ss << CreateIndent(_indentLvl) + _prefix;
		ss << "Return" << (exprs.size() == 0 ? "" : ": ");

		for (auto const& it : exprs)
			ss << std::endl << it->ToString(_indentLvl + 1);

		return ss.str();
	}

	TypeResult ASTReturnStmt::_TypeCheck(Environment* _env)
	{
		TypeList exprTypes;
		for (auto const& it : exprs)
		{
			TypeResult exprTypeRes = it->TypeCheck(_env);
			if (!exprTypeRes.IsSuccess()) { return exprTypeRes; }
			else { exprTypes.push_back(exprTypeRes.GetValue()); }
		}

		int numExprs = exprTypes.size();
		if (numExprs == 0) { return TypeResult::GenSuccess(Type::UNIT); }
		else if (numExprs == 1) { return TypeResult::GenSuccess(exprTypes[0]); }
		else { return TypeResult::GenSuccess(TypePtr(new TupleType(exprTypes))); }
	}

	InstructionSet ASTReturnStmt::GenerateCode(ProgramInfo& _progInfo)
	{
		InstructionSet instrs;
		int retByteAmt = 0;

		for (int i = exprs.size() - 1; i >= 0; i--)
		{
			ASTExpr* expr = exprs[i];
			auto exprInstrs = expr->GenerateCode(Argument::CreateStackTop(), _progInfo);

			instrs.insert(instrs.end(), exprInstrs.begin(), exprInstrs.end());
			retByteAmt += expr->GetTypeSysType()->GetByteSize();
		}

		instrs.push_back(new InstrReturn(Argument(ArgType::VALUE, retByteAmt)));

		return instrs;
	}

	CodePathNode* ASTReturnStmt::GetCodePath() { return new CodePathNode(nullptr, nullptr, this, false); }
#pragma endregion

#pragma region Program Info
	ProgramInfo::LabeledWhileLoop ProgramInfo::GenWhileLoopLabels()
	{
		std::string beginLabel = "LoopBegin" + std::to_string(numLoopLabels);
		std::string popLabel = "LoopPop" + std::to_string(numLoopLabels);
		std::string endLabel = "LoopEnd" + std::to_string(numLoopLabels);

		labeledCtrlInstrs.insert_or_assign(beginLabel, InstructionSet());
		labeledCtrlInstrs.insert_or_assign(popLabel, InstructionSet());
		labeledCtrlInstrs.insert_or_assign(endLabel, InstructionSet());

		labels.insert_or_assign(beginLabel, nullptr);
		labels.insert_or_assign(popLabel, nullptr);
		labels.insert_or_assign(endLabel, nullptr);
		numLoopLabels++;

		return { beginLabel, popLabel, endLabel };
	}

	ProgramInfo::LabeledForLoop ProgramInfo::GenForLoopLabels()
	{
		static int numLoopLabels = 0;

		std::string beginLabel = "LoopBegin" + std::to_string(numLoopLabels);
		std::string popLabel = "LoopPop" + std::to_string(numLoopLabels);
		std::string thenLabel = "LoopThen" + std::to_string(numLoopLabels);
		std::string endLabel = "LoopEnd" + std::to_string(numLoopLabels);

		labeledCtrlInstrs.insert_or_assign(beginLabel, InstructionSet());
		labeledCtrlInstrs.insert_or_assign(popLabel, InstructionSet());
		labeledCtrlInstrs.insert_or_assign(thenLabel, InstructionSet());
		labeledCtrlInstrs.insert_or_assign(endLabel, InstructionSet());

		labels.insert_or_assign(beginLabel, nullptr);
		labels.insert_or_assign(popLabel, nullptr);
		labels.insert_or_assign(endLabel, nullptr);
		labels.insert_or_assign(thenLabel, nullptr);

		numLoopLabels++;
		return { beginLabel, popLabel, thenLabel, endLabel };
	}

	ProgramInfo::LabeledIf ProgramInfo::GenIfLabels(bool _hasElse)
	{
		static int numIfLabels = 0;

		std::string beginLabel = "IfBegin" + std::to_string(numIfLabels);
		std::string thenLabel = "IfThen" + std::to_string(numIfLabels);
		std::string elseLabel = "IfElse" + std::to_string(numIfLabels);
		std::string endLabel = "IfEnd" + std::to_string(numIfLabels);

		//No Need to add being label since no instructions will jump to it
		labeledCtrlInstrs.insert_or_assign(thenLabel, InstructionSet());
		labeledCtrlInstrs.insert_or_assign(elseLabel, InstructionSet());
		labeledCtrlInstrs.insert_or_assign(endLabel, InstructionSet());
		labels.insert_or_assign(beginLabel, nullptr); //Add it here so we can print it out
		labels.insert_or_assign(thenLabel, nullptr);
		labels.insert_or_assign(elseLabel, nullptr);
		labels.insert_or_assign(endLabel, nullptr);

		if (_hasElse)
		{
			labeledCtrlInstrs.insert_or_assign(elseLabel, InstructionSet());
			labels.insert_or_assign(elseLabel, nullptr);
		}

		numIfLabels++;
		return { beginLabel, thenLabel, elseLabel, endLabel };
	}

	void ProgramInfo::AddFuncDecl(std::string _label, Instruction* _instr)
	{
		labels.insert_or_assign(_label, _instr);
	}

	void ProgramInfo::SetLabelInstr(std::string _label, Instruction* _instr)
	{
		if (labels.find(_label) == labels.end()) { throw std::runtime_error("ProgramInfo::SetLabelInstr() - Label does not exist!"); }
		else if (labels[_label]) { throw std::runtime_error("ProgramInfo::SetLabelInstr() - Loop label already set!"); }
		else { labels[_label] = _instr; }
	}
#pragma endregion

#pragma region Code Path
	CodePathNode::CodePathNode(CodePathNode* _left, CodePathNode* _right, ASTStmt* _stmt, bool _appendable)
	{
		left = _left;
		right = _right;
		statement = _stmt;
		appendable = _appendable;
	}

	CodePathNode::~CodePathNode()
	{
		delete left;
		delete right;
	}

	void CodePathNode::Append(ASTStmt* _stmt, const std::set<ASTStmtType>& _overrides)
	{
		if (!left && !right)
		{
			if (!appendable && _overrides.find(statement->GetStmtType()) == _overrides.end())
				return;

			left = _stmt->GetCodePath();
			return;
		}

		if (left)
			left->Append(_stmt, _overrides);

		if (right)
			right->Append(_stmt, _overrides);
	}

	void CodePathNode::SetAppendable(const std::set<ASTStmtType>& _types)
	{
		if (!left && !right)
		{
			if (_types.find(statement->GetStmtType()) != _types.end())
				appendable = true;

			return;
		}

		if (left) { left->SetAppendable(_types); }
		if (right) { right->SetAppendable(_types); }
	}

	std::vector<CodePathNode*> CodePathNode::GetTerminalNodes()
	{
		if (!left && !right)
			return { this };

		std::vector<CodePathNode*> result;

		if (left)
		{
			result = left->GetTerminalNodes();

			if (right)
			{
				auto rightTerms = right->GetTerminalNodes();
				result.insert(result.end(), rightTerms.begin(), rightTerms.end());
			}
		}
		else { result = right->GetTerminalNodes(); }

		return result;
	}

	std::set<ASTStmt*> CodePathNode::GetTerminalStmts()
	{
		if (!left && !right)
			return { statement };

		std::set<ASTStmt*> result;

		if (left)
			result = left->GetTerminalStmts();

		if (right)
		{
			auto rightTerms = right->GetTerminalStmts();
			result.insert(rightTerms.begin(), rightTerms.end());
		}

		return result;
	}

	void PrintCodePath(CodePathNode* _root, int _space)
	{
		const int spaceLen = 10;

		if (!_root)
			return;

		// Increase distance between levels  
		_space += spaceLen;

		// Process right child first  
		PrintCodePath(_root->right, _space);

		// Print current node after space  
		// count  
		std::cout << std::endl;
		for (int i = spaceLen; i < _space; i++)
			std::cout << " ";

		std::cout << ASTStmtTypeToString(_root->statement->GetStmtType()) << "\n";

		// Process left child  
		PrintCodePath(_root->left, _space);
	}

	void CodePathNode::Print() { PrintCodePath(this, 0); }
#pragma endregion
}