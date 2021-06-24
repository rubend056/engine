#!/bin/bash
# set -e


tname=${tname:-"template"}
cname=${cname:-${name}}
TEMPLATES_DIR=${TEMPLATES_DIR:-"src/libs/templates"}
DEST_DIR=${DEST_DIR:-"src/libs"}

if [ -z "${name}" ]
then
	echo "Define 'name' variable, ex:   name=test will create test.h and test.cpp"
	echo "You can define 'name' (output file name), 'tname' (template name), and 'cname' (class name)"
else
	sed "s@class_name@${cname}@g" "${TEMPLATES_DIR}/${tname}.h" > "${DEST_DIR}/include/${name}.h"
	# sed "s@template@$1@g; s@parent@$2@g" "${TEMPLATES_DIR}/template.cpp" > "${DEST_DIR}/src/$2$1.cpp"
	sed "s@file_name@${name}@g; s@class_name@${cname}@g; s#path#${DEST_DIR}/include/#g" "${TEMPLATES_DIR}/${tname}.cpp" > "${DEST_DIR}/src/${name}.cpp"
fi