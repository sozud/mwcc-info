# mwcc-info: compilers branch

Host compilers the exact-match measurement in sozud/mweppc builds the decompiled sources with (each original was built
with one of these), so its CI can fetch them in one download:

    https://github.com/sozud/mwcc-info/archive/refs/heads/compilers.tar.gz

| Directory | What | Used for |
| --------- | ---- | -------- |
| `cw92/` | CodeWarrior for Windows 9.2 command-line tools (archive.org `cwpro9`, `CW_Windows_9.2.exe`, Tools/Other_Metrowerks_Tools/Command_Line_Tools) | mwccpsp b219, mwccps2 b210 |
| `cwpro6/`, `cwpro53/`, `cwpro5/` | CodeWarrior Pro 6, 5.3 and 5 `mwcc.exe` (as mweppc's `tools/download_tool.py` extracts them from archive.org) | mwccps2 b69, b52 |
| `lib/` | MSL and runtime sources and headers from the CodeWarrior discs (mweppc's `download_tool.py lib`) | b69's and b52's MSL/runtime units |

`SHA256SUMS` lists every file.
