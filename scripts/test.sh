#!/bin/bash

set -e

# Bazel test

bazel test $(bazel query //...) --action_env=LIBRARY_PATH=$LIBRARY_PATH --nocache_test_results
