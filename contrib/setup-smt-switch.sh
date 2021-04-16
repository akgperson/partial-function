#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
DEPS=$DIR/../deps

usage () {
    cat <<EOF
Usage: $0 [<option> ...]

Sets up the smt-switch API for interfacing with SMT solvers through a C++ API.

-h, --help              display this message and exit
--debug                 build a debug version
EOF
    exit 0
}

while [ $# -gt 0 ]
do
    case $1 in
        -h|--help) usage;;
        --debug)
            CONF_OPTS=--debug;;
        *) die "unexpected argument: $1";;
    esac
    shift
done

mkdir -p $DEPS

if [ ! -d "$DEPS/smt-switch" ]; then
    cd $DEPS
    git clone https://github.com/makaimann/smt-switch
    cd smt-switch
    git checkout -f 970f5aaa9f262f30f26f85f2de9364be33483d7b
    ./travis-scripts/download-cvc4.sh
    ./configure.sh --cvc4 --msat --msat-home=../mathsat --prefix=local --static $CONF_OPTS
    cd build
    make -j$(nproc)
    make test
    make install
    cd $DIR
else
    echo "$DEPS/smt-switch already exists. If you want to rebuild, please remove it manually."
fi

if [ -f $DEPS/smt-switch/local/lib/libsmt-switch.a ] && [ -f $DEPS/smt-switch/local/lib/libsmt-switch-msat.a ] && [ -f $DEPS/smt-switch/local/lib/libsmt-switch-cvc4.a ]; then \
    echo "It appears smt-switch with MathSAT and CVC4 was successfully installed to $DEPS/smt-switch/local."
else
    echo "Building smt-switch failed."
    echo "You might be missing some dependencies."
    echo "Please see the github page for installation instructions: https://github.com/makaimann/smt-switch"
    exit 1
fi
