import * as path from `path`;
import * as builder from `../../../../Scripts/Builder`;

export async function build()
{
    await builder.buildLib
    ({
        includePaths:
        [
            path.join(__dirname, `..`, `..`, `..`, `MinLib`, `Includes`),
            path.join(__dirname, `..`, `..`, `..`, `WinApi`, `Includes`),
            path.join(__dirname, `..`, `..`, `Includes`),
        ],
        defines: [],
        sourcesPath: path.join(__dirname, `Sources`),
        outputPath: path.join(__dirname, `Binaries`, `SysLib.lib`),
    });
}

await build();
