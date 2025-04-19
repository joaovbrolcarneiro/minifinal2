/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* main.c                                             :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/04/15 00:00:00 by hde-barr          #+#    #+#             */
/* Updated: 2025/04/15 00:00:01 by hde-barr         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_part2.h"

int	g_exit_code = 0;

/*
** konopwd:
** - If the trimmed input equals "pwd", prints the current directory.
*/

void	typealize_call_loop(t_token *token, char **env)
{
	t_token	*current_token;

	current_token = token;
	while (current_token)
	{
		typealize(current_token, env);
		current_token = current_token->next;
	}
}

void	merg_tok_excep_cll_revloop(t_token *token)
{
	t_token	*first;
	t_token	*current;

	first = token;
	current = last_token(first);
	while (current && current != first)
	{
		token = get_prev_node(current, first);
		if (token)
			merge_to_token_exception(token);
		current = token;
	}
	if (first)
		merge_to_token_exception(first);
}

void	quote_handler_call_loop(t_token *token, char **env)
{
	int	is_unclosed;

	while (token)
	{
		is_unclosed = 0;
		token->value = quote_handler(token, env, &is_unclosed);
		if (is_unclosed == 1)
			token->err = 1;
		else
			token->err = 0;
		token = token->next;
	}
}

/*
** main:
*/
int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	g_exit_code = 0;
	init_shell(&shell, envp);
	signal(SIGINT, handle_ctrl_c);
	signal(SIGQUIT, SIG_IGN);
	readline_loop(&shell);
	cleanup_shell(&shell);
	return (g_exit_code);
}

bool	is_builtin(const char *cmd)
{
	if (!cmd)
		return (false);
	if (ft_strcmp(cmd, "echo") == 0 || \
		ft_strcmp(cmd, "cd") == 0 || \
		ft_strcmp(cmd, "pwd") == 0 || \
		ft_strcmp(cmd, "export") == 0 || \
		ft_strcmp(cmd, "unset") == 0 || \
		ft_strcmp(cmd, "exit") == 0)
	{
		return (true);
	}
	return (false);
}