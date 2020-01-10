#pragma once

enum TokenType {
	TT_INTEGER_LIT, TT_FLOAT_LIT, TT_STRING_LIT, TT_IDENTIFIER,
	TT_TRUE, TT_FALSE, TT_VOID,
	TT_SEMICOLON, TT_PERIOD, TT_EQUALS, TT_COLON, TT_COMMA,
	TT_PLUS, TT_MINUS, TT_TIMES, TT_DIVIDE,
	TT_LT, TT_GT, TT_LTEQ, TT_GTEQ, TT_EQEQ, TT_NEQ, TT_NOT,
	TT_LPAREN, TT_RPAREN, TT_LBRACKET, TT_RBRACKET,
	TT_IF, TT_ELSE, TT_LET, TT_FOR, TT_THEN, TT_FINALLY, TT_WHILE,
	TT_DEF, TT_FUNC, TT_RETURN, TT_BREAK, TT_CONTINUE, TT_DO,
	TT_TYPE_INT, TT_TYPE_FLOAT, TT_TYPE_STRING, TT_TYPE_BOOL,
	TT_ENDOFFILE, TT_INVALID
};

struct Token
{
	TokenType type;
	std::string value;
	Position position;

	Token();
	Token(TokenType _type, int _line, int _col);
	Token(TokenType _type, int _line, int _col, std::string _val);

	std::string ToString();
};

struct TokenReader
{
	Token* tokens;
	int index;
	Token current;

	TokenReader();
	TokenReader(Token* _tokens);

	Token* Advance();
	Token* GetCurPtr();
	TokenType GetCurType();
	Position GetCurPosition();
	bool AtEOF();
};

typedef ResultT<Token> TokenResult;
typedef ResultT<std::vector<Token>> TokensResult;

TokenResult TokenizeStringLiteral(char* _chars, int& _offset, int _line, int _col);
TokenResult TokenizeNumberLiteral(char* _chars, int& _offset, int _line, int _col);
TokenResult TokenizeWord(char* _chars, int& _offset, int _line, int _col);
TokensResult Tokenize(char* _srcChars, int _srcLength);
TokensResult TokenizeFile(const char* _path, std::vector<std::string>& _fileLines);
