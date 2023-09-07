/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-meij <bde-meij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 10:52:07 by bde-meij          #+#    #+#             */
/*   Updated: 2023/01/13 12:51:35 by bde-meij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../bighell.h"

int	check_if_builtin(t_bighell *bighell)
{
	char	**cmd_tmp;

	cmd_tmp = search_cmd(bighell, bighell->pipe_section);
	if (!cmd_tmp[0])
		return (free_dubarray(cmd_tmp), 0);
	if ((ft_strcmp("pwd", cmd_tmp[0]) == 0) || \
		(ft_strcmp("env", cmd_tmp[0]) == 0) || \
		(ft_strcmp("exit", cmd_tmp[0]) == 0) || \
		(ft_strcmp("unset", cmd_tmp[0]) == 0) || \
		(ft_strcmp("export", cmd_tmp[0]) == 0) || \
		(ft_strcmp("echo", cmd_tmp[0]) == 0) || \
		(ft_strcmp("cd", cmd_tmp[0]) == 0))
		return (free_dubarray(cmd_tmp), 1);
	return (free_dubarray(cmd_tmp), 0);
}

int	engage_children(t_bighell *bighell, pid_t *pid)
{
	int			sd[2][2];
	int			rd_write;

	rd_write = 0;
	while (bighell->pipe_section <= bighell->n_pipes)
	{
		if (bighell->pipe_section < bighell->n_pipes)
			guard_fd(pipe(sd[rd_write]));
		if (bighell->pipe_section == 1 && bighell->n_pipes > 1)
			first_cmd(bighell, sd[rd_write]);
		else if (bighell->pipe_section < bighell->n_pipes)
			command_loop(bighell, \
			sd[!rd_write][0], sd[rd_write]);
		else
			last_cmd(bighell, sd[!rd_write][0], pid);
		rd_write = !rd_write;
		bighell->pipe_section++;
	}
	return (0);
}

int	pipex(t_bighell *bighell)
{
	int			wstatus;
	pid_t		pid;
	sig_t		process_sig[2];

	wstatus = 0;
	process_sig[0] = signal(SIGINT, process_signal);
	process_sig[1] = signal(SIGQUIT, quit_process_signal);
	wstatus = engage_children(bighell, &pid);
	waitpid(pid, &wstatus, 0);
	while (bighell->n_pipes - 1 > 0)
	{
		wait(NULL);
		bighell->n_pipes--;
	}
	signal(SIGINT, process_sig[0]);
	signal(SIGQUIT, process_sig[1]);
	if (wstatus == 1)
		return (1);
	if (wstatus == 2)
		return (130);
	if (wstatus == 3)
		return (131);
	if (WIFEXITED(wstatus))
		return (WEXITSTATUS(wstatus));
	return (print_errors(138), 138);
}

int	cmd_exe(t_bighell *bighell)
{
	bighell->pipe_section = 1;
	signal(SIGINT, SIG_IGN);
	make_heredocs(bighell);
	signal(SIGINT, general_signal);
	signal(SIGQUIT, SIG_IGN);
	if (bighell->n_pipes >= 2)
		g_exit_code = pipex(bighell);
	else if (check_if_builtin(bighell))
	{
		if (open_infiles(bighell) == 0 && open_outfiles(bighell) == 0)
			builtins(bighell);
	}
	else
		g_exit_code = pipex(bighell);
	reset_std(bighell);
	unlink_heredocs(bighell);
	free_token_list(bighell->head_token);
	return (0);
}
