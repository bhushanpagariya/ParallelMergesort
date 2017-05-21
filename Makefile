CC = g++
#CFLAGS =
CFLAGS = -fopenmp
COPTFLAGS = -O3 -g
LDFLAGS =

default:
	@echo "=================================================="
	@echo "To build your OpenMP code, use:"
	@echo "  make mergesort-omp    # For Mergesort"
	@echo ""
	@echo "To clean this subdirectory (remove object files"
	@echo "and other junk), use:"
	@echo "  make clean"
	@echo "=================================================="

# Mergesort driver using OpenMP
mergesort-omp: driver.o parallel-mergesort.o
	$(CC) $(CFLAGS) $(COPTFLAGS) -o $@ $^

%.o: %.cc
	$(CC) $(CFLAGS) $(COPTFLAGS) -o $@ -c $<

clean:
	rm -f core *.o *~ mergesort-omp

# eof
