#!/bin/bash

# Tamaño de matriz
N=4096

# Tamaños de bloque
block_sizes=(4 16 32)

# Números de hilos
num_threads=(4 8 16)

for bs in "${block_sizes[@]}"
do
    for nt in "${num_threads[@]}"
    do
        export OMP_NUM_THREADS=$nt
        echo "Ejecutando con tamaño de bloque $bs y $nt hilos"
        ./SecMulMatCuaBloP $N $bs $nt
    done
done
