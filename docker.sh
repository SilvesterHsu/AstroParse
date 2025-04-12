#!/bin/bash

set -e

DOCKER_IMG="silvesterhsu/astroparse:dev"
CONTAINER_NAME="AstroParse"
BASE_DIRECTORY=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)

# Logging functions
log_info() {
  printf "[\e[34m\e[1mINFO\e[0m] $*\n" >&2
}

log_error() {
  printf "[\033[0;31mERROR\e[0m] $*\n" >&2
}

log_warning() {
  printf "[\033[0;33mWARNING\e[0m] $*\n" >&2
}

log_success() {
  printf "[\e[32m\e[1m OK \e[0m] $*\n" >&2
}

# Display usage
show_usage() {
  local script_name=$(basename "${BASH_SOURCE[0]}")
  cat << EOF
Usage:
  ${script_name} [run|init|help]
  run              Run container (default).
  init             Init container.
  help             Display this help message.
EOF
}

# Check Docker environment
check_docker() {
  log_info "[1/4] Checking Docker environment ..."
  if ! docker --version &>/dev/null; then
    log_error "Docker is not installed. Please install Docker first."
    exit 1
  fi
}

# Check Docker image
check_docker_image() {
  log_info "[2/4] Checking if Docker image \"${DOCKER_IMG}\" exists ..."
  if ! docker image inspect "${DOCKER_IMG}" &>/dev/null; then
    log_info "Docker image \"${DOCKER_IMG}\" not found. Pulling image ..."
    docker pull "${DOCKER_IMG}" || {
      log_error "Failed to pull Docker image \"${DOCKER_IMG}\"."
      exit 1
    }
  else
    log_success "Docker image \"${DOCKER_IMG}\" already exists."
  fi
}

# Check Docker container
check_docker_container() {
  log_info "[3/4] Checking container \"${CONTAINER_NAME}\" ..."
  if docker ps -a --format "{{.Names}}" | grep -q "${CONTAINER_NAME}"; then
    log_info "Container \"${CONTAINER_NAME}\" exists. Stopping and removing it ..."
    docker stop "${CONTAINER_NAME}" &>/dev/null
    docker rm "${CONTAINER_NAME}" &>/dev/null
  fi
}

# Run Docker container
run_docker_container() {
  log_info "[4/4] Running container \"${CONTAINER_NAME}\" ..."
  docker run --runtime=nvidia \
    -it -d \
    --shm-size=4G \
    --restart always \
    --name "$CONTAINER_NAME" \
    -v "$BASE_DIRECTORY:/${CONTAINER_NAME}" \
    -v "$HOME:/home/$USER" \
    -w "/${CONTAINER_NAME}" \
    -v /etc/localtime:/etc/localtime:ro \
    -v /etc/resolv.conf:/etc/resolv.conf:ro \
    --net host \
    --add-host "$CONTAINER_NAME:127.0.0.1" \
    --hostname "$CONTAINER_NAME" \
    "$DOCKER_IMG" \
    /bin/bash
}

# Start Docker container
start_docker_container() {
  log_info "Checking container \"${CONTAINER_NAME}\" ..."
  if docker ps -a --format "{{.Names}}" | grep -q "${CONTAINER_NAME}"; then
    log_info "Container \"${CONTAINER_NAME}\" exists, starting it ..."
    docker start "${CONTAINER_NAME}" &>/dev/null
    docker exec -it "${CONTAINER_NAME}" /bin/bash
  else
    log_error "Container \"${CONTAINER_NAME}\" does not exist. Please run \"${0} init\" first."
    exit 1
  fi
}

# Main function
main() {
  local cmd="run"
  if [ $# -ge 1 ]; then
    cmd=$1
    shift
  fi

  case "${cmd}" in
    run)
      start_docker_container
      ;;
    init)
      check_docker
      check_docker_image
      check_docker_container
      run_docker_container
      ;;
    help|usage)
      show_usage
      ;;
    *)
      show_usage
      ;;
  esac
}

# Execute the main function
main "$@"
