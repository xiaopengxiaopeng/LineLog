#include "line_log/line_log.h"

int main() {
  logx::LineLog(logx::Level::Info, "main.cc", 10) << "服务" << "已启动" << logx::EndLine{};
  logx::LineLog(logx::Level::Debug, "main.cc", 11) << "细节" << logx::EndLine{};
  logx::LineLog(logx::Level::Error, "main.cc", 12) << "连接失败" << logx::EndLine{};
  logx::LineLog::SetMaxLevel(logx::Level::Debug);
  logx::LineLog(logx::Level::Debug, "main.cc", 14) << "细节" << logx::EndLine{};
  return 0;
}
