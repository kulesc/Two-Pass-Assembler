assembly: Assembly.o Error.o main.o RelocationTable.o RelocationTableEntry.o StringTokenizer.o Symbol.o SymbolTable.o
	g++ -std=c++0x -o assembly -g Assembly.o Error.o main.o RelocationTable.o RelocationTableEntry.o StringTokenizer.o Symbol.o SymbolTable.o

Assembly.o: Assembly.cpp Assembly.h
	g++ -std=c++0x -c -g Assembly.cpp 

Error.o: Error.cpp Error.h
	g++ -std=c++0x -c -g Error.cpp 

main.o: main.cpp Assembly.h StringTokenizer.h Error.h
	g++ -std=c++0x -c -g main.cpp

RelocationTable.o: RelocationTable.cpp RelocationTable.h
	g++ -std=c++0x -c -g RelocationTable.cpp

RelocationTableEntry.o: RelocationTableEntry.cpp RelocationTableEntry.h
	g++ -std=c++0x -c -g RelocationTableEntry.cpp

StringTokenizer.o: StringTokenizer.cpp StringTokenizer.h
	g++ -std=c++0x -c -g StringTokenizer.cpp

SymbolTable.o: SymbolTable.cpp SymbolTable.h
	g++ -std=c++0x -c -g SymbolTable.cpp

Symbol.o: Symbol.cpp Symbol.h
	g++ -std=c++0x -c -g Symbol.cpp 

clean:
	rm Symbol.o
	rm SymbolTable.o
	rm StringTokenizer.o
	rm RelocationTableEntry.o
	rm RelocationTable.o
	rm main.o
	rm Error.o
	rm Assembly.o
	rm assembly
