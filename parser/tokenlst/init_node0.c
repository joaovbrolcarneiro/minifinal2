/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_node0.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrol-ca <jbrol-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 16:52:43 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/20 22:10:52 by jbrol-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_part2.h"

t_token *new_token_super(t_token *token)
{
	return (token);
}

t_token	*create_and_init_token(char *input, int start, int token_len)
{
	t_token	*new_token;

	new_token = hb_malloc(sizeof(t_token));
	if (!new_token)
		return (perror("konosubash: split_input failed"), NULL);
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

t_token	*handler_args_file(t_token *token, t_token *first)
{
	t_token	*current;
	t_token	*next;

	current = token;
	while (current)
	{
		next = current->next;
		if (current->coretype == REDIR)
		{
			redir_handler_file(current, first);
		}
		else
		{
			cmd_handler_args(current, first);
		}
		current = next;
	}
	return (first);
}

t_token	*redir_handler_file(t_token *token, t_token *first)
{
	t_token	*file_node;

	if (!token || token->coretype != REDIR || !token->next
		|| token->next->rank == RANK_S)
		return (token);
	file_node = rm_node_lst(token->next, first);
	if (file_node)
	{
		token->file = file_node->value;
	}
	else
	{
		token->file = NULL;
	}
	return (token);
}
