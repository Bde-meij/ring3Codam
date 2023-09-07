/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expand_utils.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbrune <rbrune@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/07 10:39:29 by rbrune        #+#    #+#                 */
/*   Updated: 2023/01/16 14:49:12 by rbrune        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../bighell.h"

char	*fill_new_input(char *new_input, char *old_input, \
char *expand_str, size_t start_index);

void	put_backslash(int quote_flag, char *new_input, size_t *i)
{
	if (quote_flag == 0)
	{
		new_input[*i] = '\\';
		(*i)++;
	}
}

char	*add_expanded_str(char *expand_str, \
char *new_input, size_t *i, int quote_flag)
{
	int	j;

	j = 0;
	quote_flag = 0;
	while (expand_str[j])
	{
		new_input[*i] = expand_str[j];
		(*i)++;
		j++;
	}
	return (new_input);
}

void	skip_space(t_bighell *bighell, size_t *i, \
size_t *n, int *check_spc)
{
	if (bighell->line[*i] == ' ')
	{
		while (bighell->line[*i] == ' ')
		{
			if (bighell->line[*i] == ' ' && bighell->line[*i + 1] \
			!= ' ' && *check_spc != 0)
				(*n)++;
			(*i)++;
		}
	}
}

int	check_expand(char *tmp_str, t_bighell *bighell)
{
	while (ft_strcmp(((t_envlist *)bighell->head_envp->content)->left_part, \
	tmp_str) != 0 && bighell->head_envp->next)
	{
		bighell->head_envp = bighell->head_envp->next;
	}
	if (ft_strcmp(((t_envlist *)bighell->head_envp->content)->left_part, \
	tmp_str) != 0)
		return (1);
	if (((t_envlist *)bighell->head_envp->content)->right_part == NULL)
	{
		return (1);
	}
	return (0);
}

char	*make_expand_str(char *old_input, size_t i, \
size_t *j)
{
	char	*tmp_str;

	*j = 0;
	while (old_input[i + *j] != ' ' && old_input[i + *j])
		(*j)++;
	tmp_str = protec(malloc((int)*j));
	*j = 1;
	while (old_input[i + *j] != ' ' && old_input[i + *j] \
	&& old_input[i + *j] != '\"' && old_input[i + *j] != '\'')
	{
		tmp_str[*j - 1] = old_input[i + *j];
		(*j)++;
	}
	tmp_str[*j - 1] = '\0';
	return (tmp_str);
}
