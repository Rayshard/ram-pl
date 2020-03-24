#pragma once

namespace ramvm {
	class Instruction;
	struct Argument;

	ResultType ParseArgument(std::string& _token, Argument& _arg);
	ResultType ParseProgram(std::ifstream& _stream, std::vector<Instruction>& _program, ResultInfo& _info);
}
