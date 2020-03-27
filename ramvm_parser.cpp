#include "pch.h"
#include "ramvm_parser.h"
#include "ramvm_lexer.h"
#include "ramvm_instruction.h"
#include "ramvm_bison_parser.hpp"

namespace ramvm {
	ResultType ParseArgument(std::string& _token, Argument& _arg)
	{
		if (std::regex_match(_token, Lexer::REGEX_REG))
		{
			std::string idx = _token.substr(1, _token.length() - 1);

			if (IsInt(idx)) { _arg = Argument(ArgType::REGISTER, std::stoi(idx)); }
			else { return ResultType::ERR_REG_IDX_OOB; }
		}
		else if (std::regex_match(_token, Lexer::REGEX_MEM_REG))
		{
			std::string idx = _token.substr(2, _token.length() - 3);

			if (IsInt(idx)) { _arg = Argument(ArgType::MEM_REG, std::stoi(idx)); }
			else { return ResultType::ERR_REG_IDX_OOB; }
		}
		else if (std::regex_match(_token, Lexer::REGEX_STACK_REG))
		{
			std::string idx = _token.substr(2, _token.length() - 3);

			if (IsInt(idx)) { _arg = Argument(ArgType::STACK_REG, std::stoi(idx)); }
			else { return ResultType::ERR_REG_IDX_OOB; }
		}
		else if (std::regex_match(_token, Lexer::REGEX_SP_OFFSET))
		{
			std::string idx = _token.substr(1, _token.length() - 2);

			if (IsInt(idx)) { _arg = Argument(ArgType::SP_OFFSET, std::stoi(idx)); }
			else { return ResultType::ERR_REG_IDX_OOB; }
		}
		else if (std::regex_match(_token, Lexer::REGEX_SP)) { _arg = Argument(ArgType::STACK_PTR, 0); }
		else if (std::regex_match(_token, Lexer::REGEX_INT_LIT))
		{
			try { _arg = Argument(ArgType::VALUE, std::stoll(_token)); }
			catch (...) { return ResultType::ERR_ARGUMENT; }
		}
		else if (std::regex_match(_token, Lexer::REGEX_FLOAT_LIT))
		{
			try { _arg = Argument(ArgType::VALUE, std::stof(_token)); }
			catch (...) { return ResultType::ERR_ARGUMENT; }
		}
		else if (std::regex_match(_token, Lexer::REGEX_DOUBLE_LIT))
		{
			try { _arg = Argument(ArgType::VALUE, std::stod(_token)); }
			catch (...) { return ResultType::ERR_ARGUMENT; }
		}
		else if (std::regex_match(_token, Lexer::REGEX_HEX_LIT))
		{
			try { _arg = Argument(ArgType::VALUE, std::stoll(_token, 0, 16)); }
			catch (...) { return ResultType::ERR_ARGUMENT; }
		}
		else { return ResultType::PARSE_ERR_INVALID_ARG; }

		return ResultType::SUCCESS;
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