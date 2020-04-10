#include "pch.h"
#include "ramvm_instruction.h"

namespace ramvm {
#pragma region Move
	InstrMove::InstrMove(DataType _dataType, Argument* _src, Argument* _dest)
		: Instruction(InstructionType::MOVE), dataType(_dataType), src(_src), dest(_dest) { }

	InstrMove::~InstrMove()
	{
		delete src;
		delete dest;
	}
#pragma endregion

#pragma region Binop
	const std::string BinopToString(Binop _type)
	{
		switch (_type)
		{
			case Binop::ADD: return "ADD";
			case Binop::SUB: return "SUB";
			case Binop::MUL: return "MUL";
			case Binop::DIV: return "DIV";
			case Binop::MOD: return "MOD";
			case Binop::POW: return "POW";
			case Binop::LSHIFT: return "LSHIFT";
			case Binop::RSHIFT: return "RSHIFT";
			case Binop::LT: return "LT";
			case Binop::GT: return "GT";
			case Binop::LTEQ: return "LTEQ";
			case Binop::GTEQ: return "GTEQ";
			case Binop::EQ: return "EQ";
			case Binop::NEQ: return "NEQ";
			case Binop::AND: return "AND";
			case Binop::OR: return "OR";
			case Binop::XOR: return "XOR";
			default: ASSERT_MSG(false, "BinopToString - Binop not handled!");
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

#pragma region AND
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::BYTE, (byte)Binop::AND): _result = DataVariant(byte(_val1.B() & _val2.B())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::INT, (byte)Binop::AND): _result = DataVariant(int(_val1.B() & _val2.I())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::LONG, (byte)Binop::AND): _result = DataVariant(rLong(_val1.B() & _val2.L())); break;

			case ConcatTriple((byte)DataType::INT, (byte)DataType::BYTE, (byte)Binop::AND): _result = DataVariant(int(_val1.I() & _val2.B())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::INT, (byte)Binop::AND): _result = DataVariant(int(_val1.I() & _val2.I())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::LONG, (byte)Binop::AND): _result = DataVariant(rLong(_val1.I() & _val2.L())); break;


			case ConcatTriple((byte)DataType::LONG, (byte)DataType::BYTE, (byte)Binop::AND): _result = DataVariant(rLong(_val1.L() & _val2.B())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::INT, (byte)Binop::AND): _result = DataVariant(rLong(_val1.L() & _val2.I())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::LONG, (byte)Binop::AND): _result = DataVariant(rLong(_val1.L() & _val2.L())); break;
#pragma endregion

#pragma region OR
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::BYTE, (byte)Binop::OR): _result = DataVariant(byte(_val1.B() | _val2.B())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::INT, (byte)Binop::OR): _result = DataVariant(int(_val1.B() | _val2.I())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::LONG, (byte)Binop::OR): _result = DataVariant(rLong(_val1.B() | _val2.L())); break;

			case ConcatTriple((byte)DataType::INT, (byte)DataType::BYTE, (byte)Binop::OR): _result = DataVariant(int(_val1.I() | _val2.B())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::INT, (byte)Binop::OR): _result = DataVariant(int(_val1.I() | _val2.I())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::LONG, (byte)Binop::OR): _result = DataVariant(rLong(_val1.I() | _val2.L())); break;


			case ConcatTriple((byte)DataType::LONG, (byte)DataType::BYTE, (byte)Binop::OR): _result = DataVariant(rLong(_val1.L() | _val2.B())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::INT, (byte)Binop::OR): _result = DataVariant(rLong(_val1.L() | _val2.I())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::LONG, (byte)Binop::OR): _result = DataVariant(rLong(_val1.L() | _val2.L())); break;
#pragma endregion

#pragma region XOR
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::BYTE, (byte)Binop::XOR): _result = DataVariant(byte(_val1.B() ^ _val2.B())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::INT, (byte)Binop::XOR): _result = DataVariant(int(_val1.B() ^ _val2.I())); break;
			case ConcatTriple((byte)DataType::BYTE, (byte)DataType::LONG, (byte)Binop::XOR): _result = DataVariant(rLong(_val1.B() ^ _val2.L())); break;

			case ConcatTriple((byte)DataType::INT, (byte)DataType::BYTE, (byte)Binop::XOR): _result = DataVariant(int(_val1.I() ^ _val2.B())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::INT, (byte)Binop::XOR): _result = DataVariant(int(_val1.I() ^ _val2.I())); break;
			case ConcatTriple((byte)DataType::INT, (byte)DataType::LONG, (byte)Binop::XOR): _result = DataVariant(rLong(_val1.I() ^ _val2.L())); break;


			case ConcatTriple((byte)DataType::LONG, (byte)DataType::BYTE, (byte)Binop::XOR): _result = DataVariant(rLong(_val1.L() ^ _val2.B())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::INT, (byte)Binop::XOR): _result = DataVariant(rLong(_val1.L() ^ _val2.I())); break;
			case ConcatTriple((byte)DataType::LONG, (byte)DataType::LONG, (byte)Binop::XOR): _result = DataVariant(rLong(_val1.L() ^ _val2.L())); break;
#pragma endregion

			default: return ResultType::ERR_DO_BINOP;
		}

		return ResultType::SUCCESS;
	}

	InstrBinop::InstrBinop(Binop _op, DataTypeTriple _argDataTypes, Argument* _src1, Argument* _src2, Argument* _dest)
		: Instruction(InstructionType::BINOP), op(_op), argDataTypes(_argDataTypes), src1(_src1), src2(_src2), dest(_dest) { }

	InstrBinop::~InstrBinop()
	{
		delete src1;
		delete src2;
		delete dest;
	}

	std::string InstrBinop::ToString()
	{
		std::stringstream ss;
		ss << BinopToString(op) << '<';
		ss << DataTypeToChar(std::get<0>(argDataTypes));
		ss << DataTypeToChar(std::get<1>(argDataTypes));
		ss << DataTypeToChar(std::get<2>(argDataTypes)) << ">(";
		ss << src1->ToString() << ", ";
		ss << src2->ToString() << ", ";
		ss << dest->ToString() << ")";
		return ss.str();
	}
#pragma endregion

#pragma region Unop
	const std::string UnopToString(Unop _type)
	{
		switch (_type)
		{
			case Unop::NEG: return "NEG";
			case Unop::NOT: return "NOT";
			default: return "UnopToString - Unop not handled!";
		}
	}

	ResultType DoUnop(Unop _op, DataVariant& _val, DataVariant& _result)
	{
		switch (ConcatDouble((byte)_val.GetType(), (byte)_op)) //Switch on Operation Type
		{
			case ConcatDouble((byte)DataType::BYTE, (byte)Unop::NEG): _result = DataVariant(byte(-_val.B())); break;
			case ConcatDouble((byte)DataType::BYTE, (byte)Unop::NOT): _result = DataVariant(byte(~_val.B())); break;

			case ConcatDouble((byte)DataType::INT, (byte)Unop::NEG): _result = DataVariant(int(-_val.I())); break;
			case ConcatDouble((byte)DataType::INT, (byte)Unop::NOT): _result = DataVariant(int(~_val.B())); break;

			case ConcatDouble((byte)DataType::FLOAT, (byte)Unop::NEG): _result = DataVariant(float(-_val.F())); break;
			case ConcatDouble((byte)DataType::FLOAT, (byte)Unop::NOT): _result = DataVariant(float(~_val.B())); break;

			case ConcatDouble((byte)DataType::DOUBLE, (byte)Unop::NEG): _result = DataVariant(double(-_val.D())); break;
			case ConcatDouble((byte)DataType::DOUBLE, (byte)Unop::NOT): _result = DataVariant(double(~_val.B())); break;

			case ConcatDouble((byte)DataType::LONG, (byte)Unop::NEG): _result = DataVariant(rLong(-_val.L())); break;
			case ConcatDouble((byte)DataType::LONG, (byte)Unop::NOT): _result = DataVariant(rLong(~_val.B())); break;

			default: return ResultType::ERR_DO_UNOP;
		}

		return ResultType::SUCCESS;
	}

	InstrUnop::InstrUnop(Unop _op, DataTypeDouble _argDataTypes, Argument* _src, Argument* _dest)
		: Instruction(InstructionType::UNOP), op(_op), argDataTypes(_argDataTypes), src(_src), dest(_dest) { }

	InstrUnop::~InstrUnop()
	{
		delete src;
		delete dest;
	}

	std::string InstrUnop::ToString()
	{
		std::stringstream ss;
		ss << UnopToString(op) << '<';
		ss << DataTypeToChar(argDataTypes.first);
		ss << DataTypeToChar(argDataTypes.second) << ">(";
		ss << src->ToString() << ", ";
		ss << dest->ToString() << ")";
		return ss.str();
	}
#pragma endregion

#pragma region Call
	InstrCall::InstrCall(std::string _label, Argument* _argsByteLen)
		: Instruction(InstructionType::CALL), label(_label), argsByteLength(_argsByteLen) { }
	InstrCall::~InstrCall() { delete argsByteLength; }
	std::string InstrCall::ToString() { return "CALL(%" + label + ", " + argsByteLength->ToString() + ")"; }
#pragma endregion

#pragma region Store
	InstrStore::InstrStore(DataType _dataType, const std::vector<Argument*>& _srcs, Argument* _dest)
		: Instruction(InstructionType::STORE), dataType(_dataType), srcs(_srcs), dest(_dest) { }

	InstrStore::~InstrStore()
	{
		for (auto const& src : srcs)
			delete src;

		delete dest;
	}

	std::string InstrStore::ToString()
	{
		std::stringstream ss;
		ss << "STORE<" << DataTypeToChar(dataType) << ">(";

		for (auto src : srcs)
			ss << src->ToString() << ", ";

		ss << dest->ToString() << ")";
		return ss.str();
	}
#pragma endregion

#pragma region Compare
	InstrCompare::InstrCompare(Argument* _src1, Argument* _src2, Argument* _len, Argument* _dest)
		: Instruction(InstructionType::COMPARE), src1(_src1), src2(_src2), length(_len), dest(_dest) { }

	InstrCompare::~InstrCompare()
	{
		delete src1;
		delete src2;
		delete length;
		delete dest;
	}
#pragma endregion

#pragma region Return
	InstrReturn::InstrReturn(Argument* _amt)
		: Instruction(InstructionType::RETURN), amt(_amt) { }

	InstrReturn::~InstrReturn() { delete amt; }
#pragma endregion

#pragma region Jump
	InstrJump::InstrJump(std::string _label)
		: Instruction(InstructionType::JUMP), label(_label) { }
#pragma endregion

#pragma region CJump
	InstrCJump::InstrCJump(std::string _label, Argument* _cond, bool _jumpOnFalse)
		: Instruction(InstructionType::CJUMP), label(_label), cond(_cond), jumpOnFalse(_jumpOnFalse) { }

	InstrCJump::~InstrCJump() { delete cond; }
#pragma endregion

#pragma region Print
	InstrPrint::InstrPrint(Argument* _start, Argument* _len)
		: Instruction(InstructionType::PRINT), start(_start), length(_len) { }

	InstrPrint::~InstrPrint()
	{
		delete start;
		delete length;
	}
#pragma endregion

#pragma region Malloc
	InstrMalloc::InstrMalloc(Argument* _size, Argument* _dest)
		: Instruction(InstructionType::MALLOC), size(_size), dest(_dest) { }

	InstrMalloc::~InstrMalloc()
	{
		delete size;
		delete dest;
	}
#pragma endregion

#pragma region Free
	InstrFree::InstrFree(Argument* _addr)
		: Instruction(InstructionType::FREE), addr(_addr) { }

	InstrFree::~InstrFree() { delete addr; }
#pragma endregion

#pragma region Push
	InstrPush::InstrPush(DataType _dataType, const std::vector<Argument*>& _srcs)
		: Instruction(InstructionType::PUSH), dataType(_dataType), srcs(_srcs) { }

	InstrPush::~InstrPush()
	{
		for (auto const& src : srcs)
			delete src;
	}

	std::string InstrPush::ToString()
	{
		std::stringstream ss;
		ss << "PUSH<" << DataTypeToChar(dataType) << ">(";

		for (auto src : srcs)
			ss << src->ToString() << ", ";

		ss.seekp(-2, ss.cur);
		ss << ')';
		return ss.str();
	}
#pragma endregion

#pragma region Pop
	bool Instruction::IsSinglePop() { return type == InstructionType::POP && ((InstrPop*)this)->IsSinglePop(); }

	InstrPop::InstrPop(DataType _type, Argument* _amt)
		: Instruction(InstructionType::POP), amt(_amt), scale(GetDataTypeSize(_type)) { }

	InstrPop::~InstrPop() { delete amt; }
	bool InstrPop::IsSinglePop() { return amt->GetType() == ArgType::VALUE && ((ValueArgument*)amt)->GetValue().i == 1; }
#pragma endregion
}
