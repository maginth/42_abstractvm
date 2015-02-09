NAME = avm
IPATH = ./header/
SPATH = ./src/
OPATH = ./obj/
SRCS = main.cpp

V ?= 0

D ?= 0

SILENCE_1 :=
SILENCE_0 :=@
SILENCE = $(SILENCE_$(V))

DEBUG_1 = -g
DEBUG_0 =


CFLAGS = $(DEBUG_$(D)) -Wall -Werror -Wextra
CC = $(SILENCE)g++
RM = $(SILENCE)rm -rf

SRC = $(addprefix $(SPATH), $(SRCS))
OBJS= $(SRC:$(SPATH)%.cpp=$(OPATH)%.o)

SKIP_1 :=
SKIP_0 := \033[A
SKIP = $(SKIP_$(V))
C = \033[1;34m
U = $(C)[$(NAME)]----->\033[0m

all: $(NAME)

$(NAME): $(OPATH) $(OBJS)
	@echo "$(U)$(C)[COMPILE:\033[1;32m DONE$(C)]\033[0m"
	@echo "\033[0;32m"
	$(CC) -o $@ $(CFLAGS) -I$(IPATH) $(OBJS)
	@echo "\033[1;31m [.working.]"
	@echo "$(SKIP)\033[A\033[2K$(SKIP)"
	@echo "$(SKIP)$(U)$(C)[BUILD  :\033[1;32m DONE$(C)]\033[0m"

$(OPATH):
	mkdir -p $(OPATH)

$(OPATH)%.o: $(SPATH)%.cpp
	@echo "$(U)$(C)[COMPILE: \033[1;31m$<\033[A\033[0m"
	@echo "\033[0;32m"
	$(CC) -o $@ $(CFLAGS) -I$(IPATH) -c $<
	@echo "\033[1;31m [.working.]"
	@echo "$(SKIP)\033[A\033[2K$(SKIP)"

clean:
	@echo "$(U)$(C)[CLEAN]\033[0;32m"
	$(RM) $(OBJS)
	@echo "$(SKIP)$(U)$(C)[CLEAN:\033[1;32m   DONE$(C)]\033[0m"

fclean: clean
	@echo "$(U)$(C)[F-CLEAN]\033[1;32m"
	$(RM) $(NAME)
	@echo "$(SKIP)$(U)$(C)[F-CLEAN:\033[1;32m DONE$(C)]\033[0m"

re: fclean all

.PHONY: clean fclean