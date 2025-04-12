ARG VARIANT=dev
FROM ubuntu:22.04 as base

ENV DEBIAN_FRONTEND noninteractive

RUN apt update && \
    apt install autoconf automake libtool curl wget make cmake g++ unzip git -y && \
    rm -rf /var/lib/apt/lists/*

# Install Bazel
RUN apt update && apt install apt-transport-https curl gnupg -y && \
    curl -fsSL https://bazel.build/bazel-release.pub.gpg | gpg --dearmor >bazel-archive-keyring.gpg && \
    mv bazel-archive-keyring.gpg /usr/share/keyrings && \
    echo "deb [arch=amd64 signed-by=/usr/share/keyrings/bazel-archive-keyring.gpg] https://storage.googleapis.com/bazel-apt stable jdk1.8" | tee /etc/apt/sources.list.d/bazel.list && \
    apt update && \
    wget https://github.com/bazelbuild/bazel/releases/download/7.3.1/bazel-7.3.1-installer-linux-x86_64.sh -O /tmp/bazel-installer.sh && \
    chmod +x /tmp/bazel-installer.sh && \
    /tmp/bazel-installer.sh

# install Clang
ENV CLANG 20
ENV CLANG_URL https://apt.llvm.org/llvm.sh
RUN apt update && apt install -y lsb-release wget software-properties-common gnupg && \
    wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - && \
    echo "deb http://apt.llvm.org/$(lsb_release -cs)/ llvm-toolchain-$(lsb_release -cs)-${CLANG} main" | tee /etc/apt/sources.list.d/llvm-toolchain.list && \
    echo "deb-src http://apt.llvm.org/$(lsb_release -cs)/ llvm-toolchain-$(lsb_release -cs)-${CLANG} main" | tee -a /etc/apt/sources.list.d/llvm-toolchain.list && \
    apt update && \
    apt install lsb-release wget software-properties-common gnupg -y && \
    apt install --no-install-recommends -y \
    clang-${CLANG} \
    clang-tidy-${CLANG} \
    clang-format-${CLANG} \
    llvm-${CLANG}-dev \
    clangd-${CLANG} \
    libclang-${CLANG}-dev \
    libclang-cpp${CLANG} && \
    rm -rf /var/lib/apt/lists/*

RUN apt update && \
    apt install lsb-release wget software-properties-common gnupg -y && \
    wget ${CLANG_URL} && \
    chmod +x llvm.sh && \
    ./llvm.sh ${CLANG} && \
    ln -s /usr/bin/clang-20 /usr/bin/clang && \
    ln -s /usr/bin/clang++-20 /usr/bin/clang++ && \
    ln -s /usr/bin/clang-tidy-20 /usr/bin/clang-tidy && \
    ln -s /usr/bin/clang-format-20 /usr/bin/clang-format && \
    ln -s /usr/bin/clangd-20 /usr/bin/clangd && \
    rm -rf llvm.sh && \
    rm -rf /var/lib/apt/lists/*

FROM base as dev-stage
COPY --from=base /usr/local/include /usr/local/include
COPY --from=base /usr/local/lib /usr/local/lib
COPY --from=base /usr/local/bin /usr/local/bin

# install Clangd
ENV CLANGD 16.0.2
ENV CLANGD_URL https://github.com/clangd/clangd/releases/download/${CLANGD}/clangd-linux-${CLANGD}.zip
WORKDIR /tmp
RUN wget ${CLANGD_URL} && \
    unzip clangd-linux-${CLANGD}.zip && \
    mv clangd_${CLANGD}/lib/clang /usr/local/lib/ && \
    mv clangd_${CLANGD}/bin/clangd /usr/local/bin/ && \
    rm -rf clangd_${CLANGD} && \
    rm -rf clangd-linux-${CLANGD}.zip

# install GDB
ENV GDB 12.1
ENV GDB_URL https://ftp.gnu.org/gnu/gdb/gdb-${GDB}.tar.gz
WORKDIR /tmp
RUN apt update && \
    apt install -y --no-install-recommends libgmp-dev && \
    rm -rf /var/lib/apt/lists/* && \
    wget ${GDB_URL} && \
    tar -zxvf gdb-${GDB}.tar.gz && \
    cd gdb-${GDB} && \
    ./configure && \
    export CPP='g++ -E' && \
    export CXX='g++' && \
    export CC='gcc' && \
    export LD='g++' && \
    make -j$(nproc) && \
    make install && \
    cd /tmp && \
    rm -rf /tmp/gdb-${GDB}*

# install zsh & fix ssh env
WORKDIR /tmp
RUN apt update && apt install zsh openssh-client openssh-server -y && \
    echo "export \$(cat /proc/1/environ |tr '\\\0' '\\\n' | xargs)" >> /etc/profile && \
    echo "export \$(cat /proc/1/environ |tr '\\\0' '\\\n' | xargs)" >> /etc/zsh/zprofile && \
    rm -rf ~/.oh-my-zsh || true && \
    curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh -o install.sh && \
    chmod +x install.sh && \
    ./install.sh --skip-chsh --unattended && \
    rm install.sh && \
    git clone http://github.com/zsh-users/zsh-autosuggestions ~/.oh-my-zsh/custom/plugins/zsh-autosuggestions && \
    git clone http://github.com/zsh-users/zsh-syntax-highlighting.git ~/.oh-my-zsh/custom/plugins/zsh-syntax-highlighting && \
    sed -e '/ZSH_THEME=/s/"robbyrussell"/"ys"/' \
    -e '/plugins=(git/s/)/ zsh-autosuggestions zsh-syntax-highlighting)/' -i ~/.zshrc && \
    chsh -s /bin/zsh root && \
    rm -rf /var/lib/apt/lists/*

# install vim & plugins
RUN apt update && \
    apt install -y --no-install-recommends \
    git vim && \
    rm -rf /var/lib/apt/lists/* && \
    git config --global core.editor vim && \
    git config --global http.sslverify false && \
    git config --global https.sslverify false && \
    git clone https://github.com/sickill/vim-monokai.git /tmp/vim-monokai && \
    mkdir -p ~/.vim/colors && \
    mv /tmp/vim-monokai/colors/*  ~/.vim/colors && \
    git config --global --unset http.sslverify && \
    git config --global --unset https.sslverify && \
    touch ~/.vimrc && \
    vim -u ~/.vimrc +PlugInstall +qall

FROM base as final-build
USER root
WORKDIR /workspace
ENV CC "clang"
ENV CXX "clang++"
ENV CPP "clang++ -E"
ENV LD "clang++"
ENV PATH $PATH:/usr/local/cuda/bin:/usr/src/tensorrt/bin
ENV LD_LIBRARY_PATH $LD_LIBRARY_PATH:/usr/local/cuda/lib64:/usr/local/cuda/targets/x86_64-linux/lib:/usr/lib/llvm-20/lib
ENV LIBRARY_PATH $LIBRARY_PATH::/usr/local/lib:/usr/local/cuda/lib64:/usr/local/cuda/targets/x86_64-linux/lib:/usr/lib/llvm-20/lib
ENV C_INCLUDE_PATH $C_INCLUDE_PATH:/usr/local/cuda/include:/usr/include/x86_64-linux-gnu:/usr/lib/llvm-20/include:/usr/include/llvm-20
ENV CPLUS_INCLUDE_PATH $CPLUS_INCLUDE_PATH:/usr/local/cuda/include://usr/include/x86_64-linux-gnu::/usr/lib/llvm-20/include:/usr/include/llvm-20

FROM dev-stage as final-dev
USER root
WORKDIR /workspace
ENV CC "clang"
ENV CXX "clang++"
ENV CPP "clang++ -E"
ENV LD "clang++"
ENV PATH $PATH:/usr/local/cuda/bin:/usr/src/tensorrt/bin
ENV LD_LIBRARY_PATH $LD_LIBRARY_PATH:/usr/local/cuda/lib64:/usr/local/cuda/targets/x86_64-linux/lib:/usr/lib/llvm-20/lib
ENV LIBRARY_PATH $LIBRARY_PATH::/usr/local/lib:/usr/local/cuda/lib64:/usr/local/cuda/targets/x86_64-linux/lib:/usr/lib/llvm-20/lib
ENV C_INCLUDE_PATH $C_INCLUDE_PATH:/usr/local/cuda/include:/usr/include/x86_64-linux-gnu:/usr/lib/llvm-20/include:/usr/include/llvm-20
ENV CPLUS_INCLUDE_PATH $CPLUS_INCLUDE_PATH:/usr/local/cuda/include://usr/include/x86_64-linux-gnu::/usr/lib/llvm-20/include:/usr/include/llvm-20

FROM final-${VARIANT} as final
