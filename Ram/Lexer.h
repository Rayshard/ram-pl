#pragma once

enum TokenType {
	TT_INTEGER_LIT, TT_FLOAT_LIT, TT_STRING_LIT, TT_TRUE,
	TT_FALSE, TT_VOID, TT_IDENTIFIER,

	TT_SEMICOLON, TT_PERIOD, TT_COLON, TT_COMMA,

	TT_INC, TT_DEC, TT_NOT, TT_BIN_NOT,

	TT_PLUS, TT_MINUS, TT_TIMES, TT_DIVIDE, TT_POW, TT_MOD,
	TT_PLUS_EQ, TT_MINUS_EQ, TT_TIMES_EQ, TT_DIVIDE_EQ, TT_POW_EQ, TT_MOD_EQ,

	TT_BIN_AND, TT_BIN_OR, TT_BIN_XOR, TT_SHIFT_LEFT, TT_SHIFT_RIGHT,
	TT_BIN_AND_EQ, TT_BIN_OR_EQ, TT_BIN_XOR_EQ, TT_SHIFT_LEFT_EQ, TT_SHIFT_RIGHT_EQ,
	
	TT_LT, TT_GT, TT_LT_EQ, TT_GT_EQ, TT_EQ_EQ, TT_NEQ, TT_EQ,
	TT_BOOL_AND, TT_BOOL_OR,
	
	TT_LPAREN, TT_RPAREN, TT_LBRACKET, TT_RBRACKET,

	TT_IF, TT_ELSE, TT_LET, TT_FOR, TT_THEN, TT_FINALLY, TT_WHILE,
	TT_DEF, TT_FUNC, TT_RETURN, TT_BREAK, TT_CONTINUE, TT_DO,

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

TokensResult Tokenize(char* _srcChars, int _srcLength);
TokensResult TokenizeFile(const char* _path, std::vector<std::string>& _fileLines);
