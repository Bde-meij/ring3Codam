/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   in_out_files.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbrune <rbrune@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/18 10:52:07 by bde-meij      #+#    #+#                 */
/*   Updated: 2023/01/16 16:43:52 by rbrune        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../bighell.h"

void	file_section(t_bighell *bighell)
{
	int	i;

	i = 1;
	while (bighell->head_token->next)
	{
		if (i == bighell->pipe_section)
			break ;
		if (((t_tokenlist *)bighell->head_token->content)->token == PIPE)
			i++;
		bighell->head_token = bighell->head_token->next;
	}
}

int	check_acces_close_fd(t_bighell *bighell, int tmp_token, int *fd)
{
	if ((access(((t_tokenlist *)bighell->head_token->content)->str, \
		R_OK) != 0) && tmp_token == INFILE)
	{
		if (access(((t_tokenlist *)bighell->head_token->content)->str, \
			F_OK) == 0)
			print_errors(-1);
		else
			print_errors(-2);
		close(*fd);
		return (1);
	}
	close(*fd);
	return (0);
}

int	open_infiles(t_bighell *bighell)
{
	t_list	*tmp;
	int		fd;
	int		tmp_token;

	tmp_token = 0;
	fd = -1;
	tmp = bighell->head_token;
	file_section(bighell);
	while (bighell->head_token->next)
	{
		tmp_token = ((t_tokenlist *)bighell->head_token->content)->token;
		if (tmp_token == PIPE)
			break ;
		if (tmp_token == INFILE || tmp_token == HERE_DOC)
		{
			if (check_acces_close_fd(bighell, tmp_token, &fd))
				return (1);
			fd = open(((t_tokenlist *)bighell->head_token->content)->str, 'x');
		}
		bighell->head_token = bighell->head_token->next;
	}
	bighell->head_token = tmp;
	if (fd != -1)
		dup2(fd, 0);
	return (0);
}

int	open_close_logic(int tmp_token, char *tmp_str, int *fd)
{
	if (tmp_token == OUTFILE_WITH_APPEND || tmp_token == OUTFILE)
		close(*fd);
	if (tmp_token == OUTFILE_WITH_APPEND)
	{
		*fd = open(tmp_str, O_WRONLY | O_APPEND | O_CREAT, 0644);
		if ((*fd < 0) && (access(tmp_str, F_OK) == 0))
			return (print_errors(-1), 1);
	}
	if (tmp_token == OUTFILE)
	{
		*fd = open(tmp_str, O_WRONLY | O_TRUNC | O_CREAT, 0644);
		if ((*fd < 0) && (access(tmp_str, F_OK) == 0))
			return (print_errors(-1), 1);
	}
	return (0);
}

int	open_outfiles(t_bighell *bighell)
{
	t_list	*tmp;
	int		tmp_token;
	int		fd;

	fd = -1;
	tmp = bighell->head_token;
	file_section(bighell);
	while (bighell->head_token->next)
	{
		tmp_token = ((t_tokenlist *)bighell->head_token->content)->token;
		if (tmp_token == PIPE)
			break ;
		if (open_close_logic(tmp_token, \
		((t_tokenlist *)bighell->head_token->content)->str, &fd))
		{
			bighell->head_token = tmp;
			return (1);
		}
		bighell->head_token = bighell->head_token->next;
	}
	bighell->head_token = tmp;
	if (fd != -1)
		dup2(fd, 1);
	return (0);
}
