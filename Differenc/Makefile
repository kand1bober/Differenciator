
CC = g++
CFLAGS = -fsanitize=address,undefined -D _DEBUG -O0 -Wall -Wextra

SOURCES_LATEX_DIR = diff_Latex_src
TO_REMOVE_LATEX = $(wildcard $(SOURCES_LATEX_DIR)/*.* )

SOURCES_DIR_DIFF = Sources
SOURCES_DIR_TREE = ../Tree/Sources

SOURCES_DIFF = $(wildcard $(SOURCES_DIR_DIFF)/*cpp ) 
SOURCES_TREE = $(wildcard $(SOURCES_DIR_TREE)/*cpp ) 

OBJECTS_DIFF = $(SOURCES_DIFF:.cpp =.o) 
OBJECTS_TREE = $(SOURCES_TREE:.cpp =.o)

EXECUTABLE = Differenciator


all: $(EXECUTABLE)


$(EXECUTABLE): $(OBJECTS_DIFF) $(OBJECTS_TREE) 
	$(CC) $(OBJECTS_DIFF) $(OBJECTS_TREE) $(CFLAGS) -o $(EXECUTABLE)


%.o: %.cpp
	$(CC) -c $(CFLAGS) -IHeaders -I../Tree/Headers $< -o $@ 


clean:
	@rm  -rf  $(SOURCES_DIFF/*.o) $(SOURCES_TREE/*.o)  $(EXECUTABLE)  ../Tree/TreeGraphh.dot  ../Tree/TreeGraph.svg  ../Tree/TreeGraph.html
	@rm  -rf  $(TO_REMOVE_LATEX)
