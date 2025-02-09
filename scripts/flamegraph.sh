#!/bin/bash

FLAMEGRAPH_REPO="$1"

PERF_DATA=$(mktemp)
OUT_PERF=$(mktemp)
SVG="$2"

perf record -F 99 -g -o $PERF_DATA -- ${@:3}

chmod a+rw $PERF_DATA

perf script -i $PERF_DATA | "$FLAMEGRAPH_REPO/stackcollapse-perf.pl" > $OUT_PERF
"$FLAMEGRAPH_REPO/flamegraph.pl" $OUT_PERF > $SVG

chmod a+rw $SVG
echo "Flamegraph saved to $SVG"
