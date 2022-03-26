NAME	:= Matt_daemon
CC		:= clang++

CFLAGS	:= -Wall -Wextra -Werror -std=c++11 -pedantic -g
DFLAGS	= -MMD -MF $(@:.o=.d)

FILES	=	daemon.cpp main.cpp tintin_reporter.cpp utils.cpp

OBJS_DIR := ./obj/
SRCS_DIR := ./src/

SRCS	:=	$(addprefix $(SRCS_DIR), $(FILES))
OBJS	:=	$(addprefix $(OBJS_DIR), $(FILES:.cpp=.o))
DEPS	:=	$(addprefix $(OBJS_DIR), $(FILES:.cpp=.d))

.PHONY	: all
all		: $(NAME)

-include $(DEPS)
$(NAME)	: $(OBJS)
	@	printf "Compiling $(NAME)\n"
	@	$(CC) $(CFLAGS) $^ -o $@

$(OBJS_DIR)%.o : $(SRCS_DIR)%.cpp
	@	mkdir -p $(dir $@)
	@	printf "Compiling: $<"
	@	$(CC) $(CFLAGS) -c $< -o $@ $(DFLAGS)
	@	printf " -> OK\n"

.PHONY	: clean
clean	:
	@	printf "Deleting $(OBJS_DIR)\n"
	@	rm -rf $(OBJS_DIR)

	-docker image rm matt-daemon --force

.PHONY	: fclean
fclean	: clean
	@	printf "Deleting $(NAME) binary\n"
	@	rm -f $(NAME)

.PHONY	: re
re		: fclean all

.PHONY	: docker
docker	:
	@	printf "Building Docker image\n"
	docker build -t matt-daemon .

	@	printf "Running Docker image\n"
	docker run --rm -it matt-daemon

.PHONY	: lint
lint	:
	@	cpplint --recursive --root=src --filter=-whitespace/tab,-legal/copyright,-build/include_subdir $(SRCS_DIR)
