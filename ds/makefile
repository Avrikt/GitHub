FILE = pq
CFLAG = -ansi -pedantic-errors -Wall -Wextra
CPPFLAG = -I./include/
gd = $(CFLAG) -g
gc = $(CFLAG) -DNDEBUG -O3
OBJS = $(FILE)_test.o $(FILE).o
CFILES = ./test/$(FILE)_test.c ./src/$(FILE).c ./src/dll.c ./src/sorted_list.c 
VLG = valgrind -s --leak-check=yes --track-origins=yes
CC = gcc
TARGET = $(FILE)

#Make all
$(TARGET): $(CFILES) 
	$(CC) $(gd) $(CFILES) -o $(TARGET) $(CPPFLAG)

run: $(TARGET)
	./$(TARGET)
	
#Make clean	
clean:
	rm -f $(TARGET) $(OBJS)

#make vlg
vlg:
	$(VLG) ./$(TARGET)
	
#Make DEBUG
debug: 
	$(CC) $(gd) -o $(TARGET) $(CFILES) $(CPPFLAG)

#Make RELEASE	
release: 
	$(CC) $(gc) -o $(TARGET) $(CFILES) $(CPPFLAG)

