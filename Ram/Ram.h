#pragma once

struct RamArgValue
{
	virtual int AsCInt() = 0;
	virtual float AsCFloat() = 0;
	virtual bool AsCBool() = 0;
	virtual const char * AsCString() = 0;
};

struct RamReturnValue
{
	virtual void SetAsInt(int) = 0;
	virtual void SetAsFloat(float) = 0;
	virtual void SetAsBool(bool) = 0;
	virtual void SetAsString(const char*) = 0;
	virtual void ThrowException(const char* _name, const char* _msg) = 0;
};

typedef RamArgValue** RamArgs;
