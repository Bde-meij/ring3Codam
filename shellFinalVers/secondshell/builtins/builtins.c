/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtins.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbrune <rbrune@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/02 12:55:03 by rbrune        #+#    #+#                 */
/*   Updated: 2023/01/16 14:51:51 by rbrune        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../bighell.h"

int	ft_exit(char **cmd_tmp, int i)
{
	if (ft_strcmp("exit", cmd_tmp[0]) == 0)
	{
		printf("exit\n");
		while (cmd_tmp[i])
			i++;
		if (i > 2)
			return (printf("minishell: exit: too many arguments\n"));
			i = 0;
		if (cmd_tmp[1])
		{
			if (cmd_tmp[1][i] == '-')
				i++;
			while (cmd_tmp[1][i])
			{
				if (ft_isdigit(cmd_tmp[1][i]) != 1)
					return (printf("minishell: exit: a: numeric \
argument required\n"));
				i++;
			}
			exit (ft_atoi(cmd_tmp[1]));
		}
		exit (0);
	}
	return (0);
}

int	ft_pwd(char **cmd_tmp)
{
	char	*tmp;

	if (ft_strcmp("pwd", cmd_tmp[0]) == 0)
	{
		tmp = getcwd(NULL, 8);
		g_exit_code = 0;
		printf("%s\n", tmp);
		free_dubarray(cmd_tmp);
		free(tmp);
		return (1);
	}
	return (0);
}

int	env(t_bighell *bighell, char **cmd_tmp)
{
	t_list	*tmp;
	int		i;

	i = 0;
	tmp = bighell->head_envp;
	if (ft_strcmp("env", cmd_tmp[0]) == 0)
	{
		while (bighell->head_envp != NULL)
		{
			if (((t_envlist *)bighell->head_envp->content)->right_part)
				printf("%s\n", ((t_envlist *) \
				bighell->head_envp->content)->full_str);
			bighell->head_envp = bighell->head_envp->next;
			i++;
		}
		bighell->head_envp = tmp;
		g_exit_code = 0;
		free_dubarray(cmd_tmp);
		return (1);
	}
	return (0);
}

int	builtins(t_bighell *bighell)
{
	char	**cmd_tmp;

	cmd_tmp = search_cmd(bighell, bighell->pipe_section);
	if (ft_exit(cmd_tmp, 0))
	{
		free_dubarray (cmd_tmp);
		return (1);
	}
	if (_export(bighell, cmd_tmp))
		return (1);
	if (cd(bighell, cmd_tmp))
	{
		free_dubarray (cmd_tmp);
		return (1);
	}
	if (env(bighell, cmd_tmp))
		return (1);
	if (ft_pwd(cmd_tmp))
		return (1);
	if (unset(bighell, cmd_tmp))
		return (1);
	if (ft_echo(cmd_tmp))
		return (1);
	free_dubarray (cmd_tmp);
	return (0);
}
