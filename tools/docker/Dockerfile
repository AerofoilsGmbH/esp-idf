FROM ubuntu:24.04

ARG DEBIAN_FRONTEND=noninteractive

# Minimal set of packages to install the tools, build applications and run them in QEMU:
#   libglib2.0-0, libpixman-1-0, libsdl2-2.0-0, libslirp0 are runtime dependencies of QEMU.
# Use IDF_APT_EXTRA_PACKAGES to install additional packages (space separated).
# Use IDF_APT_REMOVE_PACKAGES to forcibly remove packages which are pulled in as dependencies but are not needed at
#   runtime (space separated, wildcards are allowed), e.g. the Mesa/LLVM libraries pulled in by libsdl2-2.0-0.
#   Note that this leaves unmet dependencies, so "apt-get -f install" is needed before installing packages later.
ARG IDF_APT_EXTRA_PACKAGES=
ARG IDF_APT_REMOVE_PACKAGES=

RUN : \
  && apt-get update \
  && apt-get install -y --no-install-recommends \
    ca-certificates \
    ccache \
    git \
    libglib2.0-0 \
    libpixman-1-0 \
    libsdl2-2.0-0 \
    libslirp0 \
    ninja-build \
    python3 \
    python3-venv \
    ${IDF_APT_EXTRA_PACKAGES} \
  && apt-get autoremove -y \
  && if [ -n "$IDF_APT_REMOVE_PACKAGES" ]; then \
    set -f \
    && packages=$(dpkg-query -W -f='${db:Status-Abbrev} ${Package}\n' $IDF_APT_REMOVE_PACKAGES 2>/dev/null \
      | awk '$1 == "ii" { print $2 }') \
    && if [ -n "$packages" ]; then dpkg --remove --force-depends $packages; fi \
    && set +f ; \
  fi \
  && rm -rf /var/lib/apt/lists/* \
  && update-alternatives --install /usr/bin/python python /usr/bin/python3 10 \
  && :

ARG IDF_INSTALL_TARGETS=all

# install build essential needed for linux target apps, which is a preview target so it is installed with "all" only
RUN if [ "$IDF_INSTALL_TARGETS" = "all" ]; then \
    apt-get update \
    && apt-get install -y build-essential \
    && apt-get autoremove -y \
    && rm -rf /var/lib/apt/lists/* ; \
  fi

# To build the image for a branch or a tag of IDF, pass --build-arg IDF_CLONE_BRANCH_OR_TAG=name.
# To build the image with a specific commit ID of IDF, pass --build-arg IDF_CHECKOUT_REF=commit-id.
# It is possible to combine both, e.g.:
#   IDF_CLONE_BRANCH_OR_TAG=release/vX.Y
#   IDF_CHECKOUT_REF=<some commit on release/vX.Y branch>.
# Use IDF_CLONE_SHALLOW=1 to perform shallow clone (i.e. --depth=1)
# Use IDF_CLONE_SHALLOW_DEPTH=X to define the depth if IDF_CLONE_SHALLOW is used (i.e. --depth=X)
# Use IDF_CLONE_FILTER to perform a partial clone (i.e. --filter=X). With IDF_CLONE_FILTER=tree:0 only the commit
#   history is downloaded, which keeps "git describe" (IDF version) working at a fraction of the size of a full clone.
# Use IDF_SPARSE_CHECKOUT_EXCLUDE to not check out directories not needed for building (space separated, gitignore
#   style patterns, e.g. "/docs/ /examples/"). A sparse checkout does not mark the working tree as dirty.
# Use IDF_SUBMODULES_EXCLUDE to skip submodules not needed for the selected targets (space separated paths).
#   Skipped submodules are marked with "update = none", so they are not checked out by later submodule updates.
# Use IDF_SUBMODULES_DEPTH to define the clone depth of the submodules (default: 1 if IDF_CLONE_SHALLOW is used,
#   full history otherwise)
# Use IDF_INSTALL_TARGETS to install tools only for selected chip targets (CSV)
# Use IDF_INSTALL_TOOLS to define the tools to install (space separated, "required" and wildcards are allowed)
# Use IDF_SKIP_TOOLS_CHECK=1 if IDF_INSTALL_TOOLS does not contain all tools required for the selected targets.
#   Otherwise the environment cannot be activated because of the missing tools.
# Use IDF_TOOLCHAIN_REMOVE_MULTILIBS to delete toolchain libraries of chips not in use (space separated multilib
#   directory names, e.g. "esp32 esp32s2" when building only for esp32s3)
# Use IDF_PYTHON_EXTRA_PACKAGES to install additional Python packages into the IDF Python environment

ARG IDF_CLONE_URL=https://github.com/espressif/esp-idf.git
ARG IDF_CLONE_BRANCH_OR_TAG=master
ARG IDF_CHECKOUT_REF=
ARG IDF_CLONE_SHALLOW=
ARG IDF_CLONE_SHALLOW_DEPTH=1
ARG IDF_CLONE_FILTER=
ARG IDF_SPARSE_CHECKOUT_EXCLUDE=
ARG IDF_SUBMODULES_EXCLUDE=
ARG IDF_SUBMODULES_DEPTH=${IDF_CLONE_SHALLOW:+1}
ARG IDF_GITHUB_ASSETS=
ARG PIP_INDEX_URL=

ENV IDF_PATH=/opt/esp/idf
ENV IDF_TOOLS_PATH=/opt/esp
ENV IDF_GITHUB_ASSETS=${IDF_GITHUB_ASSETS}

RUN echo IDF_CHECKOUT_REF=$IDF_CHECKOUT_REF IDF_CLONE_BRANCH_OR_TAG=$IDF_CLONE_BRANCH_OR_TAG && \
    git clone \
      ${IDF_CLONE_SHALLOW:+--depth=${IDF_CLONE_SHALLOW_DEPTH}} \
      ${IDF_CLONE_FILTER:+--filter=${IDF_CLONE_FILTER}} \
      ${IDF_CLONE_BRANCH_OR_TAG:+-b $IDF_CLONE_BRANCH_OR_TAG} \
      $IDF_CLONE_URL $IDF_PATH && \
    git config --system --add safe.directory $IDF_PATH && \
    cd $IDF_PATH && \
    if [ -n "$IDF_CHECKOUT_REF" ]; then \
      if [ -n "$IDF_CLONE_SHALLOW" ]; then \
        git fetch origin --depth=${IDF_CLONE_SHALLOW_DEPTH} ${IDF_CHECKOUT_REF}; \
      fi && \
      git checkout $IDF_CHECKOUT_REF; \
    fi && \
    if [ -n "$IDF_SPARSE_CHECKOUT_EXCLUDE" ]; then \
      git sparse-checkout set --no-cone '/*' $(for pattern in $IDF_SPARSE_CHECKOUT_EXCLUDE; do echo "!$pattern"; done); \
    fi && \
    for path in $IDF_SUBMODULES_EXCLUDE; do \
      git config "submodule.$path.update" none; \
    done && \
    git submodule update --init --recursive ${IDF_SUBMODULES_DEPTH:+--depth=${IDF_SUBMODULES_DEPTH}}

ARG IDF_INSTALL_TOOLS="required qemu*"
ARG IDF_SKIP_TOOLS_CHECK=0
ARG IDF_TOOLCHAIN_REMOVE_MULTILIBS=
ARG IDF_PYTHON_EXTRA_PACKAGES=

# Install all the required tools
RUN : \
  && update-ca-certificates --fresh \
  && $IDF_PATH/tools/idf_tools.py --non-interactive install ${IDF_INSTALL_TOOLS} --targets=${IDF_INSTALL_TARGETS} \
  && $IDF_PATH/tools/idf_tools.py --non-interactive install cmake \
  && rm -rf $IDF_TOOLS_PATH/tools/cmake/*/doc $IDF_TOOLS_PATH/tools/cmake/*/man \
  && for multilib in $IDF_TOOLCHAIN_REMOVE_MULTILIBS; do \
    find $IDF_TOOLS_PATH/tools -path "*-esp-elf/*" -type d -name "$multilib" -prune -exec rm -rf {} + ; \
  done \
  && PIP_NO_CACHE_DIR=1 $IDF_PATH/tools/idf_tools.py --non-interactive install-python-env \
  && if [ -n "$IDF_PYTHON_EXTRA_PACKAGES" ]; then \
    $IDF_TOOLS_PATH/python_env/idf*_env/bin/python -m pip install --no-cache-dir \
      --constraint $IDF_TOOLS_PATH/espidf.constraints.*.txt ${IDF_PYTHON_EXTRA_PACKAGES}; \
  fi \
  && rm -rf $IDF_TOOLS_PATH/dist /root/.cache \
  && :

# Add get_idf as alias
RUN echo 'alias get_idf=". /opt/esp/idf/export.sh"' >> /root/.bashrc

# The constraint file has been downloaded and the right Python package versions installed. No need to check and
# download this at every invocation of the container.
ENV IDF_PYTHON_CHECK_CONSTRAINTS=no

ENV IDF_SKIP_TOOLS_CHECK=${IDF_SKIP_TOOLS_CHECK}

# Submodules excluded via IDF_SUBMODULES_EXCLUDE must not be initialised again by the build system.
ENV IDF_SKIP_CHECK_SUBMODULES=${IDF_SUBMODULES_EXCLUDE:+1}

# Ccache is installed, enable it by default
ENV IDF_CCACHE_ENABLE=1

# All caches written during a build are located below IDF_CACHE_PATH. Mount a (persistent) directory there,
# e.g. "-v $PWD/.cache:/opt/esp/cache", to reuse the caches across containers (e.g. with actions/cache).
ENV IDF_CACHE_PATH=/opt/esp/cache
ENV CCACHE_DIR=${IDF_CACHE_PATH}/ccache
# Hash the content (not the mtime) of the compiler and of the specs files generated in the build directory,
# otherwise every clean build (e.g. in CI) misses the cache.
ENV CCACHE_COMPILERCHECK=content
ENV IDF_COMPONENT_CACHE_PATH=${IDF_CACHE_PATH}/component-manager
ENV PIP_CACHE_DIR=${IDF_CACHE_PATH}/pip

COPY entrypoint.sh /opt/esp/entrypoint.sh
RUN chmod +x /opt/esp/entrypoint.sh
ENTRYPOINT [ "/opt/esp/entrypoint.sh" ]
CMD [ "/bin/bash" ]
