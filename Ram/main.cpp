#include "pch.h"
#include "Environment.h"
#include "Value.h"
#include "Parser.h"
#include "Interpreter.h"
#include "RamDLL.h"

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		std::cout << "Expected one argument to the path of the Ram source file." << std::endl;
		return 1;
	}

	const char* mainFilePath = argv[1];

	Interpreter::Init(mainFilePath);
	SharedValue endVal = Interpreter::RunFile(mainFilePath, GetFileName(mainFilePath, false));

	if(endVal->GetType() != VEXCEPTION) { std::cout << "Program has exited with code (0)." << std::endl; }
	else
	{
		std::cout << "Program has exited with code (1)." << std::endl << std::endl;
		std::cout << endVal->ToString() << std::endl;
	}

	delete Environment::GLOBAL;
	Interpreter::SRC_LINES.clear();
	RamDLL::FreeAll();
	return 0;
}
