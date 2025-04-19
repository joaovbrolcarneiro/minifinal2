/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrol-ca <jbrol-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 18:05:15 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/17 17:52:03 by jbrol-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_part2.h"


t_token	*delegated_by_input_handler(char *input, char **env) // ALTEREI - JOAO
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
t_token *delegated_by_input_handler(char *input, char **env) -- FUNCAO ANTIGA
{
	t_token *first;
    t_token *token;

	if( is_too_long_input_exption(input))
		return (NULL);
    token = split_input(input, 0);
	first = token;
	quote_handler_call_loop(token, env);
	typealize_call_loop(token, env);
	merg_tok_excep_cll_revloop(token);
	return (token);
}*/

void	input_handler(t_shell *shell, char *input) // - ALTEREI JOAO
{
	t_token		*token;
	t_node_tree	*tree;
	bool		parse_error;

	tree = NULL;
	token = delegated_by_input_handler(input, shell->env);
	parse_error = has_parser_error(token);
	if (!parse_error)
	{
		process_variable_assignments(shell, token);
		expand_token_list_no_assignments(token, shell->env);
		token = handler_args_file(token, token);
		parser_cmd_no_found(token, shell->env);
		tree = init_yggdrasil(token);
		if (tree)
			execute_ast(shell, tree);
		else if (!has_parser_error(token))
			g_exit_code = 0;
	}
	else
	{
		g_exit_code = 2;
	}
	// free_tokens(token); free?
	// free_ast(tree); free?
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