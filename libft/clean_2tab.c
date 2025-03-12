/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_2tab.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 13:42:34 by root              #+#    #+#             */
/*   Updated: 2024/11/14 13:42:51 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	clean_2dtab(char **goodbye)
{
	unsigned int	i;

	i = 0;
	if (goodbye)
	{
		while (goodbye[i])
			free(goodbye[i++]);
		free(goodbye);
	}
}