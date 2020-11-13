#!/bin/bash -e

CC=clang

echo "building..."
make optimizer_benchmark_masumoto.bin >/dev/null 2>&1
make optimizer_benchmark_masumoto.o0.bin >/dev/null 2>&1
make optimizer_benchmark_masumoto.host.bin optimizer_benchmark_masumoto.host_o3.bin >/dev/null 2>&1

echo "running optimizer_benchmark_masumoto.o0.bin (by compilium)..."
( time ./optimizer_benchmark_masumoto.o0.bin | tee optimizer_benchmark_masumoto.o0.stdout.txt ) 2>&1 | grep real

echo "running optimizer_benchmark_masumoto.bin (by compilium)..."
( time ./optimizer_benchmark_masumoto.bin | tee optimizer_benchmark_masumoto.stdout.txt ) 2>&1 | grep real

echo "running optimizer_benchmark_masumoto.host.bin (by host compiler)..."
( time ./optimizer_benchmark_masumoto.host.bin | tee optimizer_benchmark_masumoto.host.stdout.txt ) 2>&1 | grep real
echo "running optimizer_benchmark_masumoto.host_o3.bin (by host compiler with -O3)..."
( time ./optimizer_benchmark_masumoto.host_o3.bin | tee optimizer_benchmark_masumoto.host_o3.stdout.txt ) 2>&1 | grep real

diff -u optimizer_benchmark_masumoto.stdout.txt optimizer_benchmark_masumoto.host.stdout.txt || { echo "stdout diff"; false; }
diff -u optimizer_benchmark_masumoto.stdout.txt optimizer_benchmark_masumoto.o0.stdout.txt || { echo "stdout diff"; false; }
diff -u optimizer_benchmark_masumoto.stdout.txt optimizer_benchmark_masumoto.host_o3.stdout.txt || { echo "stdout diff"; false; }
echo "OK"
