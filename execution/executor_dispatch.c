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
    int status = 0; // Status from command execution or error
    int original_fds[2];
    int fds_saved = 0;
    t_node_tree *current;
    t_node_tree *command_node = NULL; // To find the command node at the end of the chain
    // Use a temporary storage for redirection nodes as we traverse down.
    // We'll apply them later in reverse order (bottom-up).
    // Using a static-sized array; a dynamic list (like a stack) would be safer for arbitrary lengths.
    t_node_tree *redir_nodes[1024]; // Array to store pointers to the redirection nodes in traversal order
    int redir_count = 0;


    // This function should be called when the AST node passed is the ROOT of a redirection sequence.
    // Based on your AST printouts, this means node->type is an AST_REDIR_* type.
    if (!node || (node->type < AST_REDIR_IN || node->type > AST_HEREDOC))
    {
         // Safety check: This indicates an issue in dispatch_ast_node calling this function.
         // It should only be called with the starting node of a redir chain.
         ft_putstr_fd("konosubash: internal error: execute_redirection_chain called with invalid node type\n", 2);
         return (1); // Indicate an internal error
    }

    // Save original file descriptors ONCE for the entire execution unit (chain + command)
    if (save_original_fds(original_fds) == 0)
        fds_saved = 1; // Flag that FDs were successfully saved
    else
    {
        // Error saving FDs
        perror("konosubash: execute_redirection_chain: Failed to save file descriptors");
        return (1); // Return error status immediately
    }

    // --- Phase 1: Traverse DOWN the AST chain (via 'left' children) ---
    // Find the command node at the end and collect all redirection nodes encountered.
    current = node; // Start traversal at the root redirection node passed to this function

    while (current)
    {
        if (current->type >= AST_REDIR_IN && current->type <= AST_HEREDOC)
        {
            // We are currently on a redirection node. Store its pointer.
            // We'll apply these later, from the last stored to the first stored.
            if (redir_count < 1024) // Check for array bounds
            {
                 redir_nodes[redir_count++] = current;
            }
            else
            {
                 // Too many redirections - report error and stop processing this chain.
                 ft_putstr_fd("konosubash: too many redirections in command\n", 2);
                 status = 1; // Indicate an error
                 command_node = NULL; // Ensure no command is executed
                 break; // Exit the traversal loop
            }

            // Move to the next node in the chain.
            // Based on your AST, the chain link is the 'left' child.
            if (current->left)
            {
                current = current->left; // Move down the left child
            }
            else if (current->right) // Fallback check for right child? Based on AST, left is the chain link.
            {
                // This case is unexpected if the chain is strictly left-leaning ending in command.
                // It could happen if a redir node's left child is NULL but right is not, and the command is linked there.
                // Let's follow right as a fallback, but the primary chain structure is left.
                 current = current->right; // Follow right child
            }
             else
             {
                 // A redirection node has no children (left and right are NULL).
                 // This implies a redirection operator was not followed by a filename and a command,
                 // or the parser failed to link the command. This is a syntax error.
                 ft_putstr_fd("konosubash: syntax error near redirection (missing command or file)\n", 2);
                 status = 2; // Standard syntax error exit code
                 command_node = NULL; // Ensure no command is executed
                 break; // Exit the traversal loop
             }
        }
        else if (current->type == AST_COMMAND)
        {
            // Found the command node at the logical end of the redirection chain.
            command_node = current;
            status = 0; // Reset status if it was set to 1 (e.g., by too many redirects before finding the command)
            break; // Exit the traversal loop
        }
        else
        {
            // Encountered an unexpected node type while traversing the chain (a node that is neither REDIR nor COMMAND).
            // This indicates an issue with the AST structure produced by the parser.
            // It means a non-redir, non-command node is linked in the chain.
            set_parser_error("internal error: unexpected node type in redirection chain traversal", NULL);
            status = 1; // Indicate an internal error
            command_node = NULL; // Ensure no command is executed
            break; // Exit the traversal loop
        }
    }

    // --- Phase 2: Apply Redirections (BOTTOM-UP / LEFT-TO-RIGHT) ---
    // We stored redir nodes from the root down. Applying them in reverse order of storage
    // means applying them from the node just before the command upwards to the root node.
    // This corresponds to processing the redirects LEFT-TO-RIGHT in the original command string.

    if (command_node && status == 0) // Only proceed if a command node was found and no error during traversal
    {
        // Iterate backwards through the stored redirection nodes
        for (int i = redir_count - 1; i >= 0; i--)
        {
            t_node_tree *redir_node_to_apply = redir_nodes[i];

            // Apply the redirection represented by this node.
            // Your existing handle_redirections function takes a single node and calls open/dup2.
            // This is exactly what we need here.
            status = handle_redirections(redir_node_to_apply);

            if (status != 0)
            {
                // An error occurred while applying *this* specific redirection (e.g., file open failed).
                // The error message is printed by handle_redirections or its helpers (like perror).
                // Stop applying further redirects in the chain and DO NOT execute the command.
                command_node = NULL; // Prevent command execution
                break; // Exit the redirection application loop
            }
        }
    }
    // Note: If status was already non-zero from Phase 1 traversal (e.g., syntax error, internal error),
    // or if a redirection application failed in Phase 2, command_node will be NULL or status will be non-zero,
    // and the command execution step below will be skipped.

     // If after traversal and applying redirects, status is 0 and command_node is NULL,
     // it means we started processing a redir node, traversed, but never found a command,
     // and no specific error was flagged inside the loop (less likely with the current loop checks).
     // As a fallback, ensure a syntax error is reported if a redir chain was processed.
     if (!command_node && status == 0 && redir_count > 0)
     {
         // Should be caught in the loop, but defensive check.
         ft_putstr_fd("konosubash: syntax error: command missing\n", 2);
         status = 2;
     }


    // --- Phase 3: Execute the Command ---
    // Execute the command if a command node was found AND no errors occurred
    // during traversal or redirection application (status == 0).
    if (command_node && status == 0)
    {
        // Execute the found command node.
        // Your execute_simple_command function should take t_shell * and t_node_tree *
        // and use command_node->args to get the command name and arguments.
        // In THIS flow, execute_redirection_chain handles the FD saving/restoring
        // for the *entire chain + command unit*.
        // So, execute_simple_command should *just* handle running the command
        // (builtin or fork/execve), assuming FDs are already set up by the redirects applied above.
        status = execute_simple_command(shell, command_node);
        // The command's exit status updates 'status'.
    }
     // If we reach here with status != 0, it's either a redir application error, command execution status,
     // or a syntax/internal error from Phase 1 or 2. Error message should be printed.


    // --- Phase 4: Restore Original File Descriptors ---
    // Restore FDs only if they were successfully saved at the beginning.
    if (fds_saved)
    {
        restore_fds(original_fds); // Restore STDIN/STDOUT to their original state
        close_fds(original_fds); // Close the saved FDs (these are copies of original 0 and 1)
    }

    // Return the final status (0 for success, non-zero for errors, including command exit status)
    return (status);
}
