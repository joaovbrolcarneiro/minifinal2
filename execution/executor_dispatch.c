/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* executor_dispatch.c                                :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/04/14 22:00:02 by hde-barr          #+#    #+#             */
/* Updated: 2025/04/14 23:00:01 by hde-barr         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

/* Part 2 of handle_redir_execution: apply redir and exec command */
static int	execute_redir_after_save(t_shell *shell, t_node_tree *node)
{
	int	status;
	int	internal_status;

	status = 0;
	internal_status = handle_redirections(node);
	if (internal_status != 0)
		status = 1;
	else
		status = execute_redir_cmd_node(shell, node);
	return (status);
}

/* Dispatches execution for redirection nodes */
int	handle_redir_execution(t_shell *shell, t_node_tree *node)
{
	int	status;
	int	original_fds[2];
	int	fds_saved;

	status = 0;
	fds_saved = 0;
	if (save_original_fds(original_fds) == 0)
		fds_saved = 1;
	else
		status = 1;
	if (status == 0)
		status = execute_redir_after_save(shell, node);
	if (fds_saved)
	{
		restore_fds(original_fds);
		close_fds(original_fds);
	}
	return (status);
}

/* Part 2 of handle_command_execution: execute command */
static int	execute_command_after_save(t_shell *shell, t_node_tree *node)
{
	int	status;

	status = execute_simple_command(shell, node);
	return (status);
}

/* Dispatches execution for simple command nodes */
int	handle_command_execution(t_shell *shell, t_node_tree *node)
{
	int	status;
	int	original_fds[2];
	int	fds_saved;

	status = 0;
	fds_saved = 0;
	if (save_original_fds(original_fds) == 0)
		fds_saved = 1;
	else
		status = 1;
	if (status == 0)
		status = execute_command_after_save(shell, node);
	if (fds_saved)
	{
		restore_fds(original_fds);
		close_fds(original_fds);
	}
	return (status);
}

/* Dispatches execution for PIPE nodes */
int	handle_pipe_execution(t_shell *shell, t_node_tree *node)
{
	return (execute_pipe_command(shell, node));
}

int execute_redirection_chain(t_shell *shell, t_node_tree *node)
{
    int status = 0;
    int original_fds[2];
    int fds_saved = 0;
    t_node_tree *current;
    t_node_tree *command_node = NULL; 
    t_node_tree *redir_nodes[1024];
    int redir_count = 0;

    if (!node || (node->type < AST_REDIR_IN || node->type > AST_HEREDOC))
    {
         ft_putstr_fd("konosubash: internal error: execute_redirection_chain called with invalid node type\n", 2);
         return (1);
    }

    if (save_original_fds(original_fds) == 0)
        fds_saved = 1;
    else
    {
        perror("konosubash: execute_redirection_chain: Failed to save file descriptors");
        return (1);
    }

    current = node; 

    while (current)
    {
        if (current->type >= AST_REDIR_IN && current->type <= AST_HEREDOC)
        {
            
            if (redir_count < 1024)
            {
                 redir_nodes[redir_count++] = current;
            }
            else
            {
                 ft_putstr_fd("konosubash: too many redirections in command\n", 2);
                 status = 1;
                 command_node = NULL;
                 break;
            }

            if (current->left)
            {
                current = current->left;
            }
            else if (current->right)
            {
                 current = current->right;
            }
             else
             {
                 ft_putstr_fd("konosubash: syntax error near redirection (missing command or file)\n", 2);
                 status = 2;
                 command_node = NULL;
                 break;
             }
        }
        else if (current->type == AST_COMMAND)
        {
            command_node = current;
            status = 0;
            break;
        }
        else
        {
            set_parser_error("internal error: unexpected node type in redirection chain traversal", NULL);
            status = 1;
            command_node = NULL;
            break;
        }
    }

    if (command_node && status == 0)
    {
        for (int i = redir_count - 1; i >= 0; i--)
        {
            t_node_tree *redir_node_to_apply = redir_nodes[i];

            status = handle_redirections(redir_node_to_apply);

            if (status != 0)
            {
                command_node = NULL;
                break;
            }
        }
    }
     if (!command_node && status == 0 && redir_count > 0)
     {
         ft_putstr_fd("konosubash: syntax error: command missing\n", 2);
         status = 2;
     }
    if (command_node && status == 0)
    {
        status = execute_simple_command(shell, command_node);
    }
    if (fds_saved)
    {
        restore_fds(original_fds);
        close_fds(original_fds);
    }
    return (status);
}
