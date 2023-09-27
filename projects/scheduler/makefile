CFLAG = -ansi -pedantic-errors -Wall -Wextra
CPPFLAG = -I./include/ -I../../ds/include/
gd = $(CFLAG) -g
gc = $(CFLAG) -DNDEBUG -O3
TEST = ./test/
SRC = ./src/
DSPATH = ../../ds/src/
CFILES = ./test/$(FILE)_test.c ./src/$(FILE).c ./src/uid.c #./src/sorted_list.c 
BIN = ./bin/
VLG = valgrind -s --leak-check=yes --track-origins=yes
CC = gcc
UID = uid
TASK = task
SCHEDULER = sched


#Make default
$(TARGET): $(CFILES) 
	$(CC) $(gd) $(CFILES) -o $(BIN)/$(TARGET) $(CPPFLAG)

#Make uid
uid: $(TEST)$(UID)_test.c $(SRC)$(UID).c
	$(CC) $(gd) $(TEST)$(UID)_test.c $(SRC)$(UID).c -o $(BIN)$(UID) $(CPPFLAG)
	$(BIN)$(UID)
	
#Make task
task: $(TEST)$(TASK)_test.c $(SRC)$(TASK).c $(SRC)$(UID).c
	$(CC) $(gd) $(TEST)$(TASK)_test.c $(SRC)$(TASK).c $(SRC)$(UID).c -o $(BIN)$(TASK) $(CPPFLAG)
	$(BIN)$(TASK)

#make scheduler dll
scheduler_dll: $(TEST)$(SCHEDULER)_test.c $(SRC)$(SCHEDULER).c $(SRC)$(TASK).c $(SRC)$(UID).c $(DSPATH)dll.c $(DSPATH)sorted_list.c $(DSPATH)pq.c
	$(CC) $(gd) $(TEST)$(SCHEDULER)_test.c $(SRC)$(SCHEDULER).c $(SRC)$(TASK).c $(SRC)$(UID).c $(DSPATH)dll.c $(DSPATH)sorted_list.c $(DSPATH)pq.c -o $(BIN)$(SCHEDULER) $(CPPFLAG)
	$(BIN)$(SCHEDULER)

#make scheduler heap
scheduler_heap: $(TEST)$(SCHEDULER)_test.c $(SRC)$(SCHEDULER).c $(SRC)$(TASK).c $(SRC)$(UID).c $(DSPATH)heap.c $(DSPATH)d_vector.c $(DSPATH)pq_heap.c
	$(CC) $(gd) $(TEST)$(SCHEDULER)_test.c $(SRC)$(SCHEDULER).c $(SRC)$(TASK).c $(SRC)$(UID).c $(DSPATH)heap.c $(DSPATH)d_vector.c $(DSPATH)pq_heap.c -o $(BIN)$(SCHEDULER) $(CPPFLAG)
	$(BIN)$(SCHEDULER)

run: $(TARGET)
	$(BIN) $(TARGET)
	
#Make clean	
clean:
	rm -f $(TARGET) $(OBJS)

#make vlg
vlg:
	$(VLG) $(BIN)$(SCHEDULER)
	
#Make DEBUG
debug: 
	$(CC) $(gd) -o $(TARGET) $(CFILES) $(CPPFLAG)

#Make RELEASE	
release: 
	$(CC) $(gc) -o $(TARGET) $(CFILES) $(CPPFLAG)

