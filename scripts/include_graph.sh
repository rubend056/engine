#!/bin/bash
set -e

P=`pwd`

./cinclude2dot --include `cat ${P}/misc/cinclude2dot_include.txt | tr "\n" ","` --quotetypes quote --merge file --exclude cereal --src libs > source.dot
dot -Tpdf source.dot -o source.pdf