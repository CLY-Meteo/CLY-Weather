CC=gcc
EXECUTABLE_NAME=cly-meteo-sorting

# Optional (use root if empty)
BUILD_FOLDER=

# Release
release : all clean_objects

# Compilation
utils.o :
	@$(CC) -c utils.c -o $(BUILD_FOLDER)utils.o

avllib.o :
	@$(CC) -c avllib.c -o $(BUILD_FOLDER)avllib.o

abrlib.o :
	@$(CC) -c abrlib.c -o $(BUILD_FOLDER)abrlib.o

tablib.o :
	@$(CC) -c tablib.c -o $(BUILD_FOLDER)tablib.o

main.o :
	@$(CC) -c main.c -o $(BUILD_FOLDER)main.o

# Everything at once
all : utils.o avllib.o abrlib.o tablib.o main.o
	@$(CC) $(BUILD_FOLDER)*.o -o $(EXECUTABLE_NAME)

# Cleaner
clean_objects :
	@rm -f $(BUILD_FOLDER)*.o

clean : clean_objects
	@rm -f $(EXECUTABLE_NAME)