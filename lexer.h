#pragma once
#include "result.h"

template<typename R, typename T> class RTLexer
{
protected:
	std::istream* stream;
	Position position;
	int tabSize;

	RTLexer() = default;
public:
	RTLexer(std::istream* _stream, int _tabSize) : stream(_stream), position(Position(1, 0)), tabSize(_tabSize) { }

	int ReadNextChar()
	{
		int result = stream->get();

		if (result == '\n')
		{
			position.line++;
			position.colomn = 0;
		}
		else if (result == '\t') { position.colomn += tabSize; }
		else { position.colomn++; }

		return result;
	}
	int PeekNextChar() { return stream->peek(); }
	Position GetPosition() { return position; }

	virtual R GetNextToken() = 0;
};
