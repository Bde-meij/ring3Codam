/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-meij <bde-meij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 15:57:30 by bde-meij          #+#    #+#             */
/*   Updated: 2023/08/14 12:39:59 by bde-meij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../bighell.h"

void	find_tokens(t_bighell *bighell, int token_num);
void	find_here_doc_n_out_append(int *tokenarr);
void	find_in_out_files(int *tokenarr);
void	find_quoted(t_bighell *bighell);

int	check_in_out_shenanigans(t_bighell *bighell, int in_out_op, int i)
{
	if ((in_out_op == INFILE_OP) && \
		(bighell->tokenarr[i] == OUTFILE_OP))
		return (print_errors(258), 1);
	if ((in_out_op == OUTFILE_OP) && \
		(bighell->tokenarr[i] == INFILE_OP))
		return (print_errors(258), 1);
	return (0);
}

int	find_unexpected_token(t_bighell *bighell, int in_out_op)
{
	size_t	i;
	size_t	tempi;

	i = 0;
	while (bighell->tokenarr[i])
	{
		tempi = i;
		while (bighell->tokenarr[i] == in_out_op && i < bighell->line_len)
		{
			i++;
			if (check_in_out_shenanigans(bighell, in_out_op, i))
				return (1);
			while (bighell->tokenarr[i] == SPACE)
			{
				tempi++;
				i++;
			}
		}
		if ((i - tempi) > 2)
			return (print_errors(258), 1);
		i++;
	}
	return (0);
}

void	find_cmds(int *tokenarr, int i)
{
	int	flag;
	int	it_is_written;

	flag = CMD;
	it_is_written = NO;
	while (tokenarr[i])
	{
		if (tokenarr[i] < 0)
		{
			tokenarr[i] = flag;
			if (flag == CMD)
				it_is_written = YES;
		}
		if ((it_is_written == YES) && (tokenarr[i + 1] > 0) && \
			(tokenarr[i + 1] != QUOTE) && (tokenarr[i + 1] != DOUBLE_QUOTE))
			flag = CMD_OPTION;
		if (tokenarr[i] == PIPE)
		{
			flag = CMD;
			it_is_written = NO;
		}
		i++;
	}
}

int	tokenize_input(t_bighell *bighell)
{
	size_t	i;

	i = 0;
	while (i < bighell->line_len)
	{
		bighell->tokenarr[i] = -1;
		i++;
	}
	i = 0;
	while (bighell->tokenarr[i])
	{
		printf("%d ", bighell->tokenarr[i]);
		i++;
	}
	printf("\n");
	i = 0;
	i = 0;
	while (bighell->compare_str[i])
	{
		find_tokens(bighell, i);
		i++;
	}
	i = 0;
	while (bighell->tokenarr[i])
	{
		printf("%d ", bighell->tokenarr[i]);
		i++;
	}
	printf("\n");
	i = 0;
	find_quoted(bighell);
	i = 0;
	while (bighell->tokenarr[i])
	{
		printf("%d ", bighell->tokenarr[i]);
		i++;
	}
	printf("\n");
	i = 0;
	if (find_unexpected_token(bighell, INFILE_OP) || \
		find_unexpected_token(bighell, OUTFILE_OP))
		return (1);
	find_here_doc_n_out_append(bighell->tokenarr);
	i = 0;
	while (bighell->tokenarr[i])
	{
		printf("%d ", bighell->tokenarr[i]);
		i++;
	}
	printf("\n");
	i = 0;
	find_in_out_files(bighell->tokenarr);
	i = 0;
	while (bighell->tokenarr[i])
	{
		printf("%d ", bighell->tokenarr[i]);
		i++;
	}
	printf("\n");
	i = 0;
	find_cmds(bighell->tokenarr, 0);
	i = 0;
	while (bighell->tokenarr[i])
	{
		printf("%d ", bighell->tokenarr[i]);
		i++;
	}
	printf("\n");
	i = 0;
	return (0);
}
