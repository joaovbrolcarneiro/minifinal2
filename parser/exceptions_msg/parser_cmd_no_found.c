/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_no_found.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrol-ca <jbrol-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 19:13:32 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/17 16:45:57 by jbrol-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_part2.h"

void	parser_cmd_no_found(t_token *token, char **env)
{
	t_token_type	type;

	while (token)
	{
		type = token->type;
		if (type != TOKEN_PIPE && type != TOKEN_ASSIGNMENT && \
			type != TOKEN_REDIR_IN && type != TOKEN_REDIR_OUT && \
			type != TOKEN_APPEND && type != TOKEN_HEREDOC)
		{
			if (token->rank <= RANK_B && \
				!search_list(token->value, env) && \
				!is_builtin(token->value))
			{
				token->type = TOKEN_WORD;
			}
			else if (token->type == TOKEN_WORD && \
					(is_builtin(token->value) || search_list(token->value, env)))
			{
				token->type = TOKEN_CMD;
				token->rank = RANK_B;
				token->coretype = TOKEN_CMD;
			}
		}
		token = token->next;
	}
}