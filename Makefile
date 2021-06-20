ROOT:=$(shell pwd)
PROJECTS_DIR:=projects
# THREADS:=$(shell nproc)
THREADS=4
BUILD_DIR=build
D=.done

.SILENT:
.PHONY: deps build run clean clean_game copy_test_game include_graph

run: engine
	./engine games/game0

deps: deps$(D)

deps$(D):
	./scripts/deps.sh
	touch deps$(D)

build: engine

$(BUILD_DIR)$(D): deps$(D)
	mkdir -p $(BUILD_DIR)
	
	cd build && cmake ..
	# cmake -S . -B $(BUILD_DIR)
	
	touch $(BUILD_DIR)$(D)

engine: $(BUILD_DIR)$(D)
	echo "Building using ${THREADS} parallel jobs";
	
	cd build && make engine -j$(THREADS)
	# cmake -B $(BUILD_DIR) --target engine -- -j$(THREADS)

clean:
	rm -rf $(BUILD_DIR) built $(wildcard *$(D)) lib
	rm -f engine engine_test CMakeCache.txt

clean_game:
	find game0 -iname "*.meta" -o -iname "*.scene" -o -iname "*.prgm" -o -iname "*.json" -delete

copy_test_game:
	rm -rf ${PROJECTS_DIR}/game1
	cp -r ${PROJECTS_DIR}/game0 ${PROJECTS_DIR}/game1

include_graph:
	./scripts/include_graph.sh

