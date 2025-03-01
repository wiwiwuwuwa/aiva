import * as fs from `fs/promises`;
import * as path from `path`;

async function findSourceFiles(dir: string): Promise<string[]>
{
    const entries = await fs.readdir(dir, { withFileTypes: true });
    const results: string[] = [];
    const allowedExtensions = [`.h`, `.hpp`, `.cpp`, `.c`, `.inl`, `.ts`];

    for (const entry of entries)
    {
        const fullPath = path.join(dir, entry.name);

        if (entry.isDirectory())
            results.push(...await findSourceFiles(fullPath));

        if (entry.isFile() && allowedExtensions.includes(path.extname(entry.name).toLowerCase()))
            results.push(fullPath);
    }

    return results;
}

async function main(): Promise<void>
{
    let sourcesPaths = [ path.join(__dirname, `..`, `Resources`, `llm.txt`) ];

    if (process.argv[2])
        sourcesPaths.push
        (
            ...await findSourceFiles(path.join(__dirname, `..`, process.argv[2])),
        );
    else
        sourcesPaths.push
        (
            ...await findSourceFiles(path.join(__dirname, `..`, `Projects`)),
            ...await findSourceFiles(path.join(__dirname, `..`, `Scripts`)),
        );

    const outputPath = path.join(__dirname, `..`, `Resources`, `cal.txt`);
    await fs.writeFile(outputPath, ``);

    for (const sourcePath of sourcesPaths)
    {
        const sourceText = await fs.readFile(sourcePath, `utf-8`);
        const outputText = `// file path: ${sourcePath}\n// file begin\n${sourceText}// file end\n\n`;
        await fs.appendFile(outputPath, outputText, `utf-8`);
    }
}

await main();
