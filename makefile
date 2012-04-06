CXXFLAGS =	-O2 -Wall -Wextra -pedantic

LIBS =		-lm

OBJS =		main.o

TARGET =	minesweeper

VPATH =		src/

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:		$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
