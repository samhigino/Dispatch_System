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
MAIN_OBJ = obj/main.o obj/2D_point.o
TEST_OBJ = obj/test.o obj/2D_point.o

# --------------------------------------------------------------
# COMPILAÇÃO
# --------------------------------------------------------------
all: dirs $(MAIN_OBJ) 
	$(CXX) $(CXXFLAGS) $(MAIN_OBJ) -o $(BIN_DIR)/$(TARGET)

dirs:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)

obj/vector.o: $(SRC_DIR)/vector.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/vector.cpp -o $(OBJ_DIR)/vector.o

obj/2D_point.o: $(SRC_DIR)/2D_point.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/2D_point.cpp -o $(OBJ_DIR)/2D_point.o

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