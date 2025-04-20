/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* split_input.c                                    :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: jbrol-ca <jbrol-ca@student.42.fr>          +#+  +:+       +#+        */
/* GPLv3+   +#+           */
/* Created: 2025/04/02 16:53:41 by hde-barr          #+#    #+#             */
/* Updated: 2025/04/19 20:27:46 by jbrol-ca         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_part2.h"

bool	proximity_exception(char *input, int i)
{
	if ((!ischarset("|<>", input[i]) && input[i] != ' ') && \
		input[i] && !ischarset("|<>", *input))
		return (true);
	return (false);
}
