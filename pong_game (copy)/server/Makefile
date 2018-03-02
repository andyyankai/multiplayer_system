CC = g++
CFLAGS = -c
SOURCES = main.cpp websocket.cpp sha1.cpp base64.cpp GameLogic.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = ponggame

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *o $(EXECUTABLE)
