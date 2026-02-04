# MBox
我的智能小工具集

## 📁 项目内容 / Projects

### 🍅 PomodoroTimer - 番茄钟
位置 / Location: `PomodoroTimer/`

一个简单实用的番茄工作法计时器，帮助你更高效地管理时间。
A simple and practical Pomodoro timer to help you manage your time more efficiently.

**特点 / Features:**
- ⏱️ 标准番茄工作法（25分钟工作 + 5分钟短休 + 15分钟长休）
- 📊 实时进度显示和完成统计
- 🔔 完成提醒功能
- 🎯 智能推荐下一个时段

**快速开始 / Quick Start:**
```bash
cd PomodoroTimer/PomodoroTimer
dotnet run
```

**详细说明 / Documentation:**
- 使用说明: [PomodoroTimer/README.md](PomodoroTimer/README.md)

---

### ☕ Joy's Coffee Shop Website
位置 / Location: `website/`

一个专业的咖啡店落地页，采用温馨的橙色主题设计。
A professional coffee shop landing page with a cozy orange theme.

**特点 / Features:**
- 🎨 温馨的橙色主题设计 / Cozy orange theme
- 📱 完全响应式 / Fully responsive
- 🚀 SEO 优化 / SEO optimized
- ⚡ 快速加载 / Fast loading

**快速开始 / Quick Start:**
```bash
cd website
python -m http.server 8000
# 访问 http://localhost:8000
```

**发布指南 / Publishing Guide:**
- 详细部署文档: [website/DEPLOYMENT.md](website/DEPLOYMENT.md)
- 发布演练: [website/WALKTHROUGH.md](website/WALKTHROUGH.md)
- 项目说明: [website/README.md](website/README.md)

### 📱 SpeedMobile - 截图整理助手
位置 / Location: `SpeedMobile/`

一个微信小程序，用于解决手机截图过多占用空间、手机屏幕太小不便整理的痛点。
A WeChat mini-program for organizing phone screenshots, solving storage issues and small screen inconvenience.

**特点 / Features:**
- 📤 批量上传截图 / Batch upload screenshots
- 💻 电脑端浏览下载 / Browse and download on PC
- 🔄 双端同步删除 / Sync deletion across devices
- 🚀 快速整理管理 / Quick organization and management

**快速开始 / Quick Start:**
```bash
# 启动后端服务
cd SpeedMobile/api
npm install
npm start

# 使用微信开发者工具打开 SpeedMobile/mp 目录
```

**项目说明 / Documentation:**
- 详细文档: [SpeedMobile/README.md](SpeedMobile/README.md)

---

### 🖼️ ImageSmall - 图片压缩工具
位置 / Location: `ImageSmall/`

一个简单易用的图片批量压缩工具，使用 C# 和 Windows Forms 开发。
A simple and easy-to-use batch image compression tool developed with C# and Windows Forms.

**特点 / Features:**
- 📁 支持批量选择和压缩多个图片文件 / Batch select and compress multiple image files
- 📂 支持选择文件夹批量导入图片 / Support folder selection for batch import
- 🎚️ 可调节压缩质量（10% - 100%） / Adjustable compression quality (10% - 100%)
- 📊 实时显示压缩进度 / Real-time compression progress display
- 💾 自定义输出目录 / Custom output directory
- 🖼️ 支持多种图片格式：JPG, PNG, BMP, GIF / Support multiple formats: JPG, PNG, BMP, GIF

**快速开始 / Quick Start:**
```bash
cd ImageSmall/ImageSmall
dotnet restore
dotnet build -c Release
# 运行 / Run
dotnet run
```

**项目说明 / Documentation:**
- 详细文档: [ImageSmall/README.md](ImageSmall/README.md)
