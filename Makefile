CC=gcc
CXXFLAGS=-g --std=c++11 -Wall
SRC=src
OBJDIR=obj
BINDIR=bin
LIBS=m stdc++
LINKS=$(addprefix -l, $(LIBS))


all: setup assn1-cli assn1-gui

setup:
	mkdir -p $(OBJDIR) $(BINDIR)

$(OBJDIR)/%.o: $(SRC)/%.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@

matrices:

number-rep:

fp-rep:

assn1-cli: $(OBJDIR)/assn1-cli.o #matrices number-rep fp-rep 
	$(CC) $(CXXFLAGS) $? -o $(BINDIR)/assn1-cli $(LINKS)

assn1-gui: matrices number-rep fp-rep

clean:
	rm *.o matrices number-rep fp-rep assn1-cli assn1-gui
	rm -r obj bin