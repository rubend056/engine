#!/bin/bash

set -x 
./cinclude2dot --include `cat cinclude2dot_include.txt | tr "\n" ","` --quotetypes quote --merge file --exclude cereal --src libs > source.dot
dot -Tpdf source.dot -o source.pdf
