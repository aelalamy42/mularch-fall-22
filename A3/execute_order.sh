#!/bin/bash
#SBATCH --chdir /scratch/balducci/A3
#SBATCH --nodes 1
#SBATCH --ntasks 1
#SBATCH --cpus-per-task 36
#SBATCH --mem 10G


echo STARTING AT `date`

numactl --physcpubind=+0-1,18 ./order
numactl --physcpubind=+0-2 ./order

echo FINISHED at `date`
