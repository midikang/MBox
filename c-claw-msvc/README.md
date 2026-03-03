# c-claw-msvc (MSVC native)

This folder is a Windows/MSVC-native clone of ../c-claw.
Original c-claw is untouched.

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
