TARGET = bin/dbview
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))

run: clean default
	 ./$(TARGET) -f ./mynewdb.db -n 
	 ./$(TARGET) -f ./mynewdb.db
	 ./$(TARGET) -f ./mynewdb.db -a "Timmy Hanks,123 Sheshire Ln.,120"
	 ./$(TARGET) -f ./mynewdb.db -a "John Wick,987 DNU Ln.,9928"
	 ./$(TARGET) -f ./mynewdb.db -a "Albus Dumbledore,8821 Howgwarts Circle,80"
	 ./$(TARGET) -f ./mynewdb.db -l

default: $(TARGET)

clean:
	rm -f obj/*.o
	rm -f bin/*
	rm -f *.db

$(TARGET): $(OBJ)
	gcc -o $@ $?

obj/%.o : src/%.c
	gcc -c $< -o $@ -Iinclude


