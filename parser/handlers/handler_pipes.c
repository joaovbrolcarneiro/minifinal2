/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_pipes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrol-ca <jbrol-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 17:50:41 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/20 21:50:56 by jbrol-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_part2.h"

t_token *last_token(t_token *token);


/*t_token	*last_token(t_token *token)
{
	if (!token)
		return (NULL);
	while (token->next)
		token = token->next;
	return (token);
}
*/
void	handler_pipes(t_token_vec *token_vec) // - alterei - JOAO
{
	if (token_vec)
	{
		if (token_vec->so_far)
		{
			if (token_vec->so_far->type == TOKEN_PIPE)
			{
				if (token_vec->first)
					last_token(token_vec->first)->next = token_vec->so_far;
			}
		}
	}
}

/*void handler_pipes(t_token_vec *token_vec) - FUNCAO ANTIGA
{
	if(token_vec)
		if(token_vec->so_far)
			if(token_vec->so_far->type == TOKEN_PIPE)
			{
				last_token(token_vec->first)->next = token_vec->so_far;
				//if(get_prev_node(token_vec->so_far, token_vec->first)->type
				// = token_vec->so_far->type)
				//	ft_printf("pipe: syntax error near unexpected token '|'");
			}
			//ft_printf("%s\n", token_vec->first->value);

}
*/
