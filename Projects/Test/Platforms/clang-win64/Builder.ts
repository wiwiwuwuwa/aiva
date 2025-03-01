import * as path from `path`;
import * as builder from `../../../../Scripts/Builder`;

builder.buildExe
({
    includePaths:
    [
        path.join(__dirname, `..`, `..`, `..`, `MinLib`, `Includes`),
        path.join(__dirname, `..`, `..`, `..`, `SysLib`, `Includes`),
    ],
    defines: [],
    sourcesPath: path.join(__dirname, `Sources`),
    libPaths:
    [
        path.join(__dirname, `..`, `..`, `..`, `SysLib`, `Platforms`, `clang-win64`, `Binaries`),
        path.join(`C:`, `Program Files (x86)`, `Windows Kits`, `10`, `Lib`, `10.0.26100.0`, `um`, `x64`),
    ],
    libraries:
    [
        `SysLib`,
        `kernel32`,
    ],
    outputPath: path.join(__dirname, `Binaries`, `Test.exe`),
});
