#pragma once
#include "ramc_token.h"
#include "ramc_typesystem.h"
#include "ramvm_instruction.h"
#include "ramvm_parser.h"

using ramvm::Instruction;
using ramvm::InstructionSet;
using ramvm::Argument;

namespace ramc {
	class ASTStmt;
	class ASTIdentifier;
	class ASTBinopEpxr;
	class ASTFuncDecl;
	class ASTVarDecl;
	struct CodePathNode;

	enum class ASTNodeType { PROGRAM, FUNCDECL, STMT, EXPR };
	enum class ASTStmtType { ASSIGNMENT, VARDECL, BLOCK, IF, WHILE, FORLOOP, BREAK, CONTINUE, RETURN };
	enum class ASTExprType { IF, LITERAL, BINOP, UNOP, IDENTIFIER, EXPR, VARDECL };

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

	struct Param { std::string name; TypePtr type; Position position; };
	typedef std::vector<Param> ParamList;

	std::string ASTStmtTypeToString(ASTStmtType _type);

#pragma region ASTNode
	class ASTNode {
	private:
		ASTNodeType type;
		Position position;
		TypePtr typeSysType;
	protected:
		ASTNode(ASTNodeType _type, Position _pos);

		virtual TypeResult _TypeCheck(Environment* _env) = 0;
	public:
		TypeResult TypeCheck(Environment* _env);

		ASTNodeType GetType() { return type; }
		TypePtr GetTypeSysType() { return typeSysType; }
		Position GetPosition() { return position; }

		virtual std::string ToString(int _indentLvl, std::string _prefix = "") = 0;
	};
#pragma endregion

#pragma region Program
	class ProgramInfo
	{
		int numLoopLabels = 0;
	public:
		struct LabeledLoop { bool isForLoop; };
		struct LabeledWhileLoop { std::string begin, pop, end; };
		struct LabeledForLoop { std::string begin, pop, then, end; };
		struct LabeledIf { std::string begin, thenClause, elseClause, end; };

		std::map<std::string, Instruction*> labels;
		std::vector<LabeledLoop*> curLoopLabels;

		InstructionSet offsetedCtrlInstrs;
		std::unordered_map<std::string, InstructionSet> labeledCtrlInstrs;


		LabeledWhileLoop GenWhileLoopLabels();
		LabeledForLoop GenForLoopLabels();
		LabeledIf GenIfLabels(bool _hasElse);
		void SetLabelInstr(std::string _label, Instruction* _instr);
	};

	class ASTProgram : public ASTNode {
		ProgramInfo info;
		std::string fileName;
		std::vector<ASTVarDecl*> varDecls;
		std::vector<ASTFuncDecl*> funcDecls;
	public:
		ASTProgram(std::string _fileName, std::vector<ASTVarDecl*> _varDecls, std::vector<ASTFuncDecl*> _funcDecls);
		~ASTProgram();

		std::string ToString(int _indentLvl, std::string _prefix = "") override;
		TypeResult _TypeCheck(Environment* _env) override;
		InstructionSet GenerateCode();

		ProgramInfo GetInfo() { return info; }
	};
#pragma endregion

#pragma region ASTFuncDecl
	class ASTFuncDecl : public ASTNode {
		std::string name;
		ParamList params;
		FuncType* funcType;
		ASTStmt* body;

	public:
		ASTFuncDecl(std::string _name, ParamList _params, TypeList _retTypes, ASTStmt* _body, Position _pos);
		~ASTFuncDecl();

		std::string ToString(int _indentLvl, std::string _prefix = "") override;
		TypeResult _TypeCheck(Environment* _env) override;
		InstructionSet GenerateCode(ProgramInfo& _progInfo);
	};
#pragma endregion

#pragma region ASTStmt
	class ASTStmt : public ASTNode {
	private:
		ASTStmtType stmtType;
	protected:
		ASTStmt(ASTStmtType _type, Position _pos)
			: ASTNode(ASTNodeType::STMT, _pos), stmtType(_type) { }

		virtual TypeResult _TypeCheck(Environment* _env) = 0;
	public:
		ASTStmtType GetStmtType() { return stmtType; }

		virtual std::string ToString(int _indentLvl, std::string _prefix = "") = 0;
		virtual InstructionSet GenerateCode(ProgramInfo& _progInfo) = 0;
		virtual CodePathNode* GetCodePath() = 0;
	};
#pragma endregion

#pragma region ASTExpr
	class ASTExpr : public ASTNode {
	private:
		ASTExprType exprType;
	protected:
		ASTExpr(ASTExprType _type, Position _pos)
			: ASTNode(ASTNodeType::EXPR, _pos), exprType(_type) { }

		virtual TypeResult _TypeCheck(Environment* _env) = 0;
	public:
		ASTExprType GetExprType() { return exprType; }

		virtual std::string ToString(int _indentLvl, std::string _prefix = "") = 0;
		virtual InstructionSet GenerateCode(Argument _dest, ProgramInfo& _progInfo) = 0;
	};
#pragma endregion

#pragma region Block
	class ASTBlock : public ASTStmt {
		std::vector<ASTStmt*> stmts;
	public:
		ASTBlock(const std::vector<ASTStmt*>& _stmts, Position _pos);
		~ASTBlock();

		std::string ToString(int _indentLvl, std::string _prefix = "") override;
		TypeResult _TypeCheck(Environment* _env) override;
		InstructionSet GenerateCode(ProgramInfo& _progInfo) override;
		CodePathNode* GetCodePath() override;
	};
#pragma endregion

#pragma region VarDeclaration
	class ASTVarDecl : public ASTStmt {
		ASTIdentifier* id;
		ASTExpr* expr;
		bool isUnderscore;
		TypePtr restraint;
	public:
		ASTVarDecl(ASTIdentifier* _id, ASTExpr* _expr, Position _pos);
		ASTVarDecl(ASTIdentifier* _id, TypePtr _restraint, ASTExpr* _expr, Position _pos);
		ASTVarDecl(ASTExpr* _expr, Position _pos);
		~ASTVarDecl();

		std::string ToString(int _indentLvl, std::string _prefix = "") override;
		TypeResult _TypeCheck(Environment* _env) override;
		InstructionSet GenerateCode(ProgramInfo& _progInfo) override;
		CodePathNode* GetCodePath() override;
	};
#pragma endregion

#pragma region If Statement
	class ASTIfStmt : public ASTStmt {
		ASTExpr* condExpr;
		ASTStmt* thenStmt, * elseStmt;
	public:
		ASTIfStmt(ASTExpr* _condExpr, ASTStmt* _thenStmt, ASTStmt* _elseStmt, Position _pos);
		~ASTIfStmt();

		std::string ToString(int _indentLvl, std::string _prefix = "") override;
		TypeResult _TypeCheck(Environment* _env) override;
		InstructionSet GenerateCode(ProgramInfo& _progInfo) override;
		CodePathNode* GetCodePath() override;
	};
#pragma endregion

#pragma region If Expression
	class ASTIfExpr : public ASTExpr {
		ASTExpr* condExpr, * thenExpr, * elseExpr;
	public:
		ASTIfExpr(ASTExpr* _condExpr, ASTExpr* _thenExpr, ASTExpr* _elseExpr, Position _pos);
		~ASTIfExpr();

		std::string ToString(int _indentLvl, std::string _prefix = "") override;
		TypeResult _TypeCheck(Environment* _env) override;
		InstructionSet GenerateCode(Argument _dest, ProgramInfo& _progInfo) override;
	};
#pragma endregion

#pragma region While Statement
	class ASTWhileStmt : public ASTStmt {
		ASTExpr* condExpr;
		ASTStmt* body;
	public:
		ASTWhileStmt(ASTExpr* _condExpr, ASTStmt* _body, Position _pos);
		~ASTWhileStmt();

		std::string ToString(int _indentLvl, std::string _prefix = "") override;
		TypeResult _TypeCheck(Environment* _env) override;
		InstructionSet GenerateCode(ProgramInfo& _progInfo) override;
		CodePathNode* GetCodePath() override;
	};
#pragma endregion

#pragma region For Statement
	class ASTForStmt : public ASTStmt {
		ASTVarDecl* initStmt;
		ASTExpr* condExpr;
		ASTStmt* body;
		ASTStmt* thenStmt;

	public:
		ASTForStmt(ASTVarDecl* _initStmt, ASTExpr* _condExpr, ASTStmt* _body, ASTStmt* _thenStmt, Position _pos);
		~ASTForStmt();

		std::string ToString(int _indentLvl, std::string _prefix = "") override;
		TypeResult _TypeCheck(Environment* _env) override;
		InstructionSet GenerateCode(ProgramInfo& _progInfo) override;
		CodePathNode* GetCodePath() override;
	};
#pragma endregion

#pragma region Break/Continue Statement
	class ASTBreakContinueStmt : public ASTStmt {
	public:
		ASTBreakContinueStmt(bool _isBreak, Position _pos);

		bool IsBreak() { return GetStmtType() == ASTStmtType::BREAK; }

		std::string ToString(int _indentLvl, std::string _prefix = "") override;
		TypeResult _TypeCheck(Environment* _env) override;
		InstructionSet GenerateCode(ProgramInfo& _progInfo) override;
		CodePathNode* GetCodePath() override;
	};
#pragma endregion

#pragma region Assignment
	class ASTAssignment : public ASTStmt {
		ASTIdentifier* id;
		ASTExpr* expr;
		AssignmentType assignType;
	public:
		ASTAssignment(ASTIdentifier* _id, ASTExpr* _expr, AssignmentType _assignType);
		~ASTAssignment();

		AssignmentType GetAssignType() { return assignType; }

		std::string ToString(int _indentLvl, std::string _prefix = "") override;
		TypeResult _TypeCheck(Environment* _env) override;
		InstructionSet GenerateCode(ProgramInfo& _progInfo) override;
		CodePathNode* GetCodePath() override;
	};
#pragma endregion

#pragma region ReturnStmt
	class ASTReturnStmt : public ASTStmt {
		std::vector<ASTExpr*> exprs;
	public:
		ASTReturnStmt(std::vector<ASTExpr*> _exprs, Position _pos);
		~ASTReturnStmt();

		std::string ToString(int _indentLvl, std::string _prefix = "") override;
		TypeResult _TypeCheck(Environment* _env) override;
		InstructionSet GenerateCode(ProgramInfo& _progInfo) override;
		CodePathNode* GetCodePath() override;
	};
#pragma endregion

#pragma region BinopExpr
	class ASTBinopExpr : public ASTExpr {
		ASTExpr* left, * right;
		BinopType op;
	public:
		ASTBinopExpr(ASTExpr* _left, ASTExpr* _right, BinopType _op);
		~ASTBinopExpr();

		BinopType GetOp() { return op; }

		std::string ToString(int _indentLvl, std::string _prefix = "") override;
		TypeResult _TypeCheck(Environment* _env) override;
		InstructionSet GenerateCode(Argument _dest, ProgramInfo& _progInfo) override;
	};
#pragma endregion

#pragma region UnopExpr
	class ASTUnopExpr : public ASTExpr {
		ASTExpr* expr;
		UnopType op;
	public:
		ASTUnopExpr(ASTExpr* _expr, UnopType _op);
		~ASTUnopExpr();

		UnopType GetOp() { return op; }

		std::string ToString(int _indentLvl, std::string _prefix = "") override;
		TypeResult _TypeCheck(Environment* _env) override;
		InstructionSet GenerateCode(Argument _dest, ProgramInfo& _progInfo) override;
	};
#pragma endregion

#pragma region Identifier
	class ASTIdentifier : public ASTExpr {
		std::string id;
		Argument source;
	public:
		ASTIdentifier(std::string _id, Position _pos);

		std::string GetID() { return id; }
		Argument GetSource() { return source; }

		std::string ToString(int _indentLvl, std::string _prefix = "") override;
		TypeResult _TypeCheck(Environment* _env) override;
		InstructionSet GenerateCode(Argument _dest, ProgramInfo& _progInfo) override;
	};
#pragma endregion

#pragma region Literal
	template<typename T> class ASTLiteral : public ASTExpr {
		LiteralType litType;
		T value;
	protected:
		ASTLiteral(LiteralType _type, T _val, Position _pos)
			: ASTExpr(ASTExprType::LITERAL, _pos)
		{
			litType = _type;
			value = _val;
		}
	public:
		T GetValue() { return value; }
		LiteralType GetLitType() { return litType; }

		std::string ToString(int _indentLvl, std::string _prefix = "") override
		{
			std::stringstream ss;
			ss << CreateIndent(_indentLvl) << _prefix;

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

		TypeResult _TypeCheck(Environment* _env) override
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

		InstructionSet GenerateCode(Argument _dest, ProgramInfo& _progInfo) override = 0;
	};

	struct ASTIntLit : public ASTLiteral<int> {
		ASTIntLit(int _val, Position _pos) : ASTLiteral(LiteralType::INT, _val, _pos) { }
		InstructionSet GenerateCode(Argument _dest, ProgramInfo& _progInfo) override;
	};

	struct ASTByteLit : public ASTLiteral<byte> {
		ASTByteLit(byte _val, Position _pos) : ASTLiteral(LiteralType::BYTE, _val, _pos) { }
		InstructionSet GenerateCode(Argument _dest, ProgramInfo& _progInfo) override;
	};

	struct ASTDoubleLit : public ASTLiteral<double> {
		ASTDoubleLit(double _val, Position _pos) : ASTLiteral(LiteralType::DOUBLE, _val, _pos) { }
		InstructionSet GenerateCode(Argument _dest, ProgramInfo& _progInfo) override;
	};

	struct ASTLongLit : public ASTLiteral<rLong> {
		ASTLongLit(rLong _val, Position _pos) : ASTLiteral(LiteralType::LONG, _val, _pos) { }
		InstructionSet GenerateCode(Argument _dest, ProgramInfo& _progInfo) override;
	};

	struct ASTFloatLit : public ASTLiteral<float> {
		ASTFloatLit(float _val, Position _pos) : ASTLiteral(LiteralType::FLOAT, _val, _pos) { }
		InstructionSet GenerateCode(Argument _dest, ProgramInfo& _progInfo) override;
	};

	struct ASTStringLit : public ASTLiteral<std::string> {
		ASTStringLit(std::string _val, Position _pos) : ASTLiteral(LiteralType::STRING, _val, _pos) {}
		InstructionSet GenerateCode(Argument _dest, ProgramInfo& _progInfo) override;
	};

	struct ASTBoolLit : public ASTLiteral<bool> {
		ASTBoolLit(bool _val, Position _pos) : ASTLiteral(LiteralType::BOOL, _val, _pos) {}
		InstructionSet GenerateCode(Argument _dest, ProgramInfo& _progInfo) override;
	};
#pragma endregion

	struct CodePathNode
	{
		CodePathNode* left, * right;
		ASTStmt* statement;
		bool appendable;

		CodePathNode(CodePathNode* _left, CodePathNode* _right, ASTStmt* _stmt, bool _appendable);
		~CodePathNode();

		void Append(ASTStmt* _stmt, const std::set<ASTStmtType>& _overrides);
		void SetAppendable(const std::set<ASTStmtType>& _types);
		std::vector<CodePathNode*> GetTerminalNodes();
		std::set<ASTStmt*> GetTerminalStmts();

		void Print();
	};
}

