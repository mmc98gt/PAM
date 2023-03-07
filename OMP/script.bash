#!/bin/bash

for tam in 128 256 512 1024
do
    for fil in 1 2 4 8
    do
        for hil in 1 2 4 8 16
        do
            echo -n "tamaño: $tam, filas: $fil, hilos: $hil, "
            ./MulMatCua $tam $fil $hil | grep -o 'segundos: [0-9]*\.[0-9]*'
        done
    done
done
