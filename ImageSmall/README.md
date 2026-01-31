# ImageSmall - 图片压缩工具

一个简单易用的图片批量压缩工具，使用 C# 和 Windows Forms 开发。

## 功能特点

- 📁 支持批量选择和压缩多个图片文件
- 📂 支持选择文件夹批量导入图片
- 🎚️ 可调节压缩质量（10% - 100%）
- 📊 实时显示压缩进度
- 💾 自定义输出目录
- 🖼️ 支持多种图片格式：JPG, PNG, BMP, GIF

## 使用方法

1. 点击"选择图片"按钮，选择要压缩的图片文件（支持多选）
2. 或点击"选择文件夹"按钮，一次导入文件夹内的图片
3. 选择输出目录（可选，默认保存到桌面的"压缩后的图片"文件夹）
4. 调整压缩质量滑块（推荐75%）
5. 点击"开始压缩"按钮开始压缩
6. 压缩完成后会显示成功和失败的数量

## 系统要求

- Windows 操作系统
- .NET 6.0 Runtime 或更高版本

## 编译说明

```bash
cd ImageSmall
dotnet restore
dotnet build -c Release
```

编译后的程序位于：`ImageSmall/bin/Release/net6.0-windows/`

## 运行

在 Windows 系统上直接运行 `ImageSmall.exe`

## 许可证

MIT License
