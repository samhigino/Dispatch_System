# --------------------------------------------------------------
# VARIÁVEIS DE AMBIENTE
# --------------------------------------------------------------
CXX = g++
CXXFLAGS = -std=c++11 -Iinclude

# --------------------------------------------------------------
# DIRETÓRIOS
# --------------------------------------------------------------
BIN_DIR = bin
INC_DIR = inc
OBJ_DIR = obj
SRC_DIR = src

# --------------------------------------------------------------
# OBJETOS
# --------------------------------------------------------------
TARGET = tp1.out
TEST_TARGET = teste.out
MAIN_OBJ = obj/main.o obj/2D_point.o obj/demand.o obj/stop.o obj/segment.o obj/demand_group.o obj/ride.o obj/event.o obj/event_scaler.o obj/simulation_manager.o
TEST_OBJ = obj/test.o obj/2D_point.o obj/demand.o obj/stop.o obj/segment.o obj/demand_group.o obj/ride.o obj/event.o obj/event_scaler.o obj/simulation_manager.o

# --------------------------------------------------------------
# COMPILAÇÃO
# --------------------------------------------------------------
all: dirs $(MAIN_OBJ) 
	$(CXX) $(CXXFLAGS) $(MAIN_OBJ) -o $(BIN_DIR)/$(TARGET)

dirs:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)

obj/2D_point.o: $(SRC_DIR)/2D_point.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/2D_point.cpp -o $(OBJ_DIR)/2D_point.o

obj/demand.o: $(SRC_DIR)/demand.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/demand.cpp -o $(OBJ_DIR)/demand.o

obj/stop.o: $(SRC_DIR)/stop.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/stop.cpp -o $(OBJ_DIR)/stop.o

obj/segment.o: $(SRC_DIR)/segment.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/segment.cpp -o $(OBJ_DIR)/segment.o

obj/demand_group.o: $(SRC_DIR)/demand_group.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/demand_group.cpp -o $(OBJ_DIR)/demand_group.o

obj/ride.o: $(SRC_DIR)/ride.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/ride.cpp -o $(OBJ_DIR)/ride.o

obj/event.o: $(SRC_DIR)/event.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/event.cpp -o $(OBJ_DIR)/event.o

obj/event_scaler.o: $(SRC_DIR)/event_scaler.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/event_scaler.cpp -o $(OBJ_DIR)/event_scaler.o

obj/simulation_manager.o: $(SRC_DIR)/simulation_manager.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/simulation_manager.cpp -o $(OBJ_DIR)/simulation_manager.o

obj/main.o: $(SRC_DIR)/main.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/main.cpp -o $(OBJ_DIR)/main.o

test: dirs $(TEST_OBJ) 
	$(CXX) $(CXXFLAGS) $(TEST_OBJ) -o $(BIN_DIR)/$(TEST_TARGET)
	@rm -f outputs/*.txt
	$(BIN_DIR)/$(TEST_TARGET)

obj/test.o: $(SRC_DIR)/test.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/test.cpp -o $(OBJ_DIR)/test.o

ex:
	@rm -f outputs/*.txt	
	$(BIN_DIR)/$(TARGET)

runtest:
	@rm -f outputs/*.txt
	$(BIN_DIR)/$(TEST_TARGET)

clean: 
	@rm -f output/*.txt
	@rm -f $(BIN_DIR)/*.out
	@rm -f $(OBJ_DIR)/*.o