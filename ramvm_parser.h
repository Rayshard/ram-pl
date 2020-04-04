#pragma once

namespace ramvm {
	class Instruction;
	class Argument;

	class ParseResult {
		std::vector<Instruction*> instrSet;
		std::unordered_map<std::string, int> labels;
		bool success;

		std::string errString;
		Position errPosition;
	public:
		bool IsSuccess();
		std::vector<Instruction*>& GetInstructionSet();
		std::unordered_map<std::string, int>& GetLabels();
		std::string ToString();

		static ParseResult GenSuccess(const std::vector<Instruction*>& _instrSet, const std::unordered_map<std::string, int>& _labels);
		static ParseResult GenError(std::string _msg, Position _pos, bool _includePos);
	};

	ResultType ParseArgument(std::string& _token, Argument& _arg);
	ParseResult ParseFile(std::istream* _stream, std::string _fileName, int _tabSize);
}
