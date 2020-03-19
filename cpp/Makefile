CFLAGS = -O
CC = g++
CFLAGS  = -std=c++14
SRC = main.cpp test.cpp node.cpp clique-router.cpp color.cpp
OBJ = $(SRC:.cpp = .o)
CliqueRouting: $(OBJ)
	$(CC) $(CFLAGS) -o clique-routing $(OBJ)
clean:
	rm -f clique-routing *.o 
