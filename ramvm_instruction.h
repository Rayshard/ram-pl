#pragma once

namespace ramvm {
	enum class InstructionType {
		HALT, MOVE, BINOP, UNOP, JUMP,
		CJUMP, CALL, RETURN, PRINT, MALLOC,
		PUSH, POP, FREE
	};

	enum class ArgType {
		VALUE, REGISTER, MEM_REG,
		STACK_REG, STACK_PTR, SP_OFFSET,
		INVALID
	};

	struct Argument {
		ArgType type;
		DataVariant value;

		Argument() = default;
		Argument(ArgType _type, DataVariant _val);
		std::string ToString();

		bool IsStackTop() { return type == ArgType::SP_OFFSET && value.AsInt() == 1; }
		bool IsStackCur() { return type == ArgType::SP_OFFSET && value.AsInt() == 0; }
		bool IsStackPrev() { return type == ArgType::SP_OFFSET && value.AsInt() == -1; }

		static Argument CreateStackTop() { return Argument(ArgType::SP_OFFSET, DataVariant(1)); }
	};

	enum class Binop {
		ADD, SUB, MUL, DIV, MOD,
		LSHIFT, RSHIFT, BIT_AND, BIT_OR, BIT_XOR, LOG_AND, LOG_OR,
		LT, GT, LTEQ, GTEQ, EQ, NEQ,
	};

	enum class Unop {
		NEG,
		LOG_NOT,
	};

	ResultType DoBinop(Binop _op, DataVariant& _val1, DataVariant& _val2, DataVariant& _result);
	ResultType DoUnop(Unop _op, DataVariant& _val, DataVariant& _result);

	class Instruction {
		InstructionType type;
	public:
		Instruction(InstructionType _type) : type(_type) {}
		InstructionType GetType() { return type; }

		bool IsSinglePop();
		virtual std::string ToString() = 0;
	};

	typedef std::vector<Instruction*> InstructionSet;

#pragma region Halt
	struct InstrHalt : Instruction {
		InstrHalt() : Instruction(InstructionType::HALT) { }

		std::string ToString() override { return "HALT"; }
	};
#pragma endregion

#pragma region Move
	struct InstrMove : Instruction {
		DataType dataType;
		Argument src, dest;

		InstrMove(DataType _dataType, Argument _src, Argument _dest);

		std::string ToString() override { return "MOV " + src.ToString() + " " + dest.ToString(); }
	};
#pragma endregion

#pragma region Binop
	struct InstrBinop : Instruction {
		Binop op;
		Argument src1, src2, dest;

		InstrBinop(Binop _op, Argument _src1, Argument _src2, Argument _dest);
		std::string ToString() override;
	};
#pragma endregion

#pragma region Unop
	struct InstrUnop : Instruction {
		Unop op;
		Argument src, dest;

		InstrUnop(Unop _op, Argument _src, Argument _dest);
		std::string ToString() override;
	};
#pragma endregion

#pragma region Call
	struct InstrCall : Instruction {
		int labelIdx, regCnt;
		std::vector<Argument> argSrcs, retDests;

		InstrCall(int _labelIdx, int _regCnt, std::vector<Argument>& _argSrcs, std::vector<Argument>& _retDests);

		std::string ToString() override { return "CALL (NEED TO HANDLE TOSTRING)"; }
	};
#pragma endregion

#pragma region Return
	struct InstrReturn : Instruction {
		std::vector<Argument> srcs;

		InstrReturn(std::vector<Argument>& _srcs);
		
		std::string ToString() override { return "RET (NEED TO HANDLE TOSTRING)"; }
	};
#pragma endregion

#pragma region Jump
	struct InstrJump : Instruction {
		int labelIdx;

		InstrJump(int _labelIdx);

		std::string ToString() override { return "JUMP " + std::to_string(labelIdx); }
	};
#pragma endregion

#pragma region CJump
	struct InstrCJump : Instruction {
		int labelIdx;
		Argument condSrc;

		InstrCJump(int _labelIdx, Argument _condSrc);

		std::string ToString() override { return "CJUMP " + std::to_string(labelIdx) + " " + condSrc.ToString(); }
	};
#pragma endregion

#pragma region Print
	struct InstrPrint : Instruction {
		Argument start, length;

		InstrPrint(Argument _start, Argument _len);

		std::string ToString() override { return "PRINT " + start.ToString() + " " + length.ToString(); }
	};
#pragma endregion

#pragma region Malloc
	struct InstrMalloc : Instruction {
		Argument size, dest;

		InstrMalloc(Argument _size, Argument _dest);

		std::string ToString() override { return "MALLOC " + size.ToString() + " " + dest.ToString(); }
	};
#pragma endregion

#pragma region Free
	struct InstrFree : Instruction {
		Argument addr;

		InstrFree(Argument _addr);

		std::string ToString() override { return "FREE " + addr.ToString(); }
	};
#pragma endregion

#pragma region Push
	struct InstrPush : Instruction {
		std::vector<Argument> srcs;

		InstrPush(Argument _src);
		InstrPush(std::vector<Argument>& _srcs);
		
		std::string ToString() override;
	};
#pragma endregion

#pragma region Pop
	struct InstrPop : Instruction {
		Argument amt;
		int scale;

		InstrPop(Argument _amt, int _scale);

		std::string ToString() override { return "POP " + amt.ToString(); }
	};
#pragma endregion
}