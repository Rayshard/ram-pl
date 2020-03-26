#include "pch.h"
#include "ramc_parser.h"
#include "ramc_lexer.h"
#include "ramc_ast.h"
#include "ramc_bison_parser.hpp"

namespace ramc {
	bool ParseResult::IsSuccess() { return success; }
	ASTNode* ParseResult::GetNode() { return IsSuccess() ? node : nullptr; }

	std::string ParseResult::ToString()
	{
		if (success) { return "SUCCESS"; }
		else { return "(" + errPosition.ToString() + ") " + errString; }
	}
	ParseResult ParseResult::GenSuccess(ASTNode* _node)
	{
		ParseResult result;
		result.success = true;
		result.node = _node;
		return result;
	}
	ParseResult ParseResult::GenError(std::string _msg, Position _pos, bool _includePos)
	{
		ParseResult result;
		result.success = false;
		result.errString = (_includePos ? "(" + _pos.ToString() + ")" : "") + _msg;
		result.errPosition = _pos;
		return result;
	}

	ParseResult ParseFile(std::istream* _stream, std::string _fileName, int _tabSize)
	{
		Lexer lexer = Lexer(_stream, true, true, _tabSize);
		ASTNode* program = nullptr;
		Position position;

		try
		{
			bison::Parser parse(lexer, program, position);

			if (parse() == 0) { return ParseResult::GenSuccess(program); }
			else { return ParseResult::GenError("", position, true); }
		}
		catch (std::runtime_error err) { return ParseResult::GenError(err.what(), position, false); }
	}
}