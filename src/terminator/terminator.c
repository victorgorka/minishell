/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminator.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vde-prad <vde-prad@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 17:44:12 by vde-prad          #+#    #+#             */
/*   Updated: 2023/05/07 09:57:22 by vde-prad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
	It sets the output and input file descriptors of the cmd
	@param inputs Structure that contains the parsed arguments
	@param data Structure that contains the 1 the fd necessary for the execution
	@param i iterator index
*/
static void	ft_inout_fd(t_inputs *inputs, t_pipe *data, int i)
{
	if (pipe(data->pp) == 1)
	{
		perror("pipe failure");
		exit(127);
	}
	data->fdout = data->pp[1];
	if (i == inputs->lenght)
		data->fdout = data->cpy_out;
	ft_setdata(inputs, data);
	dup2(data->fdin, STDIN_FILENO);
	close(data->fdin);
	data->fdin = data->pp[0];
	dup2(data->fdout, STDOUT_FILENO);
	close(data->fdout);
}

static int	ft_builtin(t_inputs *inputs, t_pipe *data)
{
	int		ret;
	char	**built_arg;

	built_arg = inputs->args->cmd_arr;
	ret = 1;
	if (ft_strncmp(inputs->args->cmd_arr[0], "echo", 5) == 0)
		ret = ft_echo(inputs);
	else if (ft_strncmp(inputs->args->cmd_arr[0], "env", 4) == 0)
		ret = ft_env(data);
	else if (ft_strncmp(inputs->args->cmd_arr[0], "pwd", 4) == 0)
		ret = ft_pwd(data);
	else if (ft_strncmp(inputs->args->cmd_arr[0], "export", 7) == 0)
		ret = ft_export(built_arg, data);
	else if (ft_strncmp(inputs->args->cmd_arr[0], "unset", 6) == 0)
		ret = ft_unset(inputs, data);
	else if (ft_strncmp(inputs->args->cmd_arr[0], "exit", 5) == 0)
		ret = ft_exit(inputs);
	else if (ft_strncmp(inputs->args->cmd_arr[0], "cd", 3) == 0)
		ret = ft_cd(built_arg, data);
	return (ret);
}

/**
	Generates 'i' number of processes for the execution of each cmd. It
	executes the cmds too.
	@param inputs Structure that contains the arguments passed by the CLI, parsed
	@param envp Environment variables
	@param data Structure that contains the file descriptors, necessary for the
				execution of the cmds
	@param i Iterator index
	@param childpid File descriptor of the child process
	@return return the childpid
*/
static int	ft_breeder(t_inputs *inputs, char **envp, t_pipe *data, int i)
{
	int		childpid;

	childpid = -1;
	ft_inout_fd(inputs, data, i);
	childpid = ft_builtin(inputs, data);
	if (childpid != -1)
	{
		signal(SIGUSR1, SIG_IGN);
		childpid = fork();
		if (childpid == -1)
			exit(127);
		if (childpid == 0)
		{
			if (access(inputs->args->cmd_arr[0], F_OK | R_OK) == 0)
				execve(inputs->args->cmd_arr[0], inputs->args->cmd_arr, envp);
			else
				execve(ft_getpath(envp, inputs->args->cmd_arr[0]),
					inputs->args->cmd_arr, envp);
			ft_putstr_fd("execve failure", 2);
			exit(127);
		}
		else
			data->childpid[i - 1] = childpid;
	}
	else
		data->childpid[i - 1] = childpid;
	return (childpid);
}

/**
	Function that deals with the execution of the commands passed by the parser.
	It iters the successive nodes of the list that contains the cmds data. It
	creates different proccesses with the function ft_breeder.
	@param inputs Structure with the necessary data for the execution of the cmds
	@param envp Environment variables
	@param i Iterator index
	@param childpid Child process file descriptor of the last cmd executed
	@param data Structure that stores the necessary file descriptors and the exit
				status
	@return Exit status
*/
int	ft_terminator(t_inputs *inputs, t_pipe *data)
{
	int		i;

	data->childpid = malloc(inputs->lenght * sizeof(int));
	data->cpy_out = dup(STDOUT_FILENO);
	data->cpy_in = dup(STDIN_FILENO);
	data->fdin = dup(data->cpy_in);
	i = 0;
	while (i++ < inputs->lenght)
	{
		ft_breeder(inputs, data->envp, data, i);
		if (inputs->args->next)
			inputs->args = inputs->args->next;
	}
	ft_antibreeder(data, inputs->lenght);
	free(data->childpid);
	signal(SIGUSR1, ft_procs_sig);
	run_to_head(&inputs->args);
	return (WEXITSTATUS(data->status));
}
// pp[0]--->lectura en pipe
// pp[1]--->escritura en pipe
