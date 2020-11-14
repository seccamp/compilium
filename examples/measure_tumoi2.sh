#!/bin/bash -e

CC=clang

echo "building..."
make tumoi2.bin >/dev/null 2>&1
make tumoi2.o0.bin >/dev/null 2>&1
make tumoi2.host.bin tumoi2.host_o3.bin >/dev/null 2>&1

../compilium  -I ../include/ -E  < tumoi2.c | tee /dev/stderr | wc

echo "running tumoi2.o0.bin (by compilium)..."
( time ./tumoi2.o0.bin | tee tumoi2.o0.stdout.txt ) 2>&1 | grep real

echo "running tumoi2.bin (by compilium)..."
( time ./tumoi2.bin | tee tumoi2.stdout.txt ) 2>&1 | grep real

echo "running tumoi2.host.bin (by host compiler)..."
( time ./tumoi2.host.bin | tee tumoi2.host.stdout.txt ) 2>&1 | grep real

echo "running tumoi2.host_o3.bin (by host compiler with -O3)..."
( time ./tumoi2.host_o3.bin | tee tumoi2.host_o3.stdout.txt ) 2>&1 | grep real

diff -u tumoi2.stdout.txt tumoi2.host.stdout.txt || { echo "stdout diff"; false; }
diff -u tumoi2.stdout.txt tumoi2.o0.stdout.txt || { echo "stdout diff"; false; }
diff -u tumoi2.stdout.txt tumoi2.host_o3.stdout.txt || { echo "stdout diff"; false; }
echo "OK"
