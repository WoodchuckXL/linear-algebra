CPP = /usr/bin/clang++

# Compile flags
CFLAGS = -g -Wall -Wextra -Werror -Wfatal-errors -pedantic $(IFLAGS)

# Linking flags
LDFLAGS = -g

# Libraries needed for linking
LDLIBS = 

INCLUDES = $(shell echo *.h)

############### Rules ###############

all: linearCalculator


## Compile step (.c files -> .o files)

# To get *any* .o file, compile its .c file with the following rule.
%.o: %.cpp $(INCLUDES)
	$(CPP) $(CFLAGS) -c $< -o $@


## Linking step (.o -> executable program)

linearCalculator: linearCalculator.o vector.o matrix.o
	$(CPP) $(LDFLAGS) $^ -o $@ $(LDLIBS)

clean:
	rm -f *.o *.out