/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* executor_redir_utils.c                             :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/04/14 22:00:00 by hde-barr          #+#    #+#             */
/* Updated: 2025/04/14 23:59:00 by hde-barr         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

/* Saves original STDIN/STDOUT and returns status (0=ok, 1=error) */
int	save_original_fds(int original_fds[2])
{
	original_fds[0] = dup(STDIN_FILENO);
	original_fds[1] = dup(STDOUT_FILENO);
	if (original_fds[0] == -1 || original_fds[1] == -1)
	{
		perror("minishell: dup original fds");
		if (original_fds[0] != -1)
			close(original_fds[0]);
		return (1);
	}
	return (0);
}

/* Finds and executes the command node associated with a redirection node */
int	execute_redir_cmd_node(t_shell *shell, t_node_tree *redir_node)
{
	t_node_tree	*cmd_node;
	int			status;

	status = 0;
	cmd_node = NULL;
	if (!redir_node)
		return (1);
	cmd_node = redir_node->left;
	if (!cmd_node || cmd_node->type != AST_COMMAND)
	{
		if (redir_node->right && redir_node->right->type == AST_COMMAND)
			cmd_node = redir_node->right;
		else
			cmd_node = NULL;
	}
	if (cmd_node)
		status = execute_ast(shell, cmd_node);
	else
	{
		ft_putstr_fd("minishell: syntax error near redirection\n", 2);
		status = 2;
	}
	return (status);
}

/* Static helper to dispatch execution based on node type */
/* Called only by execute_ast within this file */
static int dispatch_ast_node(t_shell *shell, t_node_tree *node)
{
    int status = 1; // Default error status

    if (!node)
        return (0); // Base case for recursive execute_ast or empty AST

    if (node->type == AST_PIPE)
    {
        // Execute a pipe node using your existing pipe handler
        status = handle_pipe_execution(shell, node);
    }
    // Check if the node type corresponds to the START of a redirection sequence.
    // Based on your AST printouts, the root of a command with redirects IS an AST_REDIR_* node.
    else if (node->type >= AST_REDIR_IN && node->type <= AST_HEREDOC)
    {
        // Call the new function to handle the entire redirection chain and command execution.
        // Pass the starting redirection node of the chain.
        status = execute_redirection_chain(shell, node);
    }
    // If the AST builder ever produces a standalone AST_COMMAND node (e.g., for commands without redirects)
    // handle it here. Based on your AST output for `ls` and `ls -l`, the root IS the COMMAND node in that case.
    // So, this branch is needed for commands without redirects.
    else if (node->type == AST_COMMAND)
    {
        // Execute a simple command node that is NOT part of a redirection chain rooted at a REDIR node.
        // This happens for commands without redirects parsed by your current system.
        // Your original handle_command_execution saves/restores FDs around execute_simple_command.
        // We need that logic here for standalone commands.
        status = handle_command_execution(shell, node); // Use your original handle_command_execution logic (that saves/restores)
    }
    // Add cases for other node types if your parser creates them in the final AST
    // (e.g., AST_ASSIGNMENT if not handled before AST, AST_WORD if it appears unexpectedly)
    else if (node->type == (t_ast_type)TOKEN_ASSIGNMENT) // Assuming this AST node type exists
    {
         // Assignments at this stage are likely parser errors or special cases.
         // Your current handler seems to do nothing, which might be appropriate if they are handled globally earlier.
         status = handle_assignment_execution(node); // Your existing handler
    }
    else if (node->type == (t_ast_type)TOKEN_WORD) // Assuming this AST node type exists in the final AST
    {
         // A bare WORD node in the AST usually means "command not found" or parser error.
         status = handle_word_token_execution(node); // Your existing handler (prints command not found)
    }
    else
    {
        // Unknown node type - indicates an issue with the AST structure produced by the parser.
        ft_putstr_fd("konosubash: execute_ast: Unknown node type ", 2);
        ft_putnbr_fd(node->type, 2);
        ft_putstr_fd("\n", 2);
        status = 1; // Indicate error
    }
    // g_exit_code is updated by execute_ast after calling dispatch_ast_node
    return (status);
}


/* Main execution entry point (lives here as requested by user) */
int	execute_ast(t_shell *shell, t_node_tree *node)
{
	int	status;

	status = 0;
	if (!node)
	{
		g_exit_code = 0;
		return (0);
	}
	status = dispatch_ast_node(shell, node);
	g_exit_code = status;
	return (status);
}

/* Handles execution attempt for TOKEN_WORD nodes */
int	handle_word_token_execution(t_node_tree *node)
{
	char	*cmd_name;

	if (node && node->content)
		cmd_name = node->content;
	else
		cmd_name = "unknown";
	ft_printf(RED "konosubash: %s: command not found\n" RESET, cmd_name);
	return (127);
}
