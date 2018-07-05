TARGET = main
GEN = gen
FUNCTIONS = functions
FUNCTIONSTASK2 = functions_task2
CXX = g++-5
MCXX = mpicxx
CXXFLAGS = -g
LIBS=  /usr/lib/libscalapack-openmpi.so.1.8.0 /usr/lib/libblacs-openmpi.so.1.1 /usr/lib/libblacsCinit-openmpi.so.1.1 /usr/lib/libblacsF77init-openmpi.so.1.1 -lpthread -lblas -llapack -lm -lgfortran

all: $(FUNCTIONS) $(FUNCTIONSTASK2) $(GEN) $(TARGET) Makefile

run:
	mpirun -np 1 ./bin/main src/parametersFile

run1:
	mpirun -np 1 ./bin/main src/parametersFile
run4:
	mpirun -np 4 ./bin/main src/parametersFile

size:
	mpirun -np 4 ./bin/main src/parametersFile s 

$(TARGET): obj/$(TARGET).o
	mkdir -p bin
	$(MCXX) $(CXXFLAGS) obj/$(TARGET).o obj/$(FUNCTIONS).o obj/$(FUNCTIONSTASK2).o -o bin/$(TARGET) $(LIBS)

$(FUNCTIONS):  obj/$(FUNCTIONS).o

$(FUNCTIONSTASK2):  obj/$(FUNCTIONSTASK2).o

obj/$(GEN).o: src/$(GEN).cpp
	mkdir -p obj
	$(CXX) $(CXXFLAGS) -c $< -o obj/$(GEN).o

obj/%.o: src/%.cpp
	mkdir -p obj
	$(MCXX) $(CXXFLAGS) -c $< -o $@

$(GEN): obj/$(GEN).o
	mkdir -p bin
	mkdir -p matrix
	$(CXX) $(CXXFLAGS) obj/$(GEN).o -o bin/$(GEN)

clean:
	rm -rf bin/*
	rm -rf *.o
	rm -rf obj/*.o
	rm -rf src/*.o
