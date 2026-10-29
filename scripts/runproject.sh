#!/bin/bash

# This BASH script can be used to create the environment
# needed for the project to run

# Find and change directory to git base folder
# This assumes the project is run inside a git folder
# One can remove this if no git is present and fix
# paths manually
GITROOT=$(git rev-parse --show-toplevel)

if [ -d $GITROOT ]; then
  cd $GITROOT
else
  echo "not a git repository, exiting..."
  exit 1
fi

# Uncomment one of these lines to choose what to execute. Default is the 'main' binary file.
EXECUTABLE="./main"
#EXECUTABLE="root -q rootlogon.C"

# Execute main binary
if [[ -e $EXECUTABLE ]]; then
  apptainer run \
    -B /run/user:/run/user \
    -B /projects:/projects \
    /projects/hep/fs11/share/mnxb11/containers/mnxb11_al9.latest.sif \
    "$EXECUTABLE"
else
  echo "ERROR main executable not found."
  echo "Current path: $PWD"
  echo "Do you want to run the project template default main? You must run make or ./buildproject.sh first."
  echo "Did you compile the executable using g++?"
  echo "Did you want to run a ROOT script? Where is it located?"
  echo "Did you forget to start the container? Are you running in the container?"
  exit 1
fi

