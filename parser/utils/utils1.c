/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* utils1.c                                         :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: jbrol-ca <jbrol-ca@student.42.fr>          +#+  +:+       +#+        */
/* GPLv3+   +#+           */
/* Created: 2025/04/02 15:39:45 by hde-barr          #+#    #+#             */
/* Updated: 2025/04/17 21:31:27 by jbrol-ca         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "minishell_part2.h"

int	ischarset(const char *set, char c)
{
	while (*set)
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

size_t	ft_strsetlen(char *s, const char *set)
{
	size_t	len;

	if (!s)
		return (0);
	len = 0;
	while (s[len] && !ischarset(set, s[len]))
		len++;
	return (len);
}

size_t	ft_strnlen(char *s, char n)
{
	size_t	len;

	if (!s)
		return (0);
	len = 0;
	while (s[len] && s[len] != n)
		len++;
	return (len);
}

long long	get_token_id(void)
{
	static long long	id;

	id = 0;
	return (id++);
}

char	*ft_strcpy(char *dest, const char *src)
{
	char	*tmp;

	tmp = dest;
	while (*src)
	{
		*dest = *src;
		dest++;
		src++;
	}
	*dest = '\0';
	return (tmp);
}
