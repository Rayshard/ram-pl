#include "pch.h"
#include "ramc_position.h"

namespace ramc {
	Position::Position(int _line, int _col)
	{
		line = _line;
		colomn = _col;
	}

	std::string Position::ToString() { return "Line: " + std::to_string(line) + ", Column: " + std::to_string(colomn); }
}