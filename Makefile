CC=gcc
CXXFLAGS=-g --std=c++11 -Wall -I/home/prash/PES/csp/csp-number-rep/include
SRC=src
OBJDIR=obj
BINDIR=bin
INC_DIR=include

MATRICES = int-matrix matrices
MATRICES_TSRC = $(addsuffix .cpp, $(MATRICES))
MATRICES_SRC = $(addprefix $(SRC)/, $(MATRICES_TSRC))

MATRICES_TOBJ = $(addsuffix .o, $(MATRICES))
MATRICES_OBJ = $(addprefix $(OBJDIR)/, $(MATRICES_TOBJ))

DEP = $(MATRICES_OBJ:.o=.d)

LIBS=m stdc++
LINKS=$(addprefix -l, $(LIBS))

all: setup assn1-cli assn1-gui

setup:
	mkdir -p $(OBJDIR) $(BINDIR)

#-include $(DEP)
$(OBJDIR)/%.o: $(SRC)/%.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@ $(LINKS) #-MM -MF $(patsubst %.o,%.d,$@)

assn1-cli: $(MATRICES_OBJ) $(NUMBER_REP_OBJ) $(FP_REP_OBJ) $(OBJDIR)/assn1-cli.o 
	$(CC) $(CXXFLAGS) $? -o $(BINDIR)/assn1-cli $(LINKS)

assn1-gui:

clean:
	rm -f $(OBJDIR)/*.o $(BINDIR)/*
	rm -r obj bin