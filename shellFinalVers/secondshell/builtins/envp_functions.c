/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   envp_functions.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbrune <rbrune@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/07 10:29:22 by rbrune        #+#    #+#                 */
/*   Updated: 2023/01/18 12:05:34 by rbrune        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../bighell.h"

int	check_for_valid_ident(t_bighell *bighell, char **cmd, int *i, int flag);

int	replace_env_arg(t_bighell *bighell, char **cmd, int i, t_list *tmp)
{
	free(((t_envlist *)bighell->head_envp->content)->full_str);
	free(((t_envlist *)bighell->head_envp->content)->left_part);
	free(((t_envlist *)bighell->head_envp->content)->right_part);
	add_content_list(bighell->head_envp->content, cmd[i]);
	bighell->head_envp = tmp;
	return (1);
}

void	add_content_list(t_envlist *envp_list, char *var)
{
	char		**split_var;

	split_var = protec(ft_split(var, '='));
	envp_list->full_str = protec(ft_strdup(var));
	envp_list->left_part = protec(ft_strdup(split_var[0]));
	if (split_var[1])
	{
		envp_list->right_part = protec(ft_strdup(split_var[1]));
	}
	else
		envp_list->right_part = NULL;
	free_dubarray(split_var);
}

char	**copy_env_to_darray(t_bighell *bighell, char **new_env)
{
	int	i;

	i = 0;
	while (bighell->head_envp)
	{
		new_env[i] = protec(ft_strdup(((t_envlist *) \
		bighell->head_envp->content)->full_str));
		bighell->head_envp = bighell->head_envp->next;
		i++;
	}
	return (new_env);
}

char	**make_array_envp(t_bighell *bighell)
{
	char	**new_env;
	int		i;
	t_list	*tmp;

	tmp = bighell->head_envp;
	i = 0;
	ft_free_old_env_array(bighell->new_env);
	while (bighell->head_envp)
	{
		bighell->head_envp = bighell->head_envp->next;
		i++;
	}
	bighell->head_envp = tmp;
	new_env = protec(malloc(sizeof(char **) * (i + 1)));
	new_env[i] = NULL;
	new_env = copy_env_to_darray(bighell, new_env);
	bighell->head_envp = tmp;
	return (new_env);
}

void	add_envp(t_bighell *bighell, char **cmd, int i, t_list *tmp)
{
	char	**splt_chk_str;
	int		flag;

	while (cmd[i])
	{
		flag = 0;
		splt_chk_str = protec(ft_split(cmd[i], '='));
		while (bighell->head_envp)
		{
			if (ft_strcmp(((t_envlist *)bighell->head_envp-> \
			content)->left_part, splt_chk_str[0]) == 0)
			{
				replace_env_arg(bighell, cmd, i, tmp);
				flag = 1;
				break ;
			}
			bighell->head_envp = bighell->head_envp->next;
		}
		bighell->head_envp = tmp;
		check_for_valid_ident(bighell, cmd, &i, flag);
		free_dubarray(splt_chk_str);
		i++;
	}
	bighell->head_envp = tmp;
	bighell->new_env = make_array_envp(bighell);
}
