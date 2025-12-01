NAME = tasks

SRCDIR = ./srcs
INCLUDE_DIR = ./include
SRCS = $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/impl/*.cpp)

OBJ = $(SRCS:.cpp=.o)

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -I$(INCLUDE_DIR) -std=c++17 -fsanitize=address -g3

INSTALL_BIN_DIR := $(HOME)/.local/bin
INSTALL_DATA_DIR := $(HOME)/.local/share/tvsk

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

install: all
	@mkdir -p $(INSTALL_BIN_DIR)
	@mkdir -p $(INSTALL_DATA_DIR)/art
	cp $(NAME) $(INSTALL_BIN_DIR)/$(NAME)
	cp art/tvsk! $(INSTALL_DATA_DIR)/art/tvsk!
	cp tasks $(INSTALL_DATA_DIR)/.tasks
	@echo "Installation complete: $(NAME) installed to $(INSTALL_BIN_DIR)"
	@echo "Data files installed to $(INSTALL_DATA_DIR)"

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all install clean fclean re
