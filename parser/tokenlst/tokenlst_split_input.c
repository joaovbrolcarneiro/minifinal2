/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* tokenlst_split_input.c                           :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/* */ /* Updated: 2025/04/16 23:55:00 by hde-barr         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_part2.h" // For proximity_exception, ischarset etc.

/* Calculates length for non-quoted tokens (operators, words) */
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

/* Calculates the length of the next token */
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

/* Initializes fields of a newly allocated token */
static void	init_token_fields(t_token *lst, char *val, char *in_seg, int len)
{
	lst->value = val;
	lst->merge_next = (!ischarset("|<>", in_seg[len]) && \
						in_seg[len] != ' ' && \
						in_seg[len] && \
						!ischarset("|<>", in_seg[0]));
	lst->args = NULL;
	lst->file = NULL;
	lst->err = 0;
	lst->literal = false;
	lst->coretype = TOKEN_WORD;
	lst->rank = RANK_C;
	lst->used = false;
	lst->id = get_token_id();
	lst->next = NULL;
}

/* Creates token, initializes, allocates next node, links, advances lst ptr */
/* Returns 0 on success, 1 on failure */
static int	create_link_token(t_token **lst_ptr, char *input_segment, int len)
{
	t_token	*lst;
	char	*token_value;

	lst = *lst_ptr;
	token_value = ft_substr(input_segment, 0, len);
	if (!token_value)
		return (perror("ft_substr split_input"), 1);
	init_token_fields(lst, token_value, input_segment, len);
	lst->next = malloc(sizeof(t_token));
	if (!lst->next)
	{
		perror("malloc next split_input");
		free(lst->value);
		lst->value = NULL;
		return (1);
	}
	ft_memset(lst->next, 0, sizeof(t_token));
	*lst_ptr = lst->next;
	return (0);
}

/* Main token splitting function */
t_token	*split_input(char *input, int i) /////////split_input ( mexi-joao)
{
	t_token	*lst;
	t_token	*first;
	int		token_len;

	lst = malloc(sizeof(t_token));
	if (!lst)
		return (perror("malloc split_input"), NULL);
	ft_memset(lst, 0, sizeof(t_token));
	first = lst;
	(void)i;
	while (input && *input)
	{
		while (*input && *input == ' ')
			input++;
		if (!*input)
			break ;
		token_len = get_token_len(input);
		if (token_len == 0)
			break ;
		if (create_link_token(&lst, input, token_len) != 0)
			return (/* TODO: free_tokens(first), */ NULL);
		input += token_len;
	}
	return (finalize_list(first, lst));
}