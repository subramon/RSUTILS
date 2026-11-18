# SEI CERT C Coding Style & Security Guidelines

This document defines mandatory coding standards and security rules for C development in this repository, aligned with the **SEI CERT C Coding Standard**. All generated or modified C code must strictly adhere to these rules.

## 1. Compiler & Static Analysis Enforcement
- **Language Standard:** Target **C11** or **C17** (`-std=c17`). Do not rely on undefined or implementation-defined compiler extensions.
- **Compiler Flags:** Always compile with strict warnings enabled and treated as errors:
  `-Wall -Wextra -Werror -pedantic -Wconversion -Wshadow -Wformat=2 -Wstrict-prototypes`
- **Hardening Flags (Debug/Test builds):** Compile and run tests with AddressSanitizer and UndefinedBehaviorSanitizer (`-fsanitize=address,undefined -fno-omit-frame-pointer`).
- **Static Analysis (`clang-tidy`):** Run `clang-tidy <file> -checks='cert-*,bugprone-*,clang-analyzer-*' -p build` before completing any task.

---

## 2. Preprocessor (`PRE`)
- **PRE30-C:** Never create universal character names through token concatenation (`##`).
- **PRE31-C:** Never invoke unsafe function-like macros with arguments containing side effects (e.g., `MAX(a++, b++)`). Prefer `static inline` functions over function-like macros whenever type safety permits.
- **PRE32-C:** Do not use preprocessor directives (`#if`, `#ifdef`, `#define`) inside macro arguments.
- **Header Guards:** Every header file must use unique `#ifndef PROJECT_PATH_FILENAME_H` include guards. Never use `#pragma once` if strict standard portability is required.

---

## 3. Declarations & Initialization (`DCL`)
- **DCL30-C:** Never return or store pointers to objects with automatic storage duration (local stack variables) outside their lifetime.
- **DCL31-C:** Declare all identifiers (functions, variables, types) before use. Never rely on implicit `int` or implicit function declarations.
- **DCL36-C:** Do not declare conflicting linkage classifications for the same identifier.
- **DCL37-C:** Never declare or define reserved identifiers (anything starting with `_` followed by an uppercase letter, or `__` anywhere).
- **Initialization:** Explicitly initialize all variables at declaration (e.g., `int count = 0;`, `void *ptr = NULL;`).
- **Const Correctness:** Mark all pointer parameters that are not modified by a function as `const` (e.g., `size_t parse_header(const uint8_t *buf, size_t len)`).

---

## 4. Expressions & Undefined Behavior (`EXP`)
- **EXP30-C:** Do not depend on the order of evaluation for side effects (never write `a[i] = i++;` or `func(i++, i);`).
- **EXP32-C:** Do not access a `volatile` object through a non-volatile lvalue.
- **EXP33-C:** Never read uninitialized memory (including uninitialized struct padding or stack arrays).
- **EXP34-C (Null Pointer Dereference):** Always validate pointers against `NULL` before dereferencing, unless explicitly guaranteed non-null by an internal contract.
- **EXP36-C:** Do not cast pointers into more strictly aligned pointer types.
- **EXP45-C:** Do not perform assignments inside selection (`if`, `switch`) or iteration (`while`, `for`) conditions unless wrapped in extra parentheses and explicitly intended.

---

## 5. Integers & Arithmetic Safety (`INT`)
- **Fixed-Width Types:** Use `<stdint.h>` types (`uint8_t`, `int32_t`, `uint64_t`) for fixed sizes and `size_t` / `ptrdiff_t` for object sizes and pointer arithmetic.
- **INT30-C (Unsigned Overflow):** Ensure unsigned integer operations never wrap around. Check `if (a > UINT32_MAX - b)` before computing `a + b`.
- **INT31-C (Conversions):** Validate value ranges before converting between signed and unsigned types or narrowing integer widths (e.g., `size_t` to `int`).
- **INT32-C (Signed Overflow):** Ensure operations on signed integers never result in overflow (which is undefined behavior in C).
- **INT33-C (Division by Zero):** Always verify that the divisor is non-zero before performing `/` or `%`. For signed division, also check for `INT_MIN / -1`.
- **INT34-C (Bitwise Shifts):** Never shift an expression by a negative number or by a count greater than or equal to the width of the promoted left operand. Never left-shift negative signed integers.

---

## 6. Arrays & Strings (`ARR`, `STR`)
- **ARR30-C:** Never form or use out-of-bounds pointers or array subscripts. Always pass explicit buffer lengths alongside array pointers (`const char *buf, size_t buf_len`).
- **ARR38-C:** Guarantee that library functions (`memcpy`, `memmove`, `memset`) do not form invalid pointers or exceed destination buffer bounds.
- **STR30-C:** Never attempt to modify string literals (`char *s = "hello";` is forbidden; always use `const char *s = "hello";`).
- **STR31-C (Null Termination):** Guarantee that storage for strings has sufficient space for character data **plus** the null terminator (`'\0'`).
- **STR32-C:** Never pass a non-null-terminated character sequence to a library function expecting a string (`strlen`, `strcpy`, `printf("%s")`).
- **Banned Unsafe Functions:** Never use `gets()`, `strcpy()`, `strcat()`, `sprintf()`, `vsprintf()`, `scanf("%s")`, or `strncpy()` (due to non-null-termination hazards). Use `snprintf()` and validate that the return value `ret >= 0 && (size_t)ret < buf_size`.

---

## 7. Memory Management (`MEM`)
- **MEM30-C (Use-After-Free):** Never access memory after it has been freed. Immediately set pointers to `NULL` after calling `free(ptr)`.
- **MEM31-C (Memory Leaks):** Free dynamically allocated memory when no longer needed. Every allocation path must have a single, deterministic cleanup path (use centralized `goto cleanup;` blocks for multi-resource functions).
- **MEM33-C:** Allocate and copy structures containing a flexible array member dynamically using `sizeof(struct s) + count * sizeof(elem)`.
- **MEM34-C:** Only free memory allocated dynamically via `malloc`, `calloc`, `realloc`, or `aligned_alloc`. Never pass stack or static pointers to `free()`.
- **MEM35-C:** Allocate sufficient memory for an object using `sizeof(*ptr)` rather than hardcoding type names (e.g., `ptr = malloc(count * sizeof(*ptr));`).
- **Zero-Size Allocations:** Never call `malloc(0)` or `realloc(ptr, 0)` (implementation-defined behavior). Validate `size > 0` before allocating.

---

## 8. Input/Output & Error Handling (`FIO`, `ERR`)
- **FIO30-C (Format String Vulnerabilities):** Never pass user-controlled or untrusted strings directly as the format string to `printf`, `fprintf`, `snprintf`, or `syslog` (always use `printf("%s", user_str)`).
- **FIO34-C:** Distinguish between characters read from a file and `EOF` by storing the return value of `fgetc()` / `getchar()` in an `int`, never a `char`.
- **FIO37-C:** Do not assume `fgets()` or `fgetws()` returns a non-empty string when successful; check for `NULL` and handle embedded null bytes.
- **FIO42-C:** Close all opened files (`fclose`) and release file descriptors along every execution and error path.
- **ERR30-C:** Set `errno = 0` before calling library functions that report errors via `errno`, and check `errno` immediately after the call before calling any other function.
- **ERR33-C (Checked Return Values):** Detect and handle standard library and system call errors. Never ignore return values from `malloc`, `calloc`, `realloc`, `fopen`, `fclose`, `fread`, `fwrite`, `snprintf`, or `pthread_*`.

---

## 9. Concurrency (`CON`)
- **CON30-C:** Clean up thread-specific storage before thread termination.
- **CON31-C:** Do not destroy a mutex while it is locked or being referenced by another thread.
- **CON32-C:** Prevent data races when accessing bit-fields from multiple threads; protect shared state with `<threads.h>` mutexes or `<stdatomic.h>` atomics.
- **CON33-C:** Avoid race conditions when using library functions; never use non-reentrant functions (`strtok`, `gmtime`, `localtime`, `strerror`) in multi-threaded code—use `strtok_r`, `gmtime_r`, `localtime_r`, or `strerror_r`.

---

## 10. Formatting & Naming Conventions
- **Indentation:** 2 spaces per indent level; no tabs. Maximum line length: 80 characters.
- **Braces:** Always use braces `{}` for `if`, `else`, `for`, `while`, and `do` statements, even for single-line bodies, to prevent dangling-else and macro-expansion bugs.
- **Naming:**
  - Functions & variables: `snake_case` (e.g., `packet_validate_checksum`).
  - Types & structs: `snake_case_t` (e.g., `ring_buffer_t`).
  - Macros & enum constants: `UPPER_SNAKE_CASE` (e.g., `MAX_PAYLOAD_BYTES`).

