/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* input_handler.c                                  :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: jbrol-ca <jbrol-ca@student.42.fr>          +#+  +:+       +#+        */
/* GPLv3+   +#+           */
/* Created: 2025/04/02 18:05:15 by hde-barr          #+#    #+#             */
/* Updated: 2025/04/20 20:57:00 by jbrol-ca         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_part2.h"

t_token	*delegated_by_input_handler(char *input, char **env)
{
	t_token	*first;
	t_token	*token;

	token = split_input(input, 0);
	first = token;
	quote_handler_call_loop(token, env);
	typealize_call_loop(token, env);
	merg_tok_excep_cll_revloop(token);
	return (first);
}

/*
** Helper to process the token list after initial splitting and error flagging.
** Includes variable assignments, expansion, args/file handling, and command finding.
** Returns the potentially modified token list head, or NULL on critical error.
*/
static t_token	*process_token_list_pipeline(t_shell *shell,
	t_token *token_list)
{
	process_variable_assignments(shell, token_list);
	expand_token_list_no_assignments(token_list, shell->env);
	token_list = handler_args_file(token_list, token_list);
	if (!token_list)
		return (NULL);
	parser_cmd_no_found(token_list, shell->env);
	return (token_list);
}

/*
** Helper to build and execute the AST from the processed token list.
** Assumes the token list is valid (non-NULL and no initial parse errors).
*/
static void	build_and_execute_ast(t_shell *shell, t_token *token_list)
{
	t_node_tree	*tree;

	tree = init_yggdrasil(token_list);
	if (tree)
	{
		execute_ast(shell, tree);
	}
	else if (g_exit_code == 0)
	{
	}
}

/*
** Handles the overall processing of user input.
** Tokenizes, processes tokens, builds AST, and executes.
** Returns the head of the processed token list for cleanup.
*/
t_token	*input_handler(t_shell *shell, char *input)
{
	t_token	*token_list;
	bool	parse_error_flagged_in_tokens;

	token_list = delegated_by_input_handler(input, shell->env);
	if (!token_list)
	{
		return (NULL);
	}
	parse_error_flagged_in_tokens = has_parser_error(token_list);
	if (!parse_error_flagged_in_tokens)
	{
		token_list = process_token_list_pipeline(shell, token_list);
		if (token_list)
		{
			build_and_execute_ast(shell, token_list);
		}
	}
	else
	{
		g_exit_code = 2;
	}
	return (token_list);
}
/*void input_handler(char **env, char *input) ---- FUNCAO ANTIGA
{
	t_token *token;
	t_node_tree *tree;

	token = delegated_by_input_handler(input, env);
	if(!token)
		return ;
	token = delegated_by_input_handler(input, env);
	token = handler_args_file(token, token);
	parser_cmd_no_found(token, env);
	//print_token_lst(token);
	tree = init_yggdrasil(token);
	//print_yggdrasil(tree, 0, "root:");
}*/