#!bin/sh

rm -rf bin
mkdir bin
cd bin

proj_name=App
proj_root_dir=$(pwd)/../

flags=(
	-std=c++11 -x -00 -w
)

# Include directories
inc=(
	-I ../third_party/include/			# Gunslinger includes
)

# Source files
src=(
	../source/main.cpp
)

libs=(
	-lopengl32
	-lkernel32 
	-luser32 
	-lshell32 
	-lgdi32 
    -lWinmm
	-lAdvapi32
)

# Build
g++ ${inc[*]} ${src[*]} ${flags[*]} ${libs[*]} -lm -o ${proj_name}

cd ..



