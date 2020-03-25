#include "pch.h"
#include "ramvm_instruction.h"

namespace ramvm {
	const std::string UnopToString(Unop _type)
	{
		switch (_type)
		{
			case Unop::NEG: return "NEG";
			case Unop::LOG_NOT: return "LNOT";
			default: return "UnopToString - Unop not handled!";
		}
	}

	const std::string BinopToString(Binop _type)
	{
		switch (_type)
		{
			case ramvm::Binop::ADD: return "ADD";
			case ramvm::Binop::SUB: return "SUB";
			case ramvm::Binop::MUL: return "MUL";
			case ramvm::Binop::DIV: return "DIV";
			case ramvm::Binop::MOD: return "MOD";
			case ramvm::Binop::LSHIFT: return "LSHIFT";
			case ramvm::Binop::RSHIFT: return "RSHIFT";
			case ramvm::Binop::LT: return "LT";
			case ramvm::Binop::GT: return "GT";
			case ramvm::Binop::LTEQ: return "LTEQ";
			case ramvm::Binop::GTEQ: return "GTEQ";
			case ramvm::Binop::EQ: return "EQ";
			case ramvm::Binop::NEQ: return "NEQ";
			case ramvm::Binop::BIT_AND: return "BAND";
			case ramvm::Binop::BIT_OR: return "BOR";
			case ramvm::Binop::BIT_XOR: return "BXOR";
			case ramvm::Binop::LOG_AND: return "LAND";
			case ramvm::Binop::LOG_OR: return "LOR";
			default: return "BinopToString - Binop not handled!";
		}
	}

	ResultType DoBinop(Binop _op, DataVariant& _val1, DataVariant& _val2, DataVariant& _result)
	{
		switch (ConcatTriple((byte)_val1.GetType(), (byte)_val2.GetType(), (byte)_op)) //Switch on Operation Type
		{
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::BYTE, (byte)Binop::ADD): _result = DataVariant(byte(_val1.AsByte() + _val2.AsByte())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::INT, (byte)Binop::ADD): _result = DataVariant(int(_val1.AsByte() + _val2.AsInt())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::BYTE, (byte)Binop::ADD): _result = DataVariant(int(_val1.AsInt() + _val2.AsByte())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::INT, (byte)Binop::ADD): _result = DataVariant(int(_val1.AsInt() + _val2.AsInt())); break;

			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::BYTE, (byte)Binop::SUB): _result = DataVariant(byte(_val1.AsByte() - _val2.AsByte())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::INT, (byte)Binop::SUB): _result = DataVariant(int(_val1.AsByte() - _val2.AsInt())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::BYTE, (byte)Binop::SUB): _result = DataVariant(int(_val1.AsInt() - _val2.AsByte())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::INT, (byte)Binop::SUB): _result = DataVariant(int(_val1.AsInt() - _val2.AsInt())); break;

			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::BYTE, (byte)Binop::MUL): _result = DataVariant(byte(_val1.AsByte() * _val2.AsByte())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::INT, (byte)Binop::MUL): _result = DataVariant(int(_val1.AsByte() * _val2.AsInt())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::BYTE, (byte)Binop::MUL): _result = DataVariant(int(_val1.AsInt() * _val2.AsByte())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::INT, (byte)Binop::MUL): _result = DataVariant(int(_val1.AsInt() * _val2.AsInt())); break;

			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::BYTE, (byte)Binop::DIV): {
				if (_val2.AsByte() != 0) { _result = DataVariant(byte(_val1.AsByte() / _val2.AsByte())); }
				else { return ResultType::ERR_DIVBYZERO; }
			} break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::INT, (byte)Binop::DIV): {
				if (_val2.AsInt() != 0) { _result = DataVariant(int(_val1.AsByte() / _val2.AsInt())); }
				else { return ResultType::ERR_DIVBYZERO; }
			} break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::BYTE, (byte)Binop::DIV): {
				if (_val2.AsByte() != 0) { _result = DataVariant(int(_val1.AsInt() / _val2.AsByte())); }
				else { return ResultType::ERR_DIVBYZERO; }
			} break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::INT, (byte)Binop::DIV): {
				if (_val2.AsInt() != 0) { _result = DataVariant(int(_val1.AsInt() / _val2.AsInt())); }
				else { return ResultType::ERR_DIVBYZERO; }
			} break;

			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::BYTE, (byte)Binop::MOD): {
				if (_val2.AsByte() != 0) { _result = DataVariant(byte(_val1.AsByte() % _val2.AsByte())); }
				else { return ResultType::ERR_DIVBYZERO; }
			} break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::INT, (byte)Binop::MOD): {
				if (_val2.AsInt() != 0) { _result = DataVariant(int(_val1.AsByte() % _val2.AsInt())); }
				else { return ResultType::ERR_DIVBYZERO; }
			} break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::BYTE, (byte)Binop::MOD): {
				if (_val2.AsByte() != 0) { _result = DataVariant(int(_val1.AsInt() % _val2.AsByte())); }
				else { return ResultType::ERR_DIVBYZERO; }
			} break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::INT, (byte)Binop::MOD): {
				if (_val2.AsInt() != 0) { _result = DataVariant(int(_val1.AsInt() % _val2.AsInt())); }
				else { return ResultType::ERR_DIVBYZERO; }
			} break;

			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::BYTE, (byte)Binop::LSHIFT): _result = DataVariant(byte(_val1.AsByte() << _val2.AsByte())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::INT, (byte)Binop::LSHIFT): _result = DataVariant(int(_val1.AsByte() << _val2.AsInt())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::BYTE, (byte)Binop::LSHIFT): _result = DataVariant(int(_val1.AsInt() << _val2.AsByte())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::INT, (byte)Binop::LSHIFT): _result = DataVariant(int(_val1.AsInt() << _val2.AsInt())); break;

			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::BYTE, (byte)Binop::RSHIFT): _result = DataVariant(byte(_val1.AsByte() >> _val2.AsByte())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::INT, (byte)Binop::RSHIFT): _result = DataVariant(int(_val1.AsByte() >> _val2.AsInt())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::BYTE, (byte)Binop::RSHIFT): _result = DataVariant(int(_val1.AsInt() >> _val2.AsByte())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::INT, (byte)Binop::RSHIFT): _result = DataVariant(int(_val1.AsInt() >> _val2.AsInt())); break;

			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::BYTE, (byte)Binop::LT): _result = DataVariant(byte(_val1.AsByte() < _val2.AsByte())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::INT, (byte)Binop::LT): _result = DataVariant(byte(_val1.AsByte() < _val2.AsInt())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::BYTE, (byte)Binop::LT): _result = DataVariant(byte(_val1.AsInt() < _val2.AsByte())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::INT, (byte)Binop::LT): _result = DataVariant(byte(_val1.AsInt() < _val2.AsInt())); break;

			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::BYTE, (byte)Binop::GT): _result = DataVariant(byte(_val1.AsByte() > _val2.AsByte())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::INT, (byte)Binop::GT): _result = DataVariant(byte(_val1.AsByte() > _val2.AsInt())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::BYTE, (byte)Binop::GT): _result = DataVariant(byte(_val1.AsInt() > _val2.AsByte())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::INT, (byte)Binop::GT): _result = DataVariant(byte(_val1.AsInt() > _val2.AsInt())); break;

			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::BYTE, (byte)Binop::LTEQ): _result = DataVariant(byte(_val1.AsByte() <= _val2.AsByte())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::INT, (byte)Binop::LTEQ): _result = DataVariant(byte(_val1.AsByte() <= _val2.AsInt())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::BYTE, (byte)Binop::LTEQ): _result = DataVariant(byte(_val1.AsInt() <= _val2.AsByte())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::INT, (byte)Binop::LTEQ): _result = DataVariant(byte(_val1.AsInt() <= _val2.AsInt())); break;

			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::BYTE, (byte)Binop::GTEQ): _result = DataVariant(byte(_val1.AsByte() >= _val2.AsByte())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::INT, (byte)Binop::GTEQ): _result = DataVariant(byte(_val1.AsByte() >= _val2.AsInt())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::BYTE, (byte)Binop::GTEQ): _result = DataVariant(byte(_val1.AsInt() >= _val2.AsByte())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::INT, (byte)Binop::GTEQ): _result = DataVariant(byte(_val1.AsInt() >= _val2.AsInt())); break;

			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::BYTE, (byte)Binop::EQ): _result = DataVariant(byte(_val1.AsByte() == _val2.AsByte())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::INT, (byte)Binop::EQ): _result = DataVariant(byte(_val1.AsByte() == _val2.AsInt())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::BYTE, (byte)Binop::EQ): _result = DataVariant(byte(_val1.AsInt() == _val2.AsByte())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::INT, (byte)Binop::EQ): _result = DataVariant(byte(_val1.AsInt() == _val2.AsInt())); break;

			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::BYTE, (byte)Binop::NEQ): _result = DataVariant(byte(_val1.AsByte() != _val2.AsByte())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::INT, (byte)Binop::NEQ): _result = DataVariant(byte(_val1.AsByte() != _val2.AsInt())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::BYTE, (byte)Binop::NEQ): _result = DataVariant(byte(_val1.AsInt() != _val2.AsByte())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::INT, (byte)Binop::NEQ): _result = DataVariant(byte(_val1.AsInt() != _val2.AsInt())); break;

			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::BYTE, (byte)Binop::BIT_AND): _result = DataVariant(byte(_val1.AsByte() & _val2.AsByte())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::INT, (byte)Binop::BIT_AND): _result = DataVariant(int(_val1.AsByte() & _val2.AsInt())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::BYTE, (byte)Binop::BIT_AND): _result = DataVariant(int(_val1.AsInt() & _val2.AsByte())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::INT, (byte)Binop::BIT_AND): _result = DataVariant(int(_val1.AsInt() & _val2.AsInt())); break;

			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::BYTE, (byte)Binop::BIT_OR): _result = DataVariant(byte(_val1.AsByte() | _val2.AsByte())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::INT, (byte)Binop::BIT_OR): _result = DataVariant(int(_val1.AsByte() | _val2.AsInt())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::BYTE, (byte)Binop::BIT_OR): _result = DataVariant(int(_val1.AsInt() | _val2.AsByte())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::INT, (byte)Binop::BIT_OR): _result = DataVariant(int(_val1.AsInt() | _val2.AsInt())); break;

			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::BYTE, (byte)Binop::BIT_XOR): _result = DataVariant(byte(_val1.AsByte() ^ _val2.AsByte())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::INT, (byte)Binop::BIT_XOR): _result = DataVariant(int(_val1.AsByte() ^ _val2.AsInt())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::BYTE, (byte)Binop::BIT_XOR): _result = DataVariant(int(_val1.AsInt() ^ _val2.AsByte())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::INT, (byte)Binop::BIT_XOR): _result = DataVariant(int(_val1.AsInt() ^ _val2.AsInt())); break;

			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::BYTE, (byte)Binop::LOG_AND): _result = DataVariant(byte((bool)_val1.AsByte() && (bool)_val2.AsByte())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::INT, (byte)Binop::LOG_AND): _result = DataVariant(byte((bool)_val1.AsByte() && (bool)_val2.AsInt())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::BYTE, (byte)Binop::LOG_AND): _result = DataVariant(byte((bool)_val1.AsInt() && (bool)_val2.AsByte())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::INT, (byte)Binop::LOG_AND): _result = DataVariant(byte((bool)_val1.AsInt() && (bool)_val2.AsInt())); break;

			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::BYTE, (byte)Binop::LOG_OR): _result = DataVariant(byte((bool)_val1.AsByte() || (bool)_val2.AsByte())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::INT, (byte)Binop::LOG_OR): _result = DataVariant(byte((bool)_val1.AsByte() || (bool)_val2.AsInt())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::BYTE, (byte)Binop::LOG_OR): _result = DataVariant(byte((bool)_val1.AsInt() || (bool)_val2.AsByte())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::INT, (byte)Binop::LOG_OR): _result = DataVariant(byte((bool)_val1.AsInt() || (bool)_val2.AsInt())); break;
		
			default: return ResultType::ERR_DO_BINOP;
		}

		return ResultType::SUCCESS;
	}

	ResultType DoUnop(Unop _op, DataVariant& _val, DataVariant& _result)
	{
		switch (ConcatDouble((byte)_val.GetType(), (byte)_op)) //Switch on Operation Type
		{
			case ConcatDouble((byte)DataType::BYTE, (byte)Unop::NEG): _result = DataVariant(byte(-_val.AsByte())); break;
			case ConcatDouble((byte)DataType::BYTE, (byte)Unop::LOG_NOT): _result = DataVariant(byte(!(bool)_val.AsByte())); break;
					   							    
			case ConcatDouble((byte)DataType::INT, (byte)Unop::NEG): _result = DataVariant(int(-_val.AsInt())); break;
			case ConcatDouble((byte)DataType::INT, (byte)Unop::LOG_NOT): _result = DataVariant(int(!(bool)_val.AsInt())); break;

			default: return ResultType::ERR_DO_UNOP;
		}

		return ResultType::SUCCESS;
	}

	bool Instruction::IsSinglePop() { return type == InstructionType::POP && ((InstrPop*)this)->amt.type == ArgType::VALUE && ((InstrPop*)this)->amt.value.AsInt() == 1; }

	Argument::Argument(ArgType _type, DataVariant _val)
	{
		type = _type;
		value = _val;
	}

	std::string Argument::ToString()
	{
		switch (type)
		{
			case ramvm::ArgType::VALUE: return value.ToString();
			case ramvm::ArgType::REGISTER: return "R" + std::to_string(value.AsInt());
			case ramvm::ArgType::MEM_REG: return "{R" + std::to_string(value.AsInt()) + "}";
			case ramvm::ArgType::STACK_PTR: return "SP";
			case ramvm::ArgType::STACK_REG: return "[R" + std::to_string(value.AsInt()) + "]";
			case ramvm::ArgType::SP_OFFSET: return "[" + std::to_string(value.AsInt()) + "]";
			case ramvm::ArgType::INVALID: return "INVAILD";
			default: return "Argument::ToString() - ArgType not handled!";
		}
	}

	InstrMove::InstrMove(DataType _dataType, Argument _src, Argument _dest)
		: Instruction(InstructionType::MOVE)
	{
		dataType = _dataType;
		src = _src;
		dest = _dest;
	}

	InstrBinop::InstrBinop(Binop _op, Argument _src1, Argument _src2, Argument _dest)
		: Instruction(InstructionType::BINOP)
	{
		op = _op;
		src1 = _src1;
		src2 = _src2;
		dest = _dest;
	}

	std::string InstrBinop::ToString()
	{
		std::stringstream ss;
		ss << BinopToString(op) << " ";
		ss << src1.ToString() << " ";
		ss << src2.ToString() << " ";
		ss << dest.ToString();
		return ss.str();
	}

	InstrUnop::InstrUnop(Unop _op, Argument _src, Argument _dest)
		: Instruction(InstructionType::UNOP)
	{
		op = _op;
		src = _src;
		dest = _dest;
	}

	std::string InstrUnop::ToString()
	{
		std::stringstream ss;
		ss << UnopToString(op) << " ";
		ss << src.ToString() << " ";
		ss << dest.ToString();
		return ss.str();
	}

	InstrCall::InstrCall(int _labelIdx, int _regCnt, std::vector<Argument>& _argSrcs, std::vector<Argument>& _retDests)
		: Instruction(InstructionType::CALL)
	{
		labelIdx = _labelIdx;
		regCnt = _regCnt;
		argSrcs = _argSrcs;
		retDests = _retDests;
	}

	InstrReturn::InstrReturn(std::vector<Argument>& _srcs)
		: Instruction(InstructionType::RETURN)
	{
		srcs = _srcs;
	}

	InstrJump::InstrJump(int _labelIdx)
		: Instruction(InstructionType::JUMP)
	{
		labelIdx = _labelIdx;
	}

	InstrCJump::InstrCJump(int _labelIdx, Argument _condSrc)
		: Instruction(InstructionType::CJUMP)
	{
		labelIdx = _labelIdx;
		condSrc = _condSrc;
	}

	InstrPrint::InstrPrint(Argument _start, Argument _len)
		: Instruction(InstructionType::PRINT)
	{
		start = _start;
		length = _len;
	}

	InstrMalloc::InstrMalloc(Argument _size, Argument _dest)
		: Instruction(InstructionType::MALLOC)
	{
		size = _size;
		dest = _dest;
	}

	InstrFree::InstrFree(Argument _addr)
		: Instruction(InstructionType::FREE)
	{
		addr = _addr;
	}

	InstrPush::InstrPush(Argument _src)
		: Instruction(InstructionType::PUSH)
	{
		srcs = { _src };
	}

	InstrPush::InstrPush(std::vector<Argument>& _srcs)
		: Instruction(InstructionType::PUSH)
	{
		srcs = _srcs;
	}

	std::string InstrPush::ToString()
	{
		std::stringstream ss;
		ss << "PUSH";

		for (auto src : srcs)
			ss << " " << src.ToString();

		return ss.str();
	}

	InstrPop::InstrPop(Argument _amt, int _scale)
		: Instruction(InstructionType::POP)
	{
		amt = _amt;
		scale = _scale;
	}
}
