#COLORS
RED					:=	\033[0;31m
GREEN				:=	\033[0;32m
YELLOW				:=	\033[0;33m
RESET_COLOR			:=	\033[0m

NAME = webserv

FLAGS = -Wall -Werror -Wextra -std=c++17

FILES =	main.cpp \
		Server.cpp \
		ServerManager.cpp \
		Config.cpp \
		#Location.cpp \
		#Socket.cpp


HEADER = include

OBJ_DIR = obj
SRC_DIR = src

SRC = $(addprefix ${SRC_DIR}/, $(FILES))
OBJ = $(addprefix ${OBJ_DIR}/, ${FILES:%.cpp=%.o})
RM		= rm -rf

all: $(NAME)

$(NAME): $(OBJ)
	@c++ $(FLAGS) -o $(NAME) $(OBJ)
	@echo "\n${GREEN}Done !${RESET_COLOR}"

${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp
	@mkdir -p ${@D}
	@printf "${YELLOW}-> Generate %-38.38s\r${RESET_COLOR}" $@
	@c++ $(FLAGS) -I $(HEADER) -c $< -o $@

clean:
	@${RM} -r ${OBJ_DIR}
	@rm -f $(OBJ)
	@printf "${RED}Obj is deleted\n${RESET_COLOR}"
	
fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re