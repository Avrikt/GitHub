#************************************************
# Developer: Avri Kehat						    *
# Description: Makefile for Priority Queue	    *
#************************************************/

FILE = pq
CC := g++
CFLAGS = -std=c++11 -pedantic-errors -Wall -Wextra
CPPFLAGS = -I./include -I/home/avri/git/utils/include
SRCDIR = ./src
TESTDIR = ./test
BINDIR = ./bin
VLG = valgrind -s --leak-check=yes --track-origins=yes

# Generate the object file names
OBJ_FILES = $(patsubst $(SRCDIR)/%.cpp,$(BINDIR)/%.o,$(wildcard $(SRCDIR)/$(FILE).cpp))
TEST_OBJ_FILES = $(patsubst $(TESTDIR)/%_test.cpp,$(BINDIR)/%_test.o,$(wildcard $(TESTDIR)/$(FILE)_test.cpp))

# Set different compiler flags for different build types
ifeq ($(b), gppd)
	CFLAGS += -Og -g
else ifeq ($(b), gppdw)
	CFLAGS += -Og -g -Werror
else ifeq ($(b), gppc)
	CFLAGS += -DNDEBUG -O3
else ifeq ($(b), gppcw)
	CFLAGS += -DNDEBUG -O3 -Werror
endif

# Default target
$(BINDIR)/$(FILE): $(OBJ_FILES) $(TEST_OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@ $(CPPFLAGS)

$(BINDIR):
	mkdir -p $(BINDIR)
	
# Compile source files
$(BINDIR)/%.o: $(SRCDIR)/%.cpp | $(BINDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(CPPFLAGS)

# Compile test files
$(BINDIR)/%_test.o: $(TESTDIR)/%_test.cpp | $(BINDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(CPPFLAGS)

# Run the executable
run: $(BINDIR)/$(FILE)
	$(BINDIR)/$(FILE)

# Clean build artifacts
clean:
	rm -f $(BINDIR)/$(FILE)*.o

# Run with Valgrind
vlg: $(BINDIR)/$(FILE)
	$(VLG) $(BINDIR)/$(FILE)

# Debug with gdb
debug: $(BINDIR)/$(FILE)
	gdb $(BINDIR)/$(FILE)

# Compress source files for release
compress:
	mkdir -p review/test
	mkdir -p review/src
	mkdir -p review/include
	
	cp $(TESTDIR)/$(FILE)_test.cpp 			review/test/
	cp ./include/$(FILE).hpp 				review/include/
	cp ~/git/utils/include/utilities.hpp 	review/include/
	cp makefile.$(FILE)      				review/
	mv review/makefile.$(FILE) 				review/makefile

	tar -czvf $(FILE).tar.gz review/
	
	rm -r review

.PHONY: run clean vlg debug compress