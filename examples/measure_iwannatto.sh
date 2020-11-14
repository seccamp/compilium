#!/bin/bash -e

CC=clang

echo "building..."
make kadai2.bin >/dev/null 2>&1
make kadai2.o0.bin >/dev/null 2>&1
make kadai2.host.bin kadai2.host_o3.bin >/dev/null 2>&1

echo "running kadai2.o0.bin (by compilium)..."
( time ./kadai2.o0.bin | tee kadai2.o0.stdout.txt ) 2>&1 | grep real

echo "running kadai2.bin (by compilium)..."
( time ./kadai2.bin | tee kadai2.stdout.txt ) 2>&1 | grep real

echo "running kadai2.host.bin (by host compiler)..."
( time ./kadai2.host.bin | tee kadai2.host.stdout.txt ) 2>&1 | grep real
echo "running kadai2.host_o3.bin (by host compiler with -O3)..."
( time ./kadai2.host_o3.bin | tee kadai2.host_o3.stdout.txt ) 2>&1 | grep real

diff -u kadai2.stdout.txt kadai2.host.stdout.txt || { echo "stdout diff"; false; }
diff -u kadai2.stdout.txt kadai2.o0.stdout.txt || { echo "stdout diff"; false; }
diff -u kadai2.stdout.txt kadai2.host_o3.stdout.txt || { echo "stdout diff"; false; }
echo "OK"

