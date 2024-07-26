#-------------------------------------  COLORS  --------------------------------
RESET			= \033[0m
BLACK 			= \033[1;30m
RED 			= \033[1;31m
GREEN 			= \033[1;32m
YELLOW		 	= \033[1;33m
BLUE			= \033[1;34m
PURPLE 			= \033[1;35m
CYAN 			= \033[1;36m
WHITE 			= \033[1;37m
CLEAR           = \033[2J\033[H

#----------------------------------- COMMANDS ----------------------------------
CXX				= c++
RM 				= rm -rf

#-----------------------------------  FLAGS  -----------------------------------
CXXFLAGS		= -Wall -Wextra -Werror -std=c++98
NPD				= --no-print-directory

#--------------------------------- FILES  ---------------------------------------
NAME 			= ircserv
INCLUDE			= includes
OBJDIR			= obj
SRCS 			= src
_SUBFOLDERS		= Auxs
VPATH			= $(SRCS) $(addprefix $(SRCS)/, $(_SUBFOLDERS))

_FILES 			= Client ClientAux Server ServerAux IRCException Aux Channel

OBJ				= $(_FILES:%=%.o)
TARGET			= $(addprefix $(OBJDIR)/, $(OBJ))
_HEADERS		= Client.hpp Server.hpp Channel.hpp IRCException.hpp Includes.hpp Macros.hpp
HDR				= $(addprefix $(INCLUDE)/, $(_HEADERS))

#---------------------------------  RULES  --------------------------------------

all: $(NAME)

$(NAME): $(OBJDIR) $(TARGET) main.cpp
	$(CXX) $(CXXFLAGS) main.cpp  $(TARGET) -I $(INCLUDE) -o $(NAME)
	echo "[$(CYAN)Compiling$(RESET)] $(CXX) $(CXXFLAGS) main.cpp $(RESET)"
	echo "[$(GREEN)Success$(RESET)] $(NAME) created!$(BOLD)$(RESET)"

$(OBJDIR)/%.o : %.cpp $(HDR)
	echo "[$(CYAN)Compiling$(RESET)] $(CXX) $(CXXFLAGS) $<$(RESET)"
	$(CXX) $(CXXFLAGS) -c $< -o $@ -I $(INCLUDE)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	$(RM) $(OBJDIR)
	echo "[$(RED)Deleting$(RESET)]  object files deleted$(BOLD)$(RESET)"

fclean: clean
	$(RM) $(NAME)
	echo "[$(RED)Deleting$(RESET)]  .a deleted$(BOLD)"

re: fclean all

run: all
	clear && ./ircserv 8080 123

.SILENT:

.PHONY: all clean fclean re