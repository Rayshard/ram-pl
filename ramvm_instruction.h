#pragma once
#include "ramvm_argument.h"

namespace ramvm {
	enum class InstructionType {
		HALT, NOOP, MOVE, BINOP, UNOP, JUMP,
		CJUMP, CALL, RETURN, PRINT, MALLOC,
		PUSH, POP, FREE, STORE, COMPARE
	};

	enum class Binop {
		ADD, SUB, MUL, DIV, MOD, POW,
		LSHIFT, RSHIFT, XOR, AND, OR,
		LT, GT, LTEQ, GTEQ, EQ, NEQ,
	};

	enum class Unop { NEG, NOT };

	ResultType DoBinop(Binop _op, DataVariant& _val1, DataVariant& _val2, DataVariant& _result);
	ResultType DoUnop(Unop _op, DataVariant& _val, DataVariant& _result);

	class Instruction {
		InstructionType type;
	public:
		Instruction(InstructionType _type) : type(_type) { }
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
		Argument* src, * dest;

		InstrMove(DataType _dataType, Argument* _src, Argument* _dest);
		~InstrMove();

		std::string ToString() override { return "MOV<" + std::string(1, DataTypeToChar(dataType)) + ">(" + src->ToString() + ", " + dest->ToString() + ")"; }
	};
#pragma endregion

#pragma region Binop
	struct InstrBinop : Instruction {
		Binop op;
		DataTypeTriple argDataTypes;
		Argument* src1, * src2, * dest;

		InstrBinop(Binop _op, DataTypeTriple _argDataTypes, Argument* _src1, Argument* _src2, Argument* _dest);
		~InstrBinop();

		DataType GetSrc1DataType() { return std::get<0>(argDataTypes); }
		DataType GetSrc2DataType() { return std::get<1>(argDataTypes); }
		DataType GetDestDataType() { return std::get<2>(argDataTypes); }

		std::string ToString() override;
	};
#pragma endregion

#pragma region Unop
	struct InstrUnop : Instruction {
		Unop op;
		DataTypeDouble argDataTypes;
		Argument* src, * dest;

		InstrUnop(Unop _op, DataTypeDouble _argDataTypes, Argument* _src, Argument* _dest);
		~InstrUnop();

		DataType GetSrcDataType() { return std::get<0>(argDataTypes); }
		DataType GetDestDataType() { return std::get<1>(argDataTypes); }

		std::string ToString() override;
	};
#pragma endregion

#pragma region Call
	struct InstrCall : Instruction {
		std::string label;
		Argument* argsByteLength; //argsByteLength: The amount of bytes to push onto the stack for the new execution frame

		InstrCall(std::string _label, Argument* _argsByteLen);
		~InstrCall();

		std::string ToString() override;
	};
#pragma endregion

#pragma region Store
	struct InstrStore : Instruction {
		DataType dataType;
		std::vector<Argument*> srcs;
		Argument* dest;

		InstrStore(DataType _dataType, const std::vector<Argument*>& _srcs, Argument* _dest);
		~InstrStore();

		std::string ToString() override;
	};
#pragma endregion

#pragma region Compare
	struct InstrCompare : Instruction {
		Argument* src1, * src2, * length, * dest;

		InstrCompare(Argument* _src1, Argument* _src2, Argument* _len, Argument* _dest);
		~InstrCompare();

		std::string ToString() override { return "COMPARE(" + src1->ToString() + ", " + src2->ToString() + ", " + length->ToString() + ", " + dest->ToString() + ")"; }
	};
#pragma endregion

#pragma region Return
	struct InstrReturn : Instruction {
		Argument* amt; //The amount of bytes from the stack to return

		InstrReturn(Argument* _amt);
		~InstrReturn();

		std::string ToString() override { return "RET(" + amt->ToString() + ")"; }
	};
#pragma endregion

#pragma region Jump
	struct InstrJump : Instruction {
		std::string label;

		InstrJump(std::string _label);

		std::string ToString() override { return "JUMP(%" + label + ")"; }
	};
#pragma endregion

#pragma region CJump
	struct InstrCJump : Instruction {
		std::string label;
		Argument* cond;
		bool jumpOnFalse;

		InstrCJump(std::string _label, Argument* _cond, bool _jumpOnFalse);
		~InstrCJump();

		std::string ToString() override { return (jumpOnFalse ? "JUMPF(%" : "JUMPT(%") + label + ", " + cond->ToString() + ")"; }
	};
#pragma endregion

#pragma region Print
	struct InstrPrint : Instruction {
		Argument* start, * length;

		InstrPrint(Argument* _start, Argument* _len);
		~InstrPrint();

		std::string ToString() override { return "PRINT(" + start->ToString() + ", " + length->ToString() + ")"; }
	};
#pragma endregion

#pragma region Malloc
	struct InstrMalloc : Instruction {
		Argument* size, * dest;

		InstrMalloc(Argument* _size, Argument* _dest);
		~InstrMalloc();

		std::string ToString() override { return "MALLOC(" + size->ToString() + ", " + dest->ToString() + ")"; }
	};
#pragma endregion

#pragma region Free
	struct InstrFree : Instruction {
		Argument* addr;

		InstrFree(Argument* _addr);
		~InstrFree();

		std::string ToString() override { return "FREE(" + addr->ToString() + ")"; }
	};
#pragma endregion

#pragma region Push
	struct InstrPush : Instruction {
		DataType dataType;
		std::vector<Argument*> srcs;

		InstrPush(DataType _dataType, const std::vector<Argument*>& _srcs);
		~InstrPush();

		std::string ToString() override;
	};
#pragma endregion

#pragma region Pop
	struct InstrPop : Instruction {
		Argument* amt;
		int scale;

		InstrPop(DataType _type, Argument* _amt);
		~InstrPop();
		bool IsSinglePop();

		std::string ToString() override { return "POP<" + std::string(1, DataTypeToChar(GetDataType())) + ">(" + amt->ToString() + ")"; }

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
	};
#pragma endregion
}