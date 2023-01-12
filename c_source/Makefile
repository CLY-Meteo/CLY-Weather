
CC=gcc
EXECUTABLE_NAME=cly-meteo-sorting

# Optional (use root if empty)
BUILD_FOLDER=

# Release
release : all clean
run_release : release run

# Compilation
avl.o :
	@$(CC) -c avl.c -o $(BUILD_FOLDER)avl.o

avr.o :
	@$(CC) -c avr.c -o $(BUILD_FOLDER)avr.o

tab.o :
	@$(CC) -c tab.c -o $(BUILD_FOLDER)tab.o

main.o :
	@$(CC) -c main.c -o $(BUILD_FOLDER)main.o

# Everything at once
all : avl.o avr.o tab.o main.o
	@$(CC) $(BUILD_FOLDER)avl.o $(BUILD_FOLDER)avr.o $(BUILD_FOLDER)tab.o $(BUILD_FOLDER)main.o -o $(EXECUTABLE_NAME)

# Cleaner
clean :
	@rm -f $(BUILD_FOLDER)*.o

# Runners
run : all
	@echo "Running $(EXECUTABLE_NAME)"
	@./$(EXECUTABLE_NAME)
