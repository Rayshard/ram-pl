#include "pch.h"
#include "Environment.h"
#include "Value.h"
#include "Parser.h"
#include "Interpreter.h"

int main()
{
	const char* mainFilePath = "C:\\Users\\raysh\\OneDrive\\Desktop\\Ram\\Ram\\Test.ram";

	SharedValue endVal = Interpreter::RunFile(mainFilePath, "Test");

	if(endVal->_type != VEXCEPTION) { std::cout << "Program has exited with code (0)." << std::endl; }
	else
	{
		std::cout << "Program has exited with code (1)." << std::endl << std::endl;
		std::cout << endVal->ToString() << std::endl;
	}

	delete Environment::GLOBAL;
	return 0;
}
