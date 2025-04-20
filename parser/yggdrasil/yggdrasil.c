/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   yggdrasil.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrol-ca <jbrol-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 17:25:45 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/20 20:24:29 by jbrol-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_part2.h"

t_token *find_right_token(t_token *token, t_token *eof)
{
	t_token *father;
	int i;

	i = 0;
	if(!token)
		return(NULL);
	if(token->next == eof || !token->next || token == eof)
		return (NULL);
	father = token;
	token = token->next;
	while (1)
	{
		if(father->rank - i == token->rank)
			return (token);
		if (father->rank - i == RANK_F)
			return (NULL);
		token = token->next;
		if(token == eof)
		{
			i++;
			token = father->next;
		}
	}
	return (token);
}

t_token *find_left_token(t_token *token, t_token *first)
{
	t_token *eof;
	int i;

	i = 0;
	if (token == first || !token || !first)
		return(NULL);
	eof = token;
	token = get_prev_node(token, first);
	while (1)
	{
		if(eof->rank - i == token->rank)
			return (token);
		//?????????
		/*if(eof->rank - i < token->rank )
			return (NULL);*/
		//?????????
		if (eof->rank - i == RANK_F)
			return (NULL);
		token = get_prev_node(token, first);
		if(token == NULL)
		{
			i++;
			token = get_prev_node(eof, first);
		}
	}
	return (token);
}

t_node_tree *new_yggnode(t_token *token)
{
    t_node_tree *new_node;

    new_node = hb_malloc(sizeof(t_node_tree));
	if (!new_node) 
        return (NULL);
    if (!token) 
	{
        free(new_node);
        return (NULL);
    }
    if (token->used == true)
	{
        free(new_node);
        return (NULL);
    }
    token->used = true;
    new_node->right = NULL;
    new_node->left = NULL;
    new_node->content = token->value;
    new_node->type = (t_ast_type)token->type;
    new_node->rank = token->rank;
    new_node->args = token->args;
    new_node->file = token->file;
    return (new_node);
}

/*funcao antiga
t_node_tree *new_yggnode(t_token *token)
{
    t_node_tree *new_node;

    new_node = hb_malloc(sizeof(t_node_tree));
    if (!new_node)
	{
        return (NULL);
	}
	if (!token)
        return (NULL);
	if (token->used == true)
		return (NULL);
	token->used = true;
	new_node->right = NULL;
	new_node->left = NULL;
	if (!new_node->right || !new_node->left)
		return (NULL);
	new_node->content = token->value;
	new_node->type = (t_ast_type)token->type;
	new_node->rank = token->rank;
	new_node->args = token->args;
	new_node->file = token->file;
    return (new_node);
}*/

t_node_tree	*make_yggdrasil(t_token *t, t_token *f, t_token *e, t_node_tree *y)////////yggdrasil
{
	//validation_tree(t, f);
	//take_args();
	if(!y)
		y = new_yggnode(t);
	y->left = new_yggnode(find_left_token(t, f));
	y->right = new_yggnode(find_right_token(t, e));
	if (y->left)
		make_yggdrasil(find_left_token(t, f), f, t, y->left);
	if (y->right)
		make_yggdrasil(find_right_token(t, e), f, e, y->right);
	return (y);
}

void st_prsr_err(const char *message, const char *token_value)
{
    // Cast the string literals and const char* variables to char* for ft_putstr_fd
    ft_putstr_fd((char *)"konosubash: ", 2); // Cast string literal
    if (message)
        ft_putstr_fd((char *)message, 2); // Cast message
    // Add token value for context if available
    if (token_value && *token_value) // Check if token_value is not NULL or empty
    {
        ft_putstr_fd((char *)" `", 2); // Cast string literal
        ft_putstr_fd((char *)token_value, 2); // Cast token_value
        ft_putstr_fd((char *)"'", 2); // Cast string literal
    }
    ft_putstr_fd((char *)"\n", 2); // Cast string literal

    g_exit_code = 2; // Standard exit code for syntax errors
}