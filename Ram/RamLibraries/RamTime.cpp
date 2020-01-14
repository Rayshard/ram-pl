#include "..//Ram.h"
#include <chrono>
#include <string>

extern "C"
{
  //Args: ()
  //Return: string
  __declspec(dllexport) void Time(RamArgs _args, RamReturnValue *_retVal)
  {
    using namespace std::chrono;

    auto time = system_clock::now().time_since_epoch();
    auto castedTime = duration_cast<nanoseconds>(time);
    _retVal->SetAsString(std::to_string(castedTime.count()).c_str());
  }

  //Args: (start:string, end:string, precision:string)
  //Return: string
  __declspec(dllexport) void GetElapsedTime(RamArgs _args, RamReturnValue *_retVal)
  {
    std::string start = std::string(_args[0]->AsCString());
    std::string end = std::string(_args[1]->AsCString());
    std::string precision = std::string(_args[2]->AsCString());

    long long convStart = 0LL, convEnd = 0LL;

    try
    {
      convStart = std::stoll(start);
    }
    catch (...)
    {
      _retVal->ThrowException("Invaild Argument", "'start' is of the wrong format!");
    }

    try
    {
      convEnd = std::stoll(end);
    }
    catch (...)
    {
      _retVal->ThrowException("Invaild Argument", "'end' is of the wrong format!");
    }

    long double elapsed = convEnd - convStart;

    if (precision == "s")
    {
      elapsed /= 1000000000.0L;
    }
    else if (precision == "ms")
    {
      elapsed /= 1000000.0L;
    }
    else if (precision == "us")
    {
      elapsed /= 1000.0L;
    }
    else if (precision != "ns")
    {
      _retVal->ThrowException("Invalid Argument", "'precision' can be 's', 'ms', 'us', or 'ns' only!");
    }

    _retVal->SetAsString(std::to_string(elapsed).c_str());
  }
}