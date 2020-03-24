#ifndef PCH_H
#define PCH_H

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <unordered_map>
#include <regex>
#include <functional>

#define IGNORE(x) (void)(x)

enum class ResultType {
	SUCCESS,
	ERR_MALLOC,
	ERR_FREE,
	ERR_MEMREAD,
	ERR_MEMWRITE,
	ERR_NOHALT,
	ERR_UNKNOWNINSTR,
	ERR_REG_IDX_OOB,
	ERR_DIVBYZERO,
	ERR_INVALID_DEST,
	ERR_ARGUMENT,
	ERR_NO_EXEC_FRAMES,
	ERR_RET_TOP_LEVEL,
	ERR_RET_DEST_IDX_OOB,
	ERR_MEMORY_LIMIT,
	ERR_STACK_READ,
	ERR_STACK_WRITE,
	ERR_POP_AMT,
	PARSE_ERR_UNKNOWN_INSTR,
	PARSE_ERR_WRONG_NUM_ARGS,
	PARSE_ERR_INVALID_ARG,
	PARSE_ERR_DUPLICATE_LABEL,
	PARSE_ERR_UNKNOWN_LABEL
};

typedef std::map<std::string, std::string> ResultInfo;

inline void PrintResult(ResultType _type)
{
	switch (_type)
	{
		case ResultType::SUCCESS: std::cout << "Success" << std::endl; break;
		case ResultType::ERR_MALLOC: std::cout << "Error: Malloc" << std::endl; break;
		case ResultType::ERR_FREE: std::cout << "Error: Free" << std::endl; break;
		case ResultType::ERR_MEMREAD: std::cout << "Error: Memory Read" << std::endl; break;
		case ResultType::ERR_MEMWRITE: std::cout << "Error: Memory Write" << std::endl; break;
		case ResultType::ERR_NOHALT: std::cout << "Error: Exit Before HALT" << std::endl; break;
		case ResultType::ERR_UNKNOWNINSTR: std::cout << "Error: Unknown Instruction" << std::endl; break;
		case ResultType::ERR_REG_IDX_OOB: std::cout << "Error: Register Index Out of Bounds" << std::endl;  break;
		case ResultType::ERR_INVALID_DEST: std::cout << "Error: Invalid Destination" << std::endl; break;
		case ResultType::ERR_ARGUMENT: std::cout << "Error: Argument" << std::endl; break;
		case ResultType::ERR_NO_EXEC_FRAMES: std::cout << "Error: No Execution Frames" << std::endl; break;
		case ResultType::ERR_RET_TOP_LEVEL: std::cout << "Error: Cannot Return From Top Level" << std::endl; break;
		case ResultType::ERR_RET_DEST_IDX_OOB: std::cout << "Error: Return Destination Index Out of Bounds" << std::endl; break;
		case ResultType::ERR_MEMORY_LIMIT: std::cout << "Error: Computer Ran Out of Memory" << std::endl; break;
		case ResultType::ERR_STACK_READ: std::cout << "Error: Stack Read" << std::endl; break;
		case ResultType::ERR_STACK_WRITE: std::cout << "Error: Stack Write" << std::endl; break;
		case ResultType::ERR_POP_AMT: std::cout << "Error: Pop Amount" << std::endl; break;
		case ResultType::PARSE_ERR_UNKNOWN_INSTR: std::cout << "Parse Error: Unknown Instruction" << std::endl; break;
		case ResultType::PARSE_ERR_WRONG_NUM_ARGS: std::cout << "Parse Error: Wrong Number of Arguments" << std::endl; break;
		case ResultType::PARSE_ERR_INVALID_ARG: std::cout << "Parse Error: Invalid Argument" << std::endl; break;
		case ResultType::PARSE_ERR_UNKNOWN_LABEL: std::cout << "Parse Error: Unknown Label" << std::endl; break;
	}
}

inline bool IsErrorResult(ResultType _type) { return _type != ResultType::SUCCESS; }

inline std::string TrimString(std::string& _str)
{
	const char* ws = " \t\n\r\f\v";

	std::string rTrim = _str.erase(_str.find_last_not_of(ws) + 1);
	return rTrim.erase(0, rTrim.find_first_not_of(ws));
}

inline std::string CreateIndent(int _indentLvl) { return std::string(_indentLvl * 5, ' '); }
#endif
