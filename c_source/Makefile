
CC=gcc
EXECUTABLE_NAME=cly-meteo-sorting

# Optional (use root if empty)
BUILD_FOLDER=

# Release
release : all clean
run_release : release run

# Compilation
math.o :
	@$(CC) -c math.c -o $(BUILD_FOLDER)math.o

avllib.o :
	@$(CC) -c avllib.c -o $(BUILD_FOLDER)avllib.o

abrlib.o :
	@$(CC) -c abrlib.c -o $(BUILD_FOLDER)abrlib.o

tablib.o :
	@$(CC) -c tablib.c -o $(BUILD_FOLDER)tablib.o

main.o :
	@$(CC) -c main.c -o $(BUILD_FOLDER)main.o

# Everything at once
all : math.o avllib.o abrlib.o tablib.o main.o
	@$(CC) $(BUILD_FOLDER)*.o -o $(EXECUTABLE_NAME)

# Cleaner
clean :
	@rm -f $(BUILD_FOLDER)*.o

# Runners
run : all
	@echo "Running $(EXECUTABLE_NAME)"
	@./$(EXECUTABLE_NAME)
