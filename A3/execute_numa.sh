#!/bin/bash
#SBATCH --chdir /home/aelalamy/A3
#SBATCH --nodes 1
#SBATCH --ntasks 1
#SBATCH --cpus-per-task 28
#SBATCH --mem 10G


echo STARTING AT `date`

numactl --physcpubind=+0-13 ./numa
numactl --physcpubind=+14-27 ./numa
numactl --physcpubind=+7-20 ./numa


echo FINISHED at `date`
