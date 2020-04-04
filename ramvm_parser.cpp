#include "pch.h"
#include "ramvm_parser.h"
#include "ramvm_lexer.h"
#include "ramvm_instruction.h"
#include "ramvm_bison_parser.hpp"

namespace ramvm {
	bool ParseResult::IsSuccess() { return success; }
	std::vector<Instruction*>& ParseResult::GetInstructionSet() { return instrSet; }
	std::unordered_map<std::string, int>& ParseResult::GetLabels() { return labels; }

	std::string ParseResult::ToString()
	{
		if (success) { return "SUCCESS"; }
		else { return errString; }
	}

	ParseResult ParseResult::GenSuccess(const std::vector<Instruction*>& _instrSet, const std::unordered_map<std::string, int>& _labels)
	{
		ParseResult result;
		result.success = true;
		result.instrSet = _instrSet;
		result.labels = _labels;
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
		std::unordered_map<std::string, int> labels;
		Position position;

		try
		{
			bison::Parser parse(lexer, program, position, labels);

			if (parse() != 0) { return ParseResult::GenError("", position, true); }
			else { return ParseResult::GenSuccess(program, labels); }
		}
		catch (std::runtime_error err) { return ParseResult::GenError(err.what(), position, true); }
	}
}