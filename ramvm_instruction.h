#pragma once

namespace ramvm {
	enum class InstructionType {
		HALT, NOOP, MOVE, BINOP, UNOP, JUMP,
		CJUMP, CALL, RETURN, PRINT, MALLOC,
		PUSH, POP, FREE, STORE, COMPARE
	};

	enum class ArgType {
		VALUE, REGISTER, MEM_REG,
		STACK_REG, STACK_PTR, SP_OFFSET,
		INVALID
	};

	struct Argument {
		ArgType type;
		DataValue value;

		Argument() : type(ArgType::INVALID), value(0) { }
		Argument(ArgType _type, DataValue _val);
		std::string ToString();

		bool IsStackTop() { return type == ArgType::SP_OFFSET && value.i == 1; }
		bool IsStackCur() { return type == ArgType::SP_OFFSET && value.i == 0; }
		bool IsStackPrev() { return type == ArgType::SP_OFFSET && value.i == -1; }

		static Argument CreateStackTop() { return Argument(ArgType::SP_OFFSET, 1); }
	};

	struct TypedArgument : Argument {
		DataType dataType;

		TypedArgument() = default;
		TypedArgument(DataType _dataType, ArgType _type, DataValue _val)
			: Argument(_type, _val), dataType(_dataType) { }
		TypedArgument(DataType _dataType, Argument _arg)
			: Argument(_arg.type, _arg.value), dataType(_dataType) { }
	};

	enum class Binop {
		ADD, SUB, MUL, DIV, MOD, POW,
		LSHIFT, RSHIFT, BIT_AND, BIT_OR, BIT_XOR, LOG_AND, LOG_OR,
		LT, GT, LTEQ, GTEQ, EQ, NEQ,
	};

	enum class Unop { NEG, LOG_NOT, BIN_NOT };

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

#pragma region NoOp
	struct InstrNoOp : Instruction {
		InstrNoOp() : Instruction(InstructionType::NOOP) { }

		std::string ToString() override { return "NOOP"; }
	};
#pragma endregion

#pragma region Move
	struct InstrMove : Instruction {
		DataType dataType;
		Argument src;
		Argument dest;

		InstrMove(DataType _dataType, Argument _src, Argument _dest);

		std::string ToString() override { return "MOV<" + std::string(1, DataTypeToChar(dataType)) + "> " + src.ToString() + " " + dest.ToString(); }
	};
#pragma endregion

#pragma region Binop
	struct InstrBinop : Instruction {
		Binop op;
		TypedArgument src1, src2, dest;


		InstrBinop(Binop _op, TypedArgument _src1, TypedArgument _src2, TypedArgument _dest);
		std::string ToString() override;
	};
#pragma endregion

#pragma region Unop
	struct InstrUnop : Instruction {
		Unop op;
		TypedArgument src, dest;

		InstrUnop(Unop _op, TypedArgument _src, TypedArgument _dest);
		std::string ToString() override;
	};
#pragma endregion

#pragma region Call
	struct InstrCall : Instruction {
		int labelIdx, regCnt;
		std::vector<TypedArgument> argSrcs;

		InstrCall(int _labelIdx, int _regCnt, const std::vector<TypedArgument>& _argSrcs);

		std::string ToString() override;
	};
#pragma endregion

#pragma region Store
	struct InstrStore : Instruction {
		std::vector<TypedArgument> srcs;
		Argument dest;

		InstrStore(const std::vector<TypedArgument>& _srcs, Argument _dest);

		std::string ToString() override;
	};
#pragma endregion

#pragma region Compare
	struct InstrCompare : Instruction {
		Argument src1, src2, length, dest;

		InstrCompare(Argument _src1, Argument _src2, Argument _len, Argument _dest);

		std::string ToString() override { return "COMPARE " + src1.ToString() + " " + src2.ToString() + " " + length.ToString() + " " + dest.ToString(); }
	};
#pragma endregion

#pragma region Return
	struct InstrReturn : Instruction {
		std::vector<TypedArgument> srcs;

		InstrReturn(const std::vector<TypedArgument>& _srcs);

		std::string ToString() override;
	};
#pragma endregion

#pragma region Jump
	struct InstrJump : Instruction {
		int instrIdx;

		InstrJump(int _instrIdx);

		std::string ToString() override { return "JUMP " + std::to_string(instrIdx); }
	};
#pragma endregion

#pragma region CJump
	struct InstrCJump : Instruction {
		int instrIdx;
		Argument condSrc;
		bool jumpOnFalse;

		InstrCJump(int _instrIdx, Argument _condSrc, bool _jumpOnFalse);

		std::string ToString() override { return (jumpOnFalse ? "JUMPF " : "JUMPT ") + std::to_string(instrIdx) + " " + condSrc.ToString(); }
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
		std::vector<TypedArgument> srcs;

		InstrPush(const std::vector<TypedArgument>& _srcs);

		std::string ToString() override;
	};
#pragma endregion

#pragma region Pop
	struct InstrPop : Instruction {
		Argument amt;
		int scale;

		InstrPop(DataType _type, Argument _amt);

		constexpr DataType GetDataType()
		{
			switch (scale)
			{
				case BYTE_SIZE: return DataType::BYTE;
				case INT_SIZE: return DataType::INT;
				case LONG_SIZE: return DataType::LONG;
				default: return DataType::UNKNOWN;
			}
		}

		std::string ToString() override { return "POP<" + std::string(1, DataTypeToChar(GetDataType())) + "> " + amt.ToString(); }
	};
#pragma endregion
}