/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* tokenlst_v2.c                                    :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: jbrol-ca <jbrol-ca@student.42.fr>          +#+  +:+       +#+        */
/* GPLv3+   +#+           */
/* Created: 2025/03/31 21:05:42 by hde-barr          #+#    #+#             */
/* Updated: 2025/04/17 19:32:41 by jbrol-ca         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_part2.h"

bool	proximity_exception(char *input, int i)
{
	if ((!ischarset("|<>", input[i]) && input[i] != ' ') && \
		input[i] && !ischarset("|<>", *input))
		return (true);
	return (false);
}

/*
** ///////////////////////////////////////////// MEXI NESSA FUNCAO /////////
** Helper function to determine and assign the correct token type and properties.
*/
t_token	*typealize(t_token *token, char **env)
{
	if (token->type == TOKEN_ASSIGNMENT) // coloquei isso de resto mudei nada
		return (token);
	if (is_pipe_super(token->value))
		return (new_token_super(new_pipe(token)));
	if (is_redir_super(token->value))
		return (new_token_super(new_redir(token)));
	if (is_cmd_super(token->value, env))
		return (new_token_super(new_cmd(token)));
	if (is_word_super(token->value))
		return (new_token_super(new_word(token)));
	if (is_eof(token->value))
		return (new_token_super(new_eof(token)));
	return (token);
}

/* A FUNCAO ABAIXO AGORA ESTA EM TOKENLST_SPLIT_INPUT
t_token	*split_input(char *input, int i) /////////split_input ( mexi-joao)
{
	t_token	*lst;
	t_token	*first;
	char	start_char;

	lst = malloc(sizeof(t_token));
	if (!lst)
		return (perror("malloc split_input"), NULL);
	first = lst;
	while (*input)
	{
		i = 1;
		while (*input && *input == ' ')
			input++;
		if (!*input)
		{
			lst->next = NULL;
			lst->value = NULL;
			break ;
		}
		start_char = *input;
		if (ischarset("\"'", start_char))
		{
			i = 1;
			while (input[i] && input[i] != start_char)
				i++;
			if (input[i] == start_char)
				i++;
			else
				i = 1;
		}
		else
		{
			i = ft_strsetlen(input, "\"' |<>");
			if (i == 0 && ischarset("|<>", start_char))
			{
				i = 1;
				if (ischarset("<>", start_char) && input[1] == start_char)
					i = 2;
			}
			else if (i == 0 && *input != '\0')
				i = 1;
			else if (i == 0 && *input == '\0')
				break ;
		}
		if (i == 0)
			i = 1;
		lst->merge_next = proximity_exception(input, i);
		lst->value = ft_substr(input, 0, i);
		if (!lst->value)
		{
			perror("ft_substr split_input");
			break ;
		}
		input += i;
		if (*input)
		{
			lst->next = malloc(sizeof(t_token));
			if (!lst->next)
			{
				perror("malloc next split_input");
				free(lst->value);
				lst->value = NULL;
				break ;
			}
			lst = lst->next;
		}
		else
		{
			lst->next = NULL;
			break ;
		}
	}
	if (lst && !*input)
		lst->next = NULL;
	if (!first || !first->value) 
	{
		if (first)
			free(first);
		return (NULL);
	}
	return (first);
}*/