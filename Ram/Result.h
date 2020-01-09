#pragma once

#include <string>

struct Position
{
	int line;
	int column;

	Position();
	Position(int _line, int _col);

	std::string ToString();
};

template <typename T>
class ResultT
{
public:
	bool success;
	T value;
	std::string message;
	Position position;

protected:
	ResultT(bool _success, T _val, std::string _msg, Position _pos);

public:
	static ResultT GenSuccess(T _val) { return ResultT(true, _val, std::string(), Position()); }
	static ResultT GenFailure(std::string _msg, Position _pos) { return ResultT(false, T(), _msg, _pos); }
};

typedef ResultT<void*> Result;

template<typename T>
inline ResultT<T>::ResultT(bool _success, T _val, std::string _msg, Position _pos)
{
	success = _success;
	value = _val;
	message = _msg;
	position = _pos;
}
