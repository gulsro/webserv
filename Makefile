NAME		=	Http
CC			=	c++

ifdef DEBUG
CPPFLAGS	=	-Wall -Wextra -Werror -fsanitize=address -g -std=c++17
else ifdef STRUCTOR
CPPFLAGS	=	-Wall -Wextra -Werror -g -D DEBUG -std=c++17
else ifdef FUNC
CPPFLAGS	=	-Wall -Wextra -Werror -g -D FUNC -std=c++17
else
CPPFLAGS	=	-Wall -Wextra -Werror
endif

SRC_FILE	=	HttpRequest.cpp HttpResponse.cpp HttpUtils.cpp httpParsing.cpp Methods.cpp main.cpp errorResponse.cpp

OBJ_DIR		=	obj/
OBJ			=	$(addprefix $(OBJ_DIR), $(SRC_FILE:.cpp=.o))

HEADERS		=	HttpRequest.hpp HttpResponse.hpp utils.hpp

CYAN_B		=	\033[1;96m
CYAN		=	\033[0;96m
GREEN_B		=	\033[1;32m
DEFAULT		=	\033[0m

all				:	$(NAME)

$(NAME)			: $(OBJ) $(OBJF)
					@$(CC) $(CPPFLAGS) -o $(NAME) $(OBJ)
					@echo	"$(CYAN_B) - HTTP is compiled - $(DEFAULT)"

$(OBJ_DIR)%.o	: %.cpp $(HEADERS) | $(OBJF)
					@mkdir -p $(@D)
					@echo	"$(CYAN_B) Compiling... $(DEFAULT)"
					@$(CC) $(CPPFLAGS) -c $< -o $@

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