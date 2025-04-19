/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* tokenlst_split_input.c                           :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/* */ /* Updated: 2025/04/16 23:55:00 by hde-barr         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_part2.h" // For proximity_exception, ischarset etc.

/* Calculates length for non-quoted tokens (operators, words) */
static int	get_unquoted_token_len(char *input)
{
	int		i;
	char	start_char;

	i = ft_strsetlen(input, "\"' |<>");
	start_char = *input;
	if (i == 0 && ischarset("|<>", start_char))
	{
		i = 1;
		if (ischarset("<>", start_char) && input[1] == start_char)
			i = 2;
	}
	else if (i == 0 && *input != '\0')
		i = 1;
	else if (i == 0 && *input == '\0')
		return (0);
	if (i == 0)
		i = 1;
	return (i);
}

/* Calculates the length of the next token */
static int	get_token_len(char *input)
{
	int		i;
	char	start_char;

	i = 1;
	start_char = *input;
	if (ischarset("\"'", start_char))
	{
		while (input[i] && input[i] != start_char)
			i++;
		if (input[i] == start_char)
			i++;
		else
			i = 1;
	}
	else
		i = get_unquoted_token_len(input);
	return (i);
}

/* Initializes fields of a newly allocated token */


/* Creates token, initializes, allocates next node, links, advances lst ptr */
/* Returns 0 on success, 1 on failure */


/* Main token splitting function */
t_token *split_input(char *input, int unused_i) // Using unused_i as the second parameter matches your function signature
{
    t_token *lst;     // Pointer to the most recently created token node
    t_token *first;   // Pointer to the head of the list
    int     start;    // Index to track the start of the current token segment in the input string
    int     token_len; // Length of the current token segment

    (void)unused_i; // Mark parameter as unused if not used in this version

    first = NULL; // Initialize head to NULL
    lst = NULL;   // Initialize pointer to the current node being processed/linked

    start = 0;
    while (input[start]) // Loop through the input string using 'start' index
    {
        // 1. Skip leading spaces for the current token segment
        while (input[start] && input[start] == ' ')
            start++;

        // If we reached the end after skipping spaces, we are done.
        if (!input[start])
            break ; // Exit the main loop

        // 'start' is now at the beginning of a non-space token segment.

        // 2. Determine the length of the current token segment
        // Use a dedicated helper function for clarity and robustness
        token_len = get_token_len(input + start); // Pass pointer to the start of the segment

        // Handle invalid or zero length token segments from non-empty input.
        // get_token_len should usually return > 0 for valid token starts from non-empty string.
        if (token_len <= 0)
        {
             // This indicates an error in get_token_len or an unexpected scenario
             // that results in a non-positive token length for a non-empty string segment.
             // Handle as an error.
            ft_putstr_fd("konosubash: split_input: Error or zero length token detected\n", 2);
             // Free the list built so far before returning NULL.
            free_token_list(first);
            g_exit_code = 1; // Indicate error
            return (NULL); // Return failure
        }

        // 3. Allocate and Initialize the Token Node
        t_token *new_token = hb_malloc(sizeof(t_token));
        if (!new_token)
        {
            perror("konosubash: split_input: hb_malloc failed for new token");
            // Free the list built so far on allocation error.
            free_token_list(first);
            g_exit_code = 1; // Indicate allocation failure
            return (NULL);
        }
        // CRITICAL FIX: Initialize the allocated struct to zero!
        ft_memset(new_token, 0, sizeof(t_token));

        // 4. Create the value string for the current token
        new_token->value = ft_substr(input, start, token_len);
        if (!new_token->value)
        {
            // Handle ft_substr allocation failure
            perror("konosubash: split_input: ft_substr failed");
            free(new_token); // Free the just allocated node
            free_token_list(first); // Free the rest of the list built so far
            g_exit_code = 1; // Indicate allocation failure
            return (NULL);
        }

        // 5. Initialize other essential token fields
        // These fields were previously uninitialized garbage!
        new_token->id = get_new_token_id(); // Assign a unique ID (add this function definition)
        // Type and rank will be refined later by typealize_call_loop.
        // Initialize them to defaults that typealize can handle.
        new_token->type = TOKEN_WORD;     // Default type before typealize
        new_token->coretype = TOKEN_WORD; // Default core type before typealize
        new_token->rank = RANK_C;         // Default rank (adjust if your ranking system needs a different base)
        new_token->used = false;          // Tokens start as unused in parsing
        new_token->err = 0;               // No error initially
        new_token->literal = false;       // Will be set by quote_handler based on quotes
        // Calculate merge_next based on the character *after* this token segment ends
        new_token->merge_next = proximity_exception(input, start + token_len); // Calculate merge_next relative to original input start

        // 6. Link the new node into the list
        if (!first)
        {
            first = new_token; // If it's the first token, set it as the head
            lst = first;       // lst also points to the first node
        }
        else
        {
            lst->next = new_token; // Link the previous token to the new token
            lst = new_token;       // Advance lst to the new token
        }

        // 7. Advance the 'start' pointer to the beginning of the next token segment
        start += token_len;

        // The loop continues, and the next iteration will handle skipping spaces
        // and finding the start of the next token from the new 'start' position.
    }

    // After the loop, the list is built and correctly null-terminated by default
    // because the last allocated node's 'next' field was zeroed by ft_memset.

    return (first); // Return the head of the created token list
}

void free_token_list(t_token *list)
{
    t_token *current;
    t_token *next;

    current = list;
    while (current)
    {
        next = current->next;
        // Free the allocated value string (if ft_substr allocated it)
        if (current->value)
            free(current->value);
        // Free the token struct itself
        free(current);
        current = next;
    }
}

long long get_new_token_id(void)
{
    static long long id = 0;
    return (id++);
}
