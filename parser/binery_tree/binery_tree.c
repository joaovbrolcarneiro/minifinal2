/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* binery_tree.c                                    :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: jbrol-ca <jbrol-ca@student.42.fr>          +#+  +:+       +#+        */
/* GPLv3+   +#+           */
/* Created: 2025/03/31 21:06:10 by hde-barr          #+#    #+#             */
/* Updated: 2025/04/17 18:26:57 by jbrol-ca         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "minishell_part2.h"

void	print_env(char **env)
{
	int	i;

	i = -1;
	if (!env)
	{
		ft_printf("env not setted");
		return ;
	}
	while (env[++i])
	{
		ft_printf("%s\n", env[i]);
	}
}

char	*domane_expantion(char **env, char *input)
{
	if (ft_strncmp(ft_strtrim(input, " "), "env", ft_strlen(input) + 3) == 0)
		print_env(env);
	if (get_envar(env, input))
		return (get_envar(env, input));
	return ("");
}
