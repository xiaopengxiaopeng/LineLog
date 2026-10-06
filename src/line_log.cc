#include "line_log/line_log.h"

#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace logx {
Level LineLog::max_level_ = Level::Info;
Level LineLog::current_level_ = Level::Info;
std::string LineLog::current_file_ = "";
int LineLog::current_line_ = 0;
bool LineLog::header_done_ = false;

namespace {

void EnableVirtualTerminal() {
#ifdef _WIN32
  HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
  if (out == nullptr || out == INVALID_HANDLE_VALUE) {
    return;
  }
  DWORD mode = 0;
  if (!GetConsoleMode(out, &mode)) {
    return;
  }
  if ((mode & ENABLE_VIRTUAL_TERMINAL_PROCESSING) == 0) {
    SetConsoleMode(out, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
  }
#endif
}

bool UseColor() {
  static const bool enabled = [] {
    EnableVirtualTerminal();
    const char* no_color = std::getenv("NO_COLOR");
    return no_color == nullptr || no_color[0] == '\0';
  }();
  return enabled;
}

std::string LocalTimeText() {
  const auto now = std::chrono::system_clock::now();
  const auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(
                          now.time_since_epoch()) %
                      1000;
  const std::time_t seconds = std::chrono::system_clock::to_time_t(now);
  std::tm calendar{};
#ifdef _WIN32
  localtime_s(&calendar, &seconds);
#else
  localtime_r(&seconds, &calendar);
#endif
  std::ostringstream text;
  text << std::put_time(&calendar, "%Y-%m-%d %H:%M:%S") << '.' << std::setfill('0')
       << std::setw(3) << static_cast<int>(millis.count());
  return text.str();
}

std::string ThreadText() {
#ifdef _WIN32
  return std::to_string(GetCurrentThreadId());
#else
  std::ostringstream text;
  text << std::this_thread::get_id();
  return text.str();
#endif
}

const char* LevelColor(Level level) {
  switch (level) {
    case Level::Error:
      return "\033[31m";
    case Level::Warn:
      return "\033[33m";
    case Level::Info:
      return "\033[32m";
    case Level::Debug:
      return "\033[36m";
  }
  return "\033[0m";
}

}  // namespace

std::string level_to_string(Level level) {
  switch (level) {
    case Level::Error:
      return "error";
    case Level::Warn:
      return "warn";
    case Level::Info:
      return "info";
    case Level::Debug:
      return "debug";
  }
  return "unknown";
}

LineLog::LineLog(Level level, const std::string& file, int line) {
  current_level_ = level;
  current_file_ = file;
  current_line_ = line;
  header_done_ = false;
}

void LineLog::SetMaxLevel(Level level) { max_level_ = level; }

LineLog& LineLog::operator<<(const std::string& message) {
  if (static_cast<int>(current_level_) > static_cast<int>(max_level_)) {
    return *this;
  }
  if (!header_done_) {
    header_done_ = true;
    if (UseColor()) {
      std::cout << LevelColor(current_level_);
    }
    std::cout << "[" << LocalTimeText() << "] "
              << "[" << level_to_string(current_level_) << "] "
              << "[" << ThreadText() << "] "
              << "[" << current_file_ << ":" << std::to_string(current_line_) << "] "
              << ": ";
  }
  std::cout << message;
  return *this;
}

LineLog& LineLog::operator<<(const EndLine&) {
  if (static_cast<int>(current_level_) > static_cast<int>(max_level_)) {
    return *this;
  }
  if (UseColor()) {
    std::cout << "\033[0m";
  }
  std::cout << std::endl;
  header_done_ = false;
  return *this;
}

}  // namespace logx
