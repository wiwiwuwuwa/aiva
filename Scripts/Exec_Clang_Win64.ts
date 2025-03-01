import * as path from `path`;
import * as builder from `./Builder`
import * as sysLib from `../Projects/SysLib/Platforms/clang-win64/Builder`
import * as test from `../Projects/Test/Platforms/clang-win64/Builder`

await sysLib.build();
await test.build();
await builder.executeCommand(path.join(__dirname, `..`, `Projects`, `Test`, `Platforms`, `clang-win64`, `Binaries`, `Test.exe`));
