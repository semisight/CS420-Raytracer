# Makefile 
# CSCI 480
# Assignment 3

# we assume the pic directory locates one level above,
# change PIC_PATH if this is not the case
PIC_PATH = $(abspath $(CURDIR)/../pic)

INCLUDE = -I$(PIC_PATH)
LIBRARIES = -L$(PIC_PATH) -framework OpenGL -framework GLUT -lpicio $(PIC_PATH)/libjpeg.a

COMPILER = clang++
COMPILERFLAGS = -g -Ofast -flto -ffast-math -std=c++11 $(INCLUDE)
WARNINGS = -Weverything -Wno-padded -Wno-c++98-compat -Wno-deprecated-declarations

PROGRAM = assign3
SOURCE = assign3.cpp vmath.cpp objects.cpp scene.cpp
OBJECT = assign3.o vmath.o objects.o scene.o

.cpp.o: 
	$(COMPILER) -c $(COMPILERFLAGS) $(WARNINGS) $<

all: $(PROGRAM)

$(PROGRAM): $(OBJECT)
	$(COMPILER) $(COMPILERFLAGS) -o $(PROGRAM) $(OBJECT) $(LIBRARIES)

clean:
	-rm -rf core *.o *~ "#"*"#" $(PROGRAM)
