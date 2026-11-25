# 变更历史

维护者：kaisawind <wind.kaisa@gmail.com>

## [1.1.0] - 2026-11-25

### 新增

- 文件 → 打开：选择 MP3/WAV，切换目录、刷新列表并播放。
- 播放模式：顺序、单曲循环、列表循环、随机。
- 播放结束后按播放模式自动切歌。
- 进度条跳转：拖动/点击进度条调用 `SeekToMilliSecond`。
- DPI 感知和自适应布局：Per-Monitor V2、`WM_DPICHANGED`、最小窗口 640×480。
- 记住上次工作目录：exe 同目录的 `MusicPlayer.ini`。
- `Range.h`：区间、上一首/下一首和播放模式索引辅助函数。
- `tests/test_range.cpp` 和 CMake `MusicPlayerTests`。

### 变更

- 仓库名改为 `MusicPlayer`。
- 源码按 `app/audio/ui/common/res` 分目录。
- 类名：`CMusicProcessor` → `CAudioPlayer`，`CViewerProcessor` → `CPlayerView`。
- VS 工程升级到 VS 18（v145，SDK 10.0.26100.0）。
- 增加 CMake + MSVC 和 CMake + zig 构建。
- libzplay 确认升级到 2.02，补充版本和校验值说明。
- 注释统一为 Doxygen 风格：`@file/@brief/@param/@return/@author/@date`，历史使用 `@par 变更历史`；旧的 `@summary/@func/@in/@out/@time/@history` 已移除。
- 作者信息统一为 `kaisawind <wind.kaisa@gmail.com>`。
- 设计文档从 xlsx 线框转换为 `docs/设计文档.md`。

### 修复

- `WM_COMMAND` 处理函数重复调用，导致播放/暂停等动作执行两次。
- 长文件名导致 `LB_GETTEXT`/`lstrcpyW` 缓冲区溢出。
- 空列表或未选中时读取未初始化文件名。
- 下一曲在最后一项越界，选中项使用 `LB_SETCURSEL` 夹紧。
- 音量滚动条无符号回绕，滚动位置夹到 0–100。
- 列表滚动条越界、PageUp/PageDown 只滚一行。
- GDI DC 释放到错误窗口，以及 `WM_PAINT` 绕过绘制 DC。
- 系统设置为“滚一屏”时鼠标滚轮失效。
- Release 构建因 `Common.cpp` 未包含 `stdafx.h` 失败。
- 打开失败时恢复当前文件和播放位置。
- 进度条使用毫秒导致超过约 65 秒后范围溢出，改为按秒显示。

### 移除

- `CFilesProcessor`、未使用的 `UpdateListTable`、`MessageBoxPrintf`、`GetZPlayError`。
- `Common.cpp` 手写整数转换，改用 `wsprintf` 数字格式。
- 未实现的重复“打开”菜单项和空 TODO。
- 原始 `docs/设计文档.xlsx`（内容已转换为 Markdown + PNG）。

---

## [1.0.0] - 2013-03-24

- 初版 Win32 音乐播放器：窗口、列表、播放控制、libzplay 封装和关于对话框。
