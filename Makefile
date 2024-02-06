.PHONY: configure
configure:
	cmake -G "Ninja" -B build

.PHONY: build
build:
	ninja -C build

.PHONY: test
test:
	ctest --test-dir build --output-on-failure

.PHONY: check
check:
	-clang-tidy -p build src/*
	-cppcheck --enable=all src

.PHONY: clean
clean:
	-@rm -rf build
	-@rm -rf build-vscode