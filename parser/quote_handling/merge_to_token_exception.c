/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   z_merge_to_token_exception.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrol-ca <jbrol-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 19:17:54 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/17 18:38:52 by jbrol-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_part2.h"


bool	merge_to_token_exception(t_token *token) // - ALTEREI - joao
{
	t_token	*first;
	t_token	*rm_node;
	char	*temp_val;

	first = token;
	if (!token || !token->next || token->merge_next == false)
		return (false);
	rm_node = rm_node_lst(token->next, first);
	if (!rm_node)
		return (false);
	temp_val = token->value;
	token->value = ft_strjoin(temp_val, rm_node->value);
	if (!token->value)
	{
		token->value = temp_val;
		return (false);
	}
	return (true);
}

/*bool merge_to_token_exception(t_token *token) - funcao antiga
{
	t_token *first;
	t_token *rm_node;
	first = token;
	if(token->merge_next == true)
	{
		rm_node = rm_node_lst(token->next, first);
		token->value = ft_strjoin(token->value, rm_node->value);
	}
	return (true);
}*/