# AGENTS.md

## What this repo is
`RSUTILS` is a C utility library plus a LuaJIT binding, built around a "qtypes" typed-scalar
system (`inc/qtypes.h`: B1/BL, I1..I8, F4/F8, UI*, SC/SV, TM/TM1). No `main`, no CMake, no single
top-level build. The two deliverables are `src/librsutils.so` (C library) and
`binding/src/libcutils.so` (Lua C module). `README.md`/`README.txt` are one-line stubs — the
Makefiles and shell runners are the real documentation.

## Layout — only what changes how you work
- `inc/` one header per utility; `src/` implementations + `Makefile` (builds the `.so`, CLI, generators).
- `binding/src/` `cutils.c` → `libcutils.so`; `binding/test/` Lua tests using `require 'libcutils'`.
- `lua/` pure-Lua modules loaded via `LUA_PATH`; codegen helpers and generated type defs live here.
- `test/C/` shell drivers that compile one test `.c` against selected `src/*.c` files and run valgrind;
  `test/Lua/` LuaJIT scripts.
- `src/DEPRECATED/`, `src/mysql_examples/`, `src/pthreads_hello_world/`, `BAJJ/`, `fork_example/`,
  `TCC/` are standalone experiments outside the main build. `nvim/`, `vim/`, `vimrc` are editor config.
- `.o`, `.so`, `a.out`, `_*`, `*.*~` are gitignored build/test artifacts already present in-tree; don't edit.

## Environment: source `to_source` first
`source to_source` from the repo root (the test runners do this too). It sets `RSUTILS_SRC_ROOT`,
`LUA_PATH`, `LUA_CPATH`, and `QCFLAGS` (gnu99, -fPIC, OpenMP, many warnings).
Gotchas: it also sets the typo'd alias `RSLUTILS_SRC_ROOT` (extra L) which `binding/src/Makefile`
and `BAJJ/Makefile` actually use — setting only `RSUTILS_SRC_ROOT` breaks those builds. The binding
build additionally pulls `${RSLUTILS_SRC_ROOT}//src/get_tbl_of_key_vals.c` from the **sibling
`~/RSLUTILS` checkout**, so it fails if that repo is absent. Requires gcc, luajit/lua, libcurl dev,
OpenMP, and valgrind for C tests.

## Build
- `cd src && make` → `librsutils.so`, `cli_asc_to_bin`, `ut_task_cpu_time`, and regenerates
  `lua/{csv_meta_t,csv_spec_t,qtypes_def}.lua`. `make gen_inc_files` regenerates headers.
- `cd binding/src && make` → `libcutils.so` (needs the sourced env, curl, and sibling RSLUTILS).
- In `src/Makefile`, `hardclean` deletes generated sources; its own comment says don't use it unless
  absolutely necessary (it breaks builds from other trees). Use `make clean`.

## Generated code — don't hand-edit
Many files are LuaJIT-generated: `src/{txt_to_I*,txt_to_F*,txt_to_UI*,I*_to_txt,F*_to_txt}.c`, many
`inc/*.h`, and `lua/{qtypes_def,csv_meta_t,csv_spec_t}.lua`. Generators/templates:
`src/gen_code_I.lua`, `src/gen_code_F.lua`, `src/print_generator.lua`,
`src/cli_extract_func_decl.lua`, `lua/cli_for_cdef.lua`, `src/*.tmpl`. Edit the generator/template,
then regenerate; direct edits get overwritten.

## Tests
- Root `./run_all.sh` is the broadest runner but still a subset: `binding/test/test_cutils.lua`,
  `test/Lua/test_tbl_to_C_2d.lua`, then `test/C/run_all.sh`. Most `test_*.lua` files are not wired in.
- Single C test: `cd test/C && bash test_<name>.sh` (env must be sourced). Each compiles one test against
  a few `src/*.c` files, runs valgrind, greps for `0 errors from 0 contexts` and `SUCCESS`, then `rm -f _*`.
- Single Lua test: `cd binding/test && luajit test_cutils.lua` or `cd test/Lua && luajit <file>.lua`.
- `test/C/run_all.sh` and `test/Lua/run_all.sh` each run only a short hardcoded list, not every test.

## Auto-loaded instructions: trust the executable source
`opencode.json` `instructions` auto-loads `CONTRIBUTING.md` and `docs/c-style-guide.md`. They prescribe
CMake + ctest + ASan/UBSan + `clang-tidy -p build` and an `src/`/`include/`/`tests/`/`build/` layout.
None of that exists here (no `CMakeLists.txt`, `build/`, `compile_commands.json`; headers are in `inc/`,
tests in `test/`). Apply the SEI CERT C coding rules as guidance, but use the Makefiles above and don't
try to run ctest/clang-tidy. There is no `.clang-format` or formatter config — match surrounding style.

## Cross-repo coupling
`inc/qtypes.h` is duplicated in `Q/UTILS/inc/` (see its header comment); changes must be copied there.

## `basalt` provider (opencode.json)
- `@ai-sdk/openai-compatible`, `baseURL` `http://192.168.1.115:8080/v1` — plain-HTTP LAN, no auth;
  if model calls fail, suspect host reachability, not repo code.
- Sole model key is literally `Qwen3.8-Flash-Next UD-IQ2_XS` (embedded space, exact case), display name
  `Basalt Coder`; provider name `Basalt Remote Node`. Add models as new keys under `provider.basalt.models`.
