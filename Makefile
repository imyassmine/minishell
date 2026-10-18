# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ LAYOUT ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

DEFAULT			= \033[0m
GREEN			= \033[1;32m
CYAN			= \033[1;36m


define ANIMATION_FRAME
				@for frame in ⠋ ⠙ ⠹ ⠸ ⠼ ⠴ ⠦ ⠧ ⠇ ⠏; do \
				printf "$$frame $(1)...\r"; \
				sleep 0.02; \
				done; \
				printf "\n"
endef


# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ VARIABLES ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

NAME =			minishell
CC =			cc
CFLAGS =		-Wall -Wextra -Werror -I include -I $(LIB_DIR)/include $(EXTRA_FLAGS)

SRC_DIR =		src/
OBJ_DIR =		obj/
LIB_DIR =		libft-extended/

LIB =			$(LIB_DIR)libft.a
READLINE =		-lreadline

SRC =			$(wildcard $(SRC_DIR)*.c $(SRC_DIR)*/*.c)
OBJ =			$(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)%.o,$(SRC))


# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ RULES ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

all:			$(NAME)

$(OBJ_DIR)%.o:	$(SRC_DIR)%.c
				@mkdir -p $(dir $@)
				@$(CC) $(CFLAGS) -c $< -o $@

$(NAME):		$(OBJ)
				@make all --no-print-directory -C $(LIB_DIR)
				$(call ANIMATION_FRAME,Building $(NAME) executable)
				@$(CC) $(CFLAGS) $(OBJ) $(LIB) $(READLINE) -o $(NAME)
				@printf "$(GREEN)✓$(DEFAULT) Built successfully ($(words $?)/$(words $(SRC)) files recompiled)\n"

clean:
				@make clean --no-print-directory -C $(LIB_DIR)
				$(call ANIMATION_FRAME,Cleaning objects)
				@rm -rf $(OBJ_DIR)

fclean:			clean
				$(call ANIMATION_FRAME,Removing executable and library)
				@rm -f $(NAME) $(LIB)

re:				fclean all

debug:
				@$(MAKE) fclean --no-print-directory
				@$(MAKE) all --no-print-directory EXTRA_FLAGS="-g3 -fsanitize=address,undefined"

test:			$(NAME)
				
				@python3 tests/smoke_test.py $(NAME)

help:
				@printf "Usage: make [target]\n"
				@printf "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n"
				@printf "$(CYAN)all$(DEFAULT)		- Build the executable $(NAME)\n"
				@printf "$(CYAN)clean$(DEFAULT)		- Clean up object files\n"
				@printf "$(CYAN)fclean$(DEFAULT)		- Clean up all object files and executable\n"
				@printf "$(CYAN)re$(DEFAULT)		- Rebuild the entire project\n"
				@printf "$(CYAN)debug$(DEFAULT)		- Rebuild with ASan + UBSan\n"
				@printf "$(CYAN)test$(DEFAULT)		- Run the automated test suite\n"
				@printf "$(CYAN)help$(DEFAULT)		- Show this help message\n"

.PHONY:			all clean fclean re debug help test
