#! /bin/bash

ARGS="--env COMMIT=$COMMIT --env PKG_CONFIG_SYSROOT_DIR=/usr/arm-linux-gnueabihf/ -it -v "$PWD":/usr/src/app -w /usr/src/app"
if [[ -f $ENV_FILE ]]; then
    ARGS="$ARGS --env-file $ENV_FILE"
fi

#if $COVERAGE ; then
#    ARGS="$ARGS --env-file ./docker/coverage.env --env RUSTFLAGS=-Cinstrument-coverage"
#fi

docker run $ARGS ghcr.io/berlingoqc/quintal/crossbuild "$@"