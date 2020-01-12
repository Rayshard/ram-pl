#include "pch.h"
#include "Lexer.h"
#include "Interpreter.h"

#pragma region Token
Token::Token()
{
	type = TT_INVALID;
	position = Position();
}

Token::Token(TokenType _type, int _line, int _col)
{
	type = _type;
	position = Position(_line, _col);
}

Token::Token(TokenType _type, int _line, int _col, std::string _val)
	: Token(_type, _line, _col)
{
	value = _val;
}

std::string Token::ToString()
{
	switch(type)
	{
		case TT_INTEGER_LIT:
			return "INT: " + value;
			break;
		case TT_FLOAT_LIT:
			return "FLOAT: " + value;
			break;
		case TT_STRING_LIT:
			return "STRING: " + value;
			break;
		case TT_SEMICOLON:
			return "SEMICOLON";
			break;
		case TT_PERIOD:
			return "PERIOD";
			break;
		case TT_PLUS:
			return "PLUS";
			break;
		case TT_MINUS:
			return "MINUS";
			break;
		case TT_TIMES:
			return "TIMES";
			break;
		case TT_DIVIDE:
			return "DIVIDE";
			break;
		case TT_EQ:
			return "EQUALS";
			break;
		case TT_IF:
			return "IF";
			break;
		case TT_ELSE:
			return "ELSE";
			break;
		case TT_LET:
			return "LET";
			break;
		case TT_TRUE:
			return "TRUE";
			break;
		case TT_FALSE:
			return "FALSE";
			break;
		case TT_IDENTIFIER:
			return "IDENTIFIER: " + value;
			break;
		case TT_ENDOFFILE:
			return "EOF";
			break;
		case TT_INVALID:
			return "INVALID";
			break;
		default:
			return "UNKNOWN";
			break;
	}
}
#pragma endregion

#pragma region TokenReader
TokenReader::TokenReader()
{
	tokens = 0;
	index = 0;
	current = Token();
}

TokenReader::TokenReader(Token* _tokens)
{
	tokens = _tokens;
	index = 0;
	current = _tokens ? _tokens[index] : Token();
}

Token* TokenReader::Advance()
{
	if(current.type != TT_ENDOFFILE)
		current = tokens[++index];

	return GetCurPtr();
}

Token* TokenReader::GetCurPtr() { return tokens + index; }
TokenType TokenReader::GetCurType() { return current.type; }
Position TokenReader::GetCurPosition() { return current.position; }
bool TokenReader::AtEOF() { return current.type == TT_ENDOFFILE; }
#pragma endregion

TokenResult TokenizeStringLiteral(char* _chars, int& _offset, int _line, int _col)
{
	int offset = 0;
	bool escaped = false;
	std::ostringstream stream;
	std::string errorMsg;

	while(true)
	{
		char curChar = _chars[offset];

		if(curChar == '\\' && !escaped)
		{
			escaped = true;
			offset++;
		}
		else if(curChar == '"' && !escaped) { break; }
		else if(curChar == 0)
		{
			offset = -1;
			errorMsg = "Missing closing \" for string literal.";
			break;
		}
		else
		{
			offset++;

			if(escaped)
			{
				if(curChar == 'n') { stream << '\n'; }
				else if(curChar == 't') { stream << '\t'; }
				else { stream << curChar; }
			}
			else { stream << curChar; }

			escaped = false;
		}
	}

	if(offset == -1) { return TokenResult::GenFailure("Tokenizing Error: " + errorMsg, Position(_line, _col)); }
	else
	{
		_offset = offset;
		return TokenResult::GenSuccess(Token(TT_STRING_LIT, _line, _col, stream.str()));
	}
}

TokenResult TokenizeNumberLiteral(char* _chars, int& _offset, int _line, int _col)
{
	int offset = 0;
	std::ostringstream stream;
	bool hasDecimal = false;
	std::string errorMsg;

	while(true)
	{
		char curChar = _chars[offset];

		if(curChar == '.')
		{
			if(hasDecimal)
			{
				offset = -1;
				errorMsg = "Too many decimals in float literal.";
				break;
			}
			else
			{
				hasDecimal = true;
				stream << curChar;
				offset++;
			}
		}
		else if(isdigit(curChar))
		{
			stream << curChar;
			offset++;
		}
		else { break; }
	}

	if(offset == -1) { return TokenResult::GenFailure("Tokenizing Error: " + errorMsg, Position(_line, _col)); }
	else
	{
		_offset = offset;
		return TokenResult::GenSuccess(hasDecimal ? Token(TT_FLOAT_LIT, _line, _col, stream.str()) : Token(TT_INTEGER_LIT, _line, _col, stream.str()));
	}
}

TokenResult TokenizeWord(char* _chars, int& _offset, int _line, int _col)
{
	int offset = 0;
	std::ostringstream stream;
	std::string errorMsg;
	bool encounteredLetter = false;

	while(true)
	{
		char curChar = _chars[offset];

		if(isalpha(curChar) || curChar == '_' || (isdigit(curChar) && encounteredLetter))
		{
			if(!encounteredLetter && isalpha(curChar))
				encounteredLetter = true;

			stream << curChar;
			offset++;
		}
		else { break; }
	}

	if(offset == -1) { return TokenResult::GenFailure("Tokenizing Error: " + errorMsg, Position(_line, _col)); }
	else
	{
		std::string word = stream.str();
		TokenType type = TT_IDENTIFIER;

		if(word == "if") { type = TT_IF; }
		else if(word == "else") { type = TT_ELSE; }
		else if(word == "let") { type = TT_LET; }
		else if(word == "true") { type = TT_TRUE; }
		else if(word == "false") { type = TT_FALSE; }
		else if(word == "for") { type = TT_FOR; }
		else if(word == "then") { type = TT_THEN; }
		else if(word == "finally") { type = TT_FINALLY; }
		else if(word == "while") { type = TT_WHILE; }
		else if(word == "def") { type = TT_DEF; }
		else if(word == "func") { type = TT_FUNC; }
		else if(word == "return") { type = TT_RETURN; }
		else if(word == "break") { type = TT_BREAK; }
		else if(word == "continue") { type = TT_CONTINUE; }
		else if(word == "do") { type = TT_DO; }

		_offset = offset;
		return TokenResult::GenSuccess((type == TT_IDENTIFIER || IsTypeName(type)) ? Token(type, _line, _col, word) : Token(type, _line, _col));
	}
}

std::pair<TokenType, int> MatchChars(char* _chars)
{
	char c1 = _chars[0];
	char c2 = c1 == 0 ? 0 : _chars[1];
	char c3 = c2 == 0 ? 0 : _chars[2];

	TokenType charType = TT_INVALID;
	int len = 1;

	switch(c1)
	{
		case ';': charType = TT_SEMICOLON; break;
		case '.': charType = TT_PERIOD; break;
		case ':': charType = TT_COLON; break;
		case ',': charType = TT_COMMA; break;
		case '(': charType = TT_LPAREN; break;
		case ')': charType = TT_RPAREN; break;
		case '{': charType = TT_LBRACKET; break;
		case '}': charType = TT_RBRACKET; break;
		case '[': charType = TT_LSQRBRACKET; break;
		case ']': charType = TT_RSQRBRACKET; break;
		case '~': charType = TT_BIN_NOT; break;

		case '+': if(c2 == '=') { len++; charType = TT_PLUS_EQ; }
				  else if(c2 == '+') { len++; charType = TT_INC; }
				  else charType = TT_PLUS; break;
		case '-': if(c2 == '=') { len++; charType = TT_MINUS_EQ; }
				  else if(c2 == '-') { len++; charType = TT_DEC; }
				  else charType = TT_MINUS; break;
		case '&': if(c2 == '=') { len++; charType = TT_BIN_AND_EQ; }
				  else if(c2 == '&') { len++; charType = TT_BOOL_AND; }
				  else charType = TT_BIN_AND; break;
		case '|': if(c2 == '=') { len++; charType = TT_BIN_OR_EQ; }
				  else if(c2 == '|') { len++; charType = TT_BOOL_OR; }
				  else charType = TT_BIN_OR; break;

		case '=': if(c2 == '=') { len++; charType = TT_EQ_EQ; }
				  else { charType = TT_EQ; } break;
		case '!': if(c2 == '=') { len++; charType = TT_NEQ; }
				  else { charType = TT_NOT; } break;
		case '*': if(c2 == '=') { len++; charType = TT_TIMES_EQ; }
				  else { charType = TT_TIMES; } break;
		case '/': if(c2 == '=') { len++; charType = TT_DIVIDE_EQ; }
				  else { charType = TT_DIVIDE; } break;
		case '^': if(c2 == '=') { len++; charType = TT_BIN_XOR_EQ; }
				  else if(c2 == '^')
		{
			if(c3 == '=') { len += 2; charType = TT_POW_EQ; }
			else { len++; charType = TT_POW; }
		}
				  else { charType = TT_BIN_XOR; } break;
		case '%': if(c2 == '=') { len++;  charType = TT_MOD_EQ; }
				  else { charType = TT_MOD; } break;
		case '<': if(c2 == '=') { len++; charType = TT_LT_EQ; }
				  else if(c2 == '<')
		{
			if(c3 == '=') { len += 2; charType = TT_SHIFT_LEFT_EQ; }
			else { len++; charType = TT_SHIFT_LEFT; }
		}
				  else { charType = TT_LT; } break;
		case '>': if(c2 == '=') { len++; charType = TT_GT_EQ; }
				  else if(c2 == '>')
		{
			if(c3 == '=') { len += 2; charType = TT_SHIFT_RIGHT_EQ; }
			else { len++; charType = TT_SHIFT_RIGHT; }
		}
				  else { charType = TT_GT; } break;
		default: charType = TT_INVALID; break;
	}

	return std::pair<TokenType, int>(charType, len);
}

TokensResult Tokenize(char* _srcChars, int _srcLength)
{
	std::vector<Token> tokens;
	int line = 1, column = 1;
	TokenResult finalResult = TokenResult::GenSuccess(Token());
	bool commentLine = false, commentText = false;

	for(int i = 0; i < _srcLength; i++)
	{
		char curChar = _srcChars[i];
		char lookAheadChar = _srcChars[i + 1];

		if(commentLine)
		{
			column++;

			if(curChar == '\n' || curChar == '\r') { commentLine = false; }
			else { continue; }
		}

		switch(curChar)
		{
			case '\n':
			case '\r': {
				line++;
				column = 1;
			} break;
			case ' ': { column++; } break;
			case '\t': { column += 4; } break;
			case '"': {
				char* startChar = _srcChars + i + 1;
				int offset = 0;

				finalResult = TokenizeStringLiteral(startChar, offset, line, column);
				if(finalResult.success)
				{
					tokens.push_back(finalResult.value);
					i += offset + 1;
					column += offset + 2;
				}
			} break;
			case '#': {
				commentLine = true;
			} break;
			default: {
				char* startChar = _srcChars + i;
				auto charTokenType = MatchChars(startChar);

				if(charTokenType.first != TT_INVALID)
				{
					tokens.push_back(Token(charTokenType.first, line, column));
					column += charTokenType.second;
					i += charTokenType.second - 1;
				}
				else
				{
					int offset = 0;

					if(isdigit(curChar)) { finalResult = TokenizeNumberLiteral(startChar, offset, line, column); }
					else if(curChar == '_' || isalpha(curChar)) { finalResult = TokenizeWord(startChar, offset, line, column); }
					else { finalResult = TokenResult::GenFailure("Tokenizing Error: Invalid character encountered", Position(line, column)); }

					if(finalResult.success)
					{
						tokens.push_back(finalResult.value);
						i += offset - 1;
						column += offset;
					}
				}
			} break;
		}

		if(!finalResult.success)
			break;
	}

	if(finalResult.success)
	{
		if(tokens.size() > 0) { tokens.push_back(Token(TT_ENDOFFILE, tokens[tokens.size() - 1].position.line, tokens[tokens.size() - 1].position.column + 1)); }
		else { tokens.push_back(Token(TT_ENDOFFILE, line, column)); }

		return TokensResult::GenSuccess(std::vector<Token>(tokens));
	}
	else { return TokensResult::GenFailure(finalResult.message, Position(line, column)); }
}

TokensResult TokenizeFile(const char* _path, std::vector<std::string>& _fileLines)
{
	std::ifstream srcFile(_path);
	std::stringstream stream;
	stream << srcFile.rdbuf();

	std::string line;
	while(std::getline(stream, line, '\n'))
	{
		int first = line.find_first_not_of(' \t');
		std::string trimmedLine;

		if(first == std::string::npos) { trimmedLine = ""; }
		else { trimmedLine = line.substr(first, (line.find_last_not_of(' \t') - first + 1)); }

		_fileLines.push_back(trimmedLine);
	}

	std::string srcText = stream.str();
	int srcLength = srcText.length();
	int bufSize = sizeof(char) * (srcLength + 1);
	char* srcChars = (char*)malloc(bufSize);

	strcpy_s(srcChars, bufSize, srcText.c_str());

	TokensResult tokenizeResult = Tokenize(srcChars, srcLength);

	free(srcChars);
	return tokenizeResult;
}
