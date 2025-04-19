/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rare_candy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrol-ca <jbrol-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 18:39:13 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/17 16:42:59 by jbrol-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_part2.h"

bool konopwd(bool cmd_exist, const char *input)/////rare_candy
{
	char *buf;
	const size_t size = 4096;

	buf = hb_malloc(size);
	if(ft_strncmp(ft_strtrim(input, " "), "pwd", 4096) == 0)
	{
		ft_printf(RDK"%s\n", getcwd(buf, size));
		return (true);
	}
	return (cmd_exist != false);
}
