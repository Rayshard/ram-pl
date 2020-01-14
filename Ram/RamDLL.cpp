#include "pch.h"
#include "RamDLL.h"
#include "Value.h"
#include "Environment.h"

std::map<int, RamDLL*> loadedDLLs;
std::map<int, std::pair<std::string, RamDLL::dll_func>> loadedDLLFuncs;

RamDLL::RamDLL(Windows::HINSTANCE _handle, int _id)
{
	handle = _handle;
	id = _id;
}

RamDLL::~RamDLL()
{
	FreeLibrary(handle);

	for(auto it : funcIds)
		loadedDLLFuncs.erase(it);
}

int RamDLL::Load(const char * _filePath)
{
	Windows::HINSTANCE handle = Windows::LoadLibrary(_filePath);

	if(handle)
	{
		int dllID = std::rand();

		while(loadedDLLs.find(dllID) != loadedDLLs.end())
			dllID = std::rand();

		RamDLL* dll = new RamDLL(handle, dllID);

		loadedDLLs.insert_or_assign(dllID, dll);
		return dllID;
	}
	else { return DLL_NOT_LOADED; }
}

bool RamDLL::Free(int _id)
{
	auto search = loadedDLLs.find(_id);
	if(search == loadedDLLs.end()) { return false; }
	else
	{
		delete search->second;
		loadedDLLs.erase(_id);
		return true;
	}
}

void RamDLL::FreeAll()
{
	for(auto it : loadedDLLs)
		delete it.second;

	loadedDLLs.clear();
	loadedDLLFuncs.clear();
}

int RamDLL::LoadFunc(int _dllID, const char* _funcName)
{
	auto search = loadedDLLs.find(_dllID);

	if(search == loadedDLLs.end())
		return DLL_NOT_LOADED;

	RamDLL* dll = search->second;
	auto funcPtr = (dll_func)GetProcAddress(dll->handle, _funcName);

	if(!funcPtr)
		return DLL_FUNC_NOT_FOUND;

	int funcID = std::rand();

	while(loadedDLLFuncs.find(funcID) != loadedDLLFuncs.end())
		funcID = std::rand();

	loadedDLLFuncs.insert_or_assign(funcID, std::make_pair(std::string(_funcName), funcPtr));
	dll->funcIds.push_back(funcID);
	return funcID;
}

FuncValue * RamDLL::GetFuncValue(int _funcID, Environment* _execEnv, std::vector<Signature> _argSigs, Signature _retSig, Position _execPos)
{
	auto search = loadedDLLFuncs.find(_funcID);
	if(search == loadedDLLFuncs.end())
		return nullptr;

	auto funcNameAndPtr = search->second;
	int argCount = (int)_argSigs.size();

	builtInFunc body = [funcNameAndPtr, argCount](Environment* _env, Position _execPos)
	{
		SharedValue retVal = SharedValue(nullptr);
		std::vector<RamArgValue*> args;

		for(int i = 0; i < argCount; i++)
		{
			SharedValue argVal = _env->GetValue("arg" + std::to_string(i), _execPos, false);
			args.push_back(argVal->ToRamArgValue());
		}

		try
		{
			RamAny* ramRetVal = new RamAny();
			funcNameAndPtr.second(args.data(), ramRetVal);

			if(ramRetVal->type == RamAny::REXCEPTION) 
			{
				std::string exceptionName = ramRetVal->_exception.first;
				std::string exceptionMsg = ramRetVal->_exception.second;
				retVal = SHARE(new ExceptionValue(exceptionName, exceptionMsg, Trace(_execPos, _env->name, _env->filePath)));
			}
			else { retVal = SHARE(IValue::FromRamValue(ramRetVal, _execPos)); }

			delete ramRetVal;
		}
		catch(const std::exception&) { retVal = SHARE(Exception_DLLClosed(Trace(_execPos, _env->name, _env->filePath))); }

		//Delete Args
		for(auto it : args)
			delete it;

		return retVal;
	};

	std::vector<std::string> argNames;

	for(int i = 0; i < argCount; i++)
		argNames.push_back("arg" + std::to_string(i));

	return new FuncValue(_execEnv, funcNameAndPtr.first, body, argNames, _argSigs, _retSig, _execPos);
}
