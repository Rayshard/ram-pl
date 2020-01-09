#include <iostream>
#include "Parser.h"

int main()
{
	Environment::GLOBAL = Environment::CreateGlobal("Program");

	const char* mainFilePath = "C:\\Users\\raysh\\OneDrive\\Desktop\\Ram\\Ram\\Test.ram";
	SharedValue endVal = RunFile(mainFilePath, Environment::GLOBAL, false);
	
	if(endVal->_type != VEXCEPTION) { std::cout << "Program has exited with code (0)." << std::endl; }
	else
	{
		std::cout << "Program has exited with code (1)." << std::endl << std::endl;
		std::cout << endVal->ToString() << std::endl;
	}

	delete Environment::GLOBAL;
	return 0;
}
