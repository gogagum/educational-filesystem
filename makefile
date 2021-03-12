all:   main 
       
main:  main.o directory.o \
              reg_file.o \
			  data_getters.o \
			  alloc.o \
			  parse_path.o \
			  menus.o \
			  open_n_create_fs_file.o
	gcc -o build/main build/main.o \
                  build/directory.o \
                  build/reg_file.o\
                  build/data_getters.o \
                  build/data_setters.o \
                  build/alloc.o \
                  build/parse_path.o \
                  build/menus.o \
                  build/open_n_create_fs_file.o

main.o:
	gcc -o build/main.o -c src/main.c

menus.o:
	gcc -o build/menus.o -c src/menus.c

directory.o: data_getters.o data_setters.o
	gcc -o build/directory.o -c src/directory.c

reg_file.o: data_getters.o data_setters.o alloc.o
	gcc -o build/reg_file.o -c src/reg_file.c

alloc.o:
	gcc -o build/alloc.o -c src/alloc.c

open_n_create_fs_file.o: data_setters.o
	gcc -o build/open_n_create_fs_file.o -c src/open_n_create_fs_file.c

data_getters.o:
	gcc -o build/data_getters.o -c src/data_getters.c

data_setters.o:
	gcc -o build/data_setters.o -c src/data_setters.c

parse_path.o:
	gcc -o build/parse_path.o -c src/parse_path.c