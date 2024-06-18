NAME		=	Http
CC			=	c++

ifdef DEBUG
CPPFLAGS	=	-Wall -Wextra -Werror -fsanitize=address -g -std=c++20
else ifdef STRUCTOR
CPPFLAGS	=	-Wall -Wextra -Werror -g -D DEBUG -std=c++20
else ifdef FUNC
CPPFLAGS	=	-Wall -Wextra -Werror -g -D FUNC -std=c++20
else
CPPFLAGS	=	-Wall -Wextra -Werror -std=c++20
endif

HEADER_DIR	=	includes/
HEADER_SRC	=	HttpRequest.hpp HttpResponse.hpp utils.hpp Location.hpp Config.hpp Server.hpp Webserv.hpp
HEADERS		=	$(addprefix $(HEADER_DIR), $(HEADER_SRC))

SRC_DIR		=	src/
SRC_FILE	=	HttpUtils.cpp  main.cpp Config.cpp Server.cpp Location.cpp \
				request/httpParsing.cpp \
				request/HttpRequest.cpp \
				request/multiPart.cpp \
				response/errorResponse.cpp \
				response/HttpResponse.cpp \
				response/Methods.cpp \

OBJ_DIR		=	obj/
OBJ			=	$(addprefix ${OBJ_DIR}, ${SRC_FILE:%.cpp=%.o})


INCLUDES	= -I$(HEADER_DIR)

CYAN_B		=	\033[1;96m
CYAN		=	\033[0;96m
GREEN_B		=	\033[1;32m
DEFAULT		=	\033[0m

all				:	$(NAME)

$(NAME)			: $(OBJ) $(OBJF)
					@$(CC) $(CPPFLAGS) -o $(NAME) $(OBJ)
					@echo	"$(CYAN_B) - HTTP is compiled - $(DEFAULT)"

$(OBJ_DIR)%.o	:$(SRC_DIR)%.cpp $(HEADERS) | $(OBJF)
					@mkdir -p $(@D)
					@echo	"$(CYAN_B) Compiling... $(DEFAULT)"
					@$(CC) $(CPPFLAGS) $(INCLUDES) -c $< -o $@

$(OBJF)			:
					@mkdir -p $(OBJ_DIR)

clean			:
					@rm -rf $(OBJ_DIR)
					@echo "$(CYAN)- Object files are cleaned - $(DEFAULT)"

fclean			: clean
					@rm -f $(NAME)
					@echo "$(CYAN)- Executable files are cleaned - $(DEFAULT)"

re				:	fclean all

.PHONY: all clean fclean re