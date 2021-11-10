#! /bin/bash
#
# Copyright 2021 Alexander Grund
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
#      http://www.boost.org/LICENSE_1_0.txt)
#
# Executes the install phase for GHA

set -ex

export SELF="${GITHUB_REPOSITORY#*/}"
export BOOST_CI_TARGET_BRANCH="${GITHUB_BASE_REF:-$GITHUB_REF}"
export BOOST_CI_SRC_FOLDER="$GITHUB_WORKSPACE"

echo "SELF=$SELF" >> $GITHUB_ENV
echo "BOOST_CI_TARGET_BRANCH=$BOOST_CI_TARGET_BRANCH" >> $GITHUB_ENV
echo "BOOST_CI_SRC_FOLDER=BOOST_CI_SRC_FOLDER" >> $GITHUB_ENV

if [[ "$B2_TOOLSET" == clang-* ]]; then
  llvmPath="/usr/lib/llvm-${B2_TOOLSET#*-}/bin"
  echo "$llvmPath" >> $GITHUB_PATH
fi

. ci/common_install.sh
echo "BOOST_ROOT=$BOOST_ROOT" >> $GITHUB_ENV

$B2_TOOLSET --version
echo "B2_TOOLSET=$B2_TOOLSET" >> $GITHUB_ENV
echo "B2_CXXSTD=$B2_CXXSTD" >> $GITHUB_ENV
echo "B2_LINKFLAGS=$B2_LINKFLAGS" >> $GITHUB_ENV
if [[ "$B2_SANITIZE" == "yes" ]]; then
  echo "B2_ASAN=1" >> $GITHUB_ENV
  echo "B2_UBSAN=1" >> $GITHUB_ENV
  if [[ -f $BOOST_CI_SRC_FOLDER/ubsan-blacklist ]]; then
    echo "B2_CXXFLAGS=-fsanitize-blacklist=libs/$SELF/ubsan-blacklist" >> $GITHUB_ENV
  fi
fi

