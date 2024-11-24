# Nombre del compilador
CXX = g++

# Opciones del compilador
CXXFLAGS = -Wall -Wextra -std=c++17 -O2

# Archivos fuente y objeto
SRC = SBR-FC.cpp utils.cpp
OBJ = $(SRC:.cpp=.o)

# Nombre del ejecutable
TARGET = SBR-FC

# Regla principal
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpieza
clean:
	rm -f $(OBJ) $(TARGET)

# Regla de phony para evitar conflictos con archivos
.PHONY: all clean
