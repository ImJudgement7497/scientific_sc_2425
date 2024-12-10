CXX       := g++
CXX_FLAGS := -std=c++17 -ggdb -Iinclude

BIN     := bin
SRC_SERIAL     := src/src_serial
INCLUDE := include

LIBRARIES   := 
SERIAL_EXEC := main_serial
SERIAL_EXEC_DEBUG := main_serial_debug

all: $(BIN)/$(EXECUTABLE)

serial: $(BIN)/$(SERIAL_EXEC)

serial_debug: $(BIN)/$(SERIAL_EXEC_DEBUG)

run: clean all
	@clear
	@./$(BIN)/$(EXECUTABLE)

run_serial: clean serial
	@clear
	@./$(BIN)/$(SERIAL_EXEC)

run_serial_debug: clean serial_debug
	@clear
	@./$(BIN)/$(SERIAL_EXEC_DEBUG)

$(BIN)/$(SERIAL_EXEC): $(SRC_SERIAL)/*.cpp | $(BIN)
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $@ $(LIBRARIES)

$(BIN)/$(SERIAL_EXEC_DEBUG): $(SRC_SERIAL)/*.cpp | $(BIN)
	$(CXX) $(CXX_FLAGS) -DDEBUG -I$(INCLUDE) $^ -o $@ $(LIBRARIES)

$(BIN):
	mkdir -p $(BIN)

clean:
	-rm -rf $(BIN)/*

parallel:
	@echo "Parallel build rule not implemented yet."
