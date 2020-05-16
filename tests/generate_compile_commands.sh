#!/bin/bash
cd "$(dirname "$0")" || exit
bear -o o3/compile_commands.json cc -c o3/{success,failure}.c
rm {success,failure}.o
