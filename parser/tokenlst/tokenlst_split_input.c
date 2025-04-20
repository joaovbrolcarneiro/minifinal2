/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* tokenlst_split_input.c                             :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/* GPLv3+   +#+           */
/* Created: 2025/04/16 23:55:00 by hde-barr          #+#    #+#             */
/* Updated: 2025/04/20 20:30:00 by hde-barr         ###   ########.fr       */ // (Update date)
/* */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_part2.h"

static int	get_unquoted_token_len(char *input)
{
	int		i;
	char	start_char;

	i = ft_strsetlen(input, "\"' |<>");
	start_char = *input;
	if (i == 0 && ischarset("|<>", start_char))
	{
		i = 1;
		if (ischarset("<>", start_char) && input[1] == start_char)
			i = 2;
	}
	else if (i == 0 && *input != '\0')
		i = 1;
	else if (i == 0 && *input == '\0')
		return (0);
	if (i == 0)
		i = 1;
	return (i);
}

static int	get_token_len(char *input)
{
	int		i;
	char	start_char;

	i = 1;
	start_char = *input;
	if (ischarset("\"'", start_char))
	{
		while (input[i] && input[i] != start_char)
			i++;
		if (input[i] == start_char)
			i++;
		else
			i = 1;
	}
	else
		i = get_unquoted_token_len(input);
	return (i);
}

long long	get_new_token_id(void)
{
	static long long	id = 0;

	return (id++);
}

static t_token	*create_and_init_token(char *input, int start, int token_len)
{
	t_token	*new_token;

	new_token = hb_malloc(sizeof(t_token));
	if (!new_token)
	{
		perror("konosubash: split_input: hb_malloc failed");
		return (NULL);
	}
	ft_memset(new_token, 0, sizeof(t_token));
	new_token->value = ft_substr(input, start, token_len);
	if (!new_token->value)
	{
		perror("konosubash: split_input: ft_substr failed");
		free(new_token);
		return (NULL);
	}
	new_token->id = get_new_token_id();
	new_token->type = TOKEN_WORD;
	new_token->coretype = TOKEN_WORD;
	new_token->rank = RANK_C;
	new_token->used = false;
	new_token->err = 0;
	new_token->literal = false;
	new_token->merge_next = proximity_exception(input, start + token_len);
	new_token->next = NULL;
	return (new_token);
}

static void	add_token_to_list(t_token **first, t_token **lst,
	t_token *new_token)
{
	if (!*first)
	{
		*first = new_token;
		*lst = *first;
	}
	else
	{
		(*lst)->next = new_token;
		*lst = new_token;
	}
}

static int	process_single_token_block(char *input, int *start,
	t_token **first, t_token **lst)
{
	int		token_len;
	t_token	*new_token;

	while (input[*start] && input[*start] == ' ')
		(*start)++;
	if (!input[*start])
		return (1);
	token_len = get_token_len(input + *start);
	if (token_len <= 0)
	{
		ft_putstr_fd("konosubash: split_input: Error or zero token len\n", 2);
		return (-1);
	}
	new_token = create_and_init_token(input, *start, token_len);
	if (!new_token)
		return (-1);
	add_token_to_list(first, lst, new_token);
	(*start) += token_len;
	return (0);
}

t_token	*split_input(char *input, int unused_i)
{
	t_token	*first;
	t_token	*lst;
	int		start;
	int		status;

	(void)unused_i;
	first = NULL;
	lst = NULL;
	start = 0;
	while (input[start])
	{
		status = process_single_token_block(input, &start, &first, &lst);
		if (status == -1)
		{
			free_token_list(first);
			g_exit_code = 1;
			return (NULL);
		}
		if (status == 1)
			break ;
	}
	return (first);
}

void	free_token_list(t_token *list)
{
	t_token	*current;
	t_token	*next;

	current = list;
	while (current)
	{
		next = current->next;
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
}