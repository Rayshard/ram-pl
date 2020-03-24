#pragma once

namespace ramvm {
	enum class InstructionType {
		HALT,
		MOVE,
		BINOP,
		UNOP,
		JUMP,
		CJUMP,
		CALL,
		RETURN,
		PRINT,
		MALLOC,
		PUSH,
		POP,
		FREE
	};

	enum class ArgType {
		VALUE,
		REGISTER,
		MEM_REG,
		STACK_REG,
		STACK_PTR,
		SP_OFFSET,
		INVALID
	};

	struct Argument {
		ArgType type;
		int value;

		Argument() = default;
		Argument(ArgType _type, int _val);
		std::string ToString();

		bool IsStackTop() { return type == ArgType::SP_OFFSET && value == 1; }
		bool IsStackCur() { return type == ArgType::SP_OFFSET && value == 0; }
		bool IsStackPrev() { return type == ArgType::SP_OFFSET && value == -1; }

		static Argument CreateStackTop() { return Argument(ArgType::SP_OFFSET, 1); }
	};

	enum class Binop {
		ADD,
		SUB,
		MUL,
		DIV,
		MOD,
		LSHIFT,
		RSHIFT,
		LT,
		GT,
		LTEQ,
		GTEQ,
		EQ,
		NEQ,
		BIT_AND,
		BIT_OR,
		BIT_XOR,
		LOG_AND,
		LOG_OR,
	};

	enum class Unop {
		NEG,
		LOG_NOT,
	};

	class Instruction {
		InstructionType type;
		std::vector<Argument> args;

		Instruction() = default;
	public:
		Instruction(InstructionType _type, std::vector<Argument> _args);
		InstructionType GetType() { return type; }

		std::string ToString();

		static Instruction CreateHalt() { return Instruction(InstructionType::HALT, {}); }

#pragma region Move
		static Instruction CreateMove(Argument _src, Argument _dest);
		Argument GetMoveSrc() { return args[0]; }
		Argument GetMoveDest() { return args[1]; }
#pragma endregion

#pragma region Binop
		static Instruction CreateBinop(Binop _op, Argument _src1, Argument _src2, Argument _dest);
		Binop GetBinopType() { return (Binop)args[0].value; }
		Argument GetBinopSrc1() { return args[1]; }
		Argument GetBinopSrc2() { return args[2]; }
		Argument GetBinopDest() { return args[3]; }
#pragma endregion

#pragma region Unop
		static Instruction CreateUnop(Unop _op, Argument _src, Argument _dest);
		Unop GetUnopType() { return (Unop)args[0].value; }
		Argument GetUnopSrc() { return args[1]; }
		Argument GetUnopDest() { return args[2]; }
#pragma endregion

#pragma region Call
		static Instruction CreateCall(int _labelIdx, int _regCnt, std::vector<Argument>& _srcsArgs, std::vector<Argument>& _retDests);
		int GetCallLabelIdx() { return args[0].value; }
		int GetCallRegCount() { return args[1].value; }
		int GetCallNumSrcs() { return args[2].value; }
		int GetCallNumDests() { return args[3].value; }
		std::vector<Argument> GetCallArgSrcs();
		std::vector<Argument> GetCallRetDests();
#pragma endregion

#pragma region Return
		static Instruction CreateReturn(std::vector<Argument>& _srcs);
		std::vector<Argument> GetReturnSrcs() { return args; }
		int GetReturnNumSrcs() { return args.size(); }
#pragma endregion

#pragma region Jump
		static Instruction CreateJump(int _labelIdx);
		int GetJumpLabelIdx() { return args[0].value; }
#pragma endregion

#pragma region CJump
		static Instruction CreateCJump(int _labelIdx, Argument _condSrc);
		int GetCJumpLabelIdx() { return args[0].value; }
		Argument GetCJumpCondSrc() { return args[1]; }
#pragma endregion

#pragma region Print
		static Instruction CreatePrint(Argument _src, Argument _len);
		Argument GetPrintSrc() { return args[0]; }
		Argument GetPrintLength() { return args[1]; }
#pragma endregion

#pragma region Malloc
		static Instruction CreateMalloc(Argument _size, Argument _dest);
		Argument GetMallocSize() { return args[0]; }
		Argument GetMallocDest() { return args[1]; }
#pragma endregion

#pragma region Free
		static Instruction CreateFree(Argument _addr);
		Argument GetFreeAddr() { return args[0]; }
#pragma endregion

#pragma region Push
		static Instruction CreatePush(std::vector<Argument>& _srcs);
		static Instruction CreatePush(Argument _src);
		std::vector<Argument> GetPushSrcs() { return args; }
		int GetPushNumSrcs() { return args.size(); }
#pragma endregion

#pragma region Pop
		static Instruction CreatePop(Argument _amt);
		Argument GetPopAmt() { return args[0]; }
		bool IsSinglePop() { return type == InstructionType::POP && args[0].type == ArgType::VALUE && args[0].value == 1; }
#pragma endregion
	};

	typedef std::vector<Instruction> InstructionSet;
}