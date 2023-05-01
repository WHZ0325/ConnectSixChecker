### 简介

用 C++ 语言写的非常简陋的六子棋 Checker，可用于人人对战，人机对战，机人对战，机机对战（

### 运行

需要在安装了 C++11 的 macOS/Linux 系统下运行。

编译：
```bash
make
```

运行：
```bash
./checker user user
```

两个参数分别表示下棋的双方，"user" 表示该方是通过命令行窗口交互的玩家，其它则表示该方是以 该字符串.cpp 为文件名的 C++ 程序。

玩家的交互规范为：
- 先手时给出两个整数 x,y，表示第一颗黑子的坐标 (x, y)。
- 否则需要给出四个整数 x0,y0,x1,y1，表示当前回合我方落子的两个位置 (x0, y0) 和 (x1, y1)。

程序的交互规范可见 [BotZone](https://wiki.botzone.org.cn/index.php?title=ConnectSix)。

### 其它

这个仓库的真实用途其实是测试一下 Git 和 GNUMake……
