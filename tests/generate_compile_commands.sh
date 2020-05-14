#!/bin/bash
cd "$(dirname "$0")"
bear -o o3/compile_commands.json cc o3/{not-,}success.c 
