# ----------------------------------------Name
NAME = ircc

# ----------------------------------------Config
HOSTNAME = localhost
PORT = 6667
PASSWORD = hi

# ----------------------------------------Files
FILES_SRCS =	main.cpp \
				Client.cpp \
				Config.cpp \
				Msg.cpp \
				utils.cpp \

FILES_OBJS = $(FILES_SRCS:.cpp=.o)

# ----------------------------------------Directories
DIR_SRCS = ./src/
DIR_OBJS = ./obj/

vpath %.cpp $(DIR_SRCS) \
			$(DIR_SRCS)cmds/

# ----------------------------------------Sources
SRCS = $(FILES_SRCS:%=$(DIR_SRCS)%)

# ----------------------------------------Objects
OBJS = $(FILES_OBJS:%=$(DIR_OBJS)%)



# ----------------------------------------Flags
CC = c++
CFLAGS = -std=c++14
# CFLAGS += -Wall -Wextra -Werror
# CFLAGS += -g -fsanitize=address

INC =  -Iinc

# ----------------------------------------Debug
ifdef DEBUG_MODE
CFLAGS += -g -fsanitize=address
endif

# ----------------------------------------Making
all:
	@$(MAKE) $(NAME) -j4
.PHONY: all

$(NAME): $(DIR_OBJS) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(INC) $(LFLAGS)


$(DIR_OBJS)%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(INC) $(DFLAGS)

$(DIR_OBJS):
	mkdir -p $@

# ----------------------------------------Debug
debug:
	$(MAKE) re DEBUG_MODE=1
.PHONY: debug

# ----------------------------------------Logs
log:
	$(MAKE) re LOG_MODE=1
.PHONY: log

# ----------------------------------------Cleaning
clean:
	rm -f $(OBJS)
.PHONY: clean

fclean: clean
	rm -f $(NAME)
.PHONY: fclean

re: fclean all
.PHONY: re


# ----------------------------------------Util
run: all
	./$(NAME) $(HOSTNAME) $(PORT) $(PASSWORD)
.PHONY: run

