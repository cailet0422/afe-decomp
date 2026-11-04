# GitHub Actions

[.github/workflows/build.yml](/.github/workflows/build.yml) builds `GAEJ01_01` (Rev 1) on every push and checks
it against the original. On the default branch (`main`) it also updates the progress chart and table in the
[README](/README.md#progress) when the numbers change, and commits them as `github-actions[bot]`.

The build needs five files of the game. They must never be committed to this repository or to any public one,
so they are kept in a **private** repository that the workflow reads with a token. Without the token the workflow
skips the build and says so.

- [The game files' repository](#the-game-files-repository)
- [The token](#the-token)
- [Running it](#running-it)
- [What stays private](#what-stays-private)

## The game files' repository

1. Create a new repository named `afe-decomp-orig` and make it **Private**.
   (Another name works too: set it in a repository variable named `ORIG_REPO`, as `owner/name`, under
   Settings → Secrets and variables → Actions → Variables.)

2. Add the files, extracted from your own copy of the game, with this layout:

   ```
   orig/GAEJ01_01/sys/main.dol
   orig/GAEJ01_01/files/foresta.rel
   orig/GAEJ01_01/files/forestd.rel
   orig/GAEJ01_01/files/foresti.rel
   orig/GAEJ01_01/files/foresto.rel
   ```

   They are about 18 MB in all. Nothing else from the disc is needed. `GAEJ01_01/sys/...` or `sys/...` at the
   root of the repository also work.

## The token

1. In your account's Settings → Developer settings → Personal access tokens → **Fine-grained tokens**, generate a
   new token:
   - Repository access: **Only select repositories** → the game files' repository only.
   - Permissions: Repository permissions → **Contents: Read-only**.
   - An expiration date you are happy to renew; when it expires, the build fails at "Checkout the game files"
     until the secret is replaced.

2. In this repository's Settings → Secrets and variables → Actions → **New repository secret**, add it with the
   name `ORIG_TOKEN`.

## Running it

- If this repository is a fork, Actions may be off: open the **Actions** tab and turn workflows on.
- Push a commit, or open Actions → Build → **Run workflow**.
- The run's summary shows the progress table. On `main`, a commit "Update the progress chart for …" follows
  when the numbers changed.
- To make the chart by hand after a local build: `python tools/progress_readme.py`.

## What stays private

- Workflows of pull requests from forks get no secrets, and the workflow does not run for pull requests at all;
  branches of this repository are built on push. Someone else's pull request cannot read the game files.
- The game files are copied into the build's `orig/` and the private checkout is deleted; nothing of the build is
  uploaded. Only `README.md` and `assets/progress-*.svg` are committed.
- Keep the game files' repository private, and the token limited to that repository and to reading.
