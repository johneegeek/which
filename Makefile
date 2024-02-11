.PHONY: buildall
buildall:
	@cmake -G "Ninja Multi-Config" -B build
	@cmake --build build 

.PHONY: build
build:
	ninja -C build

.PHONY: configure
configure:
	cmake -G "Ninja Multi-Config" -B build 

.PHONY: release
release:
	@cmake -G "Ninja Multi-Config" -B build -DVERSION_INCLUDE_BUILDID=OFF --fresh
	@cmake --build build --config Release

.PHONY: test
test:
	ctest --test-dir build --output-on-failure

.PHONY: check
check:
	-clang-tidy -p build src/*.cpp
	-cppcheck --enable=all --suppress=missingIncludeSystem -I src/include src/*.cpp

.PHONY: clean
clean:
	-@rm -rf build
	-@rm -rf build-vscode
	-@rm -f which.exe