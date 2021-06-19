#!/bin/bash
set -e


tname=${tname:-"template"}
cname=${cname:-${name}}
TEMPLATES_DIR=${TEMPLATES_DIR:-"libs/templates"}
DEST_DIR=${DEST_DIR:-"libs"}

if [ -z "${name}" ]
then
	echo "Define 'name' variable, ex:   name=test will create test.h and test.cpp"
	echo "You can define 'name' (output file name), 'tname' (template name), and 'cname' (class name)"
elif
	sed "s@template@${cname}@g" "${TEMPLATES_DIR}/${tname}.h" > "${DEST_DIR}/include/${name}.h"
	# sed "s@template@$1@g; s@parent@$2@g" "${TEMPLATES_DIR}/template.cpp" > "${DEST_DIR}/src/$2$1.cpp"
	sed "s@includename@${name}@g" "${TEMPLATES_DIR}/${tname}.cpp" > "${DEST_DIR}/src/${name}.cpp"
fi
