/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_yggdrasil.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 17:16:27 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/02 17:17:01 by hde-barr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_part2.h"

void print_yggdrasil(t_node_tree *yggnode, int num_tabs, char *leg)
{
	char *arrow;
	char *tab = "         ";
	int i;

	i = 0;
	while(i++ < num_tabs)
		printf("%s", tab);
	num_tabs++;
	if((ft_strcmp("L:", leg) != 0 && ft_strcmp("R:",leg)) != 0)
		arrow = "---->";
	else
		arrow = "╰─-->";
	if(!yggnode)
		return ;
	printf("%s %s{ %s ", arrow, leg, yggnode->content);

	if(yggnode->args && !yggnode->file)
		printf("(args):[%s", yggnode->args[0]);
	else if (!yggnode->file)
		printf("}\n");
	if(yggnode->args)
		for (size_t i = 1; yggnode->args[i]; i++)
			printf(", %s", yggnode->args[i]);
	if(yggnode->file && !yggnode->args)
		printf("(file):[%s", yggnode->file);
	if(yggnode->args || yggnode->file)
		printf("] }\n");

	if(yggnode->right)
	{
		leg = "R:";
		print_yggdrasil(yggnode->right, num_tabs, leg);
	}
	if(yggnode->left)
	{
		leg = "L:";
		print_yggdrasil(yggnode->left, num_tabs, leg);	
	}
}