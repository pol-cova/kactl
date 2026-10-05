# Team guide

How our team works on this notebook for nationals.

## Get the latest PDF

Every push to `main` rebuilds the notebook automatically. The newest version is always here:

**https://github.com/pol-cova/kactl/releases/download/latest/kactl.pdf**

Each pull request also builds a PDF: open the PR → **Checks** → *Build notebook PDF* → download `kactl-pdf` under **Artifacts**. Use it to check the layout before merging.

## How the notebook is built

- `content/kactl.tex` is the main file (team name, members, cover page).
- Each topic is a folder in `content/` (`graph/`, `strings/`, `math/`, ...).
- Each folder's `chapter.tex` decides what goes in the PDF. One line per algorithm:
  ```latex
  \kactlimport{Dijkstra.h}
  ```
  Commented-out lines (`% \kactlimport{...}`) are in the repo but **not** in the PDF.
- Run `make showexcluded` to list every algorithm that isn't in the PDF.

## Adding an algorithm

1. Create `content/<topic>/MyAlgo.h`. Copy the header format from any existing file:
   ```cpp
   /**
    * Author: Your Name
    * Date: 2026-10-05
    * License: CC0
    * Source: where it came from
    * Description: One or two lines on what it does and how to use it.
    * Time: O(N \log N)
    * Status: tested on <problem link>
    */
   #pragma once
   ```
2. Add `\kactlimport{MyAlgo.h}` to that folder's `chapter.tex`.
3. Test it: submit it to a real problem, and/or add a stress test in `stress-tests/<topic>/`.
4. Open a pull request. **Another teammate reviews it** before merging. Reviewing is how everyone learns the code.

## Working together

- **Never push straight to `main`.** Make a branch, open a PR:
  ```bash
  git checkout -b graph/add-hld
  # edit files
  git add -A && git commit -m "Add heavy-light decomposition"
  git push -u origin graph/add-hld
  ```
- Split ownership by topic so you rarely edit the same files:

  | Member | Topics |
  | --- | --- |
  | _name_ | Graphs, trees |
  | _name_ | Math, number theory, combinatorics |
  | _name_ | Strings, data structures, geometry |

- Only one person edits `content/kactl.tex` (team page, layout) at a time.

## Building locally (optional)

Needs `pdflatex` (TeX Live on Linux/macOS, MiKTeX on Windows).

```bash
make kactl   # full build → kactl.pdf
make fast    # quicker single pass
make test    # run stress tests
```

## Before the contest

- [ ] Confirm the page limit, paper size, and font rules for nationals.
- [ ] Remove anything nobody on the team understands.
- [ ] Add `\columnbreak` / `\newpage` in `chapter.tex` files to fix awkward page splits.
- [ ] Print it and use it in at least two full mock contests.
- [ ] Know the 6-character hash in each snippet's top-right corner: it lets you check you typed the code correctly (`content/contest/hash.sh`).
