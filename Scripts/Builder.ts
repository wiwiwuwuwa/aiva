import * as fs from `fs/promises`;
import * as path from `path`;
import * as child_process from `child_process`;

export async function executeCommand(command: string, args: string[] = []): Promise<void>
{
    await new Promise((resolve, reject) =>
    {
        const proc = child_process.spawn(command, args,
        {
            stdio: `inherit`,
            shell: false,
        });

        proc.on(`error`, reject);
        proc.on(`close`, (code: number) =>
        {
            if (code === 0)
                resolve();
            else
                reject(new Error(`Exit code ${code}.`));
        });
    });
}

export async function findSourceFiles(dir: string): Promise<string[]>
{
    const entries = await fs.readdir(dir, { withFileTypes: true });
    const results: string[] = [];

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

export async function buildExe
({
    includePaths,
    defines,
    sourcesPath,
    libPaths,
    libraries,
    outputPath,
}: {
    includePaths: string[];
    defines: string[];
    sourcesPath: string;
    libPaths: string[];
    libraries: string[];
    outputPath: string;
}
): Promise<void>
{
    await fs.mkdir(path.dirname(outputPath), { recursive: true });

    await executeCommand(`clang++`,
    [
        `-std=c++17`,
        `-O3`,
        `-flto`,
        `-march=native`,
        ...includePaths.map(includePath => `-I${includePath}`),
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
        ...defines.map(define => `-D${define}`),
        ...await findSourceFiles(sourcesPath),
        ...libPaths.map(libPath => `-L${libPath}`),
        ...libraries.map(library => `-l${library}`),
        `-fuse-ld=lld`,
        `-Wl,/entry:Main`,
        `-Wl,/subsystem:console`,
        `-Wl,/nodefaultlib`,
        `-o`,
        outputPath,
    ]);
}

export async function buildLib
({
    includePaths,
    defines,
    sourcesPath,
    outputPath,
}: {
    includePaths: string[];
    defines: string[];
    sourcesPath: string;
    outputPath: string;
}
): Promise<void>
{
    await fs.mkdir(path.dirname(outputPath), { recursive: true });

    const sourceFiles = await findSourceFiles(sourcesPath);
    const objectFiles = sourceFiles.map(sourceFile => path.join(path.dirname(sourceFile), `${path.basename(sourceFile, `.cpp`)}.o`));

    await Promise.all
    (
        sourceFiles
        .map((sourceFile, index) => ({ src: sourceFile, obj: objectFiles[index] }))
        .map(async entry =>
        {
            await executeCommand(`clang++`,
            [
                `-std=c++17`,
                `-O3`,
                `-flto`,
                `-march=native`,
                ...includePaths.map(includePath => `-I${includePath}`),
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
                ...defines.map(define => `-D${define}`),
                `-c`,
                entry.src,
                `-o`,
                entry.obj,
            ]);
        })
    );

    await executeCommand(`llvm-ar`,
    [
        `crs`,
        outputPath,
        ...objectFiles,
    ]);

    await Promise.all
    (
        objectFiles.map(objectFile => fs.unlink(objectFile))
    );
}
