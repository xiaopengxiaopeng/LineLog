# LineLog

A small C++17 logger. One temporary object carries the level, file name, and line number. `<<` appends the message, and `EndLine` finishes the line.

```cpp
#include "line_log/line_log.h"

logx::LineLog(logx::Level::Info, "main.cc", 10) << "服务" << "已启动" << logx::EndLine{};
```

A printed line looks like this:

```text
[2026-10-06 23:13:05.123] [info] [18432] [main.cc:10] : 服务已启动
```

The four brackets are local time (`YYYY-MM-DD HH:MM:SS.mmm`), level, operating-system thread id, then file and line. The time and thread id above are examples. `"服务"` and `"已启动"` are concatenated with nothing between them.

Levels are `Error = 0`, `Warn = 1`, `Info = 2`, and `Debug = 3`. A larger number is finer. A line is printed only when its level is less than or equal to the shared maximum. The default maximum is `Info`, so `Debug` is dropped, including its newline, until `SetMaxLevel(Level::Debug)`.

The whole line is colored by level: error red, warn yellow, info green, debug cyan. The color is reset before the newline. Set a non-empty `NO_COLOR` to print plain text.

Only one line can be assembled at a time. The demo output is shown in [docs/spec.html](docs/spec.html).

## Build

Requires Bazel 9.2.0.

```bash
bazel run //:line_log_demo
```

On Windows, put the Visual Studio path in `user.bazelrc`. That file is listed in `.gitignore` and is not part of this repository.

## License

MIT. See [LICENSE](LICENSE).
