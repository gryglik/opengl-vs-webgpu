all: build

run-opengl: build
	@echo [!] Uruchamianie programu
	./build/app opengl

run-webgpu: build
	@echo [!] Uruchamianie programu
	./build/app webgpu

build-cmake:
	@echo [!] Budowanie projektu CMAKE
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++

build: build-cmake
	@echo [!] Budowanie projektu MAKE
	cmake --build build

clean:
	@echo [!] Czyszczenie katalogu budowania
	rm -rf build
