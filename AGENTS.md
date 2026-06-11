# AGENTS.md

## Build & Run

```
cd scr && bun run build    # compile → bin/aiva.exe
cd scr && bun run start    # run (has intentional infinite loop)
```

Compiler: `C:\programs\llvm\bin\clang++.exe` — Clang++ targeting `x86_64-pc-windows-msvc`.

## Project Structure

Single-header freestanding C++17 library. No stdlib. One file: `src/aiva.hpp` + `src/main.cpp`.

All code lives in `aiva.hpp` as separate logical sections (each with `.hpp` declaration and `.inl` definition in the same file). Order matters — later sections depend on earlier ones.

## Style (strict — match exactly)

- East-const: `uintptr_t const` not `const uintptr_t`
- Allman braces, 4-space indent
- camelCase params, SCREAMING_SNAKE_CASE constants
- Trailing return types on template functions: `auto Foo() noexcept -> RetType`
- `noexcept` on ALL user functions (`-fno-exceptions` makes it meaningful for documentation)
- `constexpr` on all CstrView/Span methods
- `inline` on variable templates and intrin functions
- `inline static` on class static data members
- `[[noreturn]]` on exit functions
- `final` on leaf classes, `NonCopyable` base for non-copyable types
- No comments unless asked

## Key Gotchas

- `void main()` is intentional — linker uses `-Wl,/entry:main`
- `while (true) {}` in main is intentional — freestanding, returning is UB
- `CreateNumber` uses 3 `#define` macros — last macro block in file, don't add more
- `CstrView` stores `char const&` to first char, default ctor binds to `*""` (static storage)
- `Span(TType&)` = size 1, `Span(TType*)` = size `data ? 1 : 0` — user's design choice
- `SpinLock::Unlock()` uses CAS loop (not simple exchange) — intentional symmetry with `Lock()`
- WinAPI functions are `extern "C"` with `__attribute__((dllimport, stdcall))` — they do have `noexcept` (C functions that return error codes, never throw)
- `OVERLAPPED` is forward-declared only (pointer needed, not definition)
- `__atomic_*` builtins and `__builtin_ia32_pause()` are compiler intrinsics, not affected by `-fno-builtin`
