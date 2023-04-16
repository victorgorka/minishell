/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setdata.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vde-prad <vde-prad@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 17:48:26 by vde-prad          #+#    #+#             */
/*   Updated: 2023/04/15 20:27:23 by vde-prad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//parser the cmd name, flags and cmd arguments in a double char pointer
// static void	ft_parserarg(char	**av, t_argdata *pdata, int ac)
// {
// 	int	i;
// 	int	j;

// 	i = 2;
// 	j = 0;
// 	pdata->full_cmd = malloc((ac - 3) * sizeof(char **));//cambiar triple puntero por estructura?
// 	pdata->cmd = malloc((ac - 3) * sizeof(char *));
// 	while (i <= ac - 2)
// 	{
// 		if (av[i][0] != 0)
// 		{
// 			pdata->full_cmd[j] = ft_split(av[i], ' ');
// 			pdata->cmd[j] = ft_strdup(pdata->full_cmd[j][0]);
// 			i++;
// 			j++;
// 		}
// 		else
// 		{
// 			ft_putstr_fd("Error: argument(s) empty\n", 2);
// 			exit(127);
// 		}
// 	}
// }

//set fdin, fdout, pp, cmd and options in data structure
void	ft_setdata(t_pipedata *pdata, char	**av)
{
	if (!access(av[1], F_OK | R_OK))
		pdata->fdin = open(av[1], O_RDONLY);
	else
	{
		ft_putstr_fd("Error: input file innaccesible\n", 2);
		exit(127);
	}
	pdata->fdout = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pdata->fdout < 0)
	{
		ft_putstr_fd("Error: output file innaccesible\n", 2);
		exit(127);
	}
}