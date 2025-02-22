extern "C"
{
    // Kernel32 declarations
    __declspec(dllimport) void *__stdcall GetStdHandle(int);
    __declspec(dllimport) int __stdcall WriteFile(void *, const char *, int, int *, void *);
    __declspec(dllimport) void __stdcall ExitProcess(int);
}

constexpr int STD_OUTPUT_HANDLE = -11;

extern "C" void Main()
{
    const char msg[] = "Hello World!\n";
    void *hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    int written;
    WriteFile(hStdOut, msg, sizeof(msg) - 1, &written, 0);
    ExitProcess(0);
}
