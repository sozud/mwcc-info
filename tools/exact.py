"""exact.py: % of each original compiler's function bytes our source reproduces exactly, one metric for all.

Totals: the exe's function table (functions.csv: start, size). Exact: hybrid build match.csv rows with
status `match`; a match counts for the table entry starting at its address (as progress_b69.py).
Writes <out>/<compiler>.json in shields.io endpoint format if --out is given.
"""
import csv, json, os, sys
H = os.path.expanduser("~/Downloads")
COMPILERS = [
    ("mwccpsp", "mwccpsp b219", f"{H}/sotn-mwccpsp/tools/mwccpsp/re/functions.csv", f"{H}/sotn-mwccpsp/build/mwccpsp/match.csv"),
    ("mwccps2", "mwccps2 b210", f"{H}/sotn-mwccps2/tools/mwccps2/re/functions.csv", f"{H}/sotn-mwccps2/build/mwccps2/match.csv"),
    ("mwccps2-b52", "mwccps2 b52", f"{H}/sotn-mwccps2-b52/tools/mwccps2/re/b52/functions.csv", f"{H}/b52-work/hyb/match.csv"),
    ("mwccps2-b69", "mwccps2 b69", f"{H}/sotn-mwccps2-b52/tools/mwccps2/re/b69/functions.csv", f"{H}/sotn-mwccps2-b52/build/mwccps2-b69/match.csv"),
]
out = sys.argv[sys.argv.index("--out") + 1] if "--out" in sys.argv else None
for key, label, fcsv, mcsv in COMPILERS:
    fns = {int(r["start"], 16): int(r["size"]) for r in csv.DictReader(open(fcsv))}
    exact = {int(r["address"], 16) for r in csv.DictReader(open(mcsv)) if r["status"] == "match"}
    hit = [a for a in fns if a in exact]
    total_b, hit_b = sum(fns.values()), sum(fns[a] for a in hit)
    pct = 100.0 * hit_b / total_b
    print(f"{label:14s} {pct:5.1f}% exact  ({hit_b // 1024}/{total_b // 1024} KB, {len(hit)}/{len(fns)} functions)"
          f"  match list {os.path.getmtime(mcsv) and __import__('time').strftime('%Y-%m-%d', __import__('time').localtime(os.path.getmtime(mcsv)))}")
    if out:
        os.makedirs(out, exist_ok=True)
        color = "brightgreen" if pct >= 90 else "green" if pct >= 75 else "yellowgreen" if pct >= 50 else "yellow" if pct >= 25 else "orange"
        json.dump({"schemaVersion": 1, "label": label, "message": f"{pct:.1f}% exact", "color": color},
                  open(os.path.join(out, key + ".json"), "w"))
