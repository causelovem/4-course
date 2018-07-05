TARGET = main
FUNCTIONS = functions
CXX = g++-5
MCXX = mpicxx
CXXFLAGS = -g
LIBS=  /usr/lib/libscalapack-openmpi.so.1.8.0 /usr/lib/libblacs-openmpi.so.1.1 /usr/lib/libblacsCinit-openmpi.so.1.1 /usr/lib/libblacsF77init-openmpi.so.1.1 -lpthread -lblas -llapack -lm -lgfortran

all: $(FUNCTIONS) $(TARGET) Makefile

run:
	mpirun -np 4 ./bin/main 3 1 2 3 4 3 2 3

$(TARGET): obj/$(TARGET).o
	mkdir -p bin
	$(MCXX) $(CXXFLAGS) obj/$(TARGET).o obj/$(FUNCTIONS).o -o bin/$(TARGET) $(LIBS)

$(FUNCTIONS):  obj/$(FUNCTIONS).o

obj/$(GEN).o: src/$(GEN).cpp
	mkdir -p obj
	$(CXX) $(CXXFLAGS) -c $< -o obj/$(GEN).o

obj/%.o: src/%.cpp
	mkdir -p obj
	$(MCXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf bin/*
	rm -rf *.o
	rm -rf obj/*.o
	rm -rf src/*.o
