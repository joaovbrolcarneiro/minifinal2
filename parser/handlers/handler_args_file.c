/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_args_file.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrol-ca <jbrol-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 17:38:59 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/19 15:26:14 by jbrol-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_part2.h"

/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* handler_args_file.c                                :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/* */ /* Updated: 2025/04/18 21:15:00 by hde-barr         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "../../includes/minishell.h"      // Adjust path
#include "../../includes/minishell_part2.h" // Adjust path

// Assumes rm_node_lst is declared (using OLD signature: t_token *rm_node_lst(t_token*, t_token*);)
// Assumes ft_strdup, ft_free_strarray, malloc, free, perror are declared

/* Helper: Reallocates args array, adding new_arg. Frees old array ptr. */
static char	**realloc_args_array(char **old_args, int old_argc, \
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
		new_args[i] = old_args[i]; // Copy pointer
		i++;
	}
	new_args[i] = ft_strdup(new_arg); // Duplicate the new argument string
	if (!new_args[i])
	{
		perror("konosubash: strdup new arg");
		free(new_args);
		// Old pointers are not freed here. Caller/GC must handle original strings.
		return (NULL);
	}
	new_args[i + 1] = NULL; // Null-terminate the new array
	if (old_args)
		free(old_args); // Free only the old array structure
	return (new_args);
}

/* Helper: Creates the initial args array {"cmd", "arg1", NULL} */
static int	create_first_arg(t_token *priv, t_token *arg_token)
{
	char	**new_args;

	if (!priv || !priv->value || !arg_token || !arg_token->value)
		return (1);
	new_args = malloc(sizeof(char *) * 3);
	if (!new_args)
		return (perror("konosubash: malloc first arg"), 1);
	// args[0] points to original command value (do not duplicate)
	new_args[0] = priv->value;
	// args[1] is a duplicate of the first argument's value
	new_args[1] = ft_strdup(arg_token->value);
	if (!new_args[1])
		return (perror("konosubash: strdup first arg"), free(new_args), 1);
	new_args[2] = NULL;
	if (priv->args) // Should be NULL, but free just in case
		free(priv->args);
	priv->args = new_args;
	return (0);
}

/* Builds priv->args array incrementally (NEW array building logic) */
void	join_and_split(t_token *priv, t_token *arg_token)
{
	int		argc;
	char	**temp_args;

	if (!priv || !arg_token || !arg_token->value)
		return ;
	if (priv->args == NULL) // First argument being added
	{
		if (create_first_arg(priv, arg_token) != 0)
			priv->args = NULL; // Ensure NULL on error
	}
	else // Subsequent argument case
	{
		argc = 0;
		while (priv->args[argc] != NULL)
			argc++;
		temp_args = realloc_args_array(priv->args, argc, arg_token->value);
		// realloc_args_array frees the old priv->args pointer on success
		if (!temp_args)
			priv->args = NULL; // Ensure NULL on error
		else
			priv->args = temp_args;
	}
	// Partner's GC handles freeing the original arg_token->value string later
}

/* Associates filename token (OLD Version - Pointer Copy) */
t_token	*redir_handler_file(t_token *token, t_token *first)
{
	t_token	*file_node;

	// OLD check using rank, maybe less robust than checking WORD type
	if (!token || token->coretype != REDIR || !token->next || \
		token->next->rank == RANK_S)
		return (token);
	// Uses OLD rm_node_lst signature
	file_node = rm_node_lst(token->next, first);
	if (file_node)
	{
		token->file = file_node->value; // Pointer copy
		// Partner's GC needs to handle file_node struct
	}
	else
	{
		token->file = NULL;
		// No error flag setting in this version
	}
	return (token);
}

/* Processes arguments for a command token (OLD Version) */
t_token	*cmd_handler_args(t_token *token, t_token *first)
{
	t_token	*redir; // Unused variable
	t_token	*arg_node;

	redir = NULL; // Initialize unused variable
	// Outer loop iterates through potential command tokens
	while (token && token->rank != RANK_S)
	{
		// Inner loop looks ahead for arguments
		while (token->next && token->next->rank != RANK_S)
		{
			if (token->next->coretype == REDIR) // Stop if redir found
				break ;
			else // Otherwise, assume it's an argument
			{
				// Uses OLD rm_node_lst signature
				arg_node = rm_node_lst(token->next, first);
				if (!arg_node)
					break ; // Stop if remove fails
				join_and_split(token, arg_node); // Call NEW array building logic
				if (!token->args)
					break; // Stop if arg building failed
				// Partner's GC handles freeing the unlinked arg_node struct
			}
		}
		if (redir) // Dead code as redir is never assigned
			add_node_lst(token, redir);
		if (redir)
			redir = NULL;
		token = token->next; // Advance to next token
	}
	return (first); // Return original head
}

/* Top-level handler for arguments and file associations (OLD Version) */
t_token	*handler_args_file(t_token *token, t_token *first)
{
	t_token	*current;

	current = token;
	while (current)
	{
		redir_handler_file(current, first); // Call OLD version
		cmd_handler_args(current, first);   // Call OLD version
		current = current->next; // Use potentially unsafe iteration
	}
	return (first);
}