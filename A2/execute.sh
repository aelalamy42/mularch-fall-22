#!/bin/bash
#SBATCH --chdir /home/aelalamy/mularch-fall-22/A2 
#SBATCH --nodes 1
#SBATCH --ntasks 1
#SBATCH --cpus-per-task 36
#SBATCH --mem 2G


echo STARTING AT `date`

./heatmap 1 10000 100 output1.csv
./heatmap 2 10000 100 output2.csv
./heatmap 4 10000 100 output4.csv
./heatmap 8 10000 100 output8.csv
./heatmap 16 10000 100 output16.csv

echo FINISHED at `date`
