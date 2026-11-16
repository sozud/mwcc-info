# mwcc-info

Progress figures for the native Metrowerks compilers in
[sozud/mweppc](https://github.com/sozud/mweppc/tree/mwccpsp), as
[shields.io endpoint](https://shields.io/badges/endpoint-badge) JSON. The compiler repository's README shows them as
badges, so updating a figure is a commit here only.

| File | Compiler |
| ---- | -------- |
| `mwccpsp.json` | mwccpsp, PSP 3.0.1 build 219 (Castlevania: Symphony of the Night) |
| `mwccps2.json` | mwccps2, PS2 3.0.1 build 210 (Persona 4) |
| `mwccps2-b52.json` | mwccps2, PS2 3.0 build 52 (Monster Hunter) |
| `mwccps2-b69.json` | mwccps2, PS2 build 69 (Silent Hill 3) |

"Exact" is the share of the original executable's function bytes (its function table) that our source reproduces byte
for byte, from the matching harness's hybrid build (`match.csv`, status `match`). Functions that behave identically
without matching byte for byte (the natives' "equivalents") are not counted.

`tools/exact.py --out .` regenerates the files from the decompilation trees' latest hybrid builds.

A badge:

    ![b69](https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/sozud/mwcc-info/main/mwccps2-b69.json)
