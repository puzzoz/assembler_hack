SOURCES=main.c code.c parser.c SymbolTable.c code.h parser.h SymbolTable.h
EXECUTABLE=assembler

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	gcc -o $(EXECUTABLE) $(SOURCES)

clean:
	rm -f $(EXECUTABLE) *.hack

