all:   main 
       
main:  main.o data_getters.o parse_path.o
	gcc -o build/main build/main.o \
	                  build/data_getters.o \
	                  build/parse_path.o

main.o:
	gcc -o build/main.ols  src/main.c

data_getters.o:
	gcc -o build/data_getters.o -c src/data_getters.c

parse_path.o:
	gcc -o build/parse_path.o -c src/parse_path.c