#!/bin/bash -e

CC=clang

echo "building..."
make moroto2.bin >/dev/null 2>&1
make moroto2.o0.bin >/dev/null 2>&1
make moroto2.host.bin moroto2.host_o3.bin >/dev/null 2>&1

../compilium  -I ../include/ -E  < moroto2.c | tee /dev/stderr | wc

echo "running moroto2.o0.bin (by compilium)..."
( time ./moroto2.o0.bin | tee moroto2.o0.stdout.txt ) 2>&1 | grep real

echo "running moroto2.bin (by compilium)..."
( time ./moroto2.bin | tee moroto2.stdout.txt ) 2>&1 | grep real

echo "running moroto2.host.bin (by host compiler)..."
( time ./moroto2.host.bin | tee moroto2.host.stdout.txt ) 2>&1 | grep real

echo "running moroto2.host_o3.bin (by host compiler with -O3)..."
( time ./moroto2.host_o3.bin | tee moroto2.host_o3.stdout.txt ) 2>&1 | grep real

# diff -u moroto2.stdout.txt moroto2.host.stdout.txt || { echo "stdout diff"; false; }
diff -u moroto2.stdout.txt moroto2.o0.stdout.txt || { echo "stdout diff"; false; }
diff -u moroto2.stdout.txt moroto2.host_o3.stdout.txt || { echo "stdout diff"; false; }
echo "OK"
