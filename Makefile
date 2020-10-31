all:
	g++ -g Search-Algorithms.cpp -o SA
run:
	./SA < Tests-Cases/$(input)

valgrind:	
	valgrind ./SA < Tests-Cases/$(input)

clean:
	rm -r -f SA		 	
