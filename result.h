#pragma once

template<typename R, typename T> class Result {
protected:
	R type;
	T value;

	std::string errString;
	Position errPosition;
	Result(R _type, T _val, std::string _errStr, Position _errPos) : type(_type), value(_val), errString(_errStr), errPosition(_errPos) { }
public:
	virtual T GetValue() = 0;
	virtual std::string ToString(bool _includePos) = 0;

	bool IsSuccess() { return type == R::SUCCESS; }
	R GetType() { return type; }
	Position GetErrorPosition() { return errPosition; }
	std::string GetErrorString() { return errString; }
};
