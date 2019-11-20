CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		BinaryFile.o Breakpoint.o Debugger.o ElfFile.o libdbg.o

LIBS =

TARGET =	libdbg

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
