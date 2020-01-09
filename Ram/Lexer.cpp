#include "Lexer.h"
#include <sstream>
#include <fstream>
#include "Grammar\Grammar.h"
#include "Interpreter.h"

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
		case TT_EQUALS:
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
		else if(word == "void") { type = TT_VOID; }
		else if(word == "int") { type = TT_TYPE_INT; }
		else if(word == "float") { type = TT_TYPE_FLOAT; }
		else if(word == "string") { type = TT_TYPE_STRING; }
		else if(word == "bool") { type = TT_TYPE_BOOL; }
		else if(word == "for") { type = TT_FOR; }
		else if(word == "then") { type = TT_THEN; }
		else if(word == "finally") { type = TT_FINALLY; }
		else if(word == "while") { type = TT_WHILE; }
		else if(word == "def") { type = TT_DEF; }
		else if(word == "func") { type = TT_FUNC; }

		_offset = offset;
		return TokenResult::GenSuccess((type == TT_IDENTIFIER || IsTypeName(type)) ? Token(type, _line, _col, word) : Token(type, _line, _col));
	}
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
			case ';': {
				tokens.push_back(Token(TT_SEMICOLON, line, column));
				column++;
			} break;
			case '+': {
				tokens.push_back(Token(TT_PLUS, line, column));
				column++;
			} break;
			case '-': {
				tokens.push_back(Token(TT_MINUS, line, column));
				column++;
			} break;
			case '*': {
				tokens.push_back(Token(TT_TIMES, line, column));
				column++;
			} break;
			case '/': {
				tokens.push_back(Token(TT_DIVIDE, line, column));
				column++;
			} break;
			case ':': {
				tokens.push_back(Token(TT_COLON, line, column));
				column++;
			} break;
			case ',': {
				tokens.push_back(Token(TT_COMMA, line, column));
				column++;
			} break;
			case '=': {
				if(lookAheadChar == '=')
				{
					tokens.push_back(Token(TT_EQEQ, line, column));
					i++;
					column += 2;
				}
				else
				{
					tokens.push_back(Token(TT_EQUALS, line, column));
					column++;
				}
			} break;
			case '(': {
				tokens.push_back(Token(TT_LPAREN, line, column));
				column++;
			} break;
			case ')': {
				tokens.push_back(Token(TT_RPAREN, line, column));
				column++;
			} break;
			case '{': {
				tokens.push_back(Token(TT_LBRACKET, line, column));
				column++;
			} break;
			case '}': {
				tokens.push_back(Token(TT_RBRACKET, line, column));
				column++;
			} break;
			case '<': {
				if(lookAheadChar == '=')
				{
					tokens.push_back(Token(TT_LTEQ, line, column));
					i++;
					column += 2;
				}
				else
				{
					tokens.push_back(Token(TT_LT, line, column));
					column++;
				}
			} break;
			case '>': {
				if(lookAheadChar == '=')
				{
					tokens.push_back(Token(TT_GTEQ, line, column));
					i++;
					column += 2;
				}
				else
				{
					tokens.push_back(Token(TT_GT, line, column));
					column++;
				}
			} break;
			case '!': {
				if(lookAheadChar == '=')
				{
					tokens.push_back(Token(TT_NEQ, line, column));
					i++;
					column += 2;
				}
				else
				{
					tokens.push_back(Token(TT_NOT, line, column));
					column++;
				}
			} break;
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
			case '.': {
				if(isdigit(lookAheadChar))
				{
					char* startChar = _srcChars + i;
					int offset = 0;

					finalResult = TokenizeNumberLiteral(startChar, offset, line, column);
					if(finalResult.success)
					{
						tokens.push_back(finalResult.value);
						i += offset - 1;
						column += offset;
					}
				}
				else
				{
					tokens.push_back(Token(TT_PERIOD, line, column));
					column++;
				}
			} break;
			case '#': {
				commentLine = true;
			} break;
			default: {
				char* startChar = _srcChars + i;
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
			} break;
		}

		if(!finalResult.success)
			break;
	}

	if(finalResult.success)
	{
		tokens.push_back(Token(TT_ENDOFFILE, line, column));
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
		size_t first = line.find_first_not_of(' \t');
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

std::string GetSrcLine(int _line) { return Interpreter::SRC_LINES[_line - 1]; }