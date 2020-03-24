#pragma once
namespace ramc {
	struct Position
	{
		int line, colomn;

		Position() = default;
		Position(int _line, int _col);

		std::string ToString();
	};
}
