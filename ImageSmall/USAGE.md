# ImageSmall 使用指南

## 快速开始

### 1. 构建程序

在项目根目录下运行以下命令：

```bash
cd ImageSmall/ImageSmall
dotnet restore
dotnet build -c Release
```

### 2. 发布独立可执行文件（推荐）

如果你想创建一个不依赖 .NET 运行时的独立可执行文件：

```bash
cd ImageSmall/ImageSmall
dotnet publish -c Release -r win-x64 --self-contained true -p:PublishSingleFile=true -p:IncludeNativeLibrariesForSelfExtract=true
```

发布后的可执行文件位于：
- `bin/Release/net6.0-windows/win-x64/publish/ImageSmall.exe`

### 3. 运行程序

双击 `ImageSmall.exe` 即可运行程序。

## 功能说明

### 选择图片
1. 点击"选择图片"按钮
2. 在文件对话框中选择一个或多个图片文件
3. 支持的格式：JPG, JPEG, PNG, BMP, GIF
4. 可以多次点击"选择图片"按钮添加更多文件

### 清空列表
点击"清空列表"按钮可以清除已选择的所有图片。

### 设置输出目录
1. 在"输出目录"文本框中输入目标文件夹路径，或
2. 点击"浏览..."按钮选择输出文件夹
3. 如果不设置，默认保存到桌面的"压缩后的图片"文件夹

### 调整压缩质量
- 使用滑块调整压缩质量（10% - 100%）
- 75% 是推荐的平衡点，既能保持较好的画质，又能有效减小文件大小
- 质量越低，压缩后的文件越小，但画质也会降低

### 开始压缩
1. 点击"开始压缩"按钮
2. 程序会逐个处理选中的图片
3. 进度条会显示当前进度
4. 压缩完成后会弹出提示框显示结果

## 注意事项

- 压缩后的文件名会在原文件名后添加 `_compressed` 后缀
- 例如：`photo.jpg` 压缩后变为 `photo_compressed.jpg`
- 程序不会覆盖原文件，原文件保持不变
- PNG 格式的压缩效果可能不如 JPG 明显

## 技术特点

- 使用 C# Windows Forms 开发
- 基于 .NET 6.0 框架
- 使用 System.Drawing.Common 进行图片处理
- 支持多线程异步处理，界面不会卡顿

## 问题反馈

如遇到问题，请在 GitHub 上提交 Issue。
