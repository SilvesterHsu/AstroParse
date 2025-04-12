#!/bin/bash

set -e

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

# Display usage
show_usage() {
  echo "Usage: $0 [dev|build]"
  echo "  dev    - Build the dev image"
  echo "  build  - Build the build image"
}

main() {
  if [ "$#" -ne 1 ]; then
    show_usage
    exit 1
  fi

  case $1 in
    dev)
      info "Building dev image..."
      docker build --progress=plain -t astroparse:dev --build-arg VARIANT=dev docker/.
      ok "Dev image built successfully."
      ;;
    build)
      info "Building build image..."
      docker build --progress=plain -t astroparse:build --build-arg VARIANT=build docker/.
      ok "Build image built successfully."
      ;;
    *)
      error "Invalid argument: $1"
      show_usage
      exit 1
      ;;
  esac
}

# Execute the main function
main "$@"
