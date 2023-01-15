
CC=gcc
EXECUTABlibLE_NAME=cly-meteo-sorting

# Optional (use root if empty)
BUILD_FOLDER=

# Release
release : all clean
run_release : release run

# Compilation
avllib.o :
	@$(CC) -c avllib.c -o $(BUILD_FOLDER)avllib.o

abrlib.o :
	@$(CC) -c abrlib.c -o $(BUILD_FOLDER)abrlib.o

tablib.o :
	@$(CC) -c tablib.c -o $(BUILD_FOLDER)tablib.o

main.o :
	@$(CC) -c main.c -o $(BUILD_FOLDER)main.o

# Everything at once
all : avllib.o abrlib.o tablib.o main.o
	@$(CC) $(BUILD_FOLDER)avllib.o $(BUILD_FOLDER)abrlib.o $(BUILD_FOLDER)tablib.o $(BUILD_FOLDER)main.o -o $(EXECUTABlibLE_NAME)

# Cleaner
clean :
	@rm -f $(BUILD_FOLDER)*.o

# Runners
run : all
	@echo "Running $(EXECUTABlibLE_NAME)"
	@./$(EXECUTABlibLE_NAME)
