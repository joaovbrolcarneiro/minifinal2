/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* quote_expansion.c                                :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/* GPLv3+   +#+           */
/* Created: 2025/04/16 20:45:00 by hde-barr          #+#    #+#             */
/* Updated: 2025/04/16 20:55:00 by hde-barr         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_part2.h"

static void	count_expansion_part(char *input, char **env, int *i, int *count)
{
	int		start;
	char	*substr;
	char	*expanded;

	start = *i + 1;
	(*i)++;
	while (input[*i] && !ischarset(QUOTE_SET, input[*i]))
		(*i)++;
	substr = ft_substr(input, start, *i - start);
	if (!substr)
		return ;
	expanded = domane_expantion(env, substr);
	*count += ft_strlen(expanded);
	// free(substr); // free?
}

int	quote_handler_counter(char *input, char **env)
{
	int	i;
	int	counter;

	i = 0;
	counter = 0;
	if (!input)
		return (0);
	while (input[i])
	{
		if (input[i] == '$')
			count_expansion_part(input, env, &i, &counter);
		else
		{
			i++;
			counter++;
		}
	}
	return (counter);
}

static bool	copy_expansion_part(t_exp_cpy_vars *v, char *input, char **env)
{
	char	*substr;
	char	*expanded;
	size_t	exp_len;

	v->start = v->i + 1;
	(v->i)++;
	while (input[v->i] && !ischarset(QUOTE_SET, input[v->i]))
		(v->i)++;
	substr = ft_substr(input, v->start, v->i - v->start);
	if (!substr)
		return (false);
	expanded = domane_expantion(env, substr);
	exp_len = ft_strlen(expanded);
	ft_strlcat(v->dst, expanded, v->count + exp_len + 1);
	v->count += exp_len;
	// free(substr); // free?
	return (true);
}

static bool	copy_and_expand_loop(t_exp_cpy_vars *v, char *input, char **env)
{
	bool	success;

	success = true;
	while (input && input[v->i] && success)
	{
		if (input[v->i] == '$')
		{
			success = copy_expansion_part(v, input, env);
		}
		else
		{
			v->dst[v->count] = input[v->i];
			(v->count)++;
			(v->i)++;
		}
	}
	return (success);
}

char	*quote_handler_cpy(int total_len, char *input, char **env)
{
	t_exp_cpy_vars	v;
	char			*trimmed_input;
	char			*result;

	trimmed_input = ft_strtrim(input, " ");
	if (trimmed_input && ft_strcmp(trimmed_input, "$") == 0)
	{
		result = ft_strdup("$");
		// free(trimmed_input); // free?
		return (result);
	}
	// free(trimmed_input); // free?
	v.dst = ft_calloc(sizeof(char), total_len + 1);
	if (!v.dst)
	{
		perror("minishell: calloc quote_handler_cpy");
		return (NULL);
	}
	v.i = 0;
	v.count = 0;

	if (!copy_and_expand_loop(&v, input, env))
	{
		// free(v.dst); // free?
		return (NULL);
	}
	v.dst[v.count] = '\0';
	return (v.dst);
}