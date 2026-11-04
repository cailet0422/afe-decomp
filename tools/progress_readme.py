#!/usr/bin/env python3
"""Draw the README's progress chart and fill in its progress table.

Reads the objdiff report of a build (build/<version>/report.json, made by
`ninja build/<version>/report.json`) and writes:

- assets/progress-light.svg and assets/progress-dark.svg, the chart at the top
  of the README (GitHub picks one by the reader's theme);
- the README's text between the progress markers: the badge links and the
  progress table, with the functions that are not matched yet.

A file is rewritten only when more than its commit and date would change, so
the stamp says when the numbers last changed. GitHub Actions runs this on every
push to main and commits what changed (.github/workflows/build.yml).

Usage:
    ninja build/GAEJ01_01/report.json
    python tools/progress_readme.py [--version GAEJ01_01]
"""

import argparse
import json
import re
import subprocess
from html import escape
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent

VERSION_NAMES = {
    "GAEJ01_00": "Rev 0 (JP)",
    "GAEJ01_01": "Rev 1 (JP)",
}

# The report's categories, in two groups: by binary and by where the code comes from.
GROUPS = [
    ("By binary", [("dol", "DOL"), ("modules", "RELs")]),
    (
        "By part",
        [
            ("game", "Game code"),
            ("sdk", "Dolphin SDK"),
            ("jsystem", "JSystem"),
            ("famicom", "NES emulator"),
            ("library", "Libraries"),
        ],
    ),
]

TABLE_NAMES = {
    "dol": "DOL (`static.dol`)",
    "modules": "RELs (`foresta`, `forestd`, `foresti`, `foresto`)",
    "game": "Game code",
    "sdk": "Dolphin SDK",
    "jsystem": "JSystem",
    "famicom": "NES emulator",
    "library": "Libraries",
}

# One hue, two steps (linked / matched only), stepped for each surface.
THEMES = {
    "light": {
        "surface": "#fcfcfb",
        "border": "#e4e3de",
        "text": "#0b0b0b",
        "text2": "#52514e",
        "track": "#f0efec",
        "linked": "#256abf",
        "matched": "#86b6ef",
    },
    "dark": {
        "surface": "#1a1a19",
        "border": "#383835",
        "text": "#f0efec",
        "text2": "#c3c2b7",
        "track": "#383835",
        "linked": "#5598e7",
        "matched": "#1c5cab",
    },
}

FONT = "-apple-system, BlinkMacSystemFont, 'Segoe UI', 'Noto Sans', Helvetica, Arial, sans-serif"

W = 720
PAD = 24
LABEL_W = 120
BAR_W = 320
BAR_H = 12
ROW_H = 30
RADIUS = 4


def pct(value):
    """A percentage as the report gives it, shown with two decimals (never 100.00 unless it is)."""
    v = float(value)
    if v >= 100.0:
        return "100%"
    return f"{min(v, 99.99):.2f}%"


def count(a, b):
    return f"{int(a):,} / {int(b):,}"


def demangle(name):
    """Class::method from a CodeWarrior name such as beginRender__10JFWDisplayFv; C names are kept."""
    m = re.match(r"^(\w+?)__(\d+)(\w+)$", name)
    if m:
        n = int(m.group(2))
        cls = m.group(3)[:n]
        if len(cls) == n:
            return f"{cls}::{m.group(1)}"
    return name


def git(*args):
    try:
        return subprocess.check_output(["git", *args], cwd=ROOT, text=True).strip()
    except (OSError, subprocess.CalledProcessError):
        return ""


def bar(x, y, m, t):
    """The track, the linked part and the matched-only part of one bar."""
    matched = min(float(m["matched_code_percent"]), 100.0)
    linked = min(float(m["complete_code_percent"]), matched)
    lw = BAR_W * linked / 100.0
    mw = BAR_W * matched / 100.0
    out = [f'<path d="{segment(x, y, BAR_W, True, True)}" fill="{t["track"]}"/>']
    if mw - lw >= 3 and lw >= 2 * RADIUS:
        # A 2px surface gap between the two fills; the rounded data end is the matched part's.
        out.append(f'<path d="{segment(x, y, lw - 1, True, False)}" fill="{t["linked"]}"/>')
        out.append(f'<path d="{segment(x + lw + 1, y, mw - lw - 1, False, True)}" fill="{t["matched"]}"/>')
    elif mw >= 2 * RADIUS:
        out.append(f'<path d="{segment(x, y, mw, True, True)}" fill="{t["linked"]}"/>')
    return out


def segment(x, y, w, round_left, round_right):
    """A horizontal bar segment with 4px corners on the ends asked for."""
    r = RADIUS
    h = BAR_H
    d = [f"M{x + (r if round_left else 0):.1f},{y}"]
    if round_right:
        d.append(f"H{x + w - r:.1f} A{r},{r} 0 0 1 {x + w:.1f},{y + r} V{y + h - r} A{r},{r} 0 0 1 {x + w - r:.1f},{y + h}")
    else:
        d.append(f"H{x + w:.1f} V{y + h}")
    if round_left:
        d.append(f"H{x + r:.1f} A{r},{r} 0 0 1 {x:.1f},{y + h - r} V{y + r} A{r},{r} 0 0 1 {x + r:.1f},{y}")
    else:
        d.append(f"H{x:.1f} V{y}")
    d.append("Z")
    return " ".join(d)


def svg(report, version, commit, date, theme):
    t = THEMES[theme]
    cats = {c["id"]: c["measures"] for c in report["categories"]}
    m = report["measures"]
    out = []

    def text(x, y, s, size=13, fill="text", weight="normal", anchor="start"):
        out.append(
            f'<text x="{x}" y="{y}" font-size="{size}" font-weight="{weight}" fill="{t[fill]}" '
            f'text-anchor="{anchor}">{escape(s)}</text>'
        )

    y = PAD + 18
    text(PAD, y, "Dōbutsu no Mori e+ decompilation", size=18, weight="600")
    y += 22
    text(PAD, y, f"{version} · {VERSION_NAMES.get(version, version)} · bytes of code", size=13, fill="text2")

    # Three headline numbers.
    y += 22
    tiles = [
        (pct(m["matched_code_percent"]), "of the code matched"),
        (pct(m["complete_code_percent"]), "of the code linked from source"),
        (count(m["matched_functions"], m["total_functions"]), "functions matched"),
    ]
    tile_w = (W - 2 * PAD - 2 * 12) / 3
    for i, (big, small) in enumerate(tiles):
        tx = PAD + i * (tile_w + 12)
        out.append(
            f'<rect x="{tx:.1f}" y="{y}" width="{tile_w:.1f}" height="64" rx="8" fill="none" '
            f'stroke="{t["border"]}"/>'
        )
        text(f"{tx + 14:.1f}", y + 30, big, size=22, weight="600")
        text(f"{tx + 14:.1f}", y + 50, small, size=12, fill="text2")
    y += 64 + 28

    # Legend.
    lx = PAD
    for key, label in (("linked", "Linked from source"), ("matched", "Matched, file not linked yet")):
        out.append(f'<rect x="{lx}" y="{y - 10}" width="12" height="12" rx="3" fill="{t[key]}"/>')
        text(lx + 18, y, label, size=12, fill="text2")
        lx += 18 + 7 * len(label) + 24
    y += 14

    bar_x = PAD + LABEL_W
    for group, items in GROUPS:
        y += 24
        text(PAD, y, group.upper(), size=11, fill="text2", weight="600")
        y += 6
        for cid, label in items:
            cm = cats.get(cid)
            if cm is None:
                continue
            y += ROW_H
            text(PAD, y, label, size=13)
            out.extend(bar(bar_x, y - BAR_H + 2, cm, t))
            out.append(
                f'<text x="{bar_x + BAR_W + 14}" y="{y}" font-size="13" fill="{t["text"]}">'
                f'<tspan font-weight="600">{pct(cm["matched_code_percent"])}</tspan>'
                f'<tspan fill="{t["text2"]}"> matched · {pct(cm["complete_code_percent"])} linked</tspan>'
                f"</text>"
            )
        y += 6

    y += 30
    text(PAD, y, "Matched: the function compiles to the original bytes. Linked: the whole file is built from source.",
         size=11, fill="text2")
    y += 16
    text(PAD, y, f"These numbers last changed at commit {commit} ({date}).", size=11, fill="text2")
    h = y + PAD - 4

    head = (
        f'<svg xmlns="http://www.w3.org/2000/svg" width="{W}" height="{h}" viewBox="0 0 {W} {h}" '
        f'font-family="{FONT}" role="img" aria-label="Decompilation progress of {version}: '
        f'{pct(m["matched_code_percent"])} of the code matched, {pct(m["complete_code_percent"])} linked">\n'
        f'<rect x="0.5" y="0.5" width="{W - 1}" height="{h - 1}" rx="12" fill="{t["surface"]}" '
        f'stroke="{t["border"]}"/>\n'
    )
    return head + "\n".join(out) + "\n</svg>\n"


def remaining(report):
    """The functions not matched yet, and the files with code that are not linked from source."""
    funcs = []
    files = []
    for u in report["units"]:
        md = u.get("metadata", {})
        name = u["name"].split("/", 1)[-1]
        for f in u.get("functions", []):
            if float(f.get("fuzzy_match_percent", 0)) < 100.0:
                funcs.append((name, demangle(f["name"]), int(f.get("size", 0)), float(f["fuzzy_match_percent"])))
        if not md.get("complete") and int(u["measures"].get("total_code", 0)) > 0:
            files.append(name)
    return funcs, files


def data_outside_source(report):
    """The data-only files that are not linked from source, and their bytes."""
    blocks = 0
    total = 0
    for u in report["units"]:
        md = u.get("metadata", {})
        if not md.get("complete") and int(u["measures"].get("total_code", 0)) == 0:
            blocks += 1
            total += int(u["measures"].get("total_data", 0))
    return blocks, total


def badges(report):
    cats = {c["id"]: c["measures"] for c in report["categories"]}

    def badge(label, value):
        v = pct(value).replace("%", "%25")
        return f"https://img.shields.io/badge/{label}-{v}-256abf"

    return "\n".join(
        [
            f"[Code Progress]: {badge('code', report['measures']['matched_code_percent'])}",
            f"[DOL Progress]: {badge('DOL', cats['dol']['matched_code_percent'])}",
            f"[REL Progress]: {badge('REL', cats['modules']['matched_code_percent'])}",
        ]
    )


def table(report, version, commit, date):
    cats = {c["id"]: c for c in report["categories"]}
    m = report["measures"]
    lines = [
        f"Measured on `{version}`, {VERSION_NAMES.get(version, version)}. These numbers last changed at commit "
        f"`{commit}` ({date}).",
        "",
        "| | Code matched | Code linked | Functions matched | Files linked |",
        "|---|---:|---:|---:|---:|",
        f"| **All** | **{pct(m['matched_code_percent'])}** | **{pct(m['complete_code_percent'])}** | "
        f"{count(m['matched_functions'], m['total_functions'])} | {count(m['complete_units'], m['total_units'])} |",
    ]
    for _, items in GROUPS:
        for cid, _label in items:
            c = cats.get(cid)
            if c is None:
                continue
            cm = c["measures"]
            lines.append(
                f"| {TABLE_NAMES[cid]} | {pct(cm['matched_code_percent'])} | {pct(cm['complete_code_percent'])} | "
                f"{count(cm['matched_functions'], cm['total_functions'])} | "
                f"{count(cm['complete_units'], cm['total_units'])} |"
            )

    funcs, files = remaining(report)
    lines.append("")
    if funcs:
        lines.append(f"Functions not matched yet ({len(funcs)}, all close):")
        lines.append("")
        for unit, func, size, fuzzy in sorted(funcs):
            lines.append(f"- `{func}` in `{unit}` ({size:,} bytes, {pct(fuzzy)} the same)")
        lines.append("")
    else:
        lines.append("Every function is matched.")
        lines.append("")
    if files:
        lines.append(
            "Files with code that are not linked from source yet, so the matching build takes them from the "
            "original's assembly: " + ", ".join(f"`{f}`" for f in sorted(files)) + "."
        )
        lines.append("")
    blocks, data = data_outside_source(report)
    lines.append(
        f"Data: {pct(m['matched_data_percent'])} of {int(m['total_data']) / 1e6:.1f} MB matched. Most of the rest, "
        f"{data / 1e6:.1f} MB, is in {blocks} blocks of the RELs' data (mostly in `forestd`, the REL with the "
        f"game's assets) that are not split into source files yet, so the build takes them from the original's "
        f"assembly."
    )
    return "\n".join(lines)


# The commit and date in the chart and the table, which say when the numbers last changed.
STAMP = re.compile(r"(commit `?)[0-9a-f]{7,40}(`? \()\d{4}-\d{2}-\d{2}(\))")


def write_if_changed(path, text):
    """Write the file only when more than its commit and date change, so an unchanged chart keeps its stamp."""
    old = path.read_text(encoding="utf-8") if path.exists() else None
    if old is not None and STAMP.sub(r"\1-\2-\3", old) == STAMP.sub(r"\1-\2-\3", text):
        print(f"{path.relative_to(ROOT)}: no change")
        return
    path.write_text(text, encoding="utf-8")
    print(f"{path.relative_to(ROOT)}: updated")


def replace_block(text, name, body):
    start = f"<!-- {name}:start -->"
    end = f"<!-- {name}:end -->"
    pattern = re.compile(re.escape(start) + r".*?" + re.escape(end), re.S)
    if not pattern.search(text):
        raise SystemExit(f"README.md has no {start} ... {end} block")
    return pattern.sub(lambda _m: f"{start}\n{body}\n{end}", text)


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--version", default="GAEJ01_01", help="game version whose report to read")
    parser.add_argument("--report", type=Path, help="report.json to read (default: build/<version>/report.json)")
    args = parser.parse_args()

    report_path = args.report or ROOT / "build" / args.version / "report.json"
    report = json.loads(report_path.read_text(encoding="utf-8"))
    commit = git("rev-parse", "--short=7", "HEAD") or "0000000"
    date = git("log", "-1", "--format=%cs") or "0000-00-00"

    for theme in THEMES:
        write_if_changed(ROOT / "assets" / f"progress-{theme}.svg", svg(report, args.version, commit, date, theme))

    readme = ROOT / "README.md"
    text = readme.read_text(encoding="utf-8")
    text = replace_block(text, "progress-badges", badges(report))
    text = replace_block(text, "progress-table", table(report, args.version, commit, date))
    write_if_changed(readme, text)


if __name__ == "__main__":
    main()
