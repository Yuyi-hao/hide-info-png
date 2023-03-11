#!/bin/sh -e

set -xe 
CC=cc
CFLAGS="-Wall -Wextra"

$CC $CFLAGS -o png png.c 
./png test1.png test2.png
