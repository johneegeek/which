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
	cmake -G Ninja -B build-lint
	-clang-tidy -p build-lint src/*.cpp 
	-cppcheck --enable=all --suppress=missingIncludeSystem --suppress=*:cxxopts* -I src/include src/*.cpp
	-lizard --modified --exclude "*/cxxopts.hpp" src

.PHONY: clean
clean:
	-@rm -rf build
	-@rm -rf build-vscode
	-@rm -rf build-lint
	-@rm -f which.exe