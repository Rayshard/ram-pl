#pragma once

struct Interpreter
{
	static std::map<std::string, std::vector<std::string>> SRC_LINES;
	static std::chrono::time_point<std::chrono::steady_clock> INIT_TIME;
	static std::string MainFilePath;
	static Environment* ENV_STD;

	static void Init(const char* _mainFilePath);
	static void SetStandardEnvironment();
	static std::string GetSrcLine(std::string _filePath, int _line);
	static SharedValue RunFile(const char* _path, std::string _name);
};

#define PRINT_ENVS 0
#define PRINT_VALS 0
#define PRINT_LINES 0

#define PRINT_VAL(val) { if(PRINT_VALS) std::cout << val->ToString() << std::endl; }
#define PRINT_ENV(env) { if(PRINT_ENVS) std::cout << std::endl << std::endl << (env).ToString() << std::endl; }
#define PRINT_LINE(filePath, line) { if(PRINT_LINES) std::cout << Interpreter::GetSrcLine(filePath, line) << std::endl; }
