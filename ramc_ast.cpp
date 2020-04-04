#include "pch.h"
#include "ramc_ast.h"
#include "ramc_environment.h"
#include "ramvm_parser.h"
#include "ramvm_instruction.h"
#include "ramvm_argument.h"

#pragma region RamVM Usings
using ramvm::Argument;
using ramvm::ValueArgument;
using ramvm::MemoryArgument;
using ramvm::RegisterArgument;
using ramvm::StackArgument;
using ramvm::StackArgType;
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
using ramvm::InstrMalloc;
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
			default: ASSERT_MSG(false, "ASTStmtTypeToString() - ASTStmtType not handled!");
		}
	}

	ASTNode::ASTNode(ASTNodeType _type, Position _pos)
	{
		type = _type;
		position = _pos;
		typeSysType = nullptr;
	}

	ASTNode::~ASTNode()
	{
		if (typeSysType)
			delete typeSysType;
	}

	TypeResult ASTNode::TypeCheck(Environment* _env)
	{
		TypeResult result = _TypeCheck(_env);

		if (result.IsSuccess())
			typeSysType = (*result.GetValue())->GetCopy();

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

	ASTNode* ASTProgram::GetCopy()
	{
		std::vector<ASTVarDecl*> copiedVarDecls;
		for (auto const& vd : varDecls)
			copiedVarDecls.push_back((ASTVarDecl*)vd->GetCopy());

		std::vector<ASTFuncDecl*> copiedFuncDecls;
		for (auto const& fd : varDecls)
			copiedFuncDecls.push_back((ASTFuncDecl*)fd->GetCopy());

		return new ASTProgram(fileName, varDecls, funcDecls);
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

		return TypeResult::GenSuccess(Type::UNIT());
	}

	InstructionSet ASTProgram::GenerateCode(Environment* _env)
	{
		InstructionSet instrs;

		//Add Top Level Variable Declarations
		for (auto const& varDecl : varDecls)
		{
			auto declInstrs = varDecl->GenerateCode(info);
			instrs.insert(instrs.end(), declInstrs.begin(), declInstrs.end());
		}

		//Forward declare and add Top Level Functions
		for (auto const& funcDecl : funcDecls)
			info.AddFuncDecl(funcDecl->GetLabel(), funcDecl->GetRegCount());

		//Add Main Function Call
		instrs.push_back(new InstrCall(ProgramInfo::MAIN_FUNC_LABEL, new ValueArgument(info.GetFuncRegCount(ProgramInfo::MAIN_FUNC_LABEL)), new ValueArgument(0)));

		//Halt so that functions don't get run without being called
		instrs.push_back(new InstrHalt());

		for (auto const& funcDecl : funcDecls)
		{
			auto declInstrs = funcDecl->GenerateCode(_env, info);
			instrs.insert(instrs.end(), declInstrs.begin(), declInstrs.end());
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

		Type* retType;
		if (_retTypes.size() == 0) { retType = Type::UNIT(); }
		else if (_retTypes.size() == 1) { retType = _retTypes[0]; }
		else { retType = new TupleType(_retTypes); }

		funcType = new FuncType(new TupleType(paramTypes), retType);
		body = _body;
	}

	ASTFuncDecl::~ASTFuncDecl()
	{
		//The types for the params and ret
		//are deleted with funcType

		delete funcType;
		delete body;
	}

	std::string ASTFuncDecl::ToString(int _indentLvl, std::string _prefix)
	{
		std::stringstream ss;

		ss << CreateIndent(_indentLvl) << _prefix << "Func Declaration:" << std::endl;
		ss << CreateIndent(_indentLvl + 1) << "Type: " << funcType->ToString(0) << std::endl;
		ss << CreateIndent(_indentLvl + 1) << "Body: " << std::endl << body->ToString(_indentLvl + 2) << std::endl;

		return ss.str();
	}

	ASTNode* ASTFuncDecl::GetCopy()
	{
		ParamList copiedParams;
		for (auto const& param : params)
			copiedParams.push_back({ param.name, param.type->GetCopy(), param.position });

		return new ASTFuncDecl(name, copiedParams, { funcType->GetRetType()->GetCopy() }, (ASTStmt*)body->GetCopy(), GetPosition());
	}

	TypeResult ASTFuncDecl::_TypeCheck(Environment* _env)
	{
		//Add Function to environment
		TypeResult declTypeRes = _env->AddFunction(name, funcType, label, GetPosition());
		if (!declTypeRes.IsSuccess())
			return declTypeRes;

		Environment subEnv(_env, false); //Local function environment

		//Type check parameters in local environment
		for (auto const& it : params)
		{
			auto source = RegisterArgument::CreateRegular(subEnv.GetNextRegIdx());
			TypeResult paramTypeRes = subEnv.AddVariable(it.name, it.type, source, it.position);
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
		return TypeResult::GenSuccess(Type::UNIT());
	}

	InstructionSet ASTFuncDecl::GenerateCode(Environment* _env, ProgramInfo& _progInfo)
	{
		InstructionSet instrs;

		//Put the values of the arguments that were pushed onto the stack by the
		//caller into the initial registers which are counted in the register count
		int paramsTotalByteSize = 0;

		for (int i = 0; i < (int)params.size(); i++)
		{
			int byteSize = params[i].type->GetByteSize();
			auto offsetArg = new StackArgument(StackArgType::SP_OFFSETED, -paramsTotalByteSize - byteSize + 1);
			auto destReg = RegisterArgument::CreateRegular(i);

			instrs.push_back(new InstrMove(DataType::INT, offsetArg, destReg));
			paramsTotalByteSize += byteSize;
		}

		//Add body instructions
		InstructionSet bodyInstrs = body->GenerateCode(_progInfo);
		instrs.insert(instrs.end(), bodyInstrs.begin(), bodyInstrs.end());

		//Add a return to the end if it is a procedure and does not have one
		//since they can optionally have returns. This is to ensure that the
		//the procedures always return
		if (funcType->IsProcedure() && instrs.back()->GetType() != InstructionType::RETURN)
			instrs.push_back(new InstrReturn(new ValueArgument(0)));

		//Add the function declaration
		_progInfo.SetFuncDeclStart(Environment::GenFuncLabel(name, funcType->GetParamsType()), instrs.front());

		return instrs;
	}
#pragma endregion

#pragma region VarDecl
	ASTVarDecl::ASTVarDecl(ASTIdentifier* _id, Type* _restraint, ASTExpr* _expr, bool _isGlobal, Position _pos)
		: ASTStmt(ASTStmtType::VARDECL, _pos)
	{
		id = _id;
		expr = _expr;
		isUnderscore = false;
		restraint = _restraint;
		isGlobal = _isGlobal;
	}

	ASTVarDecl::ASTVarDecl(ASTIdentifier* _id, ASTExpr* _expr, bool _isGlobal, Position _pos)
		: ASTVarDecl(_id, nullptr, _expr, _isGlobal, _pos) { }

	ASTVarDecl::ASTVarDecl(ASTExpr* _expr, Position _pos)
		: ASTVarDecl(nullptr, nullptr, _expr, false, _pos)
	{
		isUnderscore = true;
	}

	ASTVarDecl::~ASTVarDecl()
	{
		delete id;
		delete expr;

		if (restraint)
			delete restraint;
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
		else if (isUnderscore) { return TypeResult::GenSuccess(Type::UNIT()); }
		else if (restraint && !Type::Matches(*exprTypeRes.GetValue(), restraint)) { return TypeResult::GenExpectation(restraint->ToString(0), (*exprTypeRes.GetValue())->ToString(0), GetPosition()); }
		else
		{
			int valByteLen = (*exprTypeRes.GetValue())->GetByteSize();
			Argument* source = nullptr;

			if (isGlobal) { source = new StackArgument(StackArgType::ABSOLUTE, _env->GetNextGlobalStackPos(valByteLen)); }
			else { source = RegisterArgument::CreateRegular(_env->GetNextRegIdx()); }

			TypeResult idRes = _env->AddVariable(id->GetID(), *exprTypeRes.GetValue(), source, GetPosition());
			if (!idRes.IsSuccess())
				return idRes;

			IGNORE(id->TypeCheck(_env)); //Set the info for the id for code generation later
			return TypeResult::GenSuccess(Type::UNIT());
		}
	}

	InstructionSet ASTVarDecl::GenerateCode(ProgramInfo& _progInfo)
	{
		if (isUnderscore)
		{
			InstructionSet instrs = expr->GenerateCode(std::shared_ptr<Argument>(StackArgument::GenStackTop()), _progInfo); //Push value onto the stack
			instrs.push_back(new InstrPop(DataType::BYTE, new ValueArgument(expr->GetTypeSysType()->GetByteSize()))); //Pop it off to trash it
			return instrs;
		}
		else
		{
			if (isGlobal) { return expr->GenerateCode(std::shared_ptr<Argument>(StackArgument::GenStackTop()), _progInfo); } //Global declarations go on the top of the stack
			else { return expr->GenerateCode(std::shared_ptr<Argument>(id->GetSource()->GetCopy()), _progInfo); } //Put value in the id's source
		}
	}

	ASTNode* ASTVarDecl::GetCopy() { return new ASTVarDecl((ASTIdentifier*)id->GetCopy(), restraint ? restraint->GetCopy() : nullptr, expr ? (ASTExpr*)expr->GetCopy() : nullptr, isGlobal, GetPosition()); }
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
			case AssignmentType::ADD_EQ: expr = new ASTBinopExpr((ASTIdentifier*)id->GetCopy(), _expr, BinopType::ADD); break;
			case AssignmentType::SUB_EQ: expr = new ASTBinopExpr((ASTIdentifier*)id->GetCopy(), _expr, BinopType::SUB); break;
			case AssignmentType::MUL_EQ: expr = new ASTBinopExpr((ASTIdentifier*)id->GetCopy(), _expr, BinopType::MUL); break;
			case AssignmentType::DIV_EQ: expr = new ASTBinopExpr((ASTIdentifier*)id->GetCopy(), _expr, BinopType::DIV); break;
			case AssignmentType::MOD_EQ: expr = new ASTBinopExpr((ASTIdentifier*)id->GetCopy(), _expr, BinopType::MOD); break;
			case AssignmentType::POW_EQ: expr = new ASTBinopExpr((ASTIdentifier*)id->GetCopy(), _expr, BinopType::POW); break;
			case AssignmentType::BIN_AND_EQ: expr = new ASTBinopExpr((ASTIdentifier*)id->GetCopy(), _expr, BinopType::BIN_AND); break;
			case AssignmentType::BIN_OR_EQ: expr = new ASTBinopExpr((ASTIdentifier*)id->GetCopy(), _expr, BinopType::BIN_OR); break;
			case AssignmentType::BIN_XOR_EQ: expr = new ASTBinopExpr((ASTIdentifier*)id->GetCopy(), _expr, BinopType::BIN_XOR); break;
			case AssignmentType::LSHIFT_EQ: expr = new ASTBinopExpr((ASTIdentifier*)id->GetCopy(), _expr, BinopType::LSHIFT); break;
			case AssignmentType::RSHIFT_EQ: expr = new ASTBinopExpr((ASTIdentifier*)id->GetCopy(), _expr, BinopType::RSHIFT); break;
			default: ASSERT_MSG(false, "ASTAssignment() - AssignmentType not handled!");
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

		if (Type::Matches(*idTypeRes.GetValue(), *exprTypeRes.GetValue())) { return TypeResult::GenSuccess(Type::UNIT()); }
		else { return TypeResult::GenMismatch("Cannot assign " + (*exprTypeRes.GetValue())->ToString(false) + " to " + (*idTypeRes.GetValue())->ToString(false), GetPosition()); }
	}

	ASTNode* ASTAssignment::GetCopy() { return new ASTAssignment((ASTIdentifier*)id->GetCopy(), (ASTExpr*)expr->GetCopy(), assignType); }
	InstructionSet ASTAssignment::GenerateCode(ProgramInfo& _progInfo) { return expr->GenerateCode(std::shared_ptr<Argument>(id->GetSource()->GetCopy()), _progInfo); }
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

		Type* leftType = *leftTypeRes.GetValue(), * rightType = *rightTypeRes.GetValue();

		switch (ConcatTriple((byte)op, (byte)leftType->GetType(), (byte)rightType->GetType()))
		{
#pragma region ADD
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG());

			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG());

			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::INT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::INT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::INT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::INT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::INT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::INT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG());

			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE());

			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE());

			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::LONG());
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::LONG());
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::LONG());
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::LONG, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::LONG, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::ADD, (byte)TypeSystemType::LONG, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG());
#pragma endregion

#pragma region SUB
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG());

			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG());

			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::INT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::INT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::INT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::INT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::INT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::INT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG());

			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE());

			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE());

			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::LONG());
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::LONG());
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::LONG());
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::LONG, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::LONG, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::SUB, (byte)TypeSystemType::LONG, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG());
#pragma endregion

#pragma region MUL
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG());

			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG());

			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::INT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::INT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::INT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::INT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::INT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::INT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG());

			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE());

			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE());

			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::LONG());
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::LONG());
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::LONG());
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::LONG, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::LONG, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::MUL, (byte)TypeSystemType::LONG, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG());
#pragma endregion

#pragma region DIV
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG());

			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG());

			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::INT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::INT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::INT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::INT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::INT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::INT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG());

			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE());

			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE());

			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::LONG());
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::LONG());
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::LONG());
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::LONG, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::LONG, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::DIV, (byte)TypeSystemType::LONG, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG());
#pragma endregion

#pragma region MOD
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG());

			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG());

			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::INT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::INT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::INT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::INT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::INT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::INT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG());

			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE());

			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE());

			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::LONG());
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::LONG());
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::LONG());
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::LONG, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::LONG, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::MOD, (byte)TypeSystemType::LONG, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG());
#pragma endregion

#pragma region POW
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG());

			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG());

			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::INT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::INT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::INT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::INT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::INT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::INT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG());

			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE());

			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE());

			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::LONG());
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::LONG());
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::LONG());
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::LONG, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::LONG, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::POW, (byte)TypeSystemType::LONG, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG());
#pragma endregion

#pragma region BIN_AND
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG());

			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG());

			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::INT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::INT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::INT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::INT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::INT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::INT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG());

			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE());

			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE());

			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::LONG());
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::LONG());
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::LONG());
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::LONG, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::LONG, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::BIN_AND, (byte)TypeSystemType::LONG, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG());
#pragma endregion

#pragma region BIN_OR
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG());

			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG());

			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::INT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::INT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::INT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::INT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::INT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::INT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG());

			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE());

			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE());

			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::LONG());
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::LONG());
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::LONG());
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::LONG, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::LONG, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::BIN_OR, (byte)TypeSystemType::LONG, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG());
#pragma endregion

#pragma region BIN_XOR
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG());

			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG());

			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::INT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::INT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::INT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::INT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::INT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::INT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG());

			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE());

			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE());

			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::LONG());
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::LONG());
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::LONG());
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::LONG, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::LONG, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::BIN_XOR, (byte)TypeSystemType::LONG, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG());
#pragma endregion

#pragma region LSHIFT
			case ConcatTriple((byte)BinopType::LSHIFT, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatTriple((byte)BinopType::LSHIFT, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatTriple((byte)BinopType::LSHIFT, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatTriple((byte)BinopType::LSHIFT, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BYTE());

			case ConcatTriple((byte)BinopType::LSHIFT, (byte)TypeSystemType::INT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::LSHIFT, (byte)TypeSystemType::INT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::LSHIFT, (byte)TypeSystemType::INT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::LSHIFT, (byte)TypeSystemType::INT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::INT());

			case ConcatTriple((byte)BinopType::LSHIFT, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::LSHIFT, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::LSHIFT, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::LSHIFT, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE());

			case ConcatTriple((byte)BinopType::LSHIFT, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::LSHIFT, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::LSHIFT, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::LSHIFT, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE());

			case ConcatTriple((byte)BinopType::LSHIFT, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::LONG());
			case ConcatTriple((byte)BinopType::LSHIFT, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::LONG());
			case ConcatTriple((byte)BinopType::LSHIFT, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::LONG());
			case ConcatTriple((byte)BinopType::LSHIFT, (byte)TypeSystemType::LONG, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG());
#pragma endregion

#pragma region RSHIFT
			case ConcatTriple((byte)BinopType::RSHIFT, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatTriple((byte)BinopType::RSHIFT, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatTriple((byte)BinopType::RSHIFT, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatTriple((byte)BinopType::RSHIFT, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BYTE());

			case ConcatTriple((byte)BinopType::RSHIFT, (byte)TypeSystemType::INT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::RSHIFT, (byte)TypeSystemType::INT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::RSHIFT, (byte)TypeSystemType::INT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT());
			case ConcatTriple((byte)BinopType::RSHIFT, (byte)TypeSystemType::INT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::INT());

			case ConcatTriple((byte)BinopType::RSHIFT, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::RSHIFT, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::RSHIFT, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatTriple((byte)BinopType::RSHIFT, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE());

			case ConcatTriple((byte)BinopType::RSHIFT, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::RSHIFT, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::RSHIFT, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatTriple((byte)BinopType::RSHIFT, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::DOUBLE());

			case ConcatTriple((byte)BinopType::RSHIFT, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::LONG());
			case ConcatTriple((byte)BinopType::RSHIFT, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::LONG());
			case ConcatTriple((byte)BinopType::RSHIFT, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::LONG());
			case ConcatTriple((byte)BinopType::RSHIFT, (byte)TypeSystemType::LONG, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG());
#pragma endregion

#pragma region LT
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());

			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());

			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::INT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::INT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::INT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::INT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::INT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::INT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());

			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());

			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());

			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::LONG, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::LONG, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT, (byte)TypeSystemType::LONG, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());
#pragma endregion

#pragma region GT
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());

			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());

			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::INT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::INT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::INT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::INT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::INT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::INT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());

			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());

			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());

			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::LONG, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::LONG, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT, (byte)TypeSystemType::LONG, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());
#pragma endregion

#pragma region LT_EQ
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());

			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());

			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());

			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());

			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());

			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LT_EQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());
#pragma endregion

#pragma region GT_EQ
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());

			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());

			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());

			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());

			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());

			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::GT_EQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());
#pragma endregion

#pragma region EQ_EQ
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());

			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());

			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());

			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());

			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());

			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::EQ_EQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());
#pragma endregion

#pragma region NEQ
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());

			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());

			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::INT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());

			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());

			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());

			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::NEQ, (byte)TypeSystemType::LONG, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());
#pragma endregion

#pragma region LOG_AND
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());

			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());

			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::INT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::INT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::INT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::INT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::INT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::INT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());

			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());

			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());

			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::LONG, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::LONG, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_AND, (byte)TypeSystemType::LONG, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());
#pragma endregion

#pragma region LOG_OR
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::BYTE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());

			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::BOOL, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());

			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::INT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::INT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::INT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::INT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::INT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::INT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());

			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::FLOAT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());

			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::DOUBLE, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());

			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::LONG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::LONG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::LONG, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::LONG, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatTriple((byte)BinopType::LOG_OR, (byte)TypeSystemType::LONG, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());
#pragma endregion
			default: return TypeResult::GenMismatch("Cannot perform operation on " + leftType->ToString(false) + " and " + rightType->ToString(false), GetPosition());
		}
	}

	InstructionSet ASTBinopExpr::GenerateCode(std::shared_ptr<Argument> _dest, ProgramInfo& _progInfo)
	{
		InstructionSet instrs;

		//Insert expression instructions
		auto leftInstrs = left->GenerateCode(std::shared_ptr<Argument>(StackArgument::GenStackTop()), _progInfo);
		auto rightInstrs = right->GenerateCode(std::shared_ptr<Argument>(StackArgument::GenStackTop()), _progInfo);
		instrs.insert(instrs.end(), leftInstrs.begin(), leftInstrs.end());
		instrs.insert(instrs.end(), rightInstrs.begin(), rightInstrs.end());

		//Get expression types
		auto src1DataType = TypeSysTypeToDataType(left->GetTypeSysType()->GetType());
		auto src2DataType = TypeSysTypeToDataType(right->GetTypeSysType()->GetType());
		auto destDataType = TypeSysTypeToDataType(this->GetTypeSysType()->GetType());
		DataTypeTriple opDataTypes = { src1DataType, src2DataType, destDataType };

		//Get expression byte sizes
		int src1ByteSize = left->GetTypeSysType()->GetByteSize();
		int src2ByteSize = right->GetTypeSysType()->GetByteSize();
		int destByteSize = this->GetTypeSysType()->GetByteSize();
		int sizeDiff = destByteSize - src1ByteSize - src2ByteSize;

		//Extend the stack to make room for the result if it takes up more space than the operands
		if (sizeDiff > 0)
			instrs.push_back(new InstrPush(DataType::BYTE, { new ValueArgument(sizeDiff) }));

		auto src2 = new StackArgument(StackArgType::SP_OFFSETED, -1 * src2ByteSize + 1);
		auto src1 = new StackArgument(StackArgType::SP_OFFSETED, src2->GetValue() - src1ByteSize);
		auto stackDest = new StackArgument(StackArgType::SP_OFFSETED, src1->GetValue());

		switch (op)
		{
			case BinopType::ADD: instrs.push_back(new InstrBinop(Binop::ADD, opDataTypes, src1, src2, stackDest)); break;
			case BinopType::SUB: instrs.push_back(new InstrBinop(Binop::SUB, opDataTypes, src1, src2, stackDest)); break;
			case BinopType::MUL: instrs.push_back(new InstrBinop(Binop::MUL, opDataTypes, src1, src2, stackDest)); break;
			case BinopType::DIV: instrs.push_back(new InstrBinop(Binop::DIV, opDataTypes, src1, src2, stackDest)); break;
			case BinopType::MOD: instrs.push_back(new InstrBinop(Binop::MOD, opDataTypes, src1, src2, stackDest)); break;
			case BinopType::POW: instrs.push_back(new InstrBinop(Binop::POW, opDataTypes, src1, src2, stackDest)); break;
			case BinopType::BIN_AND: instrs.push_back(new InstrBinop(Binop::AND, opDataTypes, src1, src2, stackDest)); break;
			case BinopType::BIN_OR: instrs.push_back(new InstrBinop(Binop::OR, opDataTypes, src1, src2, stackDest)); break;
			case BinopType::BIN_XOR: instrs.push_back(new InstrBinop(Binop::XOR, opDataTypes, src1, src2, stackDest)); break;
			case BinopType::LSHIFT: instrs.push_back(new InstrBinop(Binop::LSHIFT, opDataTypes, src1, src2, stackDest)); break;
			case BinopType::RSHIFT: instrs.push_back(new InstrBinop(Binop::RSHIFT, opDataTypes, src1, src2, stackDest)); break;
			case BinopType::LT: instrs.push_back(new InstrBinop(Binop::LT, opDataTypes, src1, src2, stackDest)); break;
			case BinopType::GT: instrs.push_back(new InstrBinop(Binop::GT, opDataTypes, src1, src2, stackDest)); break;
			case BinopType::LT_EQ: instrs.push_back(new InstrBinop(Binop::LTEQ, opDataTypes, src1, src2, stackDest)); break;
			case BinopType::GT_EQ: instrs.push_back(new InstrBinop(Binop::GTEQ, opDataTypes, src1, src2, stackDest)); break;
			case BinopType::EQ_EQ: instrs.push_back(new InstrBinop(Binop::EQ, opDataTypes, src1, src2, stackDest)); break;
			case BinopType::NEQ: instrs.push_back(new InstrBinop(Binop::NEQ, opDataTypes, src1, src2, stackDest)); break;
			case BinopType::LOG_AND: instrs.push_back(new InstrBinop(Binop::AND, opDataTypes, src1, src2, stackDest)); break;
			case BinopType::LOG_OR: instrs.push_back(new InstrBinop(Binop::OR, opDataTypes, src1, src2, stackDest)); break;
			default: ASSERT_MSG(false, "BinopExpr::GenerateCode - BinopType not handled!");
		}

		//Create Pop to remove the bytes past the result
		if (sizeDiff < 0)
			instrs.push_back(new InstrPop(DataType::BYTE, new ValueArgument(-sizeDiff)));

		//Create Move to move result to destination
		if (!_dest->IsStackTop())
		{
			instrs.push_back(new InstrMove(destDataType, new StackArgument(StackArgType::SP_OFFSETED, -1 * destByteSize + 1), _dest->GetCopy()));
			instrs.push_back(new InstrPop(destDataType, new ValueArgument(1)));
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

		Type* exprType = *typeRes.GetValue();

		switch (ConcatDouble((byte)op, (byte)exprType->GetType()))
		{
			case ConcatDouble((byte)UnopType::NEG, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatDouble((byte)UnopType::NEG, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatDouble((byte)UnopType::NEG, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT());
			case ConcatDouble((byte)UnopType::NEG, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatDouble((byte)UnopType::NEG, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatDouble((byte)UnopType::NEG, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG());

			case ConcatDouble((byte)UnopType::LOG_NOT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatDouble((byte)UnopType::LOG_NOT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatDouble((byte)UnopType::LOG_NOT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatDouble((byte)UnopType::LOG_NOT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatDouble((byte)UnopType::LOG_NOT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::BOOL());
			case ConcatDouble((byte)UnopType::LOG_NOT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::BOOL());

			case ConcatDouble((byte)UnopType::BIN_NOT, (byte)TypeSystemType::BYTE): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatDouble((byte)UnopType::BIN_NOT, (byte)TypeSystemType::BOOL): return TypeResult::GenSuccess(Type::BYTE());
			case ConcatDouble((byte)UnopType::BIN_NOT, (byte)TypeSystemType::INT): return TypeResult::GenSuccess(Type::INT());
			case ConcatDouble((byte)UnopType::BIN_NOT, (byte)TypeSystemType::FLOAT): return TypeResult::GenSuccess(Type::FLOAT());
			case ConcatDouble((byte)UnopType::BIN_NOT, (byte)TypeSystemType::DOUBLE): return TypeResult::GenSuccess(Type::DOUBLE());
			case ConcatDouble((byte)UnopType::BIN_NOT, (byte)TypeSystemType::LONG): return TypeResult::GenSuccess(Type::LONG());

			default: return TypeResult::GenMismatch("Cannot perform operation on " + exprType->ToString(false), GetPosition());
		}
	}

	InstructionSet ASTUnopExpr::GenerateCode(std::shared_ptr<Argument> _dest, ProgramInfo& _progInfo)
	{
		InstructionSet instrs = expr->GenerateCode(std::shared_ptr<Argument>(StackArgument::GenStackTop()), _progInfo);

		auto srcDataType = TypeSysTypeToDataType(expr->GetTypeSysType()->GetType());
		auto destDataType = TypeSysTypeToDataType(this->GetTypeSysType()->GetType());
		DataTypeDouble opDataTypes = { srcDataType , destDataType };

		int srcByteSize = expr->GetTypeSysType()->GetByteSize();
		int destByteSize = this->GetTypeSysType()->GetByteSize();
		int sizeDiff = destByteSize - srcByteSize;

		//Extend the stack to make room for the result if it takes up more space than the operand
		if (sizeDiff > 0)
			instrs.push_back(new InstrPush(DataType::BYTE, { new ValueArgument(sizeDiff) }));

		auto src = new StackArgument(StackArgType::SP_OFFSETED, -1 * srcByteSize + 1);
		auto stackDest = new StackArgument(StackArgType::SP_OFFSETED, src->GetValue() - fmax(0, sizeDiff));

		switch (op)
		{
			case UnopType::LOG_NOT: instrs.push_back(new InstrUnop(Unop::NOT, opDataTypes, src, stackDest)); break;
			case UnopType::NEG: instrs.push_back(new InstrUnop(Unop::NEG, opDataTypes, src, stackDest)); break;
			case UnopType::BIN_NOT: instrs.push_back(new InstrUnop(Unop::NOT, opDataTypes, src, stackDest)); break;
			default: throw std::runtime_error("UnopExpr::GenerateCode - UnopType not handled!");
		}

		//Create Pop to remove the bytes past the result if any
		if (sizeDiff < 0)
			instrs.push_back(new InstrPop(DataType::BYTE, new ValueArgument(-sizeDiff)));

		//Create Move to move result to destination
		if (!_dest->IsStackTop())
		{
			instrs.push_back(new InstrMove(destDataType, new StackArgument(StackArgType::SP_OFFSETED, -1 * destByteSize + 1), _dest->GetCopy()));
			instrs.push_back(new InstrPop(destDataType, new ValueArgument(1)));
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
		TypeResult result = _env->GetVariableType(id, GetPosition());

		if (result.IsSuccess())
			source = _env->GetVarSource(id)->GetCopy();

		return result;
	}

	InstructionSet ASTIdentifier::GenerateCode(std::shared_ptr<Argument> _dest, ProgramInfo& _progInfo) { return { new InstrMove(TypeSysTypeToDataType(this->GetTypeSysType()->GetType()), GetSource()->GetCopy(), _dest->GetCopy()) }; }
#pragma endregion

#pragma region Literal
	InstructionSet ASTIntLit::GenerateCode(std::shared_ptr<Argument> _dest, ProgramInfo& _progInfo) { return { new InstrMove(DataType::INT, new ValueArgument(GetValue()),  _dest->GetCopy()) }; }
	ASTNode* ASTIntLit::GetCopy() { return new ASTIntLit(GetValue(), GetPosition()); }

	InstructionSet ASTFloatLit::GenerateCode(std::shared_ptr<Argument> _dest, ProgramInfo& _progInfo) { return { new InstrMove(DataType::FLOAT, new ValueArgument(GetValue()),  _dest->GetCopy()) }; }
	ASTNode* ASTFloatLit::GetCopy() { return new ASTFloatLit(GetValue(), GetPosition()); }

	InstructionSet ASTStringLit::GenerateCode(std::shared_ptr<Argument> _dest, ProgramInfo& _progInfo) { return { new InstrMove(DataType::INT, new ValueArgument(0),  _dest->GetCopy()) }; }
	ASTNode* ASTStringLit::GetCopy() { return new ASTStringLit(GetValue(), GetPosition()); }

	InstructionSet ASTBoolLit::GenerateCode(std::shared_ptr<Argument> _dest, ProgramInfo& _progInfo) { return { new InstrMove(DataType::BYTE, new ValueArgument((byte)(GetValue() ? 1 : 0)),  _dest->GetCopy()) }; }
	ASTNode* ASTBoolLit::GetCopy() { return new ASTBoolLit(GetValue(), GetPosition()); }

	InstructionSet ASTByteLit::GenerateCode(std::shared_ptr<Argument> _dest, ProgramInfo& _progInfo) { return { new InstrMove(DataType::BYTE, new ValueArgument(GetValue()),  _dest->GetCopy()) }; }
	ASTNode* ASTByteLit::GetCopy() { return new ASTByteLit(GetValue(), GetPosition()); }

	InstructionSet ASTDoubleLit::GenerateCode(std::shared_ptr<Argument> _dest, ProgramInfo& _progInfo) { return { new InstrMove(DataType::DOUBLE, new ValueArgument(GetValue()),  _dest->GetCopy()) }; }
	ASTNode* ASTDoubleLit::GetCopy() { return new ASTDoubleLit(GetValue(), GetPosition()); }

	InstructionSet ASTLongLit::GenerateCode(std::shared_ptr<Argument> _dest, ProgramInfo& _progInfo) { return { new InstrMove(DataType::LONG, new ValueArgument(GetValue()),  _dest->GetCopy()) }; }
	ASTNode* ASTLongLit::GetCopy() { return new ASTLongLit(GetValue(), GetPosition()); }
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

	ASTNode* ASTFuncCallExpr::GetCopy()
	{
		ExprList copiedArgs;

		for (auto const& arg : args)
			copiedArgs.push_back((ASTExpr*)arg->GetCopy());

		return new ASTFuncCallExpr(funcName, copiedArgs, GetPosition());
	}

	TypeResult ASTFuncCallExpr::_TypeCheck(Environment* _env)
	{
		TypeList argTypes;
		for (auto const& arg : args)
		{
			TypeResult argTypeRes = arg->TypeCheck(_env);
			if (!argTypeRes.IsSuccess())
				return argTypeRes;

			argTypes.push_back((*argTypeRes.GetValue())->GetCopy());
		}

		Type* paramsType = new TupleType(argTypes);
		TypeResult funcRetTypeRes = _env->GetFunctionRetType(funcName, paramsType, GetPosition());
		if (!funcRetTypeRes.IsSuccess())
			return funcRetTypeRes;

		funcLabel = Environment::GenFuncLabel(funcName, paramsType);

		delete paramsType;
		return funcRetTypeRes;
	}

	InstructionSet ASTFuncCallExpr::GenerateCode(std::shared_ptr<Argument> _dest, ProgramInfo& _progInfo)
	{
		InstructionSet instrs;
		int argsByteLen = 0;

		//Push arguments onto stack in reverse order so first arg is on top
		//when the callee starts reading them
		for (int i = args.size() - 1; i >= 0; i--)
		{
			ASTExpr* arg = args[i];
			auto argInstrs = arg->GenerateCode(std::shared_ptr<Argument>(StackArgument::GenStackTop()), _progInfo);

			instrs.insert(instrs.end(), argInstrs.begin(), argInstrs.end());
			argsByteLen += arg->GetTypeSysType()->GetByteSize();
		}

		//Insert Call
		int funcRegCnt = _progInfo.GetFuncRegCount(funcLabel);
		instrs.push_back(new InstrCall(funcLabel, new ValueArgument(funcRegCnt), new ValueArgument(argsByteLen)));

		//Move value to destination
		if (!_dest->IsStackTop())
		{
			auto destDataType = TypeSysTypeToDataType(this->GetTypeSysType()->GetType());
			int retByteSize = this->GetTypeSysType()->GetByteSize();
			instrs.push_back(new InstrMove(destDataType, new StackArgument(StackArgType::SP_OFFSETED, -retByteSize + 1), _dest->GetCopy()));
			instrs.push_back(new InstrPop(DataType::BYTE, new ValueArgument(retByteSize)));
		}

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
		else if (!(*condTypeRes.GetValue())->Matches(Type::BOOL())) { return TypeResult::GenExpectation(Type::BOOL()->ToString(0), (*condTypeRes.GetValue())->ToString(0), condExpr->GetPosition()); }

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

		return TypeResult::GenSuccess(Type::UNIT());
	}

	InstructionSet ASTIfStmt::GenerateCode(ProgramInfo& _progInfo)
	{
		auto ifLabels = _progInfo.GenIfLabels(elseStmt); //Gen Labels
		int condByteSize = condExpr->GetTypeSysType()->GetByteSize();

		InstructionSet instrs;
		InstructionSet condInstrs = condExpr->GenerateCode(std::shared_ptr<Argument>(StackArgument::GenStackTop()), _progInfo);
		InstructionSet thenInstrs = thenStmt->GenerateCode(_progInfo);

		instrs.insert(instrs.end(), condInstrs.begin(), condInstrs.end()); //Insert Condition Instructions
		_progInfo.SetLabelInstr(ifLabels.begin, condInstrs.front()); //Set If Begin

		if (elseStmt)
		{
			instrs.push_back(new InstrCJump(ifLabels.thenClause, StackArgument::GenStackCur(), false)); //Jump to then clause if cond == true

			//If cond == false
			instrs.push_back(new InstrPop(DataType::BYTE, new ValueArgument(condByteSize))); //Pop off condition

			InstructionSet elseInstrs = elseStmt->GenerateCode(_progInfo);
			_progInfo.SetLabelInstr(ifLabels.elseClause, elseInstrs.front()); //Set Else Begin
			instrs.insert(instrs.end(), elseInstrs.begin(), elseInstrs.end()); //Insert Else Instructions
			instrs.push_back(new InstrJump(ifLabels.elseClause)); //Jump to If End
		}
		else { instrs.push_back(new InstrCJump(ifLabels.end, StackArgument::GenStackCur(), true)); } //Jump to end if cond == false

		//If cond == true
		instrs.push_back(new InstrPop(DataType::BYTE, new ValueArgument(condByteSize))); //Pop off condition
		instrs.insert(instrs.end(), thenInstrs.begin(), thenInstrs.end()); //Insert Then Instructions
		_progInfo.SetLabelInstr(ifLabels.thenClause, thenInstrs.front()); //Set Then Begin

		//To Set the End Label to the right spot
		if (elseStmt) { instrs.push_back(new InstrNoOp()); }
		else { instrs.push_back(new InstrPop(DataType::BYTE, new ValueArgument(condByteSize))); } //Pop off condition

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
		else if (!(*condTypeRes.GetValue())->Matches(Type::BOOL())) { return TypeResult::GenExpectation(Type::BOOL()->ToString(0), (*condTypeRes.GetValue())->ToString(0), condExpr->GetPosition()); }

		TypeResult thenTypeRes = thenExpr->TypeCheck(_env);
		if (!thenTypeRes.IsSuccess())
			return thenTypeRes;

		TypeResult elseTypeRes = elseExpr->TypeCheck(_env);
		if (!elseTypeRes.IsSuccess())
			return elseTypeRes;

		if (Type::Matches(*thenTypeRes.GetValue(), *elseTypeRes.GetValue())) { return thenTypeRes; }
		else { return TypeResult::GenExpectation((*thenTypeRes.GetValue())->ToString(0), (*elseTypeRes.GetValue())->ToString(false), elseExpr->GetPosition()); }
	}

	InstructionSet ASTIfExpr::GenerateCode(std::shared_ptr<Argument> _dest, ProgramInfo& _progInfo)
	{
		auto ifLabels = _progInfo.GenIfLabels(true); //Gen Labels
		int condByteSize = condExpr->GetTypeSysType()->GetByteSize();
		int resultByteSize = this->GetTypeSysType()->GetByteSize();

		InstructionSet instrs = condExpr->GenerateCode(std::shared_ptr<Argument>(StackArgument::GenStackTop()), _progInfo);
		InstructionSet thenInstrs = thenExpr->GenerateCode(std::shared_ptr<Argument>(StackArgument::GenStackTop()), _progInfo);
		InstructionSet elseInstrs = elseExpr->GenerateCode(std::shared_ptr<Argument>(StackArgument::GenStackTop()), _progInfo);

		instrs.push_back(new InstrCJump(ifLabels.thenClause, StackArgument::GenStackCur(), false)); //Jump to then clause if cond == true

		//If cond == false
		instrs.push_back(new InstrPop(DataType::BYTE, new ValueArgument(condByteSize))); //Pop off condition
		instrs.insert(instrs.end(), elseInstrs.begin(), elseInstrs.end()); //Insert Else Instructions
		instrs.push_back(new InstrJump(ifLabels.end)); //Jump to If End

		//If cond == true
		instrs.push_back(new InstrPop(DataType::BYTE, new ValueArgument(condByteSize))); //Pop off condition
		instrs.insert(instrs.end(), thenInstrs.begin(), thenInstrs.end()); //Insert Then Instructions
		instrs.push_back(new InstrNoOp()); //Used to set the end label to the right spot

		//Set Labelled Instructions
		_progInfo.SetLabelInstr(ifLabels.begin, instrs.front()); //Set If Begin
		_progInfo.SetLabelInstr(ifLabels.thenClause, thenInstrs.front()); //Set Then Begin
		_progInfo.SetLabelInstr(ifLabels.elseClause, elseInstrs.front()); //Set Else Begin
		_progInfo.SetLabelInstr(ifLabels.end, instrs.back()); //Set If End

		//Move value to destination
		if (!_dest->IsStackTop())
		{
			auto destDataType = TypeSysTypeToDataType(this->GetTypeSysType()->GetType());
			instrs.push_back(new InstrMove(destDataType, new StackArgument(StackArgType::SP_OFFSETED, -resultByteSize + 1), _dest->GetCopy()));
			instrs.push_back(new InstrPop(destDataType, new ValueArgument(1)));
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
		else if (!(*condTypeRes.GetValue())->Matches(Type::BOOL())) { return TypeResult::GenExpectation(Type::BOOL()->ToString(0), (*condTypeRes.GetValue())->ToString(0), condExpr->GetPosition()); }

		Environment subEnv(_env, true);
		return body->TypeCheck(&subEnv);
	}

	InstructionSet ASTWhileStmt::GenerateCode(ProgramInfo& _progInfo)
	{
		auto loopLabels = _progInfo.GenLoopLabels(); //Gen Labels

		InstructionSet instrs;
		InstructionSet condInstrs = condExpr->GenerateCode(std::shared_ptr<Argument>(StackArgument::GenStackTop()), _progInfo);
		InstructionSet bodyInstrs = body->GenerateCode(_progInfo);

		_progInfo.PushCurLoopLabels(&loopLabels); //Push Loop Section
		instrs.insert(instrs.end(), condInstrs.begin(), condInstrs.end()); //Insert condition instructions
		_progInfo.SetLabelInstr(loopLabels.begin, instrs.front()); //Set Loop Begin
		_progInfo.SetLabelInstr(loopLabels.begin, instrs.front()); //Set Loop Reset
		instrs.push_back(new InstrCJump(loopLabels.pop, StackArgument::GenStackCur(), true)); //JUMPF to Loop Pop
		instrs.push_back(new InstrPop(DataType::BYTE, new ValueArgument(BYTE_SIZE))); //Pop off condition if doing body
		instrs.insert(instrs.end(), bodyInstrs.begin(), bodyInstrs.end()); //Insert body instructions
		instrs.push_back(new InstrJump(loopLabels.reset)); //Jump back to condition instructions
		instrs.push_back(new InstrPop(DataType::BYTE, new ValueArgument(BYTE_SIZE))); //Pop off condition if not doing body
		_progInfo.SetLabelInstr(loopLabels.pop, instrs.back()); //Set Loop Pop
		instrs.push_back(new InstrNoOp());
		_progInfo.SetLabelInstr(loopLabels.end, instrs.back()); //Set Loop End
		_progInfo.PopCurLoopLabels(); //Pop Loop Sectio

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
		else if (!(*condTypeRes.GetValue())->Matches(Type::BOOL())) { return TypeResult::GenExpectation(Type::BOOL()->ToString(0), (*condTypeRes.GetValue())->ToString(0), condExpr->GetPosition()); }

		TypeResult bodyTypeRes = body->TypeCheck(&subEnv);
		if (!bodyTypeRes.IsSuccess())
			return bodyTypeRes;

		return thenStmt->TypeCheck(&subEnv);
	}

	InstructionSet ASTForStmt::GenerateCode(ProgramInfo& _progInfo)
	{
		auto loopLabels = _progInfo.GenLoopLabels(); //Gen Labels

		InstructionSet instrs;
		InstructionSet initInstrs = initStmt->GenerateCode(_progInfo);
		InstructionSet condInstrs = condExpr->GenerateCode(std::shared_ptr<Argument>(StackArgument::GenStackTop()), _progInfo);
		InstructionSet bodyInstrs = body->GenerateCode(_progInfo);
		InstructionSet thenInstrs = thenStmt->GenerateCode(_progInfo);

		_progInfo.PushCurLoopLabels(&loopLabels); //Begin Loop Section
		instrs.insert(instrs.end(), initInstrs.begin(), initInstrs.end()); //Insert init instructions
		instrs.insert(instrs.end(), condInstrs.begin(), condInstrs.end()); //Insert condition instructions
		_progInfo.SetLabelInstr(loopLabels.begin, condInstrs.front()); //Set Loop Begin
		instrs.push_back(new InstrCJump(loopLabels.pop, StackArgument::GenStackCur(), true)); //JUMPF to Loop End
		instrs.push_back(new InstrPop(DataType::BYTE, new ValueArgument(BYTE_SIZE))); //Pop off condition if doing body
		instrs.insert(instrs.end(), bodyInstrs.begin(), bodyInstrs.end()); //Insert body instructions
		instrs.insert(instrs.end(), thenInstrs.begin(), thenInstrs.end()); //Insert then instructions
		_progInfo.SetLabelInstr(loopLabels.reset, thenInstrs.front()); //Set Loop Reset
		instrs.push_back(new InstrJump(loopLabels.begin)); //Jump back to condition instructions
		instrs.push_back(new InstrPop(DataType::BYTE, new ValueArgument(BYTE_SIZE))); //Pop off condition if not doing body
		_progInfo.SetLabelInstr(loopLabels.pop, instrs.back()); //Set Loop Pop
		instrs.push_back(new InstrNoOp());
		_progInfo.SetLabelInstr(loopLabels.end, instrs.back()); //Set Loop End
		_progInfo.PopCurLoopLabels(); //End Loop Section 

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

	ASTNode* ASTBlock::GetCopy()
	{
		std::vector<ASTStmt*> copiedStmts;

		for (auto const& stmt : stmts)
			copiedStmts.push_back((ASTStmt*)stmt->GetCopy());

		return new ASTBlock(copiedStmts, GetPosition());
	}

	TypeResult ASTBlock::_TypeCheck(Environment* _env)
	{
		for (auto it = stmts.begin(); it != stmts.end(); it++)
		{
			TypeResult typeRes = (*it)->TypeCheck(_env);
			if (!typeRes.IsSuccess())
				return typeRes;
		}

		return TypeResult::GenSuccess(Type::UNIT());
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

	TypeResult ASTBreakContinueStmt::_TypeCheck(Environment* _env) { return TypeResult::GenSuccess(Type::UNIT()); }

	InstructionSet ASTBreakContinueStmt::GenerateCode(ProgramInfo& _progInfo)
	{
		ProgramInfo::LabeledLoop* curLoopLabels = _progInfo.GetCurLoopLabels();
		return { new InstrJump(IsBreak() ? curLoopLabels->pop : curLoopLabels->reset) };
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

	ASTNode* ASTReturnStmt::GetCopy()
	{
		ExprList copiedExprs;

		for (auto const& expr : exprs)
			copiedExprs.push_back((ASTExpr*)expr->GetCopy());

		return new ASTReturnStmt(copiedExprs, GetPosition());
	}

	TypeResult ASTReturnStmt::_TypeCheck(Environment* _env)
	{
		TypeList exprTypes;
		for (auto const& it : exprs)
		{
			TypeResult exprTypeRes = it->TypeCheck(_env);
			if (!exprTypeRes.IsSuccess()) { return exprTypeRes; }
			else { exprTypes.push_back((*exprTypeRes.GetValue())->GetCopy()); }
		}

		int numExprs = exprTypes.size();
		if (numExprs == 0) { return TypeResult::GenSuccess(Type::UNIT()); }
		else if (numExprs == 1) { return TypeResult::GenSuccess(exprTypes[0]); }
		else { return TypeResult::GenSuccess(new TupleType(exprTypes)); }
	}

	InstructionSet ASTReturnStmt::GenerateCode(ProgramInfo& _progInfo)
	{
		InstructionSet instrs;
		int retByteAmt = 0;

		for (int i = exprs.size() - 1; i >= 0; i--)
		{
			ASTExpr* expr = exprs[i];
			auto exprInstrs = expr->GenerateCode(std::shared_ptr<Argument>(StackArgument::GenStackTop()), _progInfo);

			instrs.insert(instrs.end(), exprInstrs.begin(), exprInstrs.end());
			retByteAmt += expr->GetTypeSysType()->GetByteSize();
		}

		instrs.push_back(new InstrReturn(new ValueArgument(retByteAmt)));

		return instrs;
	}

	CodePathNode* ASTReturnStmt::GetCodePath() { return new CodePathNode(nullptr, nullptr, this, false); }
#pragma endregion

#pragma region ArrayInit
	ASTArrayInit::ASTArrayInit(std::vector<ASTExpr*> _elemExprs, Position _pos)
		: ASTExpr(ASTExprType::ARRAY_INIT, _pos), elemExprs(_elemExprs), restraint(nullptr) { }

	ASTArrayInit::~ASTArrayInit()
	{
		for (auto const& it : elemExprs)
			delete it;

		if (restraint)
			delete restraint;
	}

	std::string ASTArrayInit::ToString(int _indentLvl, std::string _prefix)
	{
		std::stringstream ss;
		ss << CreateIndent(_indentLvl) + _prefix;
		ss << "Array Initializer" << (elemExprs.size() == 0 ? "" : ": ");

		for (auto const& it : elemExprs)
			ss << std::endl << it->ToString(_indentLvl + 1);

		return ss.str();
	}

	ASTNode* ASTArrayInit::GetCopy()
	{
		ExprList copiedExprs;

		for (auto const& expr : elemExprs)
			copiedExprs.push_back((ASTExpr*)expr->GetCopy());

		return new ASTArrayInit(copiedExprs, GetPosition());
	}

	TypeResult ASTArrayInit::_TypeCheck(Environment* _env)
	{
		//Typecheck first element to get restraint
		TypeResult restraintRes = elemExprs[0]->TypeCheck(_env);
		if (!restraintRes.IsSuccess()) { return restraintRes; }
		else { restraint = (*restraintRes.GetValue())->GetCopy(); }

		for (int i = 1; i < elemExprs.size(); i++)
		{
			ASTExpr* elemExpr = elemExprs[i];

			TypeResult exprTypeRes = elemExpr->TypeCheck(_env);
			if (!exprTypeRes.IsSuccess()) { return exprTypeRes; }
			else if (!Type::Matches(restraint, *exprTypeRes.GetValue())) { return TypeResult::GenExpectation(restraint->ToString(0), (*exprTypeRes.GetValue())->ToString(0), elemExpr->GetPosition()); }
		}

		return TypeResult::GenSuccess(new ArrayType(restraint));
	}

	InstructionSet ASTArrayInit::GenerateCode(std::shared_ptr<Argument> _dest, ProgramInfo& _progInfo)
	{
		int mallocLength = restraint->GetByteSize() * elemExprs.size();
		InstructionSet instrs;

		instrs.push_back(new InstrMalloc(new ValueArgument(mallocLength), StackArgument::GenStackTop())); //Allocate memory

		//Store values
		int offset = 1;

		for (int i = 0; i < (int)elemExprs.size(); i++)
		{
			auto addr = std::shared_ptr<Argument>(new MemoryArgument(StackArgument::GenStackCur(), i * restraint->GetByteSize() - 1));
			InstructionSet code = elemExprs[i]->GenerateCode(addr, _progInfo);

			instrs.insert(instrs.end(), code.begin(), code.end());
		}

		//Create Move to move address to destination
		if (!_dest->IsStackTop())
		{
			instrs.push_back(new InstrMove(DataType::INT, new StackArgument(StackArgType::SP_OFFSETED, -INT_SIZE + 1), _dest->GetCopy()));
			instrs.push_back(new InstrPop(DataType::INT, new ValueArgument(1)));
		}

		//TODO: Add address to functions garbage collector

		return instrs;
	}
#pragma endregion

#pragma region Program Info
	std::string ProgramInfo::MAIN_FUNC_LABEL = Environment::GenFuncLabel("Main", *std::make_shared<TupleType*>(new TupleType({})));

	ProgramInfo::LabeledLoop ProgramInfo::GenLoopLabels()
	{
		static int numLoopLabels = 0;

		std::string postfix = std::to_string(numLoopLabels);
		std::string beginLabel = "LoopBegin" + postfix;
		std::string popLabel = "LoopPop" + postfix;
		std::string resetLabel = "LoopReset" + postfix;
		std::string endLabel = "LoopEnd" + postfix;

		numLoopLabels++;
		return { beginLabel, popLabel, resetLabel, endLabel };
	}

	ProgramInfo::LabeledIf ProgramInfo::GenIfLabels(bool _hasElse)
	{
		static int numIfLabels = 0;

		std::string postfix = std::to_string(numIfLabels);
		std::string beginLabel = "IfBegin" + postfix;
		std::string thenLabel = "IfThen" + postfix;
		std::string elseLabel = "IfElse" + postfix;
		std::string endLabel = "IfEnd" + postfix;

		numIfLabels++;
		return { beginLabel, thenLabel, elseLabel, endLabel };
	}

	void ProgramInfo::AddFuncDecl(std::string _label, int _regCnt)
	{
		ASSERT_MSG(funcDecls.find(_label) == funcDecls.end(), "ProgramInfo::AddFuncDecl - Function delcaration '" + _label + "' already exists!");
		ASSERT_MSG(labelledInstrs.find(_label) == labelledInstrs.end(), "ProgramInfo::SetFuncDeclStart - Label '" + _label + "' already in use!");
		funcDecls.insert_or_assign(_label, _regCnt);
		SetLabelInstr(_label, nullptr);
	}

	void ProgramInfo::SetFuncDeclStart(std::string _label, Instruction* _start)
	{
		ASSERT_MSG(funcDecls.find(_label) != funcDecls.end(), "ProgramInfo::SetFuncDeclStart - Function delcaration '" + _label + "' does not exist!");
		SetLabelInstr(_label, _start);
	}

	int ProgramInfo::GetFuncRegCount(std::string _label)
	{
		ASSERT_MSG(funcDecls.find(_label) != funcDecls.end(), "ProgramInfo::GetFuncDecl - Function delcaration '" + _label + "' does not exist!");
		return funcDecls[_label];
	}

	void ProgramInfo::SetLabelInstr(std::string _label, Instruction* _instr)
	{
		ASSERT_MSG(labelledInstrs.find(_label) == labelledInstrs.end() || labelledInstrs[_label] == nullptr, "ProgramInfo::SetLabelInstr - The label '" + _label + "' has already been assigned to!");
		labelledInstrs.insert_or_assign(_label, _instr);
	}

	void ProgramInfo::PushCurLoopLabels(LabeledLoop* _labels) { labelledLoopStack.push_back(_labels); }
	void ProgramInfo::PopCurLoopLabels() { labelledLoopStack.pop_back(); }

	ProgramInfo::LabeledLoop* ProgramInfo::GetCurLoopLabels()
	{
		ASSERT_MSG(!labelledLoopStack.empty(), "ProgramInfo::LabeledLoop - Code generation is not currently in a loop!");
		return labelledLoopStack.back();
	}

	std::unordered_map<std::string, Instruction*> ProgramInfo::GetLabelledInstrs() { return labelledInstrs; }

	std::unordered_map<std::string, int> ProgramInfo::GetLabels(const std::vector<Instruction*>& _instrs)
	{
		std::unordered_map<std::string, int> result;

		for (auto const& it : labelledInstrs)
		{
			auto search = std::find(_instrs.begin(), _instrs.end(), it.second);

			ASSERT_MSG(search != _instrs.end(), "ProgramInfo::MatchLabels - Could not find associated label for instruction!");
			result.insert_or_assign(it.first, std::distance(_instrs.begin(), search));
		}

		return result;
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