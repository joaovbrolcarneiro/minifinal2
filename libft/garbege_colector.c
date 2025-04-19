/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbege_colector.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 17:48:57 by hde-barr          #+#    #+#             */
/*   Updated: 2025/04/15 17:31:18 by hde-barr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "libft.h"

size_t **garbege(void *trash)
{
    static size_t *garbege_lst[30000000];
    static size_t i = 0;

    if(trash == NULL)
    {
        return(garbege_lst[i] = NULL, i = 0, garbege_lst);
    }
    garbege_lst[i] = trash;
    i++;
    return (garbege_lst);
}

void minigarbege_colector(void)
{
    size_t**garbege_lst;
    size_t i = 0;

    garbege_lst = garbege(NULL);
    while (garbege_lst[i])
    {
        free(garbege_lst[i]);
        garbege_lst[i] = NULL;
        i++;
    }
    
}

void *hb_malloc(size_t size)
{
    void *trash;
    
    trash = malloc(size);
    garbege(trash);
    //kazutrash(trash);
    return (trash);
}
