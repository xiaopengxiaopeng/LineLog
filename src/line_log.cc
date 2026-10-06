#include "line_log/line_log.h"
#include <iostream>
#include<string>

namespace logx {
  Level LineLog::max_level_ = Level::Info;
  Level LineLog::current_level_ = Level::Info;
  std::string LineLog::current_file_ = "";
  int LineLog::current_line_ = 0;
  bool LineLog::header_done_ = false;
  std::string level_to_string(Level level) {
    switch (level) {
      case Level::Error: return "error";
      case Level::Warn: return "warn";
      case Level::Info: return "info";
      case Level::Debug: return "debug";
    }
    return "unknown";
  }
  LineLog::LineLog(Level level, const std::string& file, int line) {
    current_level_ = level;
    current_file_ = file;
    current_line_ = line;
    header_done_ = false;
  }
    void LineLog::SetMaxLevel(Level level){
      max_level_ = level;
    }
    LineLog& LineLog::operator<<(const std::string& message){
      if(static_cast<int>(current_level_) > static_cast<int>(max_level_)){
        return *this;
      }else{
        if(!header_done_){
          header_done_ = true;
          std::cout << "[" << level_to_string(current_level_) << "] " << current_file_ << ":" << std::to_string(current_line_) << ": ";
        }
        std::cout << message;
        return *this;
      }
    }
    LineLog& LineLog::operator<<(const EndLine&){
      if(static_cast<int>(current_level_) > static_cast<int>(max_level_)){
        return *this;
      }else{
        std::cout << std::endl;
        header_done_ = false;
        return *this;
      }
    }



}
