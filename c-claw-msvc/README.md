# c-claw-msvc (MSVC native)

This folder is a Windows/MSVC-native clone of ../c-claw.
Original c-claw is untouched.

## Commands

| Command | Description |
|---------|-------------|
| `onboard` | Initialize AI provider configuration |
| `chat <prompt>` | Chat with history context |
| `ask <prompt>` | Single question (no history) |
| `gateway [port]` | Start HTTP gateway (default: 8080) |
| `cron add <interval> <prompt>` | Add a scheduled task |
| `daemon` | Run background service |
| `organize [--dry-run] <folder>` | Organize folder files by type |

### organize — Folder Organization / 整理文件夹

Scans the specified folder and moves files into category subdirectories:

| Subdirectory | Extensions |
|---|---|
| `Documents` | doc, docx, xls, xlsx, ppt, pptx, pdf, txt, md, csv, rtf, xml, json, yaml, … |
| `Images` | jpg, jpeg, png, gif, bmp, ico, tiff, svg, webp, psd, ai, … |
| `Videos` | mp4, avi, mkv, mov, wmv, flv, m4v, webm, rmvb, … |
| `Audio` | mp3, wav, flac, aac, ogg, wma, m4a, opus, … |
| `Archives` | zip, rar, 7z, tar, gz, bz2, xz, cab, iso, … |
| `Code` | c, cpp, h, py, js, ts, java, go, rs, html, css, sh, bat, … |
| `Executables` | exe, msi, dll, sys, jar, apk, … |
| `Fonts` | ttf, otf, woff, woff2, … |
| `Others` | everything else |

```powershell
# Preview only — no files are moved
cclaw organize --dry-run C:\Users\me\Downloads

# Actually organize the folder
cclaw organize C:\Users\me\Downloads
```

Only top-level files in the given folder are moved; existing subdirectories are left untouched.

## Build with .sln (recommended)

1. Open `D:\Lab\2025\mbox\c-claw-msvc\cclaw-msvc.sln` in Visual Studio 2022.
2. Select `Debug|x64` or `Release|x64`.
3. Build the `cclaw-msvc` project.

Output binary path:

- `D:\Lab\2025\mbox\c-claw-msvc\build\Debug\cclaw.exe`
- `D:\Lab\2025\mbox\c-claw-msvc\build\Release\cclaw.exe`

## Dependency note (libcurl)

This project enables vcpkg manifest mode via `vcpkg.json`.

If Visual Studio reports missing `libcurl.lib` or `curl/curl.h`:

1. Install Visual Studio C++ workload.
2. Install vcpkg and run:

```powershell
vcpkg integrate install
```

3. Reopen the solution and build again.

If PowerShell reports `vcpkg` is not recognized:

```powershell
$env:Path += ";D:\tools\vcpkg"
vcpkg version
```

Or use the full path directly:

```powershell
& "D:\tools\vcpkg\vcpkg.exe" integrate install
```

## Optional: CMake build path

`CMakeLists.txt` is also included, but the `.sln/.vcxproj` route is the primary path for this folder.
