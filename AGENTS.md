# AGENTS.md - Coding Agent Guidelines for AIVA

## Project Overview

AIVA is a freestanding C++17 Windows application framework with:
- Custom memory management (heap allocator)
- Fiber-based coroutine system
- Windows GUI management
- DirectX Graphics Infrastructure (DXGI) bindings
- No standard library dependency (uses `-nostdinc`, `-nostdlib`, `-ffreestanding`)

## Directory Structure

```
src/          # Current source code (main.cpp + aiva.hpp monolithic header)
old/          # Legacy modular source files (.hpp/.inl/.cpp pattern)
scr/          # Build scripts (Bun/TypeScript builder)
bin/          # Build output (aiva.exe)
.vscode/      # VS Code settings
```

## Build Commands

### Prerequisites
- LLVM/Clang++ installed at `C:\programs\llvm\bin\clang++.exe`
- Windows SDK 10.0.26100.0 libs at `C:\Program Files (x86)\Windows Kits\10\Lib\10.0.26100.0\um\x64`
- Bun runtime installed

### Build the Project
```bash
cd scr
bun install           # Install dependencies (first time only)
bun run build         # Compile the project
```

### Run the Executable
```bash
cd scr
bun run start         # Or directly: ../bin/aiva.exe
```

## Compiler Configuration

The project uses Clang++ with these flags:
- **Standard:** `-std=c++17`
- **Warnings:** `-Wall -Wextra -Wpedantic -Werror` (warnings are errors)
- **Optimization:** `-O3 -flto`
- **Target:** `--target=x86_64-pc-windows-msvc -march=x86-64-v4`
- **Freestanding:** `-nostdinc -nostdlib -ffreestanding`
- **Disabled Features:** `-fno-rtti -fno-exceptions -fno-threadsafe-statics -fno-stack-protector`
- **Linker:** `-fuse-ld=lld -Wl,/entry:Main -Wl,/subsystem:console -Wl,/nodefaultlib`
- **Libraries:** `dxgi`, `dxguid`, `uuid`, `user32`, `kernel32`

## Testing

No test framework is currently configured. The project has no automated tests.

## Code Style Guidelines

### File Organization
- **Current approach:** Monolithic single-header design in `src/aiva.hpp`
- **Legacy approach:** Separate `.hpp` (declarations), `.inl` (template implementations), `.cpp` (compiled code)
- Section comments use: `// ------------------------------------` followed by the logical file name

### Formatting
- **Line endings:** LF (`\n`) - configured in `.vscode/settings.json`
- **Trailing whitespace:** Trimmed automatically
- **Final newline:** Always insert one
- **Indentation:** 4 spaces (inferred from code)
- **Braces:** Allman style (opening brace on new line for functions/classes)
- **Single-line blocks:** Use braces with content on same line: `{ return x; }`

### Naming Conventions

| Element | Convention | Example |
|---------|------------|---------|
| Namespaces | PascalCase | `Aiva::Templates`, `Aiva::WinApi` |
| Classes/Structs | PascalCase | `SpinLock`, `LinkedList`, `ManageObject` |
| Interface classes | Prefix with `I` or `A` (abstract) | `IAllocator`, `ACoroutine` |
| Type aliases | PascalCase with `_t` suffix | `RemoveReference_t`, `Number_t`, `SpinLockScope_t` |
| Member types | PascalCase with `_t` suffix | `Type_t` |
| Template parameters | Prefix with `T` | `TType`, `TArgs`, `TAction` |
| Functions/Methods | PascalCase | `GetNumberOfCores()`, `InitSystem()` |
| Member variables | Prefix with `m_` | `m_data`, `m_size`, `m_locked` |
| Global/Static variables | Prefix with `G` | `GLock`, `GInitialized`, `GHeapAlloc` |
| Constants | Prefix with `k` | `kAnyWorkerMask`, `kWindowClassName` |
| Enums | PascalCase, members also PascalCase | `NumberType::Int`, `NumberSign::Sign` |
| Macros | SCREAMING_SNAKE_CASE | `CheckNoEntry()` |

### Type Definitions
- Use custom primitive types defined in `Aiva` namespace:
  - `sint8_t`, `sint16_t`, `sint32_t`, `sint64_t` (signed integers)
  - `uint8_t`, `uint16_t`, `uint32_t`, `uint64_t` (unsigned integers)
  - `float32_t`, `float64_t` (floating point)
  - `sintptr_t`, `uintptr_t` (pointer-sized integers)
  - `size_t`, `byte_t`, `nullptr_t`
- Use `auto` for local variables when type is obvious
- Use `auto const` for immutable values
- Use explicit types for member variables

### Const Correctness
- Prefer `const` parameters: `TType const& value`, `size_t const size`
- Use `const` after pointer for pointer constness: `TType *const ptr`
- Const methods where applicable

### Error Handling
- **No exceptions** - exceptions are disabled (`-fno-exceptions`)
- Use `CheckNoEntry()` macro for unreachable code paths and fatal errors
- Pattern: Check condition, call `CheckNoEntry()` if failed
```cpp
if (!handle)
    CheckNoEntry();
```
- For recoverable states, use early returns or default values
- System exit functions: `System::ExitSuccess()`, `System::ExitFailure()`

### Memory Management
- **No global new/delete** - custom allocators only
- Use `Memory::GetHeapAlloc()` for heap allocations
- Allocator methods:
  - `Alloc(size)` / `Free(span)` - raw byte allocation
  - `AllocArray<T>(size)` / `FreeArray(span)` - typed array allocation
  - `Create<T>(args...)` / `Delete(ref)` - construct/destruct single object
  - `CreateArray<T>(size, args...)` / `DeleteArray(span)` - construct/destruct array
- Use `ManageObject<T>` for static storage with manual construction/destruction
- Use `Span<T>` for memory views (size + pointer pair)

### Class Design Patterns
- Inherit from `NonCopyable` to delete copy/move operations
- Static-only classes: Delete default constructor
- System singletons pattern:
```cpp
class SystemName final {
public:
    static void InitSystem();
    static void ShutSystem();
private:
    SystemName() = delete;
    static SpinLock GLock;
    static bool GInitialized;
};
```

### Template Metaprogramming
- Use `Void_t<>` for SFINAE
- Use `TypeIdentity<T>` for type identity
- Use `Templates::Forward<T>()` and `Templates::Move()` (custom implementations)
- Suffix type traits with `_t`: `RemoveReference_t<T>`

### Windows API Integration
- WinAPI types/functions declared in `Aiva::WinApi` namespace with `extern "C"`
- Use `__attribute__((dllimport, stdcall))` for WinAPI function declarations
- COM interfaces in `Aiva::WinCom` namespace
- DXGI interfaces in `Aiva::Dxgi` namespace

### Concurrency
- Use `SpinLock` for synchronization
- Use RAII lock guards: `SpinLockScope_t const lockScope{ GLock };`
- Use `Intrin::AtomicCompareExchange()` and `Intrin::AtomicExchange()` for atomics
- Use `Intrin::YieldProcessor()` for spin-wait loops
- Use fiber-based `Coroutines::Spawn()` for async operations

### Comments
- Use `//` for single-line comments
- Empty function bodies: Use `//` placeholder
```cpp
CoroutineContol(ACoroutine& coroutine) : m_coroutine{ coroutine }
{
    //
}
```
- Section separators for logical file boundaries in monolithic header

## TypeScript/Build Script Style (scr/)

- **Runtime:** Bun
- **Module system:** ESNext modules
- **Strict mode:** Enabled
- **Imports:** Use `* as` pattern: `import * as fs from 'fs/promises';`
- **Template literals:** Use backticks with path.join for paths
- **Error handling:** Try/catch with `unknown` type, log with template string

## Important Constraints

1. **No standard library** - Do not use `<iostream>`, `<vector>`, `<string>`, etc.
2. **No exceptions** - Use error codes or `CheckNoEntry()` for failures
3. **No RTTI** - Do not use `dynamic_cast` or `typeid`
4. **No global new/delete** - Use custom allocators
5. **Entry point** is `void Main()` - not `main()`
6. **Windows-only** - All platform code assumes Windows x64
