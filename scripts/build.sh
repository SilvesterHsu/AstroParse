#!/bin/bash

set -e

# Bazel build
bazel build $(bazel query //...) --action_env=LIBRARY_PATH=$LIBRARY_PATH
