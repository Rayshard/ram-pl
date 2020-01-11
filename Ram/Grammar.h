#pragma once

inline bool IsTypeName(TokenType _type)
{
	return _type == TT_IDENTIFIER || _type == TT_VOID;
}

inline bool IsControlFLow(TokenType _type)
{
	return _type == TT_BREAK || _type == TT_CONTINUE || _type == TT_RETURN;
}

inline bool IsControlFLow(std::string _keyword)
{
	return _keyword == "break" || _keyword == "continue" || _keyword == "return";
}