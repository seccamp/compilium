#!/bin/bash -e
echo "building..."
make tajima_optimizer_benchmark.bin >/dev/null 2>&1
make tajima_optimizer_benchmark.host.bin tajima_optimizer_benchmark.host_o3.bin >/dev/null 2>&1
echo "running tajima_optimizer_benchmark.bin (by compilium)..."
( time ./tajima_optimizer_benchmark.bin ) 2>&1 | grep real
echo "running tajima_optimizer_benchmark.host.bin (by host compiler)..."
( time ./tajima_optimizer_benchmark.host.bin ) 2>&1 | grep real
echo "running tajima_optimizer_benchmark.host_o3.bin (by host compiler with -O3)..."
( time ./tajima_optimizer_benchmark.host_o3.bin ) 2>&1 | grep real
