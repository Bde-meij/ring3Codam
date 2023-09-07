/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizers.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-meij <bde-meij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 15:07:58 by bde-meij          #+#    #+#             */
/*   Updated: 2022/12/08 10:22:41 by bde-meij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../bighell.h"

void	find_tokens(t_bighell *bighell, int token_num)
{
	int	i;

	i = 0;
	while (bighell->line[i])
	{
		if (bighell->line[i] == bighell->compare_str[token_num])
			bighell->tokenarr[i] = token_num;
		else if (bighell->tokenarr[i] == -1)
			bighell->tokenarr[i] = WORD;
		i++;
	}
}

void	find_quoted(t_bighell *bighell)
{
	size_t	i;

	i = 0;
	while (bighell->line[i])
	{
		if (bighell->line[i] == '\'')
		{
			i++;
			while (bighell->line[i] != '\'' && i < bighell->line_len)
			{
				bighell->tokenarr[i] = SINGLE_QUOTED_STR;
				i++;
			}
		}
		if (bighell->line[i] == '\"')
		{
			i++;
			while ((bighell->line[i] != '\"') && i < bighell->line_len)
			{
				bighell->tokenarr[i] = DOUBLE_QUOTED_STR;
				i++;
			}
		}
		i++;
	}
}

void	find_here_doc_n_out_append(int *tokenarr)
{
	int	i;

	i = 0;
	while (tokenarr[i])
	{
		if ((tokenarr[i] == OUTFILE_OP) && (tokenarr[i + 1] == OUTFILE_OP))
		{
			tokenarr[i] = OUT_APPEND_OP;
			tokenarr[i + 1] = OUT_APPEND_OP;
			i += 2;
		}
		if ((tokenarr[i] == INFILE_OP) && (tokenarr[i + 1] == INFILE_OP))
		{
			tokenarr[i] = HERE_DOC_OP;
			tokenarr[i + 1] = HERE_DOC_OP;
			i += 2;
		}
		i++;
	}
}

void	find_in_out_files(int *tokenarr)
{
	int	i;
	int	tmp;

	i = 0;
	tmp = 0;
	while (tokenarr[i])
	{
		if ((tokenarr[i] == INFILE_OP) || (tokenarr[i] == OUTFILE_OP) || \
			(tokenarr[i] == HERE_DOC_OP) || (tokenarr[i] == OUT_APPEND_OP))
		{
			tmp = tokenarr[i] * 10;
			i++;
			while ((tokenarr[i] > 0) && (tokenarr[i]))
				i++;
			while ((tokenarr[i] < 0) && (tokenarr[i]))
			{
				tokenarr[i] = tmp;
				i++;
			}
		}
		if (tokenarr[i])
			i++;
	}
}
