#include <string>

namespace logx {
  enum class Level { Error = 0, Warn = 1, Info = 2, Debug = 3 };
    struct EndLine {};
    class LineLog {
      public:
        LineLog(Level level, const std::string& file, int line);
        static void SetMaxLevel(Level level);
        LineLog& operator<<(const std::string& message);
        LineLog& operator<<(const EndLine& end_line);

      private:
        static Level max_level_;
        static Level current_level_;
        static std::string current_file_;
        static int current_line_;
        static bool header_done_;
    };
}
