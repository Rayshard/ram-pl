#include "pch.h"

RamValue *CIntToRamValue(int _val) { return new RamInt(_val); }
RamValue *CFloatToRamValue(float _val) { return new RamFloat(_val); }
RamValue *CStringToRamValue(const char *_val, int _length) { return new RamString(std::string(_val)); }
RamValue *CBoolToRamValue(int _val) { return new RamBool(_val); }