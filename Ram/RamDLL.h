#pragma once

class FuncValue;

#define DLL_NOT_LOADED  -1
#define DLL_FUNC_NOT_FOUND -2

class RamDLL
{
private:
	Windows::HINSTANCE handle;
	std::vector<int> funcIds;
	int id;

	RamDLL(Windows::HINSTANCE _handle, int _id);
	~RamDLL();
public:
	typedef void(*dll_func)(RamArgs, RamReturnValue*);

	int GetID() { return id; }

	static int Load(const char* _filePath);
	static bool Free(int _id);
	static void FreeAll();
	static int LoadFunc(int _dllID, const char* _funcName);
	static FuncValue * GetFuncValue(int _funcID, Environment* _execEnv, std::vector<Signature> _argSigs, Signature _retSig, Position _execPos);
};