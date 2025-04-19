/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrol-ca <jbrol-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 18:05:15 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/19 20:52:43 by jbrol-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_part2.h"


t_token	*delegated_by_input_handler(char *input, char **env) // ALTEREI - JOAO
{
	t_token	*first;
	t_token	*token;

	token = split_input(input, 0);
	first = token;
	quote_handler_call_loop(token, env);
	typealize_call_loop(token, env);
	merg_tok_excep_cll_revloop(token);
	return (first);
}

/*
t_token *delegated_by_input_handler(char *input, char **env) -- FUNCAO ANTIGA
{
	t_token *first;
    t_token *token;

	if( is_too_long_input_exption(input))
		return (NULL);
    token = split_input(input, 0);
	first = token;
	quote_handler_call_loop(token, env);
	typealize_call_loop(token, env);
	merg_tok_excep_cll_revloop(token);
	return (token);
}*/

t_token *input_handler(t_shell *shell, char *input) // Change return type
{
    t_token     *token_list; // Head of the token list returned by delegated_by_input_handler
    t_node_tree *tree;       // Root of the AST, built from the token list
    bool         parse_error_flagged_in_tokens; // Tracks errors like unclosed quotes within tokens

    tree = NULL; // Initialize AST pointer to NULL

    // 1. Tokenization and initial processing (quotes, types, merge)
    // delegated_by_input_handler returns the token list head (or NULL for empty/space-only input or split_input failure)
    token_list = delegated_by_input_handler(input, shell->env);

    // --- CRITICAL CHECK: Handle case where token_list is NULL ---
    // This happens for empty input string, space-only input, or split_input allocation failure.
    // If token_list is NULL, there are no tokens to process further.
    if (!token_list) {
        // If split_input failed due to allocation, g_exit_code is already set to 1.
        // If input was empty/space-only, split_input returns NULL, g_exit_code remains 0 (correct behavior - do nothing).
        // No further parsing or execution is needed.
        // The token list is already NULL, nothing to free related to the token list here before returning.
        // Cleanup of input string (free(input)) happens outside input_handler in readline_loop.
        // g_exit_code is already set appropriately by split_input or is 0.
        return (NULL); // Return NULL if split_input failed or input was empty/space-only
    }

    // If token_list is NOT NULL, we have a valid token list to process.

    // 2. Check for parsing errors flagged *in* the token list itself
    // (e.g., unclosed quotes flagged by quote_handler, or errors in split_input that set token->err).
    // has_parser_error must handle being called with a non-NULL list.
    parse_error_flagged_in_tokens = has_parser_error(token_list); // Use 'token_list' as the head


    // If there are no parsing errors flagged in the token list, continue building AST and executing.
    if (!parse_error_flagged_in_tokens)
    {
        // 3. Continue processing the non-empty, non-error token list pipeline
        // These functions operate on the list *after* merging, quotes, types.
        // They MUST handle being called with a valid (non-NULL) token list head.
        process_variable_assignments(shell, token_list); // Operates on 'token_list' (the list head)
        expand_token_list_no_assignments(token_list, shell->env); // Operates on 'token_list'

        // handler_args_file modifies the list structure by removing nodes and copying pointers!
        // This is problematic and likely interferes with subsequent AST building if not done perfectly.
        // For the current approach, assume it's needed for AST builder and handles non-NULL input.
        // It returns the (potentially modified) head of the list.
        // Ensure handler_args_file doesn't return NULL unless it's a critical error (e.g., allocation failure).
        token_list = handler_args_file(token_list, token_list); // Operates on 'token_list', returns the head.

        // Check if token_list became NULL after handler_args_file due to a critical error.
        // If it returned NULL, g_exit_code should be set within handler_args_file.
        if (!token_list) {
            // If list became NULL due to a critical handler_args_file error, g_exit_code is set.
            // The token list nodes created *before* handler_args_file might need freeing,
            // but handler_args_file modifies the list, making full freeing complex.
            // The free_token_list function needs to handle lists potentially modified by rm_node_lst.
            return (NULL); // Return NULL if list became NULL due to a critical handler_args_file error
        }

        parser_cmd_no_found(token_list, shell->env); // Operates on 'token_list', updates CMD/WORD types

        // 4. Build the AST from the final state of the token list
        // init_yggdrasil uses the list head (token_list)
        // Ensure init_yggdrasil handles NULL input gracefully if token_list became NULL before this point.
        tree = init_yggdrasil(token_list);

        // 5. Execute the AST if successfully built
        if (tree)
        {
            execute_ast(shell, tree); // Execute the AST
            // g_exit_code is set by execute_ast based on command exit status or execution error.

            // 6. Free the AST after execution. Implement free_ast.
            // Assuming AST freeing happens *inside* input_handler for now.
            // ft_printf("DEBUG: Freeing AST...\n"); // Optional debug
            // free_ast(tree); // Implement free_ast
        }
        else if (g_exit_code == 0)
        {
             // If tree is NULL, but g_exit_code is still 0, it means AST building resulted
             // in nothing executable (like only assignments, or empty commands after processing)
             // and no parsing error was flagged. Exit status should be 0.
             // g_exit_code is already 0.
        }
        // If tree is NULL and g_exit_code != 0, AST building failed due to a parser error.
        // Error message printed by set_parser_error.
    }
    else
    {
        // Error flagged by has_parser_error (from split_input or quote_handler)
        // g_exit_code should already be set (e.g., 2 for syntax errors).
        // Error message printed by set_parser_error or quote_handler.
        // The token list might need freeing here, but we'll manage that in readline_loop.
        g_exit_code = 2; // Ensure syntax error exit code if not set already
    }

    // --- Return the token list head for cleanup ---
    // The list created by split_input needs to be freed eventually.
    // Return the head pointer to the caller (readline_loop) for proper cleanup.
    // This pointer might be the original head or a new head if handlers changed it.
    // The responsibility of freeing the token list falls to readline_loop now.
    return (token_list); // Return the head of the processed list (can be NULL if delegated failed)
}

/*void input_handler(char **env, char *input) ---- FUNCAO ANTIGA
{
	t_token *token;
	t_node_tree *tree;

	token = delegated_by_input_handler(input, env);
	if(!token)
		return ;
	token = delegated_by_input_handler(input, env);
	token = handler_args_file(token, token);
	parser_cmd_no_found(token, env);
	//print_token_lst(token);
	tree = init_yggdrasil(token);
	//print_yggdrasil(tree, 0, "root:");
}*/