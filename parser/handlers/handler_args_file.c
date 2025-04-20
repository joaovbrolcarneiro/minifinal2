/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* handler_args_file.c                               :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/* GPLv3+   +#+           */
/* Created: 2025/04/02 17:38:59 by hde-barr          #+#    #+#             */
/* Updated: 2025/04/19 15:26:14 by jbrol-ca         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_part2.h"

static char	**realloc_args_array(char **old_args, int old_argc,
	const char *new_arg)
{
	char	**new_args;
	int		i;

	new_args = malloc(sizeof(char *) * (old_argc + 2));
	if (!new_args)
		return (perror("konosubash: malloc args array"), NULL);
	i = 0;
	while (i < old_argc)
	{
		new_args[i] = old_args[i];
		i++;
	}
	new_args[i] = ft_strdup(new_arg);
	if (!new_args[i])
	{
		perror("konosubash: strdup new arg");
		free(new_args);
		return (NULL);
	}
	new_args[i + 1] = NULL;
	if (old_args)
		free(old_args);
	return (new_args);
}

static int	create_first_arg(t_token *priv, t_token *arg_token)
{
	char	**new_args;

	if (!priv || !priv->value || !arg_token || !arg_token->value)
		return (1);
	new_args = malloc(sizeof(char *) * 3);
	if (!new_args)
		return (perror("konosubash: malloc first arg"), 1);
	new_args[0] = priv->value;
	new_args[1] = ft_strdup(arg_token->value);
	if (!new_args[1])
		return (perror("konosubash: strdup first arg"), free(new_args), 1);
	new_args[2] = NULL;
	if (priv->args)
		free(priv->args);
	priv->args = new_args;
	return (0);
}

void	join_and_split(t_token *priv, t_token *arg_token)
{
	int		argc;
	char	**temp_args;

	if (!priv || !arg_token || !arg_token->value)
		return ;
	if (priv->args == NULL)
	{
		if (create_first_arg(priv, arg_token) != 0)
			priv->args = NULL;
	}
	else
	{
		argc = 0;
		while (priv->args[argc] != NULL)
			argc++;
		temp_args = realloc_args_array(priv->args, argc, arg_token->value);
		if (!temp_args)
			priv->args = NULL;
		else
			priv->args = temp_args;
	}
}

static t_token	*attach_args_to_token(t_token *start_token, t_token *first)
{
	t_token	*arg_token;
	t_token	*current;

	current = start_token->next;
	while (current && current->rank != RANK_S)
	{
		if (current->coretype == REDIR)
			break ;
		arg_token = rm_node_lst(current, first);
		if (!arg_token)
			break ;
		join_and_split(start_token, arg_token);
		if (!start_token->args)
			break ;
		current = start_token->next;
	}
	return (current);
}

t_token	*cmd_handler_args(t_token *token, t_token *first)
{
	t_token	*current;

	current = token;
	while (current && current->rank != RANK_S)
	{
		attach_args_to_token(current, first);
		current = current->next;
	}
	return (first);
}
