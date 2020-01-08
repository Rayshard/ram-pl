#pragma once

#include <string>
#include <iostream>
#include "..\Lexer.h"
#include "Value.h"
#include "Statement.h"
#include "Expression.h"

inline bool IsTypeName(TokenType _type)
{
	return _type == TT_IDENTIFIER || _type == TT_TYPE_INT || _type == TT_TYPE_FLOAT || _type == TT_TYPE_BOOL || _type == TT_TYPE_STRING;
}