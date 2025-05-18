serpent : main_nana.c fonction_nana.c fonction_nana.h
	gcc *.c -o serpent -lSDL2
run_serpent :
	./serpent
run_valgrind : serpent
	valgrind --tools = memcheck --leak-check = yes --leak-resolution = low --show-reachable=yes ./serpent
clean :
	rm serpent
profile:
	gcc -Wall -pg fonction_nana.c main_nana.c -lm -lpthread -o serpent	
	
	

