/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lkd_lst2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrol-ca <jbrol-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 16:38:32 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/17 16:55:54 by jbrol-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "minishell_part2.h"

t_token *last_token(t_token *token)
{
	if(token)
		while (token->next)
		{
			token = token->next;
		}
	return (token);
}

t_token	*remap_lst(t_token *token) // mexi - joao
{
	t_token		*first;
	t_token_vec	*token_vec;

	first = NULL;
	token_vec = NULL;
	while (token)
	{
		handler_pipes(token_vec);
		if (first == NULL)
		{
			if (token_vec)
				first = token_vec->first;
		}
		if (token_vec)
			token = token_vec->so_far;
		else
			token = NULL;
		if (token)
		{
			if (token->type == TOKEN_PIPE)
				token = token->next;
			if (token)
				token = token->next;
		}
	}
	return (first);
}

/*t_token *remap_lst(t_token *token) ---- FUNCAO ANTIGA
{
	t_token *endof;
	t_token *first;
	t_token_vec *token_vec;

	first = hb_malloc(sizeof(t_token));
	first = NULL;
	while (token)
	{
	//	token_vec = cmd_handler_args(token);
		handler_pipes(token_vec);
		if( first == NULL)
			first = token_vec->first;
		token = token_vec->so_far;
		if(token)
			if(token_vec->so_far->type == TOKEN_PIPE)
				token = token->next;
	}
	return(first);
}
*/