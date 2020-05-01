#!/bin/bash

set -e

ZIP=$1
DIR=$2

unzip $ZIP -d $DIR
cp MakefileTemplate $DIR/Makefile
cd $DIR
mv src/* ./
mv include/KHR/* ./
mv include/glad/* ./
rm -r src include
# We're now left with the files
sed -i 's#<glad/glad.h>#"glad.h"#g' glad.c
sed -i 's#<KHR/khrplatform.h>#"khrplatform.h"#g' glad.h
cd ..