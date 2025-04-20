/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_token_lst.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrol-ca <jbrol-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 16:04:52 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/20 21:45:39 by jbrol-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_part2.h"

/*char *print_type(t_token *lst) FUNCAO ANTIGA
{
	if (!lst->value)
		return ("Root node");
	if (lst->type == TOKEN_PIPE)
		return ("PIPE");
	if (lst->type == TOKEN_REDIR_IN)
		return ("REDIR IN");
	if (lst->type == TOKEN_REDIR_OUT)
		return ("REDIR OUT");
	if (lst->type == TOKEN_APPEND)
		return ("APPEND");
	if (lst->type == TOKEN_HEREDOC)
		return ("HEREDOC");
	if (lst->type == TOKEN_CMD)
		return ("COMMAND");
	if (lst->type == TOKEN_WORD)
		return ("WORD");
	if (lst->type == TOKEN_EOF)
		return ("EOF");
	return (NULL);
}
	*/
char	*print_type(t_token *lst)
{
	if (!lst)
		return ("NULL_TOKEN_POINTER");
	if (lst->type == TOKEN_PIPE)
		return ("PIPE");
	else if (lst->type == TOKEN_REDIR_IN)
		return ("REDIR IN");
	else if (lst->type == TOKEN_REDIR_OUT)
		return ("REDIR OUT");
	else if (lst->type == TOKEN_APPEND)
		return ("APPEND");
	else if (lst->type == TOKEN_HEREDOC)
		return ("HEREDOC");
	else if (lst->type == TOKEN_CMD)
		return ("COMMAND");
	else if (lst->type == TOKEN_WORD)
		return ("WORD");
	else if (lst->type == TOKEN_EOF)
		return ("EOF");
	else if (lst->type == REDIR)
		return ("REDIR_CORE");
	else if (lst->type == TOKEN_ASSIGNMENT)
		return ("ASSIGNMENT");
	else
		return ("UNKNOWN_TYPE");
}

static void	print_single_token_details(t_token *token, const char *null_msg)
{
	char	*type_str;
	char	*value_str;

	type_str = print_type(token);
	if (token->value)
		value_str = token->value;
	else
		value_str = (char *)null_msg;
	printf("%s( %s -> %s )%s\n", BLUE, type_str, value_str, PINK);
}

void	print_token_lst(t_token *lst)
{
	const char	*null_msg;
	const char	*error_msg;

	if (!lst)
	{
		error_msg = RED "  (Token list is NULL)\n" RESET;
		write(2, error_msg, ft_strlen(error_msg));
		return ;
	}
	null_msg = "(null value)";
	while (lst)
	{
		print_single_token_details(lst, null_msg);
		lst = lst->next;
	}
}
