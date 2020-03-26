#pragma once

template<typename T> struct TToken {
	T type;
	Position position;
	std::string value;

	TToken() = default;
	TToken(Position _pos) : type(T::INVAILD), position(_pos), value("") { }
	TToken(T _type, Position _pos, std::string _val) : type(type), position(_pos), value(_val) { }
	
	virtual std::string ToString(bool _includePos) = 0;
};