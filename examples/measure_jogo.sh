#!/bin/bash -e
echo "building..."
make jogo.bin >/dev/null 2>&1
make jogo.o0.bin >/dev/null 2>&1
make jogo.host.bin jogo.host_o3.bin >/dev/null 2>&1
echo "running jogo.bin (by compilium)..."
( time ./jogo.bin | tee jogo.stdout.txt ) 2>&1 | grep real
echo "running jogo.o0.bin (by compilium)..."
( time ./jogo.o0.bin | tee jogo.o0.stdout.txt ) 2>&1 | grep real
echo "running jogo.host.bin (by host compiler)..."
( time ./jogo.host.bin | tee jogo.host.stdout.txt ) 2>&1 | grep real
echo "running jogo.host_o3.bin (by host compiler with -O3)..."
( time ./jogo.host_o3.bin | tee jogo.host_o3.stdout.txt ) 2>&1 | grep real

diff -u jogo.stdout.txt jogo.o0.stdout.txt || { echo "stdout diff"; false; }
diff -u jogo.stdout.txt jogo.host.stdout.txt || { echo "stdout diff"; false; }
diff -u jogo.stdout.txt jogo.host_o3.stdout.txt || { echo "stdout diff"; false; }
echo "OK"


