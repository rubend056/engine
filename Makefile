ROOT:=$(shell pwd)
PROJECTS_DIR:=projects
THREADS:=$(shell nproc || echo 4)
BUILD_DIR:=build
GAMES_DIR:=games
D:=.done

.SILENT:
.PHONY: deps configure engine build run clean clean_game copy_test_game include_graph

start: engine
	./engine games/game0
test: engine_test
	./engine_test

deps: deps$(D)

deps$(D):
	./scripts/deps.sh
	touch deps$(D)

build: engine

configure: deps$(D)
	mkdir -p $(BUILD_DIR)
	
	cd $(BUILD_DIR) && cmake ..
	# cmake -S . -B $(BUILD_DIR)

# Cache for configure, if $(BUILD_DIR)$(D) is used as dependency then it won't run if $(BUILD_DIR)$(D) has been made
$(BUILD_DIR)$(D): configure
	touch $(BUILD_DIR)$(D)	

engine: $(BUILD_DIR)$(D)
	echo "Building using ${THREADS} parallel jobs";
	
	cd build && make engine -j$(THREADS)
	# cmake -B $(BUILD_DIR) --target engine -- -j$(THREADS)

engine_test: $(BUILD_DIR)$(D)
	echo "Building using ${THREADS} parallel jobs";
	cd build && make engine_test -j$(THREADS)

clean:
	rm -rf $(BUILD_DIR) built $(wildcard *$(D)) lib
	rm -f engine engine_test CMakeCache.txt

clean_game:
	# find . \( -type f -name '&*' -or -type f -name '$*$' \) -exec rm -v {} \;
	# find games -iname "*.meta" -o -iname "*.scene" -o -iname "*.prgm" -o -iname "*.json" -delete
	find ${GAMES_DIR} -iname "*.meta" -delete
	find ${GAMES_DIR} -iname "*.scene" -delete
	find ${GAMES_DIR} -iname "*.prgm" -delete
	find ${GAMES_DIR} -iname "*.json" -delete

copy_test_game:
	rm -rf ${PROJECTS_DIR}/game1
	cp -r ${PROJECTS_DIR}/game0 ${PROJECTS_DIR}/game1

include_graph:
	./scripts/include_graph.sh

