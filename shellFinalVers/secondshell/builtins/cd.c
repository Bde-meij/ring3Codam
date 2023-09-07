/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbrune <rbrune@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/20 13:00:17 by rbrune        #+#    #+#                 */
/*   Updated: 2023/01/16 14:51:55 by rbrune        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../bighell.h"

void	edit_pwds_two(t_bighell *bighell, char *tmp_str, t_list	*tmp)
{
	while (bighell->head_envp)
	{
		if (ft_strcmp(((t_envlist *)bighell->head_envp->content)->left_part, \
			"OLDPWD") == 0)
		{
			tmp_str = ft_strjoin("OLDPWD=", bighell->current_path);
			free(((t_envlist *)bighell->head_envp->content)->full_str);
			free(((t_envlist *)bighell->head_envp->content)->left_part);
			free(((t_envlist *)bighell->head_envp->content)->right_part);
			add_content_list(bighell->head_envp->content, tmp_str);
			bighell->head_envp = tmp;
			bighell->new_env = make_array_envp(bighell);
			free(tmp_str);
			break ;
		}
		bighell->head_envp = bighell->head_envp->next;
	}
	bighell->head_envp = tmp;
}

void	free_env_list_content(t_bighell *bighell)
{
	free(((t_envlist *)bighell->head_envp->content)->full_str);
	free(((t_envlist *)bighell->head_envp->content)->left_part);
	free(((t_envlist *)bighell->head_envp->content)->right_part);
}

int	edit_pwds(t_bighell *bighell)
{
	t_list	*tmp;
	char	*tmp_str;
	char	*tmp_path;

	tmp_str = NULL;
	tmp = bighell->head_envp;
	while (bighell->head_envp)
	{
		if (ft_strcmp(((t_envlist *)bighell->head_envp->content)->left_part, \
			"PWD") == 0)
		{
			tmp_path = getcwd(NULL, 0);
			tmp_str = ft_strjoin("PWD=", tmp_path);
			free_env_list_content(bighell);
			add_content_list(bighell->head_envp->content, tmp_str);
			bighell->new_env = make_array_envp(bighell);
			free (tmp_str);
			free(tmp_path);
			break ;
		}
		bighell->head_envp = bighell->head_envp->next;
	}
	bighell->head_envp = tmp;
	edit_pwds_two(bighell, tmp_str, tmp);
	return (0);
}

int	cd(t_bighell *bighell, char **cmd_tmp)
{
	int		i;

	free(bighell->current_path);
	bighell->current_path = getcwd(NULL, 8);
	if (ft_strcmp("cd", cmd_tmp[0]) == 0)
	{
		i = 0;
		if (!cmd_tmp[1])
		{
			printf("Invalid input, give argument\n");
			g_exit_code = 1;
			return (1);
		}
		if (chdir(cmd_tmp[1]))
		{
			printf("%s: No such file or directory\n", cmd_tmp[1]);
			g_exit_code = 1;
			return (1);
		}
		edit_pwds(bighell);
		g_exit_code = 0;
		return (1);
	}
	return (0);
}
