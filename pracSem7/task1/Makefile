TARGET = main
GEN = gen
FUNCTIONS = functions
CXX = g++-5
MCXX = mpicxx
CXXFLAGS = -g
LIBS=  /usr/lib/libscalapack-openmpi.so.1.8.0 /usr/lib/libblacs-openmpi.so.1.1 /usr/lib/libblacsCinit-openmpi.so.1.1 /usr/lib/libblacsF77init-openmpi.so.1.1 -lpthread -lblas -llapack -lm -lgfortran

all: $(FUNCTIONS) $(TARGET) $(GEN) Makefile

run:
	mpirun -np 9 ./bin/$(TARGET) matrix/fileR matrix/fileH 9 0.01 10

gen_new:
	./bin/gen matrix/fileR 9
	sleep 1
	./bin/gen matrix/fileH 9

$(TARGET): obj/$(TARGET).o
	mkdir -p bin
	mkdir -p matrix
	$(MCXX) $(CXXFLAGS) obj/$(TARGET).o obj/$(FUNCTIONS).o -o bin/$(TARGET) $(LIBS)

$(GEN): obj/$(GEN).o
	mkdir -p bin
	mkdir -p matrix
	$(CXX) $(CXXFLAGS) obj/$(GEN).o -o bin/$(GEN)

$(FUNCTIONS):  obj/$(FUNCTIONS).o

obj/$(GEN).o: src/$(GEN).cpp
	mkdir -p obj
	$(CXX) $(CXXFLAGS) -c $< -o obj/$(GEN).o

obj/%.o: src/%.cpp
	mkdir -p obj
	$(MCXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf bin/*
	rm -rf matrix/*
	rm -rf *.o
	rm -rf obj/*.o
	rm -rf src/*.o
	rm -rf file*
