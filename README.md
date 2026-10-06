# LineLog

A small C++17 logger. A temporary object carries the level, file name, and line number. `<<` appends text, and `EndLine` finishes the line.

Messages finer than the shared maximum level produce no output, including the newline. The default maximum is `Info`, so `Debug` stays silent until `SetMaxLevel(Level::Debug)`.

```cpp
#include "line_log/line_log.h"

logx::LineLog(logx::Level::Info, "main.cc", 10) << "服务" << "已启动" << logx::EndLine{};
```

That prints `[info] main.cc:10: 服务已启动`. The two strings are concatenated with nothing between them. Behavior and the demo output are described in [docs/spec.html](docs/spec.html).

## Build

Requires Bazel 9.2.0.

```bash
bazel run //:line_log_demo
```

On Windows, put the Visual Studio path in `user.bazelrc`. That file is listed in `.gitignore` and is not part of this repository.

## License

MIT. See [LICENSE](LICENSE).
