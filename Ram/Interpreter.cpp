#include "pch.h"
#include "Interpreter.h"
#include "Parser.h"
#include "Statement.h"
#include "Environment.h"
#include "Value.h"

std::map<std::string, std::vector<std::string>> Interpreter::SRC_LINES = std::map<std::string, std::vector<std::string>>();

std::string Interpreter::GetSrcLine(std::string _filePath, int _line) { return Interpreter::SRC_LINES[_filePath][_line - 1]; }

SharedValue Interpreter::RunFile(const char* _path, std::string _name)
{
	std::vector<std::string> fileLines;
	TokensResult tokenizationResult = TokenizeFile(_path, fileLines);

	Interpreter::SRC_LINES.insert_or_assign(_path, fileLines);
	Environment::GLOBAL = Environment::CreateGlobal(_name, _path);

	std::string path = _path;

	if(tokenizationResult.success)
	{
		std::cout << "Tokenized!" << std::endl;

		FileParseResult parsingResult = Parse(tokenizationResult.value.data());

		if(parsingResult.success)
		{
			std::cout << "Parsed!" << std::endl << std::endl;

			std::vector<IStatement*> statements = parsingResult.value;
			std::cout << "Running " + path << std::endl;

			SharedValue endVal(nullptr);

			if(statements.size() > 0)
			{
				CodeBlock program = CodeBlock(statements, statements[0]->_position, _name);
				endVal = program.Execute(Environment::GLOBAL);
			}
			else { endVal = SharedValue(new VoidValue(Position())); }

			std::cout << "Successfully Ran " + path << std::endl << std::endl;
			return endVal;
		}
		else { return SharedValue(Exception_CompilationError("There was a parsing error in " + path + "\n\n" + parsingResult.message, Trace(parsingResult.position, _name, _path))); }
	}
	else { return SharedValue(Exception_CompilationError("There was a tokenizing error in " + path + "\n\n" + tokenizationResult.message, Trace(tokenizationResult.position, _name, _path))); }
}