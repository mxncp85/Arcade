##
## EPITECH PROJECT, 2024
## arcade
## File description:
## Makefile
##

NCURSES_FLAGS = -lncurses
SFML_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system
ALL_FLAGS = ${NCURSES_FLAGS} ${SFML_FLAGS}

SNAKE_SRC = src/games/snake/Snake.cpp
SNAKE_OBJ = $(SNAKE_SRC:.cpp=.o)
SNAKE_NAME = arcade_snake.so

NCURSE_SRC = src/graphic/ncurses/Ncurses.cpp
NCURSE_OBJ = $(NCURSE_SRC:.cpp=.o)
NCURSE_NAME = arcade_ncurse.so

SFML_SRC = src/graphic/sfml/Sfml.cpp
SFML_OBJ = $(SFML_SRC:.cpp=.o)
SFML_NAME = arcade_sfml.so

NAME = arcade
SRC = 	src/core/Main.cpp	\
		src/core/Screen.cpp

OBJ = $(SRC:.cpp=.o)

%.o: %.cpp
	g++ -c $< -o $@ -fPIC

all: Sprites.hpp $(NAME) $(NCURSE_NAME) $(SFML_NAME) $(SNAKE_NAME)

$(NAME): $(OBJ)
	g++ -o $@ $^ ${ALL_FLAGS} -g

$(NCURSE_NAME): $(NCURSE_OBJ)
	g++ -shared -o $@ $^ $(NCURSES_FLAGS) -g

$(SFML_NAME): $(SFML_OBJ)
	g++ -shared -o $@ $^ $(SFML_FLAGS) -g

$(SNAKE_NAME): $(SNAKE_OBJ)
	g++ -shared -o $@ $^ $(SNAKE_FLAGS) -g

fclean:
	rm -rf *.o
	rm -rf src/games/menu/*.o
	rm -rf src/games/snake/*.o
	rm -rf src/graphic/ncurses/*.o
	rm -rf src/graphic/sfml/*.o
	rm -rf src/core/*.o
	rm -rf *.so
	rm -rf arcade

re: fclean all

Sprites.hpp: Assets/assets.csv
	@echo "Generating $@ from [$^]"
	@touch $@
	@chmod +w $@
	@echo -e "/*\n** EPITECH PROJECT, 2024\n** Arcade\n** File" \
	"description:\n** Auto-generated file that lists all available" \
	"assets\n** Do not modify by hand.\n*/" > $@
	@echo >> $@
	@echo '#include <array>' >> $@
	@echo '#include <string>' >> $@
	@echo >> $@
	@echo "#ifndef SPRITES_HPP_" >> $@
	@echo "#define SPRITES_HPP_" >> $@
	@echo >> $@
	@echo 'namespace arca {' >> $@
	@echo '    enum SpriteId {' >> $@
	@awk -F ',' '{print "        " $$2 " = " NR-1 ","}' $^ >> $@
	@echo >> $@
	@echo '        SPRITE_COUNT,' >> $@
	@echo '    };' >> $@
	@echo >> $@
	@echo '    const std::array<std::string, SPRITE_COUNT> assetNames = {' >> $@
	@awk -F ',' '{print "        " $$1 ","}' $^ >> $@
	@echo '    };' >> $@
	@echo '}' >> $@
	@echo >> $@
	@echo "#endif /* !SPRITES_HPP_ */" >> $@
	@chmod -w $@
