/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_loop.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrol-ca <jbrol-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 18:31:39 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/17 17:02:22 by jbrol-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_part2.h"

void	readline_loop(t_shell *shell) // ALTEREI - JOAO
{
	char	*input;

	while (1)
	{
		ft_putstr_fd("💥", 1);
		konopwd(true, "pwd");
		input = readline(TITLE);
		if (!input)
		{
			ft_putstr_fd("exit\n", STDOUT_FILENO);
			cleanup_shell(shell);
			exit(g_exit_code);
		}
		if (*input)
			add_history(input);
		if (!ft_strlen(input))
		{
			free(input);
			continue ;
		}
		input_handler(shell, input);
		free(input);
	}
}

/* FUNCAO ANTIGA
void readline_loop(char **env)
{
    while (1)
    {
		minigarbege_colector();
		ft_printf("💥"ULI);
		konopwd(true, "pwd");
		ft_printf(RST);
    	char *input = readline(TITLE);
		if(!input)//Ctrl+D
			exit(0);
		if(*input)
			add_history(input);
		is_minishell_exit(input);
		if(!ft_strlen(input))
			continue;
		input_handler(env, input);
	}
}*/
