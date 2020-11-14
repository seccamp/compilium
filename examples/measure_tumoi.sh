#!/bin/bash -e

CC=clang

echo "building..."
make tumoi.bin >/dev/null 2>&1
make tumoi.o0.bin >/dev/null 2>&1
make tumoi.host.bin tumoi.host_o3.bin >/dev/null 2>&1

echo "running tumoi.o0.bin (by compilium)..."
( time ./tumoi.o0.bin | tee tumoi.o0.stdout.txt ) 2>&1 | grep real

echo "running tumoi.bin (by compilium)..."
( time ./tumoi.bin | tee tumoi.stdout.txt ) 2>&1 | grep real

echo "running tumoi.host.bin (by host compiler)..."
( time ./tumoi.host.bin | tee tumoi.host.stdout.txt ) 2>&1 | grep real
echo "running tumoi.host_o3.bin (by host compiler with -O3)..."
( time ./tumoi.host_o3.bin | tee tumoi.host_o3.stdout.txt ) 2>&1 | grep real

diff -u tumoi.stdout.txt tumoi.host.stdout.txt || { echo "stdout diff"; false; }
diff -u tumoi.stdout.txt tumoi.o0.stdout.txt || { echo "stdout diff"; false; }
diff -u tumoi.stdout.txt tumoi.host_o3.stdout.txt || { echo "stdout diff"; false; }
echo "OK"
