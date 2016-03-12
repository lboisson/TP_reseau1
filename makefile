cc = gcc
CFLAGS = -Wall



SRC= $(wildcard ./*.c)
OBJ= $(SRC:.c=.o)
EXE= $(SRC:.c=)

NO_COLOR=\033[0m
OK_COLOR=\033[32;01m
ERROR_COLOR=\033[31;01m
WARN_COLOR=\033[33;01m

OK_STRING=$(OK_COLOR)[OK]$(NO_COLOR)
ERROR_STRING=$(ERROR_COLOR)[ERRORS]$(NO_COLOR)
WARN_STRING=$(WARN_COLOR)[WARNINGS]$(NO_COLOR)

OUTERROR=2> temp.log || touch temp.errors
DISPLAY=@if test -e temp.errors; then echo $@ "\t\t$(ERROR_STRING)" && cat temp.log; elif test -s temp.log; then echo $@ "\t\t$(WARN_STRING)\n" && cat temp.log; else echo $@ "\t\t$(OK_STRING)"; fi;rm -f temp.errors temp.log

all: $(EXE)

%.o: %.c # U CHEATER
	@$(CC) $(CFLAGS) -o $@ $< $(OUTERROR)
	$(DISPLAY)

clean:
	@rm -f $(EXE) $(OUTERROR)
	$(DISPLAY)
	@rm -f $(OBJ) $(OUTERROR)
	$(DISPLAY)
