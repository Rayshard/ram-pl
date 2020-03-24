#pragma once
#include "ramc_lexer.h"

namespace ramc {
	class ASTNode; 

	class ParseResult {
		ASTNode* node;
		bool success;

		std::string errString;
		Position errPosition;
	public:
		bool IsSuccess();
		ASTNode* GetNode();
		std::string ToString();

		static ParseResult GenSuccess(ASTNode* _node);
		static ParseResult GenError(std::string _msg, Position _pos, bool _includePos);
	};

	ParseResult ParseFile(std::istream* _stream, std::string _fileName, int _tabSize);
}

