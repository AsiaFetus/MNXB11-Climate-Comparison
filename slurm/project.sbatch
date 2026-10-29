#!/bin/sh

# This SBATCH example assumes you have a script that 
# takes care of preparing the environment for the 
# job. In this case I am reusing the runproject.sh
# script inside ../scripts

# Submit this job from the repository root with:
# sbatch --chdir="$PWD" slurm/project.sbatch

#SBATCH -J "MNXB11 Project Team 1"
#SBATCH --time=00:05:00
#SBATCH -A hep2023-1-6
#SBATCH --mem 1G

scripts/runproject.sh
