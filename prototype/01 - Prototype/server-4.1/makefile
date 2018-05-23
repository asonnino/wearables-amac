

ifeq ($(wildcard RF24/Makefile.inc), )
	$(error Configuration not found. Run ./configure first)
endif

include RF24/Makefile.inc


LIBS=-l$(LIB)

main: *.cpp
	g++ -o main *.cpp -include *.h $(LIBS)