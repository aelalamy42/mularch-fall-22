#!/bin/bash
#SBATCH --chdir /scratch/balducci/A2
#SBATCH --nodes 1
#SBATCH --ntasks 1
#SBATCH --cpus-per-task 36
#SBATCH --mem 2G


echo STARTING AT `date`

./sharing 1 200000000 32
./sharing 2 200000000 32
./sharing 4 200000000 32
./sharing 8 200000000 32

./heatmap 1 10000 100 output1.csv
./heatmap 2 10000 100 output2.csv
./heatmap 4 10000 100 output4.csv
./heatmap 8 10000 100 output8.csv
./heatmap 16 10000 100 output16.csv

echo FINISHED at `date`
