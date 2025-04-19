/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_token_lst.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrol-ca <jbrol-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 16:04:52 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/17 18:49:41 by jbrol-ca         ###   ########.fr       */
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
	
	/* Prints token list details to STDOUT (for debugging) */
	/* Uses allowed printf and write functions */
	void	print_token_lst(t_token *lst)
	{
		char	*type_str;
		char	*null_msg;
		char	*error_msg;
	
		if (!lst)
		{
			error_msg = RED "  (Token list is NULL)\n" RESET;
			write(2, error_msg, ft_strlen(error_msg));
			return ;
		}
		null_msg = "(null value)";
		while (lst)
		{
			type_str = print_type(lst);
			printf("%s( %s -> %s )%s\n",
				BLUE, type_str,
				(lst->value ? lst->value : null_msg),
				PINK);
			lst = lst->next;
		}
	}
	