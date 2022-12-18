#!/bin/sh
# This script is used to set up the CMU devcontainer environment
git submodule add git@github.com:paprikaw/my-devcontainers.git my-devcontainers

ln -s my-devcontainers/NJU-OS .devcontainer
