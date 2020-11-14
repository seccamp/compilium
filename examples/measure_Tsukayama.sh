#!/bin/bash -e
echo "building..."
make tsukayama.bin >/dev/null 2>&1
make tsukayama.host.bin tsukayama.host_o3.bin >/dev/null 2>&1
echo "running (by compilium)..."
( time ./tsukayama.bin ) 2>&1 | grep real
echo "running (by host compiler)..."
( time ./tsukayama.host.bin ) 2>&1 | grep real
echo "running (by host compiler with -O3)..."
( time ./tsukayama.host_o3.bin ) 2>&1 | grep real
