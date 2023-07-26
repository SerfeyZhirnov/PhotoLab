all: clean build

build:
	cmake -S . -B build
	cmake --build build

install: build
	cp -rf ./build/PhotoLab.app $(HOME)/Applications/

uninstall:
	rm -rf $(HOME)/Applications/PhotoLab.app

clean:
	find ./ -name "build" -type d -exec rm -rf {} +

dist: clean
	tar -czf PhotoLab.tgz ./*

tests: clean
	cmake -S ./tests -B ./tests/build -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gcc
	cmake --build ./tests/build
	@cd tests/build/; ./PhotoLab_test

coverage: clean
	cmake -S ./tests -B ./tests/build -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gcc
	cmake --build ./tests/build
	@cd tests/build/; make test_coverage
	@cd tests/build/; open ./test_coverage/index.html

style: build
	@cd build/; make clang-format

cppcheck: build
	@cd build/; make cppcheck
