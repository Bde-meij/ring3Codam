/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbrune <rbrune@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/16 12:40:31 by rbrune        #+#    #+#                 */
/*   Updated: 2023/01/18 10:19:52 by rbrune        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "bighell.h"

int	g_exit_code;

int	unclosed_quotes(char *str);
int	check_blank(char *str);

int	count_pipes(t_bighell *bighell)
{
	t_list	*tmp;
	int		n_pipes;

	tmp = bighell->head_token;
	n_pipes = 1;
	while (bighell->head_token->next)
	{
		if (((t_tokenlist *)bighell->head_token->content)->token == PIPE)
			n_pipes++;
		bighell->head_token = bighell->head_token->next;
	}
	bighell->head_token = tmp;
	return (n_pipes);
}

int	lexer(t_bighell *bighell, char *str)
{
	if (unclosed_quotes(str))
	{
		free(str);
		return (131);
	}
	bighell->compare_str = " <>|\'\" ";
	bighell->line = str;
	add_history(bighell->line);
	if (ft_strcmp(str, "\"\"") == 0 || ft_strcmp(str, "\'\'") == 0)
		return (print_errors(127), 127);
	bighell->line = parse_dollar_sign(bighell->line, bighell);
	if (check_blank(bighell->line))
		return (1);
	if (!*bighell->line)
		return (1);
	bighell->line_len = ft_strlen(bighell->line);
	bighell->tokenarr = protec(malloc((bighell->line_len + 1) * sizeof(int)));
	bighell->tokenarr[bighell->line_len] = '\0';
	tokenize_input(bighell);
	enlist(bighell, 0);
	bighell->n_pipes = count_pipes(bighell);
	free(bighell->tokenarr);
	return (0);
}

int	instructor(t_bighell *bighell, char *str)
{
	if (str == NULL)
	{
		printf("BigHell: ");
		printf("\x1b[1A");
		printf("exit\n");
		exit(0);
	}
	if (str[0] < 32)
	{
		free(str);
		return (0);
	}
	if (lexer(bighell, str))
		return (0);
	cmd_exe(bighell);
	free(bighell->line);
	return (0);
}

int	main(int ac, char *av[], char **envp)
{
	struct termios	term_struct;
	t_bighell		bighell;

	(void)ac;
	(void)av;
	g_exit_code = 0;
	bighell.new_env = NULL;
	bighell.in = dup(0);
	bighell.out = dup(1);
	tcgetattr(STDIN_FILENO, &term_struct);
	tcgetattr(STDOUT_FILENO, &term_struct);
	tcgetattr(STDERR_FILENO, &term_struct);
	term_struct.c_lflag &= ~(ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &term_struct);
	make_envp_list(envp, &bighell);
	signal(SIGINT, general_signal);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		instructor(&bighell, readline("BigHell: "));
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &term_struct);
		tcsetattr(STDOUT_FILENO, TCSAFLUSH, &term_struct);
		tcsetattr(STDERR_FILENO, TCSAFLUSH, &term_struct);
	}
}
