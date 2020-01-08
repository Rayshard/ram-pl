#pragma once

#include <string>

template <typename T>
class ResultT
{
public:
	bool success;
	T value;
	std::string message;

protected:
	ResultT(bool _success, T _val, std::string _msg);

public:
	static ResultT GenSuccess(T _val) { return ResultT(true, _val, std::string()); }
	static ResultT GenFailure(std::string _msg) { return ResultT(false, T(), _msg); }
};

typedef ResultT<void*> Result;

template<typename T>
inline ResultT<T>::ResultT(bool _success, T _val, std::string _msg)
{
	success = _success;
	value = _val;
	message = _msg;
}
