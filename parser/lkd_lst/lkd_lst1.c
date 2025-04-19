/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lkd_lst1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrol-ca <jbrol-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 16:38:32 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/17 16:47:37 by jbrol-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "minishell_part2.h"

t_token *get_lastone_nodeof_rank(t_token *lst, t_ranking this_ranking)
{
	t_token *current_lastone;

	current_lastone = NULL;
	while (lst)
	{
		if (lst->rank == this_ranking)
			current_lastone = lst;
		if (!lst->next)
			return (current_lastone);
		lst = lst->next;
	}
	return (NULL);
}

t_token *get_prev_node(t_token *node, t_token *lst)
{
	if(!node || !lst)
		return (NULL);
	if(node->id == lst->id)
		return (NULL);
	while (lst)
	{
	//ft_printf("ok\n");
		if(!lst->next)
			return (NULL);
		if(lst->next->id == node->id)
			return (lst);
		lst = lst->next;
	}
	return (NULL);
}

t_token *untie_node(t_token *node, t_token *lst)
{
	t_token *prev;

	if(!node)
		return(NULL);
	if(!node->value)
		return(NULL);
	prev = get_prev_node(node, lst);
	if(prev)
		prev->next = node->next;
	if (!prev && !node->next)
		return (NULL);
	node->next = NULL;
	if(!prev)
		return(NULL);
	return (prev->next);
}

t_token *rm_node_lst(t_token *token, t_token *first)
{
    t_token *prev;

    if(!token || !first)
        return (NULL);
    prev = get_prev_node(token, first);
    if(!prev)
        return (NULL);
    prev->next = token->next;
    return (token);
}

void	add_node_lst(t_token *dst_lst, t_token *token)
{
	t_token	*tmp;

	if (!dst_lst || !token)
		return ;
	tmp = dst_lst->next;
	dst_lst->next = token;
	token->next = tmp;
}