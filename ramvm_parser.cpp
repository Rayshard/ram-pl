#include "pch.h"
#include "ramvm_parser.h"
#include "ramvm_lexer.h"
#include "ramvm_instruction.h"
#include "ramvm_bison_parser.hpp"

namespace ramvm {
	ResultType ParseArgument(std::string& _token, Argument& _arg)
	{
		return ResultType::ERR_ARGUMENT;
		/*if (std::regex_match(_token, std::regex("(byte|int|float|double|long)[(]-?(0|[1-9][0-9]*)(\\.[0-9]+)?[)]")) ||
			std::regex_match(_token, std::regex("-?0|[1-9][0-9]*")) ||
			std::regex_match(_token, std::regex("R(0|[1-9][0-9]*)")) ||
			std::regex_match(_token, std::regex("\\{R(0|[1-9][0-9]*)\\}")) ||
			std::regex_match(_token, std::regex("\\[R(0|[1-9][0-9]*)\\]")) ||
			std::regex_match(_token, std::regex("\\[(0|1|-[1-9][0-9]*)\\]")) ||
			_token == "SP")
		{
			ResultType result;
			DataVariant value;

			//Get Value Data Type
			switch (_token[0])
			{
				case 'b': value = DataVariant(DataType::BYTE); break;
				case 'i': value = DataVariant(DataType::INT); break;
				case 'f': value = DataVariant(DataType::FLOAT); break;
				case 'd': value = DataVariant(DataType::DOUBLE); break;
				case 'l': value = DataVariant(DataType::LONG); break;
				default: value = DataVariant(DataType::INT); break;
			}

			//Get Number in Token
			if (_token != "SP")
			{
				std::string subStr = _token.substr(_token.find_first_of("-0123456789"));
				subStr.erase(subStr.find_last_of("0123456789") + 1);
				result = ParseNumericLiteral(subStr, value);
			}

			if (IsErrorResult(result))
				return result;

			switch (_token[0])
			{
				case 'R': _arg = Argument(ArgType::REGISTER, value); break;
				case '{': _arg = Argument(ArgType::MEM_REG, value); break;
				case '[': {
					if (isdigit(_token[1]) || _token[1] == '-') { _arg = Argument(ArgType::SP_OFFSET, value); }
					else { _arg = Argument(ArgType::STACK_REG, value); }
				} break;
				case 'S': _arg = Argument(ArgType::STACK_PTR, value); break;
				default: _arg = Argument(ArgType::VALUE, value);
			}

			return ResultType::SUCCESS;
		}
		else { return ResultType::PARSE_ERR_INVALID_ARG; }*/
	}

	bool ParseResult::IsSuccess() { return success; }
	std::vector<Instruction*>& ParseResult::GetInstructionSet() { return instrSet; }

	std::string ParseResult::ToString()
	{
		if (success) { return "SUCCESS"; }
		else { return errString; }
	}
	ParseResult ParseResult::GenSuccess(std::vector<Instruction*>& _instrSet)
	{
		ParseResult result;
		result.success = true;
		result.instrSet = _instrSet;
		return result;
	}
	ParseResult ParseResult::GenError(std::string _msg, Position _pos, bool _includePos)
	{
		ParseResult result;
		result.success = false;
		result.errString = (_includePos ? "(" + _pos.ToString() + ") " : "") + _msg;
		result.errPosition = _pos;
		return result;
	}

	ParseResult ParseFile(std::istream* _stream, std::string _fileName, int _tabSize)
	{
		Lexer lexer = Lexer(_stream, _tabSize);
		std::vector<Instruction*> program;
		std::map<std::string, int> labels;
		std::map<Instruction*, std::pair<std::string, Position>> ctrlInstrs;
		Position position;

		try
		{
			bison::Parser parse(lexer, program, position, labels, ctrlInstrs);

			if (parse() != 0)
				return ParseResult::GenError("", position, true);

			//Convert Control Instruction Labels
			for (auto it = ctrlInstrs.begin(); it != ctrlInstrs.end(); it++)
			{
				auto search = labels.find(it->second.first);

				if (search != labels.end())
				{
					Instruction* instr = it->first;
					if (instr->GetType() == InstructionType::JUMP) { ((InstrJump*)instr)->labelIdx = labels[it->second.first]; }
					else if (instr->GetType() == InstructionType::CJUMP) { ((InstrCJump*)instr)->labelIdx = labels[it->second.first]; }
					else if (instr->GetType() == InstructionType::CALL) { ((InstrCall*)instr)->labelIdx = labels[it->second.first]; }
				}
				else { return ParseResult::GenError("Unknown label: " + it->second.first, it->second.second, true); }
			}

			return ParseResult::GenSuccess(program);
		}
		catch (std::runtime_error err) { return ParseResult::GenError(err.what(), position, true); }
	}
}