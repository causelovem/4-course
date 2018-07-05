TARGET = reflection
CXX = mpicxx
CXXFLAGS = -g -O3

all: $(TARGET) Makefile

run: 
	# mpirun -np 5 reflection 10

	rm -rf *.out
	rm -rf *.err
	
	mpisubmit.bg -n 1 -w 00:30:00 ./$(TARGET) -- 1024 n; \
	mpisubmit.bg -n 1 -w 00:30:00 ./$(TARGET) -- 1024 n; \
	mpisubmit.bg -n 1 -w 00:30:00 ./$(TARGET) -- 2048 n; \
	mpisubmit.bg -n 1 -w 00:30:00 ./$(TARGET) -- 2048 n; \
	mpisubmit.bg -n 1 -w 00:30:00 ./$(TARGET) -- 3072 n; \
	mpisubmit.bg -n 1 -w 00:30:00 ./$(TARGET) -- 3072 n; \
	for i in 32 64 128 ; do \
		mpisubmit.bg -n $$i -w 00:02:00 ./$(TARGET) -- 1024 n; \
		mpisubmit.bg -n $$i -w 00:02:00 ./$(TARGET) -- 1024 n; \
		mpisubmit.bg -n $$i -w 00:02:00 ./$(TARGET) -- 2048 n; \
		mpisubmit.bg -n $$i -w 00:02:00 ./$(TARGET) -- 2048 n; \
		mpisubmit.bg -n $$i -w 00:02:00 ./$(TARGET) -- 3072 n; \
		mpisubmit.bg -n $$i -w 00:02:00 ./$(TARGET) -- 3072 n; \
	done \

run1:
	for i in 256 512 1024 ; do \
		mpisubmit.bg -n $$i -w 00:02:00 ./$(TARGET) -- 1024 n; \
		mpisubmit.bg -n $$i -w 00:02:00 ./$(TARGET) -- 1024 n; \
		mpisubmit.bg -n $$i -w 00:02:00 ./$(TARGET) -- 2048 n; \
		mpisubmit.bg -n $$i -w 00:02:00 ./$(TARGET) -- 2048 n; \
		mpisubmit.bg -n $$i -w 00:02:00 ./$(TARGET) -- 3072 n; \
		mpisubmit.bg -n $$i -w 00:02:00 ./$(TARGET) -- 3072 n; \
	done \

run2:
	for i in 2 4 8 16 ; do \
		mpisubmit.bg -n $$i -w 00:02:00 ./$(TARGET) -- 1024 n; \
		mpisubmit.bg -n $$i -w 00:02:00 ./$(TARGET) -- 1024 n; \
		mpisubmit.bg -n $$i -w 00:02:00 ./$(TARGET) -- 2048 n; \
		mpisubmit.bg -n $$i -w 00:02:00 ./$(TARGET) -- 2048 n; \
		mpisubmit.bg -n $$i -w 00:02:00 ./$(TARGET) -- 3072 n; \
		mpisubmit.bg -n $$i -w 00:02:00 ./$(TARGET) -- 3072 n; \
	done \

test:
	mpisubmit.bg -n 1 -w 00:05:00 ./$(TARGET) -- 3072

$(TARGET): $(TARGET).cpp
	$(CXX) $(CXXFLAGS) $(TARGET).cpp -o $(TARGET)

convert:
	# rm -rf res/*
	rm -rf restime/*

	# mv *.out res
	python convertRes.py restime/ res/*

clean:
	rm -rf $(TARGET)
	rm -rf *.o

llq:
	llq -W | grep $$USER
