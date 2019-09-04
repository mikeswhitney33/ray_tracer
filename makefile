GCC=g++
O=-O3
F=-fopenmp
MAIN=src/main.cpp
FAC=src/factories/*.cpp
GEO=src/geometry/*.cpp
MAT=src/materials/*.cpp


main:
	$(GCC) $(O) $(F) $(MAIN) $(FAC) $(GEO) $(MAT) -o ray-tracer.out

clean:
	rm ray-tracer.out