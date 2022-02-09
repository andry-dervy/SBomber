#pragma once

#include <string>
#include <cstring>

namespace MyTools {

class ILogger {
public:
  virtual ~ILogger() = default;
  virtual void OpenLogFile(const std::string& FN) = 0;
  virtual void CloseLogFile() = 0;
  virtual void WriteToLog(const std::string& str) = 0;
  virtual void WriteToLog(const std::string& str, int n) = 0;
  virtual void WriteToLog(const std::string& str, double d) = 0;
};

namespace FileLogger {

class FileLoggerSingletone: public ILogger
{
public:
  static ILogger& getInstance();

  virtual void OpenLogFile(const std::string& FN);
  virtual void CloseLogFile();
  virtual void WriteToLog(const std::string& str);
  virtual void WriteToLog(const std::string& str, int n);
  virtual void WriteToLog(const std::string& str, double d);

private:
  static std::ofstream logOut;

  FileLoggerSingletone() = default;
  ~FileLoggerSingletone() = default;
  FileLoggerSingletone(const FileLoggerSingletone& logger) = delete;
  FileLoggerSingletone& operator=(const FileLoggerSingletone&) = delete;
  FileLoggerSingletone(FileLoggerSingletone&& logger) = delete;
  FileLoggerSingletone& operator=(FileLoggerSingletone&&) = delete;
};

} // namespace FileLogger

class LoggerSingleton: public ILogger
{
public:
  static ILogger& getInstance();

  virtual void OpenLogFile(const std::string& FN);
  virtual void CloseLogFile();
  virtual void WriteToLog(const std::string& str);
  virtual void WriteToLog(const std::string& str, int n);
  virtual void WriteToLog(const std::string& str, double d);

private:
  size_t counter;

  LoggerSingleton():counter(0){}
  ~LoggerSingleton() = default;
  LoggerSingleton(const LoggerSingleton& logger) = delete;
  LoggerSingleton& operator=(const LoggerSingleton&) = delete;
  LoggerSingleton(LoggerSingleton&& logger) = delete;
  LoggerSingleton& operator=(LoggerSingleton&&) = delete;

  std::string GetCurDateTime() const;
  std::string printLog(size_t& cnt, const std::string& curDateTime,const std::string& str) const;
};

} // namespace MyTools
