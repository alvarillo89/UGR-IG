# Variables:
CXX= g++
CXXFLAGS= -g -DXWINDOWS -Wall -I/usr/include -I$(INC) -L/usr/lib
LIBS= -lglut -lGLU  -lGL -lX11 -lXext  -lc -lm
SRC= src/
INC= include/
OBJ= obj/
BIN= bin/

# Reglas para construir el programa principal:
$(BIN)Practica5: $(OBJ)file_ply_stl.o $(OBJ)objetos.o $(OBJ)practica5.o
	$(CXX) $(CXXFLAGS) -o $(BIN)Practica5 $^ $(LIBS)

$(OBJ)objetos.o: $(SRC)objetos.cc $(INC)objetos.h $(INC)vertex.h
	$(CXX) $(CXXFLAGS) -o $(OBJ)objetos.o -c $(SRC)objetos.cc $(LIBS) 

$(OBJ)practica5.o: $(SRC)practica5.cc $(INC)vertex.h $(INC)objetos.h
	$(CXX) $(CXXFLAGS) -o $(OBJ)practica5.o -c $(SRC)practica5.cc $(LIBS)

$(OBJ)file_ply_stl.o: $(SRC)file_ply_stl.cc $(INC)file_ply_stl.h
	$(CXX) $(CXXFLAGS) -o $(OBJ)file_ply_stl.o -c $(SRC)file_ply_stl.cc $(LIBS)  


# Limpiar
clean:
	rm $(OBJ)*.o
	rm $(BIN)Practica5
