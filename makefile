# Debug

CC = gcc

all:   main 

debug: CCFLAGS = -DDEBUG
debug: main

# Release

main:  main.o directory.o \
              reg_file.o \
			  data_getters.o \
			  alloc.o \
			  menus.o \
			  open_n_create_fs_file.o
	$(CC) $(CCFLAGS) -o build/main \
    build/main.o \
    build/directory.o \
    build/reg_file.o\
    build/data_getters.o \
    build/data_setters.o \
    build/alloc.o \
    build/menus.o \
    build/open_n_create_fs_file.o

main.o:
	$(CC) $(CCFLAGS) -o build/main.o -c src/main.c

menus.o: directory.o
	$(CC) $(CCFLAGS) -o build/menus.o -c src/menus.c

directory.o: data_getters.o data_setters.o
	$(CC) $(CCFLAGS) -o build/directory.o -c src/directory.c

reg_file.o: data_getters.o data_setters.o alloc.o
	$(CC) $(CCFLAGS) -o build/reg_file.o -c src/reg_file.c

alloc.o:
	$(CC) $(CCFLAGS) -o build/alloc.o -c src/alloc.c

open_n_create_fs_file.o: data_setters.o
	$(CC) $(CCFLAGS) -o build/open_n_create_fs_file.o -c src/open_n_create_fs_file.c

data_getters.o:
	$(CC) $(CCFLAGS) -o build/data_getters.o -c src/data_getters.c

data_setters.o:
	$(CC) $(CCFLAGS) -o build/data_setters.o -c src/data_setters.c
