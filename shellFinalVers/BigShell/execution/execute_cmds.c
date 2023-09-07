/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-meij <bde-meij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/14 12:44:43 by rbrune            #+#    #+#             */
/*   Updated: 2023/01/18 11:48:40 by bde-meij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../bighell.h"

void	execute_cmd(t_bighell *bighell)
{
	char	**command;
	char	*command_path;

	if (open_infiles(bighell))
		exit (1);
	if (open_outfiles(bighell))
		exit (1);
	command = search_cmd(bighell, bighell->pipe_section);
	if (!command[0])
		exit(0);
	if (check_if_builtin(bighell))
	{
		builtins(bighell);
		exit (0);
	}
	command_path = cmd_env_path(command[0], bighell, 0);
	execve(command_path, command, bighell->new_env);
	if (access(command[0], F_OK) == 0 && access(command[0], X_OK) < 0)
	{
		print_errors(126);
		exit(126);
	}
	print_errors(127);
	exit(127);
}

void	first_cmd(t_bighell *bighell, int pipe_out[2])
{
	pid_t	sid;

	sid = guard_fork(fork());
	if (sid == 0)
	{
		dup2(pipe_out[1], 1);
		close(pipe_out[1]);
		close(pipe_out[0]);
		execute_cmd(bighell);
	}
	close(pipe_out[1]);
}

void	command_loop(t_bighell *bighell, int pipe_in, int pipe_out[2])
{
	pid_t	sid;

	sid = guard_fork(fork());
	if (sid == 0)
	{
		dup2(pipe_out[1], 1);
		dup2(pipe_in, 0);
		close(pipe_in);
		close(pipe_out[1]);
		close(pipe_out[0]);
		execute_cmd(bighell);
	}
	close(pipe_in);
	close(pipe_out[1]);
}

void	last_cmd(t_bighell *bighell, int pipe_in, int *pid)
{
	*pid = guard_fork(fork());
	if (*pid == 0)
	{
		dup2(pipe_in, 0);
		execute_cmd(bighell);
	}
	close(pipe_in);
}
