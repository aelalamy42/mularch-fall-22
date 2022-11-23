#!/bin/bash
#SBATCH --chdir /home/aelalamy/A3
#SBATCH --nodes 1
#SBATCH --ntasks 1
#SBATCH --cpus-per-task 28
#SBATCH --mem 10G


echo STARTING AT `date`

echo --  LOCAL MODE --
numactl --localalloc ./numa
echo --  REMOTE MODE --
numactl --membind=0 --cpunodebind=1 ./numa
echo --  INTERLEAVE MODE --
numactl --interleave=all ./numa


echo FINISHED at `date`
