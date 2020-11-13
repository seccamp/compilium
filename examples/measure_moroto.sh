#!/bin/bash -e

CC=clang

echo "building..."
make moroto.bin >/dev/null 2>&1
make moroto.o0.bin >/dev/null 2>&1
make moroto.host.bin moroto.host_o3.bin >/dev/null 2>&1

echo "running moroto.o0.bin (by compilium)..."
( time ./moroto.o0.bin | tee moroto.o0.stdout.txt ) 2>&1 | grep real

echo "running moroto.bin (by compilium)..."
( time ./moroto.bin | tee moroto.stdout.txt ) 2>&1 | grep real

echo "running moroto.host.bin (by host compiler)..."
( time ./moroto.host.bin | tee moroto.host.stdout.txt ) 2>&1 | grep real
echo "running moroto.host_o3.bin (by host compiler with -O3)..."
( time ./moroto.host_o3.bin | tee moroto.host_o3.stdout.txt ) 2>&1 | grep real

diff -u moroto.stdout.txt moroto.host.stdout.txt || { echo "stdout diff"; false; }
diff -u moroto.stdout.txt moroto.o0.stdout.txt || { echo "stdout diff"; false; }
diff -u moroto.stdout.txt moroto.host_o3.stdout.txt || { echo "stdout diff"; false; }
echo "OK"
