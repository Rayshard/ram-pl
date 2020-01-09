#include "pch.h"
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

Trace::Trace(Position _pos, std::string _scope, std::string _fileName)
{
	position = _pos;
	scope = _scope;
	fileName = _fileName;
}

std::string Trace::ToString() { return "at " + position.ToString() + " in " + scope + " in " + fileName; }