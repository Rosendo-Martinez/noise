src := $(wildcard ./src/*)
obj := $(patsubst ./src/%.cpp, ./bin/%.o, $(src))

include := -I./include
program := main.exe


# Build commands -------------------------------------

all : ./bin/$(program)

./bin/$(program) : $(obj)
	g++ -o ./bin/$(program) $(obj)

./bin/%.o : ./src/%.cpp
	g++ -c $^ -o $@ $(include)


# Utility commands --------------------------------------

print :
	echo ${src}
	echo ${include}
	echo ${program}
	echo ${obj}

clean :
	rm -f ./bin/*

run : all
	./bin/$(program)