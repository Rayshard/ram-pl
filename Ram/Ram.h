#pragma once

struct RamValue
{
	virtual int AsCInt() = 0;
	virtual float AsCFloat() = 0;
	virtual bool AsCBool() = 0;
	virtual const char* AsCString() = 0;
};

typedef RamValue** RamArgs;

RamValue* CIntToRamValue(int);
RamValue* CFloatToRamValue(float);
RamValue* CStringToRamValue(const char*, int);
RamValue* CBoolToRamValue(int);