all: clean build

build:
	cmake -S . -B build
	cmake --build build

style: build
	@cd build/; make clang-format

cppcheck: build
	@cd build/; make cppcheck

clean:
	find ./ -name "build" -type d -exec rm -rf {} +
