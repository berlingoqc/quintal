#! /bin/bash

ARGS="--env COMMIT=$COMMIT -it -v "$PWD":/usr/src/app/device -w /usr/src/app/device"
if [[ -f $ENV_FILE ]]; then
    ARGS="$ARGS --env-file $ENV_FILE"
fi

#if $COVERAGE ; then
#    ARGS="$ARGS --env-file ./docker/coverage.env --env RUSTFLAGS=-Cinstrument-coverage"
#fi

docker run $ARGS pibuild "$@"