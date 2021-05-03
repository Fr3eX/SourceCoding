.PHONY: all ,clean,clean_test
.SUFFIXES:



CC=gcc
CFLAGS=
LDFLAGS=
DEBUG=

RM=rm


SRC=src
OBJ=obj
TEST=tests
BIN_DIR=bin
TEST_BINDIR=$(TEST)/$(BIN_DIR)

SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SRCS))
TESTS=$(wildcard $(TEST)/$(SRC)/*.c)


TEST_BINS=$(patsubst $(TEST)/$(SRC)/%.c,$(TEST_BINDIR)/%,$(TESTS))
BIN=$(BIN_DIR)/prog


all:$(BIN)

release: CFLAGS=-Wall -O2 -DNDEBUF
release: clean
release: $(BIN)

test:$(OBJS) $(TEST_BINS)
	for test in $(TEST_BINS);do ./$$test;done

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $@

$(OBJ)/%.o:$(SRC)/%.c $(SRC)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/%.o:$(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@



$(TEST_BINDIR)/%:$(TEST)/$(SRC)/%.c
	$(CC) $(CFLAGS) $< $(OBJS) -o $@ -lcriterion	

clean_test:
	$(RM) $(TEST_BINS)
clean:
	$(RM) $(OBJS) $(BIN)
