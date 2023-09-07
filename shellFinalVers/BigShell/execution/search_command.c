/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-meij <bde-meij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 12:16:17 by bde-meij          #+#    #+#             */
/*   Updated: 2023/01/13 11:02:11 by bde-meij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../bighell.h"

char	*search_list(t_bighell *bighell, int look_for)
{
	char	*found;
	t_list	*tmp;

	tmp = bighell->head_token;
	while (bighell->head_token->next)
	{
		if (((t_tokenlist *)bighell->head_token->content)->token == look_for)
		{
			found = ((t_tokenlist *)bighell->head_token->content)->str;
			bighell->head_token = tmp;
			return (found);
		}
		bighell->head_token = bighell->head_token->next;
	}
	bighell->head_token = tmp;
	return (NULL);
}

int	cmd_length(t_bighell *bighell, int nth_instance)
{
	t_list	*tmp;
	int		length;
	int		i;

	i = 1;
	length = 1;
	tmp = bighell->head_token;
	while ((bighell->head_token->next) && (i <= nth_instance))
	{
		if (((t_tokenlist *)bighell->head_token->content)->token == PIPE)
			i++;
		if (i == nth_instance && \
			(((t_tokenlist *)bighell->head_token->content)->token == CMD || \
			((t_tokenlist *)bighell->head_token->content)->token == CMD_OPTION))
			length++;
		bighell->head_token = bighell->head_token->next;
	}
	bighell->head_token = tmp;
	return (length);
}

char	**make_cmd_with_option(t_bighell *bighell, char **cmd)
{
	int	n;

	n = 1;
	cmd[0] = protec(ft_strdup \
	(((t_tokenlist *)bighell->head_token->content)->str));
	while (bighell->head_token->next)
	{
		if (((t_tokenlist *)bighell->head_token->content)->token == CMD_OPTION)
		{
			cmd[n] = protec(ft_strdup \
			(((t_tokenlist *)bighell->head_token->content)->str));
			n++;
		}
		if (((t_tokenlist *)bighell->head_token->content)->token == PIPE)
			break ;
		bighell->head_token = bighell->head_token->next;
	}
	return (cmd);
}

char	**search_cmd_loop(t_bighell *bighell, char **cmd, int nth_instance)
{
	int	i;

	i = 1;
	while (bighell->head_token->next)
	{
		if (((t_tokenlist *)bighell->head_token->content)->token == PIPE)
			i++;
		if ((i == nth_instance) && \
			(((t_tokenlist *)bighell->head_token->content)->token == CMD))
			return (make_cmd_with_option(bighell, cmd));
		if (bighell->head_token->next)
			bighell->head_token = bighell->head_token->next;
	}
	return (cmd);
}

char	**search_cmd(t_bighell *bighell, int nth_instance)
{
	char	**cmd;
	int		length;
	t_list	*tmp;
	int		i;

	i = 0;
	tmp = bighell->head_token;
	length = cmd_length(bighell, nth_instance);
	cmd = protec(ft_calloc(sizeof(char *), length + 1));
	cmd[length] = NULL;
	cmd = search_cmd_loop(bighell, cmd, nth_instance);
	bighell->head_token = tmp;
	return (cmd);
}
