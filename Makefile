NAME = tasks

SRCDIR = ./srcs
INCLUDE_DIR = ./include
SRCS = $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/impl/*.cpp)

OBJ = $(SRCS:.cpp=.o)

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -I$(INCLUDE_DIR) -std=c++17 -fsanitize=address -g3

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
