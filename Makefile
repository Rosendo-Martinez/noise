src := $(wildcard ./src/*)
obj := $(patsubst ./src/%.cpp, ./bin/%.o, $(src))

link := -lglfw -ldl
include := -I./include -I./glad/include
program := main.exe


# Build commands -------------------------------------

all : ./bin/$(program)

./bin/$(program) : $(obj) ./bin/glad.o
	g++ -o ./bin/$(program) $(obj) ./bin/glad.o $(link)

./bin/%.o : ./src/%.cpp
	g++ -c $^ -o $@ $(include)

./bin/glad.o : ./glad/src/glad.c
	g++ -c $^ -o $@ -I./glad/include


# Utility commands --------------------------------------

print :
	echo ${src}
	echo ${include}
	echo ${program}
	echo ${obj}

clean :
	rm -f $(obj) ./bin/$(program)

run : all
	./bin/$(program)