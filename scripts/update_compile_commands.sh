#!/bin/bash

set -e

# This script is used to update the compile_commands.json file in the root of the project.
# This file is used by clang-tidy and other tools to get the compile commands for each file.
# This script should be run after building the project with bazel

info() {
  echo -e "\033[0;34m[INFO]\033[0m $1"
}

error() {
  echo -e "\033[0;31m[ERROR]\033[0m $1"
}

warning() {
  echo -e "\033[0;33m[WARNING]\033[0m $1"
}

ok() {
  echo -e "\033[0;32m[OK]\033[0m $1"
}


info "Updating compile_commands.json file\n"

bazel build $(bazel query //...) --action_env=LIBRARY_PATH=$LIBRARY_PATH || true
bazel run @hedron_compile_commands//:refresh_all

ok "Successfully updated compile_commands.json file"
