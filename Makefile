debugger: debugger_main.cpp Debugger.h Debugger.cpp Breakpoint.h Breakpoint.cpp

debugger: Debugger.cpp debugger_main.cpp Breakpoint.cpp
	$(CXX) -Wall -W -O0 -g -fpermissive -o debugger Debugger.cpp debugger_main.cpp Breakpoint.cpp

clean:
	rm -f debugger
