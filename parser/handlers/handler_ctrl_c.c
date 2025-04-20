/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* handler_ctrl_c.c                                 :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/* GPLv3+   +#+           */
/* Created: 2025/04/02 17:56:44 by hde-barr          #+#    #+#             */
/* Updated: 2025/04/02 17:57:05 by hde-barr         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_part2.h"

void	handle_ctrl_c(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}
