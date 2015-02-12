
EXE=testAG
CC=g++
INC=
OBJS  = $(patsubst %.cpp,%.o,$(wildcard *.cpp))

$(EXE): $(OBJS)
	$(CC) $(INC) $(OBJS) -o $@
%.o: %.cpp
	$(CC) $(INC) -c $< -o $@


clean:       
	rm *.o $(EXE)

