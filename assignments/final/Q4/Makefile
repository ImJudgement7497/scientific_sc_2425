CXX       := g++
MPI 	  := mpic++
CXX_FLAGS := -std=c++17 -ggdb -Iinclude -O3 -funroll-all-loops

BIN     := bin

SRC_SERIAL     := src/src_serial
INCLUDE_SERIAL := include/include_serial

SRC_PARALLEL := src/src_parallel
INCLUDE_PARALLEL := include/include_serial

LIBRARIES   := 

SERIAL_EXEC := main_serial
SERIAL_EXEC_DEBUG := main_serial_debug
SERIAL_EXEC_VIS := main_serial_vis

PARALLEL_EXEC := main_parallel
PARALLEL_EXEC_DEBUG := main_parallel_debug
PARALLEL_EXEC_VIS := main_parallel_vis
PARALLEL_EXEC_MPI := main_parallel_mpi

serial: $(BIN)/$(SERIAL_EXEC)

serial_debug: $(BIN)/$(SERIAL_EXEC_DEBUG)

serial_vis: $(BIN)/$(SERIAL_EXEC_VIS)

parallel: $(BIN)/$(PARALLEL_EXEC)

parallel_debug: $(BIN)/$(PARALLEL_EXEC_DEBUG)

parallel_vis: $(BIN)/$(PARALLEL_EXEC_VIS)

parallel_mpi: $(BIN)/$(PARALLEL_EXEC_MPI)

clean:
	@rm -rf $(BIN)/*

$(BIN)/$(SERIAL_EXEC): $(SRC_SERIAL)/*.cpp | $(BIN)
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE_SERIAL) $^ -o $@ $(LIBRARIES)

$(BIN)/$(SERIAL_EXEC_DEBUG): $(SRC_SERIAL)/*.cpp | $(BIN)
	$(CXX) $(CXX_FLAGS) -DDEBUG -I$(INCLUDE_SERIAL) $^ -o $@ $(LIBRARIES)

$(BIN)/$(SERIAL_EXEC_VIS): $(SRC_SERIAL)/*.cpp | $(BIN)
	$(CXX) $(CXX_FLAGS) -DVIS -I$(INCLUDE_SERIAL) $^ -o $@ $(LIBRARIES)

$(BIN)/$(PARALLEL_EXEC): $(SRC_PARALLEL)/*.cpp | $(BIN)
	$(CXX) $(CXX_FLAGS) -fopenmp -I$(INCLUDE_PARALLEL) $^ -o $@ $(LIBRARIES)

$(BIN)/$(PARALLEL_EXEC_DEBUG): $(SRC_PARALLEL)/*.cpp | $(BIN)
	$(CXX) $(CXX_FLAGS) -DDEBUG -I$(INCLUDE_PARALLEL) $^ -o $@ $(LIBRARIES)

$(BIN)/$(PARALLEL_EXEC_VIS): $(SRC_PARALLEL)/*.cpp | $(BIN)
	$(CXX) $(CXX_FLAGS) -fopenmp -DVIS -I$(INCLUDE_PARALLEL) $^ -o $@ $(LIBRARIES)

$(BIN)/$(PARALLEL_EXEC_MPI): $(SRC_PARALLEL)/*.cpp | $(BIN)
	$(MPI) $(CXX_FLAGS) -fopenmp -I$(INCLUDE_PARALLEL) $^ -o $@ $(LIBRARIES)

$(BIN):
	mkdir -p $(BIN)


