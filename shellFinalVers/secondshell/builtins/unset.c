/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   unset.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbrune <rbrune@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/30 16:07:43 by rbrune        #+#    #+#                 */
/*   Updated: 2022/12/09 12:52:33 by rbrune        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../bighell.h"

int	del_list_begin(t_bighell *bighell, t_list *tmp_head)
{
	bighell->head_envp = bighell->head_envp->next;
	free(((t_envlist *)bighell->head_envp->content)->full_str);
	free(((t_envlist *)bighell->head_envp->content)->left_part);
	free(((t_envlist *)bighell->head_envp->content)->right_part);
	free(((t_envlist *)bighell->head_envp->content));
	free(tmp_head);
	bighell->new_env = make_array_envp(bighell);
	return (1);
}

int	del_list_mid_and_end(t_bighell *bighell, t_list	*tmp_head)
{
	t_list	*tmp;

	tmp = NULL;
	if (!bighell->head_envp->next->next)
	{
		free(((t_envlist *)bighell->head_envp->next->content)->full_str);
		free(((t_envlist *)bighell->head_envp->next->content)->left_part);
		free(((t_envlist *)bighell->head_envp->next->content)->right_part);
		free(((t_envlist *)bighell->head_envp->next->content));
		free(bighell->head_envp->next);
		bighell->head_envp->next = NULL;
	}
	else
	{
		tmp = bighell->head_envp->next->next;
		free(((t_envlist *)bighell->head_envp->next->content)->full_str);
		free(((t_envlist *)bighell->head_envp->next->content)->left_part);
		free(((t_envlist *)bighell->head_envp->next->content)->right_part);
		free(((t_envlist *)bighell->head_envp->next->content));
		free(bighell->head_envp->next);
		bighell->head_envp->next = tmp;
	}
	bighell->head_envp = tmp_head;
	bighell->new_env = make_array_envp(bighell);
	return (1);
}

void	del_list(t_bighell *bighell, char**cmd_tmp, int i, t_list *tmp_head)
{
	while (cmd_tmp[i])
	{
		if (ft_strcmp(((t_envlist *)bighell->head_envp->content) \
		->left_part, cmd_tmp[i]) == 0)
			del_list_begin(bighell, tmp_head);
		while (bighell->head_envp->next)
		{
			if (ft_strcmp(((t_envlist *)bighell->head_envp->next->content) \
			->left_part, cmd_tmp[i]) == 0)
				del_list_mid_and_end(bighell, tmp_head);
			bighell->head_envp = bighell->head_envp->next;
		}		
		bighell->head_envp = tmp_head;
		i++;
	}
}

int	unset(t_bighell *bighell, char **cmd_tmp)
{
	t_list	*tmp_head;
	int		i;

	i = 1;
	tmp_head = bighell->head_envp;
	if (ft_strcmp("unset", cmd_tmp[0]) == 0)
	{
		del_list(bighell, cmd_tmp, i, tmp_head);
		free_dubarray (cmd_tmp);
		g_exit_code = 0;
		return (1);
	}
	return (0);
}
