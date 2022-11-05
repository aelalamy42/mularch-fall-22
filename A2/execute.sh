#!/bin/bash
#SBATCH --chdir /scratch/balducci/A2
#SBATCH --nodes 1
#SBATCH --ntasks 1
#SBATCH --cpus-per-task 16
#SBATCH --mem 2G


echo STARTING AT `date`

./sharing 1 200000000 32
./sharing 2 200000000 32
./sharing 4 200000000 32
./sharing 8 200000000 32

# ./heatmap 1 10000 100 output.csv

echo FINISHED at `date`
