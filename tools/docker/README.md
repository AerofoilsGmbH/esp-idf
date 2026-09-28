# ESP-IDF Docker Image (minimal ESP32-S3 variant)

This Dockerfile builds an image with ESP-IDF and the tools needed to build ESP-IDF projects. Without build arguments it
behaves like the upstream image. With the arguments below it produces a minimal image (~1.6 GB) that can only:

- build applications for **ESP32-S3**
- run and test them in **QEMU** (incl. `pytest-embedded`)

For general usage of the upstream image see the
[IDF Docker Image](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/tools/idf-docker-image.html)
guide.

## Contents

| Included | Not included |
|---|---|
| ESP-IDF (commit history for `git describe`, no `docs/` and `examples/`) | Toolchains for other chips, RISC-V toolchain |
| `xtensa-esp-elf` toolchain (ESP32-S3 libraries only, newlib and picolibc) | ULP toolchains (`esp32ulp-elf`, `riscv32-esp-elf`) |
| `qemu-xtensa`, `esp-rom-elfs` | GDB, OpenOCD, clangd |
| CMake (from `idf_tools.py`), Ninja, ccache | Submodules for other chips' BT libraries, BLE Mesh, BLE Audio, OpenThread |
| Python environment (core + `pytest-embedded-idf`, `pytest-embedded-qemu`) | Linux target (`build-essential`), host test tools (`lcov`, `ruby`, ...) |

Supported platforms: `linux/amd64`, `linux/arm64` (QEMU is only released for these).

## Building

Multi-platform images need a `docker-container` builder (one-time setup):

```bash
docker buildx create --name idf-builder --driver docker-container --use
```

Build and push the image (run from the repository root):

```bash
docker buildx build tools/docker \
  --platform linux/amd64,linux/arm64 \
  --build-arg IDF_CLONE_URL=https://github.com/AerofoilsGmbH/esp-idf.git \
  --build-arg IDF_CLONE_BRANCH_OR_TAG=aero/v6.1 \
  --build-arg IDF_CLONE_FILTER=tree:0 \
  --build-arg IDF_SUBMODULES_DEPTH=1 \
  --build-arg IDF_SPARSE_CHECKOUT_EXCLUDE="/docs/ /examples/" \
  --build-arg IDF_SUBMODULES_EXCLUDE="components/bt/controller/lib_esp32 components/bt/controller/lib_esp32c2/esp32c2-bt-lib components/bt/controller/lib_esp32c5/esp32c5-bt-lib components/bt/controller/lib_esp32c6/esp32c6-bt-lib components/bt/controller/lib_esp32h2/esp32h2-bt-lib components/bt/controller/lib_esp32h4/esp32h4-bt-lib components/bt/controller/lib_esp32s31/esp32s31-bt-lib components/bt/esp_ble_audio/lib/lib components/bt/esp_ble_mesh/lib/lib components/openthread/lib components/openthread/openthread" \
  --build-arg IDF_INSTALL_TARGETS=esp32s3 \
  --build-arg IDF_INSTALL_TOOLS="xtensa-esp-elf qemu-xtensa esp-rom-elfs" \
  --build-arg IDF_SKIP_TOOLS_CHECK=1 \
  --build-arg IDF_TOOLCHAIN_REMOVE_MULTILIBS="esp32 esp32s2" \
  --build-arg IDF_APT_REMOVE_PACKAGES="mesa-libgallium libllvm* libicu* libxml2 perl perl-modules-* libperl*" \
  --build-arg IDF_PYTHON_EXTRA_PACKAGES="pytest-embedded-idf pytest-embedded-qemu" \
  -t ghcr.io/aerofoilsgmbh/esp-idf:v6.1-esp32s3 \
  --push
```

For a local single-platform image replace `--platform ... --push` with `--platform linux/arm64 --load` (or `linux/amd64`).

The IDF version (`IDF_VER`) is taken from `git describe`, so the release tag (e.g. `v6.1`) must exist in the cloned
repository. Otherwise the version falls back to the previous tag (e.g. `v6.1-dev-...`).

### Build arguments

| Argument | Default | Description |
|---|---|---|
| `IDF_CLONE_URL` | upstream GitHub | Repository to clone |
| `IDF_CLONE_BRANCH_OR_TAG` | `master` | Branch or tag to clone |
| `IDF_CHECKOUT_REF` | | Commit to check out after cloning |
| `IDF_CLONE_SHALLOW`, `IDF_CLONE_SHALLOW_DEPTH` | , `1` | Shallow clone (breaks `git describe`, prefer `IDF_CLONE_FILTER`) |
| `IDF_CLONE_FILTER` | | Partial clone filter; `tree:0` fetches only the commit history |
| `IDF_SPARSE_CHECKOUT_EXCLUDE` | | Paths not to check out (gitignore patterns, space separated) |
| `IDF_SUBMODULES_EXCLUDE` | | Submodule paths to skip (space separated) |
| `IDF_SUBMODULES_DEPTH` | `1` if shallow, else full | Clone depth of the submodules |
| `IDF_INSTALL_TARGETS` | `all` | Targets to install tools for (CSV) |
| `IDF_INSTALL_TOOLS` | `required qemu*` | Tools to install via `idf_tools.py` (CMake is always installed) |
| `IDF_SKIP_TOOLS_CHECK` | `0` | Set to `1` if `IDF_INSTALL_TOOLS` omits tools marked as required |
| `IDF_TOOLCHAIN_REMOVE_MULTILIBS` | | Toolchain library directories of unused chips to delete |
| `IDF_APT_EXTRA_PACKAGES` | | Additional apt packages |
| `IDF_APT_REMOVE_PACKAGES` | | Installed-but-unneeded apt packages to force-remove (wildcards allowed) |
| `IDF_PYTHON_EXTRA_PACKAGES` | | Additional Python packages for the IDF environment |
| `IDF_GITHUB_ASSETS`, `PIP_INDEX_URL` | | Mirrors for tool downloads and Python packages |

## Running

The entrypoint activates the IDF environment, so `idf.py` can be called directly:

```bash
docker run --rm -v $PWD:/project -w /project ghcr.io/aerofoilsgmbh/esp-idf:v6.1-esp32s3 idf.py build
```

Interactive shell:

```bash
docker run --rm -it -v $PWD:/project -w /project ghcr.io/aerofoilsgmbh/esp-idf:v6.1-esp32s3
```

Run tests in QEMU with `pytest-embedded` (after `idf.py build`):

```bash
docker run --rm -v $PWD:/project -w /project ghcr.io/aerofoilsgmbh/esp-idf:v6.1-esp32s3 \
  pytest --embedded-services idf,qemu --target esp32s3 --app-path . --build-dir build
```

Warnings about tools that are not installed (GDB, OpenOCD, ...) on startup are expected.

## Caching

All caches written during a build are located below `/opt/esp/cache`:

| Path | Content |
|---|---|
| `/opt/esp/cache/ccache` | Compiler cache (`CCACHE_DIR`) |
| `/opt/esp/cache/component-manager` | Downloaded managed components (`IDF_COMPONENT_CACHE_PATH`) |
| `/opt/esp/cache/pip` | pip downloads (`PIP_CACHE_DIR`) |

Mount one directory there to reuse the caches across runs:

```bash
docker run --rm -v $PWD:/project -v $PWD/.idf-cache:/opt/esp/cache -w /project \
  ghcr.io/aerofoilsgmbh/esp-idf:v6.1-esp32s3 idf.py build
```

`CCACHE_COMPILERCHECK=content` is set, so a clean build directory still hits the cache. Limit the cache size with
`-e CCACHE_MAXSIZE=1G`.

### GitHub Actions

```yaml
# Must run before the cache is restored. The files are owned by root (written by the container), so they are removed
# from within a container as well.
- name: Clean cache directory
  run: >
    docker run --rm --entrypoint rm
    -v ${{ runner.temp }}:/runner-temp
    ghcr.io/aerofoilsgmbh/esp-idf:v6.1-esp32s3
    -rf /runner-temp/idf-cache

- uses: actions/cache@v4
  with:
    path: ${{ runner.temp }}/idf-cache
    key: idf-esp32s3-${{ github.sha }}
    restore-keys: idf-esp32s3-

- name: Build
  run: >
    docker run --rm
    -v ${{ github.workspace }}:/project
    -v ${{ runner.temp }}/idf-cache:/opt/esp/cache
    -e CCACHE_MAXSIZE=1G
    -w /project
    ghcr.io/aerofoilsgmbh/esp-idf:v6.1-esp32s3
    idf.py build
```

The unique key saves an updated cache after every run; `restore-keys` restores the newest one. The clean step makes
sure only the restored cache is used, even on self-hosted runners where a previous job may have left files behind.

The cache is kept in `runner.temp` (outside the checkout), so it neither appears in `git status` nor leaves root-owned
files in the workspace. Untracked files do not affect the `--dirty` suffix of the version, so a cache directory inside
the project (as in the local example above) only needs a `.gitignore` entry.

## Limitations

- Only ESP32-S3 can be built. Projects using BLE Mesh, BLE Audio or OpenThread need the corresponding paths removed
  from `IDF_SUBMODULES_EXCLUDE`; projects using the ULP coprocessor need `esp32ulp-elf` or `riscv32-esp-elf` in
  `IDF_INSTALL_TOOLS`.
- `IDF_APT_REMOVE_PACKAGES` leaves unmet apt dependencies. Run `apt-get -f install` before installing further packages
  in a derived image.
- Submodule checks of the build system are disabled (`IDF_SKIP_CHECK_SUBMODULES=1`) when submodules are excluded.
