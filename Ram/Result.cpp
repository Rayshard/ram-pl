#include "Result.h"

Position::Position()
{
	line = 1;
	column = 1;
}

Position::Position(int _line, int _col)
{
	line = _line;
	column = _col;
}

std::string Position::ToString() { return "(Line: " + std::to_string(line) + ", Column: " + std::to_string(column) + ")"; }