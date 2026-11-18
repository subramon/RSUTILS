# Contributing to This Project

Thank you for contributing! Whether you are fixing a bug, improving documentation, or adding a new feature, please follow the workflow and engineering standards outlined below.

## 1. Repository Layout
Keep files organized according to the project directory structure:
- `include/` — Public header files (`*.h`). Only expose stable, documented APIs here.
- `src/` — Implementation files (`*.c`) and internal-only headers (`*_internal.h`).
- `tests/` — Unit and integration tests (`test_*.c`).
- `docs/` — Architecture notes and coding standards (including `docs/c-style-guide.md`).

## 2. Development Setup & Build Commands
This project targets **C17** and uses **CMake** with strict compiler warnings and runtime sanitizers enabled for development builds.

### Configure & Build (Debug + Sanitizers)
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_SANITIZERS=ON
cmake --build build -j
```

### Run Test Suite
All pull requests must pass the full test suite with zero memory leaks or sanitizer errors:
```bash
ctest --test-dir build --output-on-failure
```

## 3. Coding & Security Standards
All C code in this repository must comply with our **SEI CERT C** guidelines in [`docs/c-style-guide.md`](docs/c-style-guide.md).

Before submitting changes, run formatting and static analysis checks:
```bash
# Format modified C and header files
clang-format -i src/*.c include/*.h tests/*.c

# Run static analysis (requires compile_commands.json in build/)
clang-tidy src/*.c -checks='cert-*,bugprone-*,clang-analyzer-*' -p build
```

### Key Non-Negotiable Rules
- **Zero Warnings:** Code must compile cleanly under `-Wall -Wextra -Werror -pedantic -Wconversion`.
- **No Unsafe String/Memory Calls:** Never use `gets()`, `strcpy()`, `strcat()`, `sprintf()`, or unchecked `strncpy()`.
- **Explicit Ownership:** Every `malloc`/`calloc` must have a clear cleanup path (`goto cleanup;`) and set pointers to `NULL` after `free()`.
- **Checked Returns:** Never ignore return values from memory allocations, file I/O, or system calls.

## 4. Writing Tests
- Every bug fix must include a regression test that fails without the fix and passes with it.
- Every new public function in `include/` must have unit tests covering:
  - Normal/valid inputs
  - Boundary conditions (`0`, `1`, `SIZE_MAX`, empty buffers)
  - Invalid inputs (`NULL` pointers, out-of-range integers, malformed data)

## 5. Git Workflow & Pull Requests
1. **Branching:** Create a descriptive branch from `main` (e.g., `fix/buffer-overflow-parser` or `feat/ring-buffer-peek`).
2. **Commits:** Keep commits atomic and focused on a single logical change. Write clear commit messages in the imperative mood:
   - `fix(parser): validate payload length before memcpy (ARR38-C)`
   - `feat(ringbuf): add non-destructive peek helper`
3. **Pull Request Checklist:** Before opening a PR, verify that:
   - [ ] `cmake --build build` succeeds with zero compiler warnings.
   - [ ] `ctest --test-dir build` passes with ASan and UBSan enabled.
   - [ ] `clang-format` and `clang-tidy` report zero issues on modified files.
   - [ ] Public header functions include Doxygen-style doc comments (`@brief`, `@param`, `@return`).

