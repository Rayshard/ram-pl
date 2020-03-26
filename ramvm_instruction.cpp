#include "pch.h"
#include "ramvm_instruction.h"

namespace ramvm {
	const std::string UnopToString(Unop _type)
	{
		switch (_type)
		{
			case Unop::NEG: return "NEG";
			case Unop::LOG_NOT: return "LNOT";
			case Unop::BIN_NOT: return "BNOT";
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
			case ramvm::Binop::POW: return "POW";
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
		switch (ConcatTriple((byte)_val1.GetType(), (byte)_val2.GetType(), (byte)_op))
		{
#pragma region ADD
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::BYTE, (byte)Binop::ADD): _result = DataVariant(byte(_val1.B() + _val2.B())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::INT, (byte)Binop::ADD): _result = DataVariant(int(_val1.B() + _val2.I())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::FLOAT, (byte)Binop::ADD): _result = DataVariant(float(_val1.B() + _val2.F())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::DOUBLE, (byte)Binop::ADD): _result = DataVariant(double(_val1.B() + _val2.D())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::LONG, (byte)Binop::ADD): _result = DataVariant(rLong(_val1.B() + _val2.L())); break;

			case ConcatTriple((byte)DataType::INT, (byte)DataType::BYTE, (byte)Binop::ADD): _result = DataVariant(int(_val1.I() + _val2.B())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::INT, (byte)Binop::ADD): _result = DataVariant(int(_val1.I() + _val2.I())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::FLOAT, (byte)Binop::ADD): _result = DataVariant(float(_val1.I() + _val2.F())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::DOUBLE, (byte)Binop::ADD): _result = DataVariant(double(_val1.I() + _val2.D())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::LONG, (byte)Binop::ADD): _result = DataVariant(rLong(_val1.I() + _val2.L())); break;

			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::BYTE, (byte)Binop::ADD): _result = DataVariant(float(_val1.F() + _val2.B())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::INT, (byte)Binop::ADD): _result = DataVariant(float(_val1.F() + _val2.I())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::FLOAT, (byte)Binop::ADD): _result = DataVariant(float(_val1.F() + _val2.F())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::DOUBLE, (byte)Binop::ADD): _result = DataVariant(double(_val1.F() + _val2.D())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::LONG, (byte)Binop::ADD): _result = DataVariant(rLong((double)_val1.F() + _val2.L())); break;

			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::BYTE, (byte)Binop::ADD): _result = DataVariant(double(_val1.D() + _val2.B())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::INT, (byte)Binop::ADD): _result = DataVariant(double(_val1.D() + _val2.I())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::FLOAT, (byte)Binop::ADD): _result = DataVariant(double(_val1.D() + _val2.F())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::DOUBLE, (byte)Binop::ADD): _result = DataVariant(double(_val1.D() + _val2.D())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::LONG, (byte)Binop::ADD): _result = DataVariant(double(_val1.D() + _val2.L())); break;

			case ConcatTriple((byte)DataType::LONG, (byte)DataType::BYTE, (byte)Binop::ADD): _result = DataVariant(rLong(_val1.L() + _val2.B())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::INT, (byte)Binop::ADD): _result = DataVariant(rLong(_val1.L() + _val2.I())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::FLOAT, (byte)Binop::ADD): _result = DataVariant(rLong(_val1.L() + (double)_val2.F())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::DOUBLE, (byte)Binop::ADD): _result = DataVariant(double(_val1.L() + _val2.D())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::LONG, (byte)Binop::ADD): _result = DataVariant(rLong(_val1.L() + _val2.L())); break;
#pragma endregion

#pragma region SUB
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::BYTE, (byte)Binop::SUB): _result = DataVariant(byte(_val1.B() - _val2.B())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::INT, (byte)Binop::SUB): _result = DataVariant(int(_val1.B() - _val2.I())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::FLOAT, (byte)Binop::SUB): _result = DataVariant(float(_val1.B() - _val2.F())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::DOUBLE, (byte)Binop::SUB): _result = DataVariant(double(_val1.B() - _val2.D())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::LONG, (byte)Binop::SUB): _result = DataVariant(rLong(_val1.B() - _val2.L())); break;

			case ConcatTriple((byte)DataType::INT, (byte)DataType::BYTE, (byte)Binop::SUB): _result = DataVariant(int(_val1.I() - _val2.B())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::INT, (byte)Binop::SUB): _result = DataVariant(int(_val1.I() - _val2.I())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::FLOAT, (byte)Binop::SUB): _result = DataVariant(float(_val1.I() - _val2.F())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::DOUBLE, (byte)Binop::SUB): _result = DataVariant(double(_val1.I() - _val2.D())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::LONG, (byte)Binop::SUB): _result = DataVariant(rLong(_val1.I() - _val2.L())); break;

			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::BYTE, (byte)Binop::SUB): _result = DataVariant(float(_val1.F() - _val2.B())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::INT, (byte)Binop::SUB): _result = DataVariant(float(_val1.F() - _val2.I())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::FLOAT, (byte)Binop::SUB): _result = DataVariant(float(_val1.F() - _val2.F())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::DOUBLE, (byte)Binop::SUB): _result = DataVariant(double(_val1.F() - _val2.D())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::LONG, (byte)Binop::SUB): _result = DataVariant(rLong((double)_val1.F() - _val2.L())); break;

			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::BYTE, (byte)Binop::SUB): _result = DataVariant(double(_val1.D() - _val2.B())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::INT, (byte)Binop::SUB): _result = DataVariant(double(_val1.D() - _val2.I())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::FLOAT, (byte)Binop::SUB): _result = DataVariant(double(_val1.D() - _val2.F())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::DOUBLE, (byte)Binop::SUB): _result = DataVariant(double(_val1.D() - _val2.D())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::LONG, (byte)Binop::SUB): _result = DataVariant(double(_val1.D() - _val2.L())); break;

			case ConcatTriple((byte)DataType::LONG, (byte)DataType::BYTE, (byte)Binop::SUB): _result = DataVariant(rLong(_val1.L() - _val2.B())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::INT, (byte)Binop::SUB): _result = DataVariant(rLong(_val1.L() - _val2.I())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::FLOAT, (byte)Binop::SUB): _result = DataVariant(rLong(_val1.L() - (double)_val2.F())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::DOUBLE, (byte)Binop::SUB): _result = DataVariant(double(_val1.L() - _val2.D())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::LONG, (byte)Binop::SUB): _result = DataVariant(rLong(_val1.L() - _val2.L())); break;
#pragma endregion

#pragma region MUL
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::BYTE, (byte)Binop::MUL): _result = DataVariant(byte(_val1.B() * _val2.B())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::INT, (byte)Binop::MUL): _result = DataVariant(int(_val1.B() * _val2.I())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::FLOAT, (byte)Binop::MUL): _result = DataVariant(float(_val1.B() * _val2.F())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::DOUBLE, (byte)Binop::MUL): _result = DataVariant(double(_val1.B() * _val2.D())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::LONG, (byte)Binop::MUL): _result = DataVariant(rLong(_val1.B() * _val2.L())); break;

			case ConcatTriple((byte)DataType::INT, (byte)DataType::BYTE, (byte)Binop::MUL): _result = DataVariant(int(_val1.I() * _val2.B())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::INT, (byte)Binop::MUL): _result = DataVariant(int(_val1.I() * _val2.I())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::FLOAT, (byte)Binop::MUL): _result = DataVariant(float(_val1.I() * _val2.F())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::DOUBLE, (byte)Binop::MUL): _result = DataVariant(double(_val1.I() * _val2.D())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::LONG, (byte)Binop::MUL): _result = DataVariant(rLong(_val1.I() * _val2.L())); break;

			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::BYTE, (byte)Binop::MUL): _result = DataVariant(float(_val1.F() * _val2.B())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::INT, (byte)Binop::MUL): _result = DataVariant(float(_val1.F() * _val2.I())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::FLOAT, (byte)Binop::MUL): _result = DataVariant(float(_val1.F() * _val2.F())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::DOUBLE, (byte)Binop::MUL): _result = DataVariant(double(_val1.F() * _val2.D())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::LONG, (byte)Binop::MUL): _result = DataVariant(rLong((double)_val1.F() * _val2.L())); break;

			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::BYTE, (byte)Binop::MUL): _result = DataVariant(double(_val1.D() * _val2.B())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::INT, (byte)Binop::MUL): _result = DataVariant(double(_val1.D() * _val2.I())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::FLOAT, (byte)Binop::MUL): _result = DataVariant(double(_val1.D() * _val2.F())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::DOUBLE, (byte)Binop::MUL): _result = DataVariant(double(_val1.D() * _val2.D())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::LONG, (byte)Binop::MUL): _result = DataVariant(double(_val1.D() * _val2.L())); break;

			case ConcatTriple((byte)DataType::LONG, (byte)DataType::BYTE, (byte)Binop::MUL): _result = DataVariant(rLong(_val1.L() * _val2.B())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::INT, (byte)Binop::MUL): _result = DataVariant(rLong(_val1.L() * _val2.I())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::FLOAT, (byte)Binop::MUL): _result = DataVariant(rLong(_val1.L() * (double)_val2.F())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::DOUBLE, (byte)Binop::MUL): _result = DataVariant(double(_val1.L() * _val2.D())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::LONG, (byte)Binop::MUL): _result = DataVariant(rLong(_val1.L() * _val2.L())); break;
#pragma endregion

#pragma region DIV
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::BYTE, (byte)Binop::DIV): {
				if (_val2.B() != 0) { _result = DataVariant(byte(_val1.B() / _val2.B())); }
				else { return ResultType::ERR_DIVBYZERO; }
			} break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::INT, (byte)Binop::DIV): {
				if (_val2.I() != 0) { _result = DataVariant(int(_val1.B() / _val2.I())); }
				else { return ResultType::ERR_DIVBYZERO; }
			} break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::FLOAT, (byte)Binop::DIV): {
				if (_val2.F() != 0) { _result = DataVariant(float(_val1.B() / _val2.F())); }
				else { return ResultType::ERR_DIVBYZERO; }
			} break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::DOUBLE, (byte)Binop::DIV): {
				if (_val2.D() != 0) { _result = DataVariant(double(_val1.B() / _val2.D())); }
				else { return ResultType::ERR_DIVBYZERO; }
			} break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::LONG, (byte)Binop::DIV): {
				if (_val2.I() != 0) { _result = DataVariant(rLong(_val1.B() / _val2.L())); }
				else { return ResultType::ERR_DIVBYZERO; }
			} break;

			case ConcatTriple((byte)DataType::INT, (byte)DataType::BYTE, (byte)Binop::DIV): {
				if (_val2.B() != 0) { _result = DataVariant(int(_val1.I() / _val2.B())); }
				else { return ResultType::ERR_DIVBYZERO; }
			} break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::INT, (byte)Binop::DIV): {
				if (_val2.I() != 0) { _result = DataVariant(int(_val1.I() / _val2.I())); }
				else { return ResultType::ERR_DIVBYZERO; }
			} break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::FLOAT, (byte)Binop::DIV): {
				if (_val2.F() != 0) { _result = DataVariant(float(_val1.I() / _val2.F())); }
				else { return ResultType::ERR_DIVBYZERO; }
			} break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::DOUBLE, (byte)Binop::DIV): {
				if (_val2.D() != 0) { _result = DataVariant(double(_val1.I() / _val2.D())); }
				else { return ResultType::ERR_DIVBYZERO; }
			} break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::LONG, (byte)Binop::DIV): {
				if (_val2.I() != 0) { _result = DataVariant(rLong(_val1.I() / _val2.L())); }
				else { return ResultType::ERR_DIVBYZERO; }
			} break;

			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::BYTE, (byte)Binop::DIV): {
				if (_val2.B() != 0) { _result = DataVariant(float(_val1.F() / _val2.B())); }
				else { return ResultType::ERR_DIVBYZERO; }
			} break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::INT, (byte)Binop::DIV): {
				if (_val2.I() != 0) { _result = DataVariant(float(_val1.F() / _val2.I())); }
				else { return ResultType::ERR_DIVBYZERO; }
			} break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::FLOAT, (byte)Binop::DIV): {
				if (_val2.F() != 0) { _result = DataVariant(float(_val1.F() / _val2.F())); }
				else { return ResultType::ERR_DIVBYZERO; }
			} break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::DOUBLE, (byte)Binop::DIV): {
				if (_val2.D() != 0) { _result = DataVariant(double(_val1.F() / _val2.D())); }
				else { return ResultType::ERR_DIVBYZERO; }
			} break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::LONG, (byte)Binop::DIV): {
				if (_val2.I() != 0) { _result = DataVariant(rLong(_val1.F() / _val2.L())); }
				else { return ResultType::ERR_DIVBYZERO; }
			} break;

			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::BYTE, (byte)Binop::DIV): {
				if (_val2.B() != 0) { _result = DataVariant(double(_val1.D() / _val2.B())); }
				else { return ResultType::ERR_DIVBYZERO; }
			} break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::INT, (byte)Binop::DIV): {
				if (_val2.I() != 0) { _result = DataVariant(double(_val1.D() / _val2.I())); }
				else { return ResultType::ERR_DIVBYZERO; }
			} break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::FLOAT, (byte)Binop::DIV): {
				if (_val2.F() != 0) { _result = DataVariant(double(_val1.D() / _val2.F())); }
				else { return ResultType::ERR_DIVBYZERO; }
			} break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::DOUBLE, (byte)Binop::DIV): {
				if (_val2.D() != 0) { _result = DataVariant(double(_val1.D() / _val2.D())); }
				else { return ResultType::ERR_DIVBYZERO; }
			} break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::LONG, (byte)Binop::DIV): {
				if (_val2.I() != 0) { _result = DataVariant(double(_val1.D() / _val2.L())); }
				else { return ResultType::ERR_DIVBYZERO; }
			} break;

			case ConcatTriple((byte)DataType::LONG, (byte)DataType::BYTE, (byte)Binop::DIV): {
				if (_val2.B() != 0) { _result = DataVariant(rLong(_val1.L() / _val2.B())); }
				else { return ResultType::ERR_DIVBYZERO; }
			} break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::INT, (byte)Binop::DIV): {
				if (_val2.I() != 0) { _result = DataVariant(rLong(_val1.L() / _val2.I())); }
				else { return ResultType::ERR_DIVBYZERO; }
			} break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::FLOAT, (byte)Binop::DIV): {
				if (_val2.F() != 0) { _result = DataVariant(rLong(_val1.L() / _val2.F())); }
				else { return ResultType::ERR_DIVBYZERO; }
			} break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::DOUBLE, (byte)Binop::DIV): {
				if (_val2.D() != 0) { _result = DataVariant(double(_val1.L() / _val2.D())); }
				else { return ResultType::ERR_DIVBYZERO; }
			} break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::LONG, (byte)Binop::DIV): {
				if (_val2.I() != 0) { _result = DataVariant(rLong(_val1.L() / _val2.L())); }
				else { return ResultType::ERR_DIVBYZERO; }
			} break;
#pragma endregion

#pragma region MOD
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::BYTE, (byte)Binop::MOD): {
				if (_val2.B() != 0) { _result = DataVariant(byte(_val1.B() % _val2.B())); }
				else { return ResultType::ERR_DIVBYZERO; }
			} break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::INT, (byte)Binop::MOD): {
				if (_val2.I() != 0) { _result = DataVariant(int(_val1.B() % _val2.I())); }
				else { return ResultType::ERR_DIVBYZERO; }
			} break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::LONG, (byte)Binop::MOD): {
				if (_val2.I() != 0) { _result = DataVariant(rLong(_val1.B() % _val2.L())); }
				else { return ResultType::ERR_DIVBYZERO; }
			} break;

			case ConcatTriple((byte)DataType::INT, (byte)DataType::BYTE, (byte)Binop::MOD): {
				if (_val2.B() != 0) { _result = DataVariant(int(_val1.I() % _val2.B())); }
				else { return ResultType::ERR_DIVBYZERO; }
			} break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::INT, (byte)Binop::MOD): {
				if (_val2.I() != 0) { _result = DataVariant(int(_val1.I() % _val2.I())); }
				else { return ResultType::ERR_DIVBYZERO; }
			} break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::LONG, (byte)Binop::MOD): {
				if (_val2.I() != 0) { _result = DataVariant(rLong(_val1.I() % _val2.L())); }
				else { return ResultType::ERR_DIVBYZERO; }
			} break;

			case ConcatTriple((byte)DataType::LONG, (byte)DataType::BYTE, (byte)Binop::MOD): {
				if (_val2.B() != 0) { _result = DataVariant(rLong(_val1.L() % _val2.B())); }
				else { return ResultType::ERR_DIVBYZERO; }
			} break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::INT, (byte)Binop::MOD): {
				if (_val2.I() != 0) { _result = DataVariant(rLong(_val1.L() % _val2.I())); }
				else { return ResultType::ERR_DIVBYZERO; }
			} break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::LONG, (byte)Binop::MOD): {
				if (_val2.I() != 0) { _result = DataVariant(rLong(_val1.L() % _val2.L())); }
				else { return ResultType::ERR_DIVBYZERO; }
			} break;
#pragma endregion

#pragma region POW
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::BYTE, (byte)Binop::POW): _result = DataVariant(byte(pow(_val1.B(), _val2.B()))); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::INT, (byte)Binop::POW): _result = DataVariant(int(pow(_val1.B(), _val2.I()))); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::FLOAT, (byte)Binop::POW): _result = DataVariant(float(pow(_val1.B(), _val2.F()))); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::DOUBLE, (byte)Binop::POW): _result = DataVariant(double(pow(_val1.B(), _val2.D()))); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::LONG, (byte)Binop::POW): _result = DataVariant(rLong(pow(_val1.B(), _val2.L()))); break;

			case ConcatTriple((byte)DataType::INT, (byte)DataType::BYTE, (byte)Binop::POW): _result = DataVariant(int(pow(_val1.I(), _val2.B()))); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::INT, (byte)Binop::POW): _result = DataVariant(int(pow(_val1.I(), _val2.I()))); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::FLOAT, (byte)Binop::POW): _result = DataVariant(float(pow(_val1.I(), _val2.F()))); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::DOUBLE, (byte)Binop::POW): _result = DataVariant(double(pow(_val1.I(), _val2.D()))); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::LONG, (byte)Binop::POW): _result = DataVariant(rLong(pow(_val1.I(), _val2.L()))); break;

			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::BYTE, (byte)Binop::POW): _result = DataVariant(float(pow(_val1.F(), _val2.B()))); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::INT, (byte)Binop::POW): _result = DataVariant(float(pow(_val1.F(), _val2.I()))); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::FLOAT, (byte)Binop::POW): _result = DataVariant(float(pow(_val1.F(), _val2.F()))); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::DOUBLE, (byte)Binop::POW): _result = DataVariant(double(pow(_val1.F(), _val2.D()))); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::LONG, (byte)Binop::POW): _result = DataVariant(rLong(pow(_val1.F(), _val2.L()))); break;

			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::BYTE, (byte)Binop::POW): _result = DataVariant(double(pow(_val1.D(), _val2.B()))); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::INT, (byte)Binop::POW): _result = DataVariant(double(pow(_val1.D(), _val2.I()))); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::FLOAT, (byte)Binop::POW): _result = DataVariant(double(pow(_val1.D(), _val2.F()))); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::DOUBLE, (byte)Binop::POW): _result = DataVariant(double(pow(_val1.D(), _val2.D()))); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::LONG, (byte)Binop::POW): _result = DataVariant(double(pow(_val1.D(), _val2.L()))); break;

			case ConcatTriple((byte)DataType::LONG, (byte)DataType::BYTE, (byte)Binop::POW): _result = DataVariant(rLong(pow(_val1.L(), _val2.B()))); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::INT, (byte)Binop::POW): _result = DataVariant(rLong(pow(_val1.L(), _val2.I()))); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::FLOAT, (byte)Binop::POW): _result = DataVariant(rLong(pow(_val1.L(), _val2.F()))); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::DOUBLE, (byte)Binop::POW): _result = DataVariant(double(pow(_val1.L(), _val2.D()))); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::LONG, (byte)Binop::POW): _result = DataVariant(rLong(pow(_val1.L(), _val2.L()))); break;
#pragma endregion

#pragma region LSHIFT
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::BYTE, (byte)Binop::LSHIFT): _result = DataVariant(byte(_val1.B() << _val2.B())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::INT, (byte)Binop::LSHIFT): _result = DataVariant(int(_val1.B() << _val2.I())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::LONG, (byte)Binop::LSHIFT): _result = DataVariant(rLong((rLong)_val1.B() << _val2.L())); break;

			case ConcatTriple((byte)DataType::INT, (byte)DataType::BYTE, (byte)Binop::LSHIFT): _result = DataVariant(int(_val1.I() << _val2.B())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::INT, (byte)Binop::LSHIFT): _result = DataVariant(int(_val1.I() << _val2.I())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::LONG, (byte)Binop::LSHIFT): _result = DataVariant(rLong((rLong)_val1.I() << _val2.L())); break;

			case ConcatTriple((byte)DataType::LONG, (byte)DataType::BYTE, (byte)Binop::LSHIFT): _result = DataVariant(rLong(_val1.L() << _val2.B())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::INT, (byte)Binop::LSHIFT): _result = DataVariant(rLong(_val1.L() << _val2.I())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::LONG, (byte)Binop::LSHIFT): _result = DataVariant(rLong(_val1.L() << _val2.L())); break;
#pragma endregion

#pragma region RSHIFT
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::BYTE, (byte)Binop::RSHIFT): _result = DataVariant(byte(_val1.B() >> _val2.B())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::INT, (byte)Binop::RSHIFT): _result = DataVariant(int(_val1.B() >> _val2.I())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::LONG, (byte)Binop::RSHIFT): _result = DataVariant(rLong(_val1.B() >> _val2.L())); break;

			case ConcatTriple((byte)DataType::INT, (byte)DataType::BYTE, (byte)Binop::RSHIFT): _result = DataVariant(int(_val1.I() >> _val2.B())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::INT, (byte)Binop::RSHIFT): _result = DataVariant(int(_val1.I() >> _val2.I())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::LONG, (byte)Binop::RSHIFT): _result = DataVariant(rLong(_val1.I() >> _val2.L())); break;

			case ConcatTriple((byte)DataType::LONG, (byte)DataType::BYTE, (byte)Binop::RSHIFT): _result = DataVariant(rLong(_val1.L() >> _val2.B())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::INT, (byte)Binop::RSHIFT): _result = DataVariant(rLong(_val1.L() >> _val2.I())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::LONG, (byte)Binop::RSHIFT): _result = DataVariant(rLong(_val1.L() >> _val2.L())); break;
#pragma endregion

#pragma region LT
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::BYTE, (byte)Binop::LT): _result = DataVariant(byte(_val1.B() < _val2.B())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::INT, (byte)Binop::LT): _result = DataVariant(byte(_val1.B() < _val2.I())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::FLOAT, (byte)Binop::LT): _result = DataVariant(byte(_val1.B() < _val2.F())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::DOUBLE, (byte)Binop::LT): _result = DataVariant(byte(_val1.B() < _val2.D())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::LONG, (byte)Binop::LT): _result = DataVariant(byte(_val1.B() < _val2.L())); break;

			case ConcatTriple((byte)DataType::INT, (byte)DataType::BYTE, (byte)Binop::LT): _result = DataVariant(byte(_val1.I() < _val2.B())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::INT, (byte)Binop::LT): _result = DataVariant(byte(_val1.I() < _val2.I())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::FLOAT, (byte)Binop::LT): _result = DataVariant(byte(_val1.I() < _val2.F())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::DOUBLE, (byte)Binop::LT): _result = DataVariant(byte(_val1.I() < _val2.D())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::LONG, (byte)Binop::LT): _result = DataVariant(byte(_val1.I() < _val2.L())); break;

			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::BYTE, (byte)Binop::LT): _result = DataVariant(byte(_val1.F() < _val2.B())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::INT, (byte)Binop::LT): _result = DataVariant(byte(_val1.F() < _val2.I())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::FLOAT, (byte)Binop::LT): _result = DataVariant(byte(_val1.F() < _val2.F())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::DOUBLE, (byte)Binop::LT): _result = DataVariant(byte(_val1.F() < _val2.D())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::LONG, (byte)Binop::LT): _result = DataVariant(byte(_val1.F() < _val2.L())); break;

			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::BYTE, (byte)Binop::LT): _result = DataVariant(byte(_val1.D() < _val2.B())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::INT, (byte)Binop::LT): _result = DataVariant(byte(_val1.D() < _val2.I())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::FLOAT, (byte)Binop::LT): _result = DataVariant(byte(_val1.D() < _val2.F())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::DOUBLE, (byte)Binop::LT): _result = DataVariant(byte(_val1.D() < _val2.D())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::LONG, (byte)Binop::LT): _result = DataVariant(byte(_val1.D() < _val2.L())); break;

			case ConcatTriple((byte)DataType::LONG, (byte)DataType::BYTE, (byte)Binop::LT): _result = DataVariant(byte(_val1.L() < _val2.B())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::INT, (byte)Binop::LT): _result = DataVariant(byte(_val1.L() < _val2.I())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::FLOAT, (byte)Binop::LT): _result = DataVariant(byte(_val1.L() < _val2.F())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::DOUBLE, (byte)Binop::LT): _result = DataVariant(byte(_val1.L() < _val2.D())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::LONG, (byte)Binop::LT): _result = DataVariant(byte(_val1.L() < _val2.L())); break;
#pragma endregion

#pragma region GT
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::BYTE, (byte)Binop::GT): _result = DataVariant(byte(_val1.B() > _val2.B())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::INT, (byte)Binop::GT): _result = DataVariant(byte(_val1.B() > _val2.I())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::FLOAT, (byte)Binop::GT): _result = DataVariant(byte(_val1.B() > _val2.F())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::DOUBLE, (byte)Binop::GT): _result = DataVariant(byte(_val1.B() > _val2.D())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::LONG, (byte)Binop::GT): _result = DataVariant(byte(_val1.B() > _val2.L())); break;

			case ConcatTriple((byte)DataType::INT, (byte)DataType::BYTE, (byte)Binop::GT): _result = DataVariant(byte(_val1.I() > _val2.B())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::INT, (byte)Binop::GT): _result = DataVariant(byte(_val1.I() > _val2.I())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::FLOAT, (byte)Binop::GT): _result = DataVariant(byte(_val1.I() > _val2.F())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::DOUBLE, (byte)Binop::GT): _result = DataVariant(byte(_val1.I() > _val2.D())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::LONG, (byte)Binop::GT): _result = DataVariant(byte(_val1.I() > _val2.L())); break;

			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::BYTE, (byte)Binop::GT): _result = DataVariant(byte(_val1.F() > _val2.B())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::INT, (byte)Binop::GT): _result = DataVariant(byte(_val1.F() > _val2.I())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::FLOAT, (byte)Binop::GT): _result = DataVariant(byte(_val1.F() > _val2.F())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::DOUBLE, (byte)Binop::GT): _result = DataVariant(byte(_val1.F() > _val2.D())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::LONG, (byte)Binop::GT): _result = DataVariant(byte(_val1.F() > _val2.L())); break;

			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::BYTE, (byte)Binop::GT): _result = DataVariant(byte(_val1.D() > _val2.B())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::INT, (byte)Binop::GT): _result = DataVariant(byte(_val1.D() > _val2.I())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::FLOAT, (byte)Binop::GT): _result = DataVariant(byte(_val1.D() > _val2.F())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::DOUBLE, (byte)Binop::GT): _result = DataVariant(byte(_val1.D() > _val2.D())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::LONG, (byte)Binop::GT): _result = DataVariant(byte(_val1.D() > _val2.L())); break;

			case ConcatTriple((byte)DataType::LONG, (byte)DataType::BYTE, (byte)Binop::GT): _result = DataVariant(byte(_val1.L() > _val2.B())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::INT, (byte)Binop::GT): _result = DataVariant(byte(_val1.L() > _val2.I())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::FLOAT, (byte)Binop::GT): _result = DataVariant(byte(_val1.L() > _val2.F())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::DOUBLE, (byte)Binop::GT): _result = DataVariant(byte(_val1.L() > _val2.D())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::LONG, (byte)Binop::GT): _result = DataVariant(byte(_val1.L() > _val2.L())); break;
#pragma endregion

#pragma region LTEQ
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::BYTE, (byte)Binop::LTEQ): _result = DataVariant(byte(_val1.B() <= _val2.B())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::INT, (byte)Binop::LTEQ): _result = DataVariant(byte(_val1.B() <= _val2.I())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::FLOAT, (byte)Binop::LTEQ): _result = DataVariant(byte(_val1.B() <= _val2.F())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::DOUBLE, (byte)Binop::LTEQ): _result = DataVariant(byte(_val1.B() <= _val2.D())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::LONG, (byte)Binop::LTEQ): _result = DataVariant(byte(_val1.B() <= _val2.L())); break;

			case ConcatTriple((byte)DataType::INT, (byte)DataType::BYTE, (byte)Binop::LTEQ): _result = DataVariant(byte(_val1.I() <= _val2.B())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::INT, (byte)Binop::LTEQ): _result = DataVariant(byte(_val1.I() <= _val2.I())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::FLOAT, (byte)Binop::LTEQ): _result = DataVariant(byte(_val1.I() <= _val2.F())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::DOUBLE, (byte)Binop::LTEQ): _result = DataVariant(byte(_val1.I() <= _val2.D())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::LONG, (byte)Binop::LTEQ): _result = DataVariant(byte(_val1.I() <= _val2.L())); break;

			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::BYTE, (byte)Binop::LTEQ): _result = DataVariant(byte(_val1.F() <= _val2.B())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::INT, (byte)Binop::LTEQ): _result = DataVariant(byte(_val1.F() <= _val2.I())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::FLOAT, (byte)Binop::LTEQ): _result = DataVariant(byte(_val1.F() <= _val2.F())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::DOUBLE, (byte)Binop::LTEQ): _result = DataVariant(byte(_val1.F() <= _val2.D())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::LONG, (byte)Binop::LTEQ): _result = DataVariant(byte(_val1.F() <= _val2.L())); break;

			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::BYTE, (byte)Binop::LTEQ): _result = DataVariant(byte(_val1.D() <= _val2.B())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::INT, (byte)Binop::LTEQ): _result = DataVariant(byte(_val1.D() <= _val2.I())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::FLOAT, (byte)Binop::LTEQ): _result = DataVariant(byte(_val1.D() <= _val2.F())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::DOUBLE, (byte)Binop::LTEQ): _result = DataVariant(byte(_val1.D() <= _val2.D())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::LONG, (byte)Binop::LTEQ): _result = DataVariant(byte(_val1.D() <= _val2.L())); break;

			case ConcatTriple((byte)DataType::LONG, (byte)DataType::BYTE, (byte)Binop::LTEQ): _result = DataVariant(byte(_val1.L() <= _val2.B())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::INT, (byte)Binop::LTEQ): _result = DataVariant(byte(_val1.L() <= _val2.I())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::FLOAT, (byte)Binop::LTEQ): _result = DataVariant(byte(_val1.L() <= _val2.F())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::DOUBLE, (byte)Binop::LTEQ): _result = DataVariant(byte(_val1.L() <= _val2.D())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::LONG, (byte)Binop::LTEQ): _result = DataVariant(byte(_val1.L() <= _val2.L())); break;
#pragma endregion

#pragma region GTEQ
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::BYTE, (byte)Binop::GTEQ): _result = DataVariant(byte(_val1.B() >= _val2.B())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::INT, (byte)Binop::GTEQ): _result = DataVariant(byte(_val1.B() >= _val2.I())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::FLOAT, (byte)Binop::GTEQ): _result = DataVariant(byte(_val1.B() >= _val2.F())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::DOUBLE, (byte)Binop::GTEQ): _result = DataVariant(byte(_val1.B() >= _val2.D())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::LONG, (byte)Binop::GTEQ): _result = DataVariant(byte(_val1.B() >= _val2.L())); break;

			case ConcatTriple((byte)DataType::INT, (byte)DataType::BYTE, (byte)Binop::GTEQ): _result = DataVariant(byte(_val1.I() >= _val2.B())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::INT, (byte)Binop::GTEQ): _result = DataVariant(byte(_val1.I() >= _val2.I())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::FLOAT, (byte)Binop::GTEQ): _result = DataVariant(byte(_val1.I() >= _val2.F())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::DOUBLE, (byte)Binop::GTEQ): _result = DataVariant(byte(_val1.I() >= _val2.D())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::LONG, (byte)Binop::GTEQ): _result = DataVariant(byte(_val1.I() >= _val2.L())); break;

			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::BYTE, (byte)Binop::GTEQ): _result = DataVariant(byte(_val1.F() >= _val2.B())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::INT, (byte)Binop::GTEQ): _result = DataVariant(byte(_val1.F() >= _val2.I())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::FLOAT, (byte)Binop::GTEQ): _result = DataVariant(byte(_val1.F() >= _val2.F())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::DOUBLE, (byte)Binop::GTEQ): _result = DataVariant(byte(_val1.F() >= _val2.D())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::LONG, (byte)Binop::GTEQ): _result = DataVariant(byte(_val1.F() >= _val2.L())); break;

			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::BYTE, (byte)Binop::GTEQ): _result = DataVariant(byte(_val1.D() >= _val2.B())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::INT, (byte)Binop::GTEQ): _result = DataVariant(byte(_val1.D() >= _val2.I())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::FLOAT, (byte)Binop::GTEQ): _result = DataVariant(byte(_val1.D() >= _val2.F())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::DOUBLE, (byte)Binop::GTEQ): _result = DataVariant(byte(_val1.D() >= _val2.D())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::LONG, (byte)Binop::GTEQ): _result = DataVariant(byte(_val1.D() >= _val2.L())); break;

			case ConcatTriple((byte)DataType::LONG, (byte)DataType::BYTE, (byte)Binop::GTEQ): _result = DataVariant(byte(_val1.L() >= _val2.B())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::INT, (byte)Binop::GTEQ): _result = DataVariant(byte(_val1.L() >= _val2.I())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::FLOAT, (byte)Binop::GTEQ): _result = DataVariant(byte(_val1.L() >= _val2.F())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::DOUBLE, (byte)Binop::GTEQ): _result = DataVariant(byte(_val1.L() >= _val2.D())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::LONG, (byte)Binop::GTEQ): _result = DataVariant(byte(_val1.L() >= _val2.L())); break;
#pragma endregion

#pragma region EQ
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::BYTE, (byte)Binop::EQ): _result = DataVariant(byte(_val1.B() == _val2.B())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::INT, (byte)Binop::EQ): _result = DataVariant(byte(_val1.B() == _val2.I())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::FLOAT, (byte)Binop::EQ): _result = DataVariant(byte(_val1.B() == _val2.F())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::DOUBLE, (byte)Binop::EQ): _result = DataVariant(byte(_val1.B() == _val2.D())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::LONG, (byte)Binop::EQ): _result = DataVariant(byte(_val1.B() == _val2.L())); break;

			case ConcatTriple((byte)DataType::INT, (byte)DataType::BYTE, (byte)Binop::EQ): _result = DataVariant(byte(_val1.I() == _val2.B())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::INT, (byte)Binop::EQ): _result = DataVariant(byte(_val1.I() == _val2.I())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::FLOAT, (byte)Binop::EQ): _result = DataVariant(byte(_val1.I() == _val2.F())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::DOUBLE, (byte)Binop::EQ): _result = DataVariant(byte(_val1.I() == _val2.D())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::LONG, (byte)Binop::EQ): _result = DataVariant(byte(_val1.I() == _val2.L())); break;

			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::BYTE, (byte)Binop::EQ): _result = DataVariant(byte(_val1.F() == _val2.B())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::INT, (byte)Binop::EQ): _result = DataVariant(byte(_val1.F() == _val2.I())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::FLOAT, (byte)Binop::EQ): _result = DataVariant(byte(_val1.F() == _val2.F())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::DOUBLE, (byte)Binop::EQ): _result = DataVariant(byte(_val1.F() == _val2.D())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::LONG, (byte)Binop::EQ): _result = DataVariant(byte(_val1.F() == _val2.L())); break;

			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::BYTE, (byte)Binop::EQ): _result = DataVariant(byte(_val1.D() == _val2.B())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::INT, (byte)Binop::EQ): _result = DataVariant(byte(_val1.D() == _val2.I())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::FLOAT, (byte)Binop::EQ): _result = DataVariant(byte(_val1.D() == _val2.F())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::DOUBLE, (byte)Binop::EQ): _result = DataVariant(byte(_val1.D() == _val2.D())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::LONG, (byte)Binop::EQ): _result = DataVariant(byte(_val1.D() == _val2.L())); break;

			case ConcatTriple((byte)DataType::LONG, (byte)DataType::BYTE, (byte)Binop::EQ): _result = DataVariant(byte(_val1.L() == _val2.B())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::INT, (byte)Binop::EQ): _result = DataVariant(byte(_val1.L() == _val2.I())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::FLOAT, (byte)Binop::EQ): _result = DataVariant(byte(_val1.L() == _val2.F())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::DOUBLE, (byte)Binop::EQ): _result = DataVariant(byte(_val1.L() == _val2.D())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::LONG, (byte)Binop::EQ): _result = DataVariant(byte(_val1.L() == _val2.L())); break;
#pragma endregion

#pragma region NEQ
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::BYTE, (byte)Binop::NEQ): _result = DataVariant(byte(_val1.B() != _val2.B())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::INT, (byte)Binop::NEQ): _result = DataVariant(byte(_val1.B() != _val2.I())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::FLOAT, (byte)Binop::NEQ): _result = DataVariant(byte(_val1.B() != _val2.F())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::DOUBLE, (byte)Binop::NEQ): _result = DataVariant(byte(_val1.B() != _val2.D())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::LONG, (byte)Binop::NEQ): _result = DataVariant(byte(_val1.B() != _val2.L())); break;

			case ConcatTriple((byte)DataType::INT, (byte)DataType::BYTE, (byte)Binop::NEQ): _result = DataVariant(byte(_val1.I() != _val2.B())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::INT, (byte)Binop::NEQ): _result = DataVariant(byte(_val1.I() != _val2.I())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::FLOAT, (byte)Binop::NEQ): _result = DataVariant(byte(_val1.I() != _val2.F())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::DOUBLE, (byte)Binop::NEQ): _result = DataVariant(byte(_val1.I() != _val2.D())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::LONG, (byte)Binop::NEQ): _result = DataVariant(byte(_val1.I() != _val2.L())); break;

			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::BYTE, (byte)Binop::NEQ): _result = DataVariant(byte(_val1.F() != _val2.B())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::INT, (byte)Binop::NEQ): _result = DataVariant(byte(_val1.F() != _val2.I())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::FLOAT, (byte)Binop::NEQ): _result = DataVariant(byte(_val1.F() != _val2.F())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::DOUBLE, (byte)Binop::NEQ): _result = DataVariant(byte(_val1.F() != _val2.D())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::LONG, (byte)Binop::NEQ): _result = DataVariant(byte(_val1.F() != _val2.L())); break;

			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::BYTE, (byte)Binop::NEQ): _result = DataVariant(byte(_val1.D() != _val2.B())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::INT, (byte)Binop::NEQ): _result = DataVariant(byte(_val1.D() != _val2.I())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::FLOAT, (byte)Binop::NEQ): _result = DataVariant(byte(_val1.D() != _val2.F())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::DOUBLE, (byte)Binop::NEQ): _result = DataVariant(byte(_val1.D() != _val2.D())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::LONG, (byte)Binop::NEQ): _result = DataVariant(byte(_val1.D() != _val2.L())); break;

			case ConcatTriple((byte)DataType::LONG, (byte)DataType::BYTE, (byte)Binop::NEQ): _result = DataVariant(byte(_val1.L() != _val2.B())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::INT, (byte)Binop::NEQ): _result = DataVariant(byte(_val1.L() != _val2.I())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::FLOAT, (byte)Binop::NEQ): _result = DataVariant(byte(_val1.L() != _val2.F())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::DOUBLE, (byte)Binop::NEQ): _result = DataVariant(byte(_val1.L() != _val2.D())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::LONG, (byte)Binop::NEQ): _result = DataVariant(byte(_val1.L() != _val2.L())); break;
#pragma endregion

#pragma region BIT_AND
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::BYTE, (byte)Binop::BIT_AND): _result = DataVariant(byte(_val1.B() & _val2.B())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::INT, (byte)Binop::BIT_AND): _result = DataVariant(int(_val1.B() & _val2.I())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::LONG, (byte)Binop::BIT_AND): _result = DataVariant(rLong(_val1.B() & _val2.L())); break;

			case ConcatTriple((byte)DataType::INT, (byte)DataType::BYTE, (byte)Binop::BIT_AND): _result = DataVariant(int(_val1.I() & _val2.B())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::INT, (byte)Binop::BIT_AND): _result = DataVariant(int(_val1.I() & _val2.I())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::LONG, (byte)Binop::BIT_AND): _result = DataVariant(rLong(_val1.I() & _val2.L())); break;


			case ConcatTriple((byte)DataType::LONG, (byte)DataType::BYTE, (byte)Binop::BIT_AND): _result = DataVariant(rLong(_val1.L() & _val2.B())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::INT, (byte)Binop::BIT_AND): _result = DataVariant(rLong(_val1.L() & _val2.I())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::LONG, (byte)Binop::BIT_AND): _result = DataVariant(rLong(_val1.L() & _val2.L())); break;
#pragma endregion

#pragma region BIT_OR
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::BYTE, (byte)Binop::BIT_OR): _result = DataVariant(byte(_val1.B() | _val2.B())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::INT, (byte)Binop::BIT_OR): _result = DataVariant(int(_val1.B() | _val2.I())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::LONG, (byte)Binop::BIT_OR): _result = DataVariant(rLong(_val1.B() | _val2.L())); break;

			case ConcatTriple((byte)DataType::INT, (byte)DataType::BYTE, (byte)Binop::BIT_OR): _result = DataVariant(int(_val1.I() | _val2.B())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::INT, (byte)Binop::BIT_OR): _result = DataVariant(int(_val1.I() | _val2.I())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::LONG, (byte)Binop::BIT_OR): _result = DataVariant(rLong(_val1.I() | _val2.L())); break;


			case ConcatTriple((byte)DataType::LONG, (byte)DataType::BYTE, (byte)Binop::BIT_OR): _result = DataVariant(rLong(_val1.L() | _val2.B())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::INT, (byte)Binop::BIT_OR): _result = DataVariant(rLong(_val1.L() | _val2.I())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::LONG, (byte)Binop::BIT_OR): _result = DataVariant(rLong(_val1.L() | _val2.L())); break;
#pragma endregion

#pragma region BIT_XOR
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::BYTE, (byte)Binop::BIT_XOR): _result = DataVariant(byte(_val1.B() ^ _val2.B())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::INT, (byte)Binop::BIT_XOR): _result = DataVariant(int(_val1.B() ^ _val2.I())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::LONG, (byte)Binop::BIT_XOR): _result = DataVariant(rLong(_val1.B() ^ _val2.L())); break;

			case ConcatTriple((byte)DataType::INT, (byte)DataType::BYTE, (byte)Binop::BIT_XOR): _result = DataVariant(int(_val1.I() ^ _val2.B())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::INT, (byte)Binop::BIT_XOR): _result = DataVariant(int(_val1.I() ^ _val2.I())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::LONG, (byte)Binop::BIT_XOR): _result = DataVariant(rLong(_val1.I() ^ _val2.L())); break;


			case ConcatTriple((byte)DataType::LONG, (byte)DataType::BYTE, (byte)Binop::BIT_XOR): _result = DataVariant(rLong(_val1.L() ^ _val2.B())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::INT, (byte)Binop::BIT_XOR): _result = DataVariant(rLong(_val1.L() ^ _val2.I())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::LONG, (byte)Binop::BIT_XOR): _result = DataVariant(rLong(_val1.L() ^ _val2.L())); break;
#pragma endregion

#pragma region LOG_AND
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::BYTE, (byte)Binop::LOG_AND): _result = DataVariant(byte(_val1.B() && _val2.B())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::INT, (byte)Binop::LOG_AND): _result = DataVariant(byte(_val1.B() && _val2.I())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::FLOAT, (byte)Binop::LOG_AND): _result = DataVariant(byte(_val1.B() && _val2.F())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::DOUBLE, (byte)Binop::LOG_AND): _result = DataVariant(byte(_val1.B() && _val2.D())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::LONG, (byte)Binop::LOG_AND): _result = DataVariant(byte(_val1.B() && _val2.L())); break;

			case ConcatTriple((byte)DataType::INT, (byte)DataType::BYTE, (byte)Binop::LOG_AND): _result = DataVariant(byte(_val1.I() && _val2.B())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::INT, (byte)Binop::LOG_AND): _result = DataVariant(byte(_val1.I() && _val2.I())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::FLOAT, (byte)Binop::LOG_AND): _result = DataVariant(byte(_val1.I() && _val2.F())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::DOUBLE, (byte)Binop::LOG_AND): _result = DataVariant(byte(_val1.I() && _val2.D())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::LONG, (byte)Binop::LOG_AND): _result = DataVariant(byte(_val1.I() && _val2.L())); break;

			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::BYTE, (byte)Binop::LOG_AND): _result = DataVariant(byte(_val1.F() && _val2.B())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::INT, (byte)Binop::LOG_AND): _result = DataVariant(byte(_val1.F() && _val2.I())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::FLOAT, (byte)Binop::LOG_AND): _result = DataVariant(byte(_val1.F() && _val2.F())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::DOUBLE, (byte)Binop::LOG_AND): _result = DataVariant(byte(_val1.F() && _val2.D())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::LONG, (byte)Binop::LOG_AND): _result = DataVariant(byte(_val1.F() && _val2.L())); break;

			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::BYTE, (byte)Binop::LOG_AND): _result = DataVariant(byte(_val1.D() && _val2.B())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::INT, (byte)Binop::LOG_AND): _result = DataVariant(byte(_val1.D() && _val2.I())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::FLOAT, (byte)Binop::LOG_AND): _result = DataVariant(byte(_val1.D() && _val2.F())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::DOUBLE, (byte)Binop::LOG_AND): _result = DataVariant(byte(_val1.D() && _val2.D())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::LONG, (byte)Binop::LOG_AND): _result = DataVariant(byte(_val1.D() && _val2.L())); break;

			case ConcatTriple((byte)DataType::LONG, (byte)DataType::BYTE, (byte)Binop::LOG_AND): _result = DataVariant(byte(_val1.L() && _val2.B())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::INT, (byte)Binop::LOG_AND): _result = DataVariant(byte(_val1.L() && _val2.I())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::FLOAT, (byte)Binop::LOG_AND): _result = DataVariant(byte(_val1.L() && _val2.F())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::DOUBLE, (byte)Binop::LOG_AND): _result = DataVariant(byte(_val1.L() && _val2.D())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::LONG, (byte)Binop::LOG_AND): _result = DataVariant(byte(_val1.L() && _val2.L())); break;
#pragma endregion

#pragma region LOG_OR
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::BYTE, (byte)Binop::LOG_OR): _result = DataVariant(byte(_val1.B() || _val2.B())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::INT, (byte)Binop::LOG_OR): _result = DataVariant(byte(_val1.B() || _val2.I())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::FLOAT, (byte)Binop::LOG_OR): _result = DataVariant(byte(_val1.B() || _val2.F())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::DOUBLE, (byte)Binop::LOG_OR): _result = DataVariant(byte(_val1.B() || _val2.D())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::LONG, (byte)Binop::LOG_OR): _result = DataVariant(byte(_val1.B() || _val2.L())); break;

			case ConcatTriple((byte)DataType::INT, (byte)DataType::BYTE, (byte)Binop::LOG_OR): _result = DataVariant(byte(_val1.I() || _val2.B())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::INT, (byte)Binop::LOG_OR): _result = DataVariant(byte(_val1.I() || _val2.I())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::FLOAT, (byte)Binop::LOG_OR): _result = DataVariant(byte(_val1.I() || _val2.F())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::DOUBLE, (byte)Binop::LOG_OR): _result = DataVariant(byte(_val1.I() || _val2.D())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::LONG, (byte)Binop::LOG_OR): _result = DataVariant(byte(_val1.I() || _val2.L())); break;

			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::BYTE, (byte)Binop::LOG_OR): _result = DataVariant(byte(_val1.F() || _val2.B())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::INT, (byte)Binop::LOG_OR): _result = DataVariant(byte(_val1.F() || _val2.I())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::FLOAT, (byte)Binop::LOG_OR): _result = DataVariant(byte(_val1.F() || _val2.F())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::DOUBLE, (byte)Binop::LOG_OR): _result = DataVariant(byte(_val1.F() || _val2.D())); break;
			case ConcatTriple((byte)DataType::FLOAT, (byte)DataType::LONG, (byte)Binop::LOG_OR): _result = DataVariant(byte(_val1.F() || _val2.L())); break;

			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::BYTE, (byte)Binop::LOG_OR): _result = DataVariant(byte(_val1.D() || _val2.B())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::INT, (byte)Binop::LOG_OR): _result = DataVariant(byte(_val1.D() || _val2.I())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::FLOAT, (byte)Binop::LOG_OR): _result = DataVariant(byte(_val1.D() || _val2.F())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::DOUBLE, (byte)Binop::LOG_OR): _result = DataVariant(byte(_val1.D() || _val2.D())); break;
			case ConcatTriple((byte)DataType::DOUBLE, (byte)DataType::LONG, (byte)Binop::LOG_OR): _result = DataVariant(byte(_val1.D() || _val2.L())); break;

			case ConcatTriple((byte)DataType::LONG, (byte)DataType::BYTE, (byte)Binop::LOG_OR): _result = DataVariant(byte(_val1.L() || _val2.B())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::INT, (byte)Binop::LOG_OR): _result = DataVariant(byte(_val1.L() || _val2.I())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::FLOAT, (byte)Binop::LOG_OR): _result = DataVariant(byte(_val1.L() || _val2.F())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::DOUBLE, (byte)Binop::LOG_OR): _result = DataVariant(byte(_val1.L() || _val2.D())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::LONG, (byte)Binop::LOG_OR): _result = DataVariant(byte(_val1.L() || _val2.L())); break;
#pragma endregion

			default: return ResultType::ERR_DO_BINOP;
		}

		return ResultType::SUCCESS;
	}

	ResultType DoUnop(Unop _op, DataVariant& _val, DataVariant& _result)
	{
		switch (ConcatDouble((byte)_val.GetType(), (byte)_op)) //Switch on Operation Type
		{
			case ConcatDouble((byte)DataType::BYTE, (byte)Unop::NEG): _result = DataVariant(byte(-_val.B())); break;
			case ConcatDouble((byte)DataType::BYTE, (byte)Unop::LOG_NOT): _result = DataVariant(byte(!_val.B())); break;
			case ConcatDouble((byte)DataType::BYTE, (byte)Unop::BIN_NOT): _result = DataVariant(byte(~_val.B())); break;

			case ConcatDouble((byte)DataType::INT, (byte)Unop::NEG): _result = DataVariant(int(-_val.I())); break;
			case ConcatDouble((byte)DataType::INT, (byte)Unop::LOG_NOT): _result = DataVariant(int(!_val.I())); break;
			case ConcatDouble((byte)DataType::INT, (byte)Unop::BIN_NOT): _result = DataVariant(int(~_val.B())); break;

			case ConcatDouble((byte)DataType::FLOAT, (byte)Unop::NEG): _result = DataVariant(float(-_val.F())); break;
			case ConcatDouble((byte)DataType::FLOAT, (byte)Unop::LOG_NOT): _result = DataVariant(float(!_val.F())); break;
			case ConcatDouble((byte)DataType::FLOAT, (byte)Unop::BIN_NOT): _result = DataVariant(float(~_val.B())); break;

			case ConcatDouble((byte)DataType::DOUBLE, (byte)Unop::NEG): _result = DataVariant(double(-_val.D())); break;
			case ConcatDouble((byte)DataType::DOUBLE, (byte)Unop::LOG_NOT): _result = DataVariant(double(!_val.D())); break;
			case ConcatDouble((byte)DataType::DOUBLE, (byte)Unop::BIN_NOT): _result = DataVariant(double(~_val.B())); break;

			case ConcatDouble((byte)DataType::LONG, (byte)Unop::NEG): _result = DataVariant(rLong(-_val.L())); break;
			case ConcatDouble((byte)DataType::LONG, (byte)Unop::LOG_NOT): _result = DataVariant(rLong(!_val.L())); break;
			case ConcatDouble((byte)DataType::LONG, (byte)Unop::BIN_NOT): _result = DataVariant(rLong(~_val.B())); break;

			default: return ResultType::ERR_DO_UNOP;
		}

		return ResultType::SUCCESS;
	}

	bool Instruction::IsSinglePop() { return type == InstructionType::POP && ((InstrPop*)this)->amt.type == ArgType::VALUE && ((InstrPop*)this)->amt.value.i == 1; }

	Argument::Argument(ArgType _type, DataValue _val)
	{
		type = _type;
		value = _val;
	}

	std::string Argument::ToString()
	{
		switch (type)
		{
			case ramvm::ArgType::VALUE: return ToHexString(value);
			case ramvm::ArgType::REGISTER: return "R" + std::to_string(value.i);
			case ramvm::ArgType::MEM_REG: return "{R" + std::to_string(value.i) + "}";
			case ramvm::ArgType::STACK_PTR: return "SP";
			case ramvm::ArgType::STACK_REG: return "[R" + std::to_string(value.i) + "]";
			case ramvm::ArgType::SP_OFFSET: return "[" + std::to_string(value.i) + "]";
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

	InstrBinop::InstrBinop(Binop _op, TypedArgument _src1, TypedArgument _src2, TypedArgument _dest)
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

	std::string InstrBinop::ToOutput()
	{
		std::stringstream ss;
		ss << BinopToString(op) << '<' << DataTypeToChar(src1.dataType) << DataTypeToChar(src2.dataType) << DataTypeToChar(dest.dataType) << "> ";
		ss << src1.ToString() << " ";
		ss << src2.ToString() << " ";
		ss << dest.ToString();
		return ss.str();
	}

	InstrUnop::InstrUnop(Unop _op, TypedArgument _src, TypedArgument _dest)
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

	std::string InstrUnop::ToOutput()
	{
		std::stringstream ss;
		ss << UnopToString(op) << '<' << DataTypeToChar(src.dataType) << DataTypeToChar(dest.dataType) << "> ";
		ss << src.ToString() << " ";
		ss << dest.ToString();
		return ss.str();
	}

	InstrCall::InstrCall(int _labelIdx, int _regCnt, const std::vector<TypedArgument>& _argSrcs)
		: Instruction(InstructionType::CALL)
	{
		labelIdx = _labelIdx;
		regCnt = _regCnt;
		argSrcs = _argSrcs;
	}

	std::string InstrCall::ToString()
	{
		std::stringstream ss;
		ss << "CALL " << labelIdx << " " << regCnt;

		for (auto src : argSrcs)
			ss << " " << src.ToString();

		return ss.str();
	}

	std::string InstrCall::ToOutput()
	{
		std::stringstream ss;
		ss << "CALL<";

		std::string argsStr;
		for (auto src : argSrcs)
		{
			ss << DataTypeToChar(src.dataType);
			argsStr += " " + src.ToString();
		}

		ss << "> " << labelIdx << " " << regCnt << argsStr;
		return ss.str();
	}

	InstrReturn::InstrReturn(const std::vector<TypedArgument>& _srcs)
		: Instruction(InstructionType::RETURN)
	{
		srcs = _srcs;
	}

	std::string InstrReturn::ToString()
	{
		std::stringstream ss;
		ss << "RET";

		for (auto src : srcs)
			ss << " " << src.ToString();

		return ss.str();
	}

	std::string InstrReturn::ToOutput()
	{
		std::stringstream ss;
		ss << "RET<";

		std::string srcsStr;
		for (auto src : srcs)
		{
			ss << DataTypeToChar(src.dataType);
			srcsStr += " " + src.ToString();
		}

		ss << ">" << srcsStr;
		return ss.str();
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

	InstrPush::InstrPush(const std::vector<TypedArgument>& _srcs)
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

	std::string InstrPush::ToOutput()
	{
		std::stringstream ss;
		ss << "PUSH<";

		std::string srcsStr;
		for (auto src : srcs)
		{
			ss << DataTypeToChar(src.dataType);
			srcsStr += " " + src.ToString();
		}

		ss << ">" << srcsStr;
		return ss.str();
	}

	InstrPop::InstrPop(DataType _type, Argument _amt)
		: Instruction(InstructionType::POP)
	{
		amt = _amt;
		scale = GetDataTypeSize(_type);
	}

	InstrStore::InstrStore(const std::vector<TypedArgument>& _srcs, Argument _dest)
		: Instruction(InstructionType::STORE)
	{
		srcs = _srcs;
		dest = _dest;
	}

	std::string InstrStore::ToString()
	{
		std::stringstream ss;
		ss << "STORE";

		for (auto src : srcs)
			ss << " " << src.ToString();

		ss << "" << dest.ToString();
		return ss.str();
	}

	std::string InstrStore::ToOutput()
	{
		std::stringstream ss;
		ss << "STORE<";

		std::string srcsStr;
		for (auto src : srcs)
		{
			ss << DataTypeToChar(src.dataType);
			srcsStr += " " + src.ToString();
		}

		ss << ">" << srcsStr << " " << dest.ToString();
		return ss.str();
	}
}
