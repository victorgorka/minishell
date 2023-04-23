/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmanzana <fmanzana@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 17:48:26 by vde-prad          #+#    #+#             */
/*   Updated: 2023/04/23 12:43:27 by fmanzana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
	It assigns the fd of the opened file to the output/input fd of the cmd
	executed.
	@param inputs Structure that contains the the parsed arguments.
	@param data Structure that contains the 1 the fd necessary for the execution.
*/
void	ft_setdata(t_inputs *inputs, t_pipe *data)
{
	if (inputs->args->ired_flag)
	{

		if (!access(inputs->args->inf[0], F_OK | R_OK))
			data->fdin = open(inputs->args->inf[0], O_RDONLY); // ¡OJO! Prueba
		else
		{
			ft_putstr_fd("No such file or directory\n", STDERR_FILENO);
			exit(127);
		}
	}

	if (inputs->args->ored_flag || inputs->args->app_flag)
	{
		if (inputs->args->ored_flag)
			data->fdout = open(inputs->args->outf[0], //¡OJO! Prueba
					O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (inputs->args->app_flag)
			data->fdout = open(inputs->args->outf[0], //¡OJO! Prueba
					O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (data->fdout < 0)
		{
			ft_putstr_fd("Error: output file innaccesible\n", STDERR_FILENO);
			exit(127);
		}
	}
}
