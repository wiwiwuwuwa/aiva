import * as child_process from 'child_process';
import * as fs from 'fs/promises';
import * as path from 'path';


const PATH_COMPILER = path.join(`C:`, `programs`, `llvm`, `bin`, `clang++.exe`);
const PATH_WINDOWS_SDK_LIBS = path.join(`C:`, `Program Files (x86)`, `Windows Kits`, `10`, `Lib`, `10.0.26100.0`, `um`, `x64`);


async function executeCommand(cwd: string, command: string, args: string[]): Promise<void>
{
    await new Promise<void>((resolve, reject) =>
    {
        const proc = child_process.spawn(command, args,
        {
            cwd: cwd,
            stdio: 'inherit',
            shell: false
        });

        proc.on(`close`, (code: number | null) =>
        {
            if (code === 0)
                resolve();
            else
                reject(new Error(`Process exited with code '${code}'`));
        });

        proc.on(`error`, (error: Error) =>
        {
            reject(error);
        });
    });
}


async function findSourceFiles(dir: string): Promise<string[]>
{
    const entries = await fs.readdir(dir, { withFileTypes: true });
    const results = [] as string[];

    for (const entry of entries)
    {
        const fullPath = path.join(dir, entry.name);

        if (entry.isDirectory())
            results.push(...await findSourceFiles(fullPath));

        if (entry.isFile() && path.extname(entry.name).toLowerCase() === `.cpp`)
            results.push(fullPath);
    }

    return results;
}


async function buildExecutable
({
    cwd,
    gccPath,
    incPaths,
    defEntries,
    srcPaths,
    libPaths,
    libEntries,
    outPath,
}: {
    cwd: string;
    gccPath: string;
    incPaths: string[];
    defEntries: string[];
    srcPaths: string[];
    libPaths: string[];
    libEntries: string[];
    outPath: string;
})
: Promise<void>
{
    await fs.mkdir(path.dirname(outPath), { recursive: true });

    await executeCommand(cwd, gccPath,
    [
        `-std=c++17`,
        `-Wall`,
        `-Wextra`,
        `-Wpedantic`,
        `-Werror`,
        `-O3`,
        `-flto`,
        `--target=x86_64-pc-windows-msvc`,
        `-march=x86-64-v4`,
        ...incPaths.map(incPath => `-I${incPath}`),
        `-nostdinc`,
        `-nostdlib`,
        `-ffreestanding`,
        `-fno-ident`,
        `-fno-rtti`,
        `-fno-exceptions`,
        `-fno-threadsafe-statics`,
        `-fno-unwind-tables`,
        `-fno-use-cxa-atexit`,
        `-fno-stack-protector`,
        `-fno-stack-check`,
        `-mno-stack-arg-probe`,
        ...defEntries.map(defEntry => `-D${defEntry}`),
        ...(await Promise.all(srcPaths.map(srcPath => findSourceFiles(srcPath)))).flat(),
        ...libPaths.map(libPath => `-L${libPath}`),
        ...libEntries.map(libEntry => `-l${libEntry}`),
        `-fuse-ld=lld`,
        `-Wl,/entry:Main`,
        `-Wl,/subsystem:console`,
        `-Wl,/nodefaultlib`,
        `-o`,
        outPath,
    ]);
}

try
{
    await buildExecutable
    ({
        cwd: path.join(__dirname, `..`),
        gccPath: PATH_COMPILER,
        incPaths: [],
        defEntries: [],
        srcPaths: [ path.join(__dirname, `..`, `src`) ],
        libPaths: [ PATH_WINDOWS_SDK_LIBS ],
        libEntries: [ `user32`, `kernel32` ],
        outPath: path.join(__dirname, `..`, `bin`, `aiva.exe`),
    });
}
catch (error: unknown)
{
    console.error(`${error}.`);
}
