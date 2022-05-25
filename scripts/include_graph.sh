#!/bin/bash
set e

P=`pwd`
DOT_DIR="${P}/misc/graph"
DEST_DIR="${DOT_DIR}"

${DOT_DIR}/cinclude2dot --include `cat ${DOT_DIR}/cinclude2dot_include.txt | tr "\n" ","`  --quotetypes quote --merge file --exclude cereal --src src > ${DEST_DIR}/source.dot
neato -Tpdf -x ${DEST_DIR}/source.dot -o ${DEST_DIR}/source.pdf