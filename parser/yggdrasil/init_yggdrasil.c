/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_yggdrasil.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrol-ca <jbrol-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 17:28:02 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/17 19:41:23 by jbrol-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_part2.h"

bool token_err(t_token *token_lst)
{
	while (token_lst)
	{
		//printf("%d\n",token_lst->err);
		if(token_lst->err == 1)
			return (true);
		token_lst = token_lst->next;
	}
	return (false);
}

t_node_tree *init_yggdrasil(t_token *token_lst)
{
	t_token *root;
	t_token *first;

	root = token_lst;
	first = token_lst;
	if(token_err(token_lst))
		return(new_yggnode(new_eof(root)));
	while (token_lst)
	{
		if(root->rank <= token_lst->rank\
		 /*|| (token_lst->type == TOKEN_REDIR_OUT\
		  || token_lst->type == TOKEN_APPEND\
		   || token_lst->type == TOKEN_HEREDOC\
		    || token_lst->type == TOKEN_REDIR_IN)*/)
			root = token_lst;
		token_lst = token_lst->next;
	}
	//root->rank = RANK_S;
	return (make_yggdrasil(root, first, NULL, NULL));
}
