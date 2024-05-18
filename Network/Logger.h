#pragma once
#include <string>
#include <time.h>
#include <concurrent_queue.h>
#include <thread>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "Macro.h"
#include "Singleton.h"

#define LOG_BUFFER_SIZE 2048

#ifdef UNICODE
	using char_t = wchar_t;
	using string_t = std::wstring;
	using fstream_t = std::wfstream;
#else
	using char_t = char;
	using string_t = std::string;
	using fstream_t = std::fstream;
#endif

enum class LogLevel : unsigned char
{
    DEBUG = 0,
    WARNING,
    ERROR,
};

#ifdef UNICODE
	constexpr const char_t* log_level_str[]
    {
    	L"[DEBUG] ",
    	L"[WARNING] ",
    	L"[ERROR] "
    };
#else
constexpr const char_t* log_level_str[]
{
	"[DEBUG] ",
	"[WARNING] ",
	"[ERROR] "
};
#endif

struct stLog
{
    LogLevel _level;
    time_t _writeTime;
    string_t _contents;
};

class Logger : public Singleton<Logger>
{
public:
	Logger() = default;
	~Logger() = default;
	
public:

	bool Initialize(const std::filesystem::path filePath, string_t fileName)
	{
		time_t rawTime = time(NULL);
		auto ret = localtime_s(&_currentDay, &rawTime);
		if(ret != 0)
			return false;

		_filePath = std::filesystem::current_path()/filePath;
		std::cout << _filePath;
		SetFileName(_currentDay, fileName);

		FileOpen();
		
		return true;
	}

	void Finalize()
	{
		_writeThread.join();
	}

	void Start()
	{
		 _writeThread = std::thread(&Logger::Execute, this);
	}

	void Stop()
	{
	}
	
	void Register(LogLevel level, const char_t* contents)
	{
		string_t logStr(contents);
		stLog log;
		log._level = level;
		log._writeTime = time(NULL);
		log._contents = logStr;

		_registeredLog.push(log);
	}

	template<typename ...Args>
	void Register(LogLevel level,const char_t* formatStr , Args... args)
	{
		char_t logStr[LOG_BUFFER_SIZE] = {0, };
#ifdef _UNICODE
		_snwprintf_s(logStr, _countof(logStr), LOG_BUFFER_SIZE, formatStr, args...);
#else
		_snprintf_s(logStr, _countof(logStr), LOG_BUFFER_SIZE, formatStr, args...);
#endif
		
		Register(level,logStr);
	}


private:
	void FileOpen()
	{
		_fileStream.open(_filePath/_fileName, std::ios::app);
		if(_fileStream.is_open() == false)
		{
			std::cout << "Error: " << errno;
			xstop();
		}
	}

	void Write()
	{
		while (_registeredLog.empty() == false)
		{
			stLog log;
			if(_registeredLog.try_pop(log) == false)
				continue;
			
			tm time;
			localtime_s(&time, &log._writeTime);

			if(time.tm_mday != _currentDay.tm_mday)
			{
				SetFileName(time);
				FileClose();
				FileOpen();
			}

			if(_logCounter >= _logFileCountThreshold)
			{
				_fileCounter++;
				SetFileName(time);
				FileClose();
				FileOpen();
			}

#ifdef UNICODE
			auto totalContents = std::format(L"{} [{:d}/{:d}/{:d}] [{:d}:{:d}:{:d}] {} [{} {}:{}]",log_level_str[static_cast<size_t>(log._level)], time.tm_year + 1900,time.tm_mon + 1, time.tm_mday
						,time.tm_hour, time.tm_min, time.tm_sec, log._contents, __FILEW__, __FUNCTIONW__, __LINE__);
#else
			auto totalContents = std::format(L"{} [{:d}/{:d}/{:d}] [{:d}:{:d}:{:d}] {} [{} {}:{}]",log_level_str[static_cast<size_t>(log._level)], time.tm_year + 1900,time.tm_mon + 1, time.tm_mday
            						,time.tm_hour, time.tm_min, time.tm_sec, log._contents, __FILE__, __FUNCTION__, __LINE__);
#endif
			_fileStream << totalContents << std::endl;
			FileClose();
			FileOpen();
		}
		
	}

	void FileClose()
	{
		_fileStream.close();
	}

	void Execute()
	{
		while(_stopFlag == false)
		{
			Write();

			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}
		
	}

private:
	template<typename T = string_t>
	void SetFileName(tm& time, T name)
	{
#ifdef UNICODE
		_fileName = std::format(L"[{:d}_{:d}_{:d}][{:d}_{:d}_{:d}]{}_{:d}.txt", time.tm_year + 1900,time.tm_mon + 1, time.tm_mday
			,time.tm_hour, time.tm_min, time.tm_sec, name, _fileCounter);
#else
		_fileName = std::format("[{:d}/{:d}/{:d}] [{:d}:{:d}:{:d}] {}_{:d}.txt", time.tm_year + 1900,time.tm_mon + 1, time.tm_mday
			,time.tm_hour, time.tm_min, time.tm_sec, name, _fileCounter));
#endif
		
	}

	void SetFileName(tm& time)
	{
		SetFileName(time,_fileName);
	}
private:
	bool _stopFlag;

	const int _logFileCountThreshold = 500;
private:
	fstream_t _fileStream;
	
	std::thread _writeThread;
	Concurrency::concurrent_queue<stLog> _registeredLog;
	
	tm _currentDay;

	std::filesystem::path _filePath;
	string_t _fileName;
	int _fileCounter;
	int _logCounter;
};

#define LOGD(...) Logger::Instance().Register(LogLevel::DEBUG, __VA_ARGS__)
#define LOGW(...) Logger::Instance().Register(LogLevel::WRANING, __VA_ARGS__)
#define LOGE(...) Logger::Instance().Register(LogLevel::ERROR, __VA_ARGS__)

