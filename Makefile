# Updated Makefile with readline paths
NAME        = minishell
CC          = cc
##CFLAGS      = -I/usr/local/opt/readline/include -Iincludes -Ilibft
CFLAGS      = -Wall -Wextra -Werror -g -I/usr/local/opt/readline/include -Iincludes -Ilibft
LDFLAGS     = -L/usr/local/opt/readline/lib -lreadline -lncurses
#READLINE_PATH = /opt/homebrew/opt/readline
#CFLAGS += -I$(READLINE_PATH)/include
#LDFLAGS += -L$(READLINE_PATH)/lib -lreadline -lncurses

# Directories
INC         = includes
LIBFT_DIR   = libft
EXEC_DIR    = execution
PARSER_TL   = parser/tokenlst
PARSER_BT   = parser/binery_tree
PARSER_TREE = parser/tree
PARSER_QUOTE = parser/quote_handling
PARSER_CANDY = parser/candy
PARSER_DEVU = parser/dev_utils
PARSER_ENVAR = parser/envar
PARSER_EXC = parser/exceptions_msg
PARSER_HAND = parser/handlers
PARSER_LST = parser/lkd_lst
PARSER_QH = parser/quote_handling
PARSER_RLOOP = parser/readline_loop
PARSER_SIZEH = parser/size_hardler
PARSER_TL = parser/tokenlst
PARSER_UTILS = parser/utils
PARSER_YGG = parser/yggdrasil

SRCS = \
    main.c \
    $(EXEC_DIR)/executor.c \
    $(EXEC_DIR)/executor_dispatch.c \
    $(EXEC_DIR)/executor_extra.c \
    $(EXEC_DIR)/executor_fd_utils.c \
    $(EXEC_DIR)/executor_pipe_utils.c \
    $(EXEC_DIR)/executor_pipes.c \
    $(EXEC_DIR)/executor_redir_utils.c \
    $(EXEC_DIR)/executor_redirections.c \
    $(EXEC_DIR)/executor_simple_cmd.c \
    $(EXEC_DIR)/executor_signals.c \
    $(EXEC_DIR)/executor_utils_builtins.c \
    $(EXEC_DIR)/executor_utils_builtins_nav.c \
    $(EXEC_DIR)/executor_utils_env1.c \
    $(EXEC_DIR)/executor_utils_env2.c \
    $(EXEC_DIR)/executor_utils_path.c \
    $(EXEC_DIR)/executor_utils_shell.c \
    $(EXEC_DIR)/executor_utils_sort.c \
    $(EXEC_DIR)/executor_utils_unset.c \
    $(EXEC_DIR)/expander.c \
    $(EXEC_DIR)/expander_utils.c \
    $(EXEC_DIR)/expander_utils2.c \
    $(EXEC_DIR)/expander_variable_assignment.c \
    $(PARSER_BT)/binery_tree.c \
    $(PARSER_CANDY)/rare_candy.c \
    $(PARSER_DEVU)/print_token_lst.c \
    $(PARSER_DEVU)/print_yggdrasil.c \
    $(PARSER_EXC)/parser_cmd_no_found.c \
    $(PARSER_HAND)/handler_args_file.c \
    $(PARSER_HAND)/handler_ctrl_c.c \
    $(PARSER_HAND)/handler_pipes.c \
    $(PARSER_HAND)/input_handler.c \
    $(PARSER_LST)/lkd_lst1.c \
    $(PARSER_LST)/lkd_lst2.c \
    $(PARSER_QH)/quote_expansion.c \
    $(PARSER_QH)/quote_handling.c \
    $(PARSER_QH)/merge_to_token_exception.c \
    $(PARSER_RLOOP)/readline_loop.c \
    $(PARSER_SIZEH)/size_hardler.c \
    $(PARSER_TL)/tokenlst.c \
    $(PARSER_TL)/tokenlst_split_input.c \
    $(PARSER_TL)/tokenlst_v2.c \
    $(PARSER_TL)/token_lst_helpers.c \
    $(PARSER_TL)/init_node0.c \
    $(PARSER_TL)/init_node1.c \
    $(PARSER_TL)/who_is_node.c \
    $(PARSER_TREE)/init_tree.c \
    $(PARSER_TREE)/tree.c \
    $(PARSER_UTILS)/utils1.c \
    $(PARSER_UTILS)/utils2.c \
    $(PARSER_YGG)/init_yggdrasil.c \
    $(PARSER_YGG)/yggdrasil.c
    ##$(PARSER_ENVAR)/envar0.c \
    $(PARSER_ENVAR)/envar1.c \
    $(PARSER_ENVAR)/envar2.c \            

OBJS        = $(SRCS:.c=.o)
LIBFT       = $(LIBFT_DIR)/libft.a

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) -I $(INC) -I $(LIBFT_DIR) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

%.o: %.c
	$(CC) $(CFLAGS) -I $(INC) -I $(LIBFT_DIR) -c $< -o $@

clean:
	@rm -f $(OBJS)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re