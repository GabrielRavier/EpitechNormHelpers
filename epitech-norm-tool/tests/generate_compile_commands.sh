#!/bin/bash
cd "$(dirname "$0")" || exit
bear -o data/o3/compile_commands.json cc -c data/o3/{success,failure}.c
bear -o data/f2/compile_commands.json cc -c data/f2/{success,failure}.c
rm {success,failure}.o
