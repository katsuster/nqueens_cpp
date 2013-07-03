
CXX = g++
CPPFLAGS = -Wall -O2 -g
LDFLAGS = -lpthread
RM = rm -f

TARGETS = nqueens

NQUEENS_OBJS = nqueens.o

all: $(TARGETS)

nqueens: $(NQUEENS_OBJS)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -o $@ $^

clean: 
	$(RM) $(TARGETS)
	$(RM) $(NQUEENS_OBJS)

.PHONY: clean
