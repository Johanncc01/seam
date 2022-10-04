CC = c++ # can be replaced by clang++

default: run

helper: helper.h helper.cpp
	$(CC) -std=c++11 -Wall -o helper -c helper.cpp

seam:  seam.h seam.cpp
	$(CC) -std=c++11 -Wall -o seam -c seam.cpp

extension:  extension.h extension.cpp
	$(CC) -std=c++11 -Wall -o extension -c extension.cpp

unit_test: unit_test.h unit_test.cpp
	 $(CC) -std=c++11 -Wall -o unit_test -c unit_test.cpp

main: helper seam unit_test extension main.cpp
	$(CC) -std=c++11 -Wall main.cpp helper seam unit_test extension -o main -std=c++11 

profile : main.cpp seam.cpp seam.h 
	 $(CC) -std=c++11 -Wall -pg -o main main.cpp helper.cpp seam.cpp -std=c++11 
	./main
	gprof ./main | python gprof2dot.py | dot -Tpng -o output.png

run: main
	./main

clean:
	rm -rf main helper seam unit_test extension gmon.out output.png *.png *~


