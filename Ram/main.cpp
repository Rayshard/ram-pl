#include <iostream>
#include "Parser.h"

int main()
{
	Environment::GLOBAL = Environment::CreateGlobal("");

	const char* mainFilePath = "C:\\Users\\raysh\\Google Drive\\Visual Studio Projects\\RayGoProgrammingLanguage\\RayGo\\Test.ram";
	IValue* endVal = RunFile(mainFilePath, Environment::GLOBAL, false);
	
	if(endVal->_type != VEXCEPTION) { std::cout << "Program has exited with code (0)." << std::endl; }
	else
	{
		std::cout << "Program has exited with code (1)." << std::endl << std::endl;
		std::cout << endVal->ToString(Environment::GLOBAL) << std::endl;
	}

	delete endVal;
	delete Environment::GLOBAL;
	return 0;
}
