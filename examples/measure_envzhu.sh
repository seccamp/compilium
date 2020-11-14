#!/bin/bash -e

CC=clang

echo "building..."
make envzhu.bin >/dev/null 2>&1
make envzhu.o0.bin >/dev/null 2>&1
make envzhu.host.bin envzhu.host_o3.bin >/dev/null 2>&1

echo "running envzhu.o0.bin (by compilium)..."
( time ./envzhu.o0.bin | tee envzhu.o0.stdout.txt ) 2>&1 | grep real

echo "running envzhu.bin (by compilium)..."
( time ./envzhu.bin | tee envzhu.stdout.txt ) 2>&1 | grep real

echo "running envzhu.host.bin (by host compiler)..."
( time ./envzhu.host.bin | tee envzhu.host.stdout.txt ) 2>&1 | grep real
echo "running envzhu.host_o3.bin (by host compiler with -O3)..."
( time ./envzhu.host_o3.bin | tee envzhu.host_o3.stdout.txt ) 2>&1 | grep real

diff -u envzhu.stdout.txt envzhu.host.stdout.txt || { echo "stdout diff"; false; }
diff -u envzhu.stdout.txt envzhu.o0.stdout.txt || { echo "stdout diff"; false; }
diff -u envzhu.stdout.txt envzhu.host_o3.stdout.txt || { echo "stdout diff"; false; }
echo "OK"
