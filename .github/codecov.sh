#! /bin/bash
#
# Copyright 2017 - 2019 James E. King III
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
#      http://www.boost.org/LICENSE_1_0.txt)
#
# Bash script to run on Github Actions to perform codecov.io integration
#

# assumes an environment variable $LIBRARY set to the boost project name
# and BOOST_ROOT to be set

set -eux

if [[ "$1" == "setup" ]]; then
    echo "B2_VARIANT=debug" >> "$GITHUB_ENV"
    echo "B2_FLAGS=cxxflags=--coverage linkflags=--coverage" >> "$GITHUB_ENV"
else
    ver=7 # default
    if [ "${B2_COMPILER%%-*}" == "g++" ]; then
        if [[ "$B2_COMPILER" =~ g\+\+- ]]; then
            ver="${B2_COMPILER##*g++-}"
        fi
    fi
    GCOV=gcov-${ver}

    # install the latest lcov we know works
    rm -rf /tmp/lcov
    pushd /tmp
    git clone -b v1.14 https://github.com/linux-test-project/lcov.git
    export PATH=/tmp/lcov/bin:$PATH
    command -v lcov
    lcov --version
    popd

    # switch back to the original source code directory
    cd "$GITHUB_WORKSPACE"
    : "${LCOV_BRANCH_COVERAGE:=1}" # Set default

    lcov --gcov-tool="$GCOV" --rc lcov_branch_coverage=${LCOV_BRANCH_COVERAGE} --base-directory "$BOOST_ROOT/libs/$LIBRARY" --directory "$BOOST_ROOT" --capture --output-file all.info
    # dump a summary on the console
    lcov --gcov-tool="$GCOV" --rc lcov_branch_coverage=${LCOV_BRANCH_COVERAGE} --list all.info

    # all.info contains all the coverage info for all projects - limit to ours
    # first we extract the interesting headers for our project then we use that list to extract the right things
    for f in $(for f2 in include/boost/*; do echo "$f2"; done | cut -f2- -d/); do echo "*/$f*"; done > /tmp/interesting
    echo headers that matter:
    cat /tmp/interesting
    xargs -L 999999 -a /tmp/interesting lcov --gcov-tool="$GCOV" --rc lcov_branch_coverage=${LCOV_BRANCH_COVERAGE} --extract all.info {} "*/libs/$LIBRARY/*" --output-file coverage.info

    # dump a summary on the console - helps us identify problems in pathing
    lcov --gcov-tool="$GCOV" --rc lcov_branch_coverage=${LCOV_BRANCH_COVERAGE} --list coverage.info

    # upload to codecov.io
    curl -s https://codecov.io/bash > .codecov
    chmod +x .codecov
    ./.codecov -f coverage.info -X gcov -x "$GCOV"
fi
