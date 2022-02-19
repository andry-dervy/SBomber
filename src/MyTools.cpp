#include "MyTools.h"

namespace MyTools {

namespace FileLogger {

ILogger& FileLoggerSingletone::getInstance()
{
  static FileLoggerSingletone theInstance;
  return theInstance;
}

void FileLoggerSingletone::OpenLogFile(const std::string& FN)
{
  logOut.open(FN, std::ios_base::out);
}

void FileLoggerSingletone::CloseLogFile()
{
  if (logOut.is_open())
  {
    logOut.close();
  }
}

void FileLoggerSingletone::WriteToLog(const std::string &str)
{
  if (logOut.is_open())
  {
    logOut << str << std::endl;
  }
}

void FileLoggerSingletone::WriteToLog(const std::string &str, int n)
{
  if (logOut.is_open())
  {
    logOut << str << n << std::endl;
  }
}

void FileLoggerSingletone::WriteToLog(const std::string &str, double d)
{
  if (logOut.is_open())
  {
    logOut << str << d << std::endl;
  }
}

} // namespace FileLogger

ILogger& LoggerSingleton::getInstance()
{
  static LoggerSingleton theInstance;
  return theInstance;
}

void LoggerSingleton::OpenLogFile(const std::string& FN)
{
  FileLogger::FileLoggerSingletone::getInstance().OpenLogFile(FN);
}

void LoggerSingleton::CloseLogFile()
{
  FileLogger::FileLoggerSingletone::getInstance().CloseLogFile();
}

std::string LoggerSingleton::GetCurDateTime() const
{
  auto cur = std::chrono::system_clock::now();
  time_t time = std::chrono::system_clock::to_time_t(cur);
  char* buf = ctime(&time);
  return std::string(buf);
}

std::string LoggerSingleton::printLog(size_t& cnt, const std::string& curDateTime,const std::string& str) const
{
  std::stringstream ss;
  ss << "[" << ++cnt << "] " << curDateTime << " - " << str;
  return ss.str();
};

void LoggerSingleton::WriteToLog(const std::string &str)
{
  FileLogger::FileLoggerSingletone::getInstance().WriteToLog(
        printLog(counter,GetCurDateTime(),str));
}

void LoggerSingleton::WriteToLog(const std::string &str, int n)
{
  FileLogger::FileLoggerSingletone::getInstance().WriteToLog(
        printLog(counter,GetCurDateTime(),str),n);
}

void LoggerSingleton::WriteToLog(const std::string &str, double d)
{
  FileLogger::FileLoggerSingletone::getInstance().WriteToLog(
        printLog(counter,GetCurDateTime(),str),d);
}

} // namespace MyTools
