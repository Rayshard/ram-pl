#pragma once

namespace ramvm {
	class Instruction;
	struct Argument;

	class ParseResult {
		std::vector<Instruction*> instrSet;
		bool success;

		std::string errString;
		Position errPosition;
	public:
		bool IsSuccess();
		std::vector<Instruction*>& GetInstructionSet();
		std::string ToString();

		static ParseResult GenSuccess(std::vector<Instruction*>& _instrSet);
		static ParseResult GenError(std::string _msg, Position _pos, bool _includePos);
	};

	ResultType ParseArgument(std::string& _token, Argument& _arg);
	ParseResult ParseFile(std::istream* _stream, std::string _fileName, int _tabSize);
}
