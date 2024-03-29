/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmanzana <fmanzana@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 17:48:26 by vde-prad          #+#    #+#             */
/*   Updated: 2023/05/07 17:18:37 by fmanzana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ft_check_inf(t_inputs *inputs, t_pipe *data)
{
	int	i;
	int	fd;

	fd = 0;
	i = 0;
	while (inputs->args->inf[i])
	{
		if (inputs->args->inf_flags[i] == 1)
		{
			if (!access(inputs->args->inf[0], F_OK | R_OK))
			{
				if (inputs->args->ired_flag == i + 1 && data->ign_inf == 0)
					data->fdin = open(inputs->args->inf[i], O_RDONLY);
			}
			else
			{
				ft_putstr_fd("minishell : ", 2);
				ft_putstr_fd(inputs->args->inf[i], 2);
				ft_putstr_fd(": No such file or directory\n", 2);
				return (1);
			}
		}
		i++;
	}
	return (0);
}

static void	ft_check_out(t_inputs *inputs, t_pipe *data)
{
	int	i;

	i = 0;
	while (i < inputs->args->outf_len)
	{
		if (inputs->args->outf_flags[i] == 1)
			data->fdout = open(inputs->args->outf[i],
					O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (inputs->args->outf_flags[i] == 2)
			data->fdout = open(inputs->args->outf[i],
					O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (i < inputs->args->outf_len - 1)
			close(data->fdout);
		if (data->fdout < 0)
		{
			ft_putstr_fd("Error: output file innaccesible\n", STDERR_FILENO);
			exit(127);
		}
		i++;
	}
}

/**
	It assigns the fd of the opened file to the output/input fd of the cmd
	executed.
	@param inputs Structure that contains the the parsed arguments.
	@param data Structure that contains the 1 the fd necessary for the execution.
*/
int	ft_setdata(t_inputs *inputs, t_pipe *data)
{
	if (inputs->args->ored_flag || inputs->args->app_flag)
		ft_check_out(inputs, data);
	if (inputs->args->inf_len)
	{
		if (inputs->args->inf_flags[inputs->args->inf_len - 1] == 1)
			data->ign_inf = 0;
		else if (inputs->args->inf_flags[inputs->args->inf_len - 1] == 2)
			data->ign_inf = 1;
		if (inputs->args->hd_flag > 0)
			ft_here_doc(inputs, data);
		return (ft_check_inf(inputs, data));
	}
	return (0);
}
