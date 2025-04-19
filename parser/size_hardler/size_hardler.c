/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   size_hardler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:20:30 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/15 15:22:43 by hde-barr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_part2.h"

static size_t aprox_cmd_uni(char *input)
{
	size_t i;

	i = 0;
	while (*input++)
	{
		if(*input && ischarset("<>", *input) && !ischarset("<>", *(input + 1)))
			i+=2;
		if(*input && ischarset("|", *input))
			i+=2;
		if(*input && ischarset("\"'", *input))
			i++;
		if(*input && ischarset(" ", *input) && !ischarset(" ", *(input + 1)))
			i+=2;
	}
	return (i);
	
}

bool is_too_long_input_exption(char *input)
{
	if (ft_strlen(input) >= 1024*200)
		return (printf(RDK"konosubash: too long spring\n"), true);
	if (aprox_cmd_uni(input) >= 2048)
		return (printf(RDK"konosubash: too long size\n"), true);
	return (false);
}