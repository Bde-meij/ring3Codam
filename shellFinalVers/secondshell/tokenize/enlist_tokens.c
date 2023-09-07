/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   enlist_tokens.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbrune <rbrune@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/21 09:30:59 by bde-meij      #+#    #+#                 */
/*   Updated: 2022/12/14 12:55:27 by rbrune        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../bighell.h"

void	add_list_and_token(t_bighell *bighell, \
char *str_for_list, int token, int *i)
{
	t_tokenlist	*token_list;

	token_list = protec(malloc(sizeof(t_tokenlist)));
	token_list->token = token;
	token_list->str = ft_strdup(str_for_list);
	ft_lstadd_back(&bighell->head_token, protec(ft_lstnew(token_list)));
	*i = 0;
	free(str_for_list);
}

int	skip_quote(int *tokenarr, int j)
{
	while ((tokenarr[j] == QUOTE || tokenarr[j] == DOUBLE_QUOTE || \
		tokenarr[j] == B_SLASH) && tokenarr[j])
		j++;
	return (j);
}

char	*give_mem(int *tokenarr, int tmp_token, int j)
{
	int		i;
	char	*mem_str;

	i = 0;
	mem_str = NULL;
	j = skip_quote(tokenarr, j);
	while (tokenarr[j] == tmp_token)
	{
		i++;
		j++;
		j = skip_quote(tokenarr, j);
	}
	mem_str = protec(ft_calloc(sizeof(char), i + 1));
	mem_str[i] = '\0';
	return (mem_str);
}

char	*copy_from_line(char *str_for_list, char *line, int *i, int *j)
{
	str_for_list[*i] = line[*j];
	(*i)++;
	(*j)++;
	return (str_for_list);
}

void	enlist(t_bighell *bighell, int i)
{
	char	*str_for_list;
	int		tmp_token;
	int		j;

	j = skip_quote(bighell->tokenarr, 0);
	tmp_token = bighell->tokenarr[j];
	str_for_list = give_mem(bighell->tokenarr, tmp_token, j);
	bighell->head_token = NULL;
	while (bighell->line[j])
	{
		j = skip_quote(bighell->tokenarr, j);
		if (!bighell->line[j])
			break ;
		if (bighell->tokenarr[j] != tmp_token)
		{
			add_list_and_token(bighell, str_for_list, tmp_token, &i);
			tmp_token = bighell->tokenarr[j];
			str_for_list = give_mem(bighell->tokenarr, tmp_token, j);
		}
		else
			str_for_list = copy_from_line(str_for_list, bighell->line, &i, &j);
	}
	if (i > 0)
		add_list_and_token(bighell, str_for_list, tmp_token, &i);
	ft_lstadd_back(&bighell->head_token, protec(ft_lstnew(NULL)));
}
