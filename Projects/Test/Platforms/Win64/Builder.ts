import * as fs from `fs/promises`;
import * as path from `path`;
import * as child_process from `child_process`;

const COMPILER_PATH = `clang++`;
const SOURCES_PATH = path.join(`Sources`);
const OUTPUT_PATH = path.join(`Binaries`, `Test.exe`);

const WINDOWS_SDK_VERSION = `10.0.26100.0`;
const WINDOWS_SDK_BASE_PATH = path.join(`C:`, `Program Files (x86)`, `Windows Kits`, `10`);
const WINDOWS_SDK_INC_PATH = path.join(WINDOWS_SDK_BASE_PATH, `Include`, WINDOWS_SDK_VERSION, `um`);
const WINDOWS_SDK_LIB_PATH = path.join(WINDOWS_SDK_BASE_PATH, `Lib`, WINDOWS_SDK_VERSION, `um`, `x64`);

const COMPILER_FLAGS = [
    `-std=c++17`,
    `-O3`,
    `-flto`,
    `-march=native`,
    `-I${WINDOWS_SDK_INC_PATH}`,
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
    `-D_WIN32_WINNT=0x0A00`,
    `-DWIN32_LEAN_AND_MEAN`,
    `-DSTRICT`,
    `-DUNICODE`,
    `-DNDEBUG`
];

const LINKER_FLAGS = [
    `-L${WINDOWS_SDK_LIB_PATH}`,
    `-lkernel32`,
    `-fuse-ld=lld`,
    `-Wl,/entry:Main`,
    `-Wl,/subsystem:console`,
    `-Wl,/nodefaultlib`,
    `-Wl,/merge:.idata=.rdata`,
    `-Wl,/merge:.edata=.rdata`,
    `-Wl,/merge:.xdata=.rdata`,
    `-Wl,/merge:.pdata=.rdata`,
    `-Wl,/merge:.rdata=.text`,
    `-Wl,/merge:.data=.text`
];

const OUTPUT_FLAGS = [`-o`, OUTPUT_PATH];

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

async function executeCommand(command: string, args: string[]): Promise<void>
{
    await new Promise((resolve, reject) =>
    {
        const proc = child_process.spawn(command, args,
            {
                stdio: `inherit`,
                shell: false
            });

        proc.on(`error`, reject);
        proc.on(`close`, (code: number) =>
        {
            if (code === 0)
                resolve();
            else
                reject(new Error(`Exit code (${code}).`));
        });
    });
}

async function build()
{
    await fs.mkdir(path.dirname(OUTPUT_PATH), { recursive: true });

    const sourceFiles = await findSourceFiles(SOURCES_PATH);
    if (sourceFiles.length <= 0)
        throw new Error(`No C++ files found in (${SOURCES_PATH}).`);

    const args = [
        ...COMPILER_FLAGS,
        ...sourceFiles,
        ...LINKER_FLAGS,
        ...OUTPUT_FLAGS  // Changed here
    ];

    await executeCommand(COMPILER_PATH, args);
}

build();
