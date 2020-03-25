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

	/*bool IsDestArgument(ArgType _type) { return _type == ArgType::MEM_REG || _type == ArgType::REGISTER || _type == ArgType::STACK_REG || _type == ArgType::SP_OFFSET; }
	bool IsMemoryArgument(ArgType _type) { return _type == ArgType::MEM_REG; }
	bool IsStackArgument(ArgType _type) { return _type == ArgType::STACK_REG; }
	bool IsLabelArg(std::string _token) { return std::regex_match(_token, std::regex("[A-Za-z_][A-Za-z0-9_]*")); }


	ResultType ParseReturn(std::vector<std::string>& _tokens, Instruction*& _instr)
	{
		std::vector<Argument> args;
		ResultType result = ParseArguments(_tokens, 1, args);

		if (IsErrorResult(result))
			return result;

		_instr = new InstrReturn(args);
		return ResultType::SUCCESS;
	}

	ResultType ParseMove(std::vector<std::string>& _tokens, Instruction*& _instr, DataType _type)
	{
		std::vector<Argument> args;
		ResultType result = ParseArguments(_tokens, 1, args);

		if (IsErrorResult(result)) { return result; }
		else if (args.size() != 2) { return ResultType::PARSE_ERR_WRONG_NUM_ARGS; }

		Argument src = args[0], dest = args[1];

		if (IsDestArgument(dest.type))
		{
			_instr = new InstrMove(_type, src, dest);
			return ResultType::SUCCESS;
		}
		else { return ResultType::PARSE_ERR_INVALID_ARG; }
	}

	ResultType ParseMalloc(std::vector<std::string>& _tokens, Instruction*& _instr)
	{
		std::vector<Argument> args;
		ResultType result = ParseArguments(_tokens, 1, args);

		if (IsErrorResult(result)) { return result; }
		else if (args.size() != 2) { return ResultType::PARSE_ERR_WRONG_NUM_ARGS; }

		_instr = new InstrMalloc(args[0], args[1]);
		return ResultType::SUCCESS;
	}

	ResultType ParseFree(std::vector<std::string>& _tokens, Instruction*& _instr)
	{
		std::vector<Argument> args;
		ResultType result = ParseArguments(_tokens, 1, args);

		if (IsErrorResult(result)) { return result; }
		else if (args.size() != 1) { return ResultType::PARSE_ERR_WRONG_NUM_ARGS; }

		_instr = new InstrFree(args[0]);
		return ResultType::SUCCESS;
	}

	ResultType ParsePrint(std::vector<std::string>& _tokens, Instruction*& _instr)
	{
		std::vector<Argument> args;
		ResultType result = ParseArguments(_tokens, 1, args);

		if (IsErrorResult(result)) { return result; }
		else if (args.size() != 2) { return ResultType::PARSE_ERR_WRONG_NUM_ARGS; }

		_instr = new InstrPrint(args[0], args[1]);
		return ResultType::SUCCESS;
	}

	ResultType ParseJump(std::vector<std::string>& _tokens, Instruction*& _instr)
	{
		if (_tokens.size() != 2) { return ResultType::PARSE_ERR_WRONG_NUM_ARGS; }
		else
		{
			if (IsLabelArg(_tokens[1]))
			{
				_instr = new InstrJump(-1);
				return ResultType::SUCCESS;
			}
			else { return ResultType::PARSE_ERR_INVALID_ARG; }
		}
	}

	ResultType ParseCJump(std::vector<std::string>& _tokens, Instruction*& _instr)
	{
		if (_tokens.size() != 3) { return ResultType::PARSE_ERR_WRONG_NUM_ARGS; }
		else
		{
			if (!IsLabelArg(_tokens[1]))
				return ResultType::PARSE_ERR_INVALID_ARG;

			std::vector<Argument> args;
			ResultType result = ParseArguments(_tokens, 2, args);

			if (IsErrorResult(result))
				return result;

			_instr = new InstrCJump(-1, args[0]);
			return ResultType::SUCCESS;
		}
	}

	ResultType ParseCall(std::vector<std::string>& _tokens, Instruction*& _instr)
	{
		if (_tokens.size() < 5) { return ResultType::PARSE_ERR_WRONG_NUM_ARGS; }
		else
		{
			if (!IsLabelArg(_tokens[1]))
				return ResultType::PARSE_ERR_INVALID_ARG;

			std::vector<Argument> args;
			ResultType result = ParseArguments(_tokens, 2, args);

			if (IsErrorResult(result))
				return result;

			Argument regCnt = args[0], argSrcCnt = args[1], retDestCnt = args[2];

			if (regCnt.type != ArgType::VALUE) { return ResultType::PARSE_ERR_INVALID_ARG; }
			else if (argSrcCnt.type != ArgType::VALUE) { return ResultType::PARSE_ERR_INVALID_ARG; }
			else if (retDestCnt.type != ArgType::VALUE) { return ResultType::PARSE_ERR_INVALID_ARG; }

			std::vector<Argument> argSrcs(args.begin() + 3, args.begin() + 3 + argSrcCnt.value.AsInt());
			std::vector<Argument> retDests;

			int retDestsArgOff = 3 + argSrcCnt.value.AsInt();
			for (int i = 0; i < retDestCnt.value.AsInt(); i++)
			{
				Argument retDest = args[retDestsArgOff + i];

				if (!IsDestArgument(retDest.type))
					return ResultType::PARSE_ERR_INVALID_ARG;

				retDests.push_back(retDest);
			}

			_instr = new InstrCall(-1, regCnt.value.AsInt(), argSrcs, retDests);
			return ResultType::SUCCESS;
		}
	}

	ResultType ParsePush(std::vector<std::string>& _tokens, Instruction*& _instr)
	{
		std::vector<Argument> args;
		ResultType result = ParseArguments(_tokens, 1, args);

		if (IsErrorResult(result)) { return result; }
		else if (args.size() == 0) { return ResultType::PARSE_ERR_WRONG_NUM_ARGS; }

		_instr = new InstrPush(args);
		return ResultType::SUCCESS;
	}

	ResultType ParsePop(std::vector<std::string>& _tokens, Instruction*& _instr, int _scale)
	{
		std::vector<Argument> args;
		ResultType result = ParseArguments(_tokens, 1, args);

		if (IsErrorResult(result)) { return result; }
		else if (args.size() > 1) { return ResultType::PARSE_ERR_WRONG_NUM_ARGS; }

		_instr = new InstrPop(args[0], _scale);
		return ResultType::SUCCESS;
	}

	ResultType ParseBinop(Binop _op, std::vector<std::string>& _tokens, Instruction*& _instr)
	{
		std::vector<Argument> args;
		ResultType result = ParseArguments(_tokens, 1, args);

		if (IsErrorResult(result)) { return result; }
		else if (args.size() != 3) { return ResultType::PARSE_ERR_WRONG_NUM_ARGS; }

		Argument arg0 = args[0], arg1 = args[1], dest = args[2];

		if (IsDestArgument(dest.type))
		{
			_instr = new InstrBinop(_op, arg0, arg1, dest);
			return ResultType::SUCCESS;
		}
		else { return ResultType::PARSE_ERR_INVALID_ARG; }
	}

	ResultType ParseUnop(Unop _op, std::vector<std::string>& _tokens, Instruction*& _instr)
	{
		std::vector<Argument> args;
		ResultType result = ParseArguments(_tokens, 1, args);

		if (IsErrorResult(result)) { return result; }
		else if (args.size() != 2) { return ResultType::PARSE_ERR_WRONG_NUM_ARGS; }

		Argument arg = args[0], dest = args[1];

		if (IsDestArgument(dest.type))
		{
			_instr = new InstrUnop(_op, arg, dest);
			return ResultType::SUCCESS;
		}
		else { return ResultType::PARSE_ERR_INVALID_ARG; }
	}

	ResultType ParseInstruction(std::vector<std::string>& _tokens, Instruction*& _instr)
	{
		std::string instrTag = _tokens[0];

		if (instrTag == "HALT") { return ParseHalt(_tokens, _instr); }
		else if (instrTag == "RET") { return ParseReturn(_tokens, _instr); }
		else if (instrTag == "MALLOC") { return ParseMalloc(_tokens, _instr); }
		else if (instrTag == "FREE") { return ParseFree(_tokens, _instr); }
		else if (instrTag == "PUSH") { return ParsePush(_tokens, _instr); }
		else if (instrTag == "POPB") { return ParsePop(_tokens, _instr, BYTE_SIZE); }
		else if (instrTag == "POPI") { return ParsePop(_tokens, _instr, INT_SIZE); }
		else if (instrTag == "MOV") { return ParseMove(_tokens, _instr, DataType::INT); }
		else if (instrTag == "PRINT") { return ParsePrint(_tokens, _instr); }
		else if (instrTag == "JUMP") { return ParseJump(_tokens, _instr); }
		else if (instrTag == "CJUMP") { return ParseCJump(_tokens, _instr); }
		else if (instrTag == "CALL") { return ParseCall(_tokens, _instr); }
		else if (instrTag == "ADD") { return ParseBinop(Binop::ADD, _tokens, _instr); }
		else if (instrTag == "SUB") { return ParseBinop(Binop::SUB, _tokens, _instr); }
		else if (instrTag == "MUL") { return ParseBinop(Binop::MUL, _tokens, _instr); }
		else if (instrTag == "DIV") { return ParseBinop(Binop::DIV, _tokens, _instr); }
		else if (instrTag == "MOD") { return ParseBinop(Binop::MOD, _tokens, _instr); }
		else if (instrTag == "LSHIFT") { return ParseBinop(Binop::LSHIFT, _tokens, _instr); }
		else if (instrTag == "RSHIFT") { return ParseBinop(Binop::RSHIFT, _tokens, _instr); }
		else if (instrTag == "LT") { return ParseBinop(Binop::LT, _tokens, _instr); }
		else if (instrTag == "GT") { return ParseBinop(Binop::GT, _tokens, _instr); }
		else if (instrTag == "LTEQ") { return ParseBinop(Binop::LTEQ, _tokens, _instr); }
		else if (instrTag == "GTEQ") { return ParseBinop(Binop::GTEQ, _tokens, _instr); }
		else if (instrTag == "EQ") { return ParseBinop(Binop::EQ, _tokens, _instr); }
		else if (instrTag == "NEQ") { return ParseBinop(Binop::NEQ, _tokens, _instr); }
		else if (instrTag == "BAND") { return ParseBinop(Binop::BIT_AND, _tokens, _instr); }
		else if (instrTag == "BOR") { return ParseBinop(Binop::BIT_OR, _tokens, _instr); }
		else if (instrTag == "BXOR") { return ParseBinop(Binop::BIT_XOR, _tokens, _instr); }
		else if (instrTag == "LAND") { return ParseBinop(Binop::LOG_AND, _tokens, _instr); }
		else if (instrTag == "LOR") { return ParseBinop(Binop::LOG_OR, _tokens, _instr); }
		else if (instrTag == "NEG") { return ParseUnop(Unop::NEG, _tokens, _instr); }
		else if (instrTag == "LNOT") { return ParseUnop(Unop::LOG_NOT, _tokens, _instr); }
		else { return ResultType::PARSE_ERR_UNKNOWN_INSTR; }
	}

	ResultType ParseProgram(std::ifstream& _stream, std::vector<Instruction*>& _program, ResultInfo& _info) {
		std::vector<Instruction*> prog;
		std::map<std::string, int> labels;
		std::map<int, std::pair<InstructionType, std::string>> jumps;
		std::string curLine;
		int lineNum = -1;

		while (std::getline(_stream, curLine)) {
			curLine = TrimString(curLine);
			lineNum++;

			int instrPtr = prog.size();
			auto lineStream = std::stringstream(curLine);
			std::vector<std::string> tokens = GetLineTokens(lineStream, ' ');

			if (tokens.size() == 0) { continue; }
			else if (tokens.size() == 1 && std::regex_match(curLine, std::regex("%[A-Za-z_][A-Za-z0-9_]*%"))) //Check if Line is a label
			{
				auto search = labels.find(curLine);
				if (search == labels.end()) { labels.insert_or_assign(curLine.substr(1, curLine.length() - 2), instrPtr); }
				else
				{
					_info.insert_or_assign("Line#", std::to_string(lineNum));
					_info.insert_or_assign("Line", curLine);
					return ResultType::PARSE_ERR_DUPLICATE_LABEL;
				}
			}
			else
			{
				Instruction* instruction = new InstrHalt();
				ResultType result = ParseInstruction(tokens, instruction);

				if (IsErrorResult(result))
				{
					_info.insert_or_assign("Line#", std::to_string(lineNum));
					_info.insert_or_assign("Line", curLine);
					return result;
				}
				else
				{
					if (instruction->GetType() == InstructionType::JUMP) { jumps.insert_or_assign(instrPtr, std::make_pair(InstructionType::JUMP, tokens[1])); }
					else if (instruction->GetType() == InstructionType::CJUMP) { jumps.insert_or_assign(instrPtr, std::make_pair(InstructionType::CJUMP, tokens[1])); }
					else if (instruction->GetType() == InstructionType::CALL) { jumps.insert_or_assign(instrPtr, std::make_pair(InstructionType::CALL, tokens[1])); }

					prog.push_back(instruction);
				}
			}
		}

		//Convert Control Labels
		for (auto it = jumps.begin(); it != jumps.end(); it++)
		{
			auto search = labels.find(it->second.second);

			if (search != labels.end())
			{
				if (it->second.first == InstructionType::JUMP) { ((InstrJump*)prog[it->first])->labelIdx = labels[it->second.second]; }
				else if (it->second.first == InstructionType::CJUMP) { ((InstrCJump*)prog[it->first])->labelIdx = labels[it->second.second]; }
				else if (it->second.first == InstructionType::CALL) { ((InstrCall*)prog[it->first])->labelIdx = labels[it->second.second]; }
			}
			else { return ResultType::PARSE_ERR_UNKNOWN_LABEL; }
		}

		_program = prog;
		return ResultType::SUCCESS;
	}*/

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
		result.errString = (_includePos ? "(" + _pos.ToString() + ")" : "") + _msg;
		result.errPosition = _pos;
		return result;
	}

	ParseResult ParseFile(std::istream* _stream, std::string _fileName, int _tabSize)
	{
		Lexer lexer = Lexer(_stream, _tabSize);
		std::vector<Instruction*> program;
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