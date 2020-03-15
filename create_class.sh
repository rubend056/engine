#!/bin/bash

set -x
#  class_name  parent_directory/

DEST_DIR="libs"

STUFF=""

sed "s/template/$1/g" "templates/template.h" > "$DEST_DIR/include/$2$1.h"
sed "s@template@$1@g; s@parent@$2@g" "templates/template.cpp" > "$DEST_DIR/src/$2$1.cpp"
