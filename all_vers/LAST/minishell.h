/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkenned <fkenned@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/07 18:28:47 by fkenned           #+#    #+#             */
/*   Updated: 2021/11/07 18:29:24 by fkenned          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"
# include <signal.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <curses.h>
# include <term.h>
# include <dirent.h>
# include "pipex/gnl/get_next_line.h"
# define AND_MODE 2
# define BOTH_MOD 12

int	g_question;

typedef struct s_parser_info
{
	int	start;
	int	end;
	int	temp;
	int	quote;
}				t_parser_info;

typedef struct s_files
{
	int				mode;
	int				pos;
	char			*filename;
	struct s_files	*next;
}				t_files;

typedef struct s_pos
{
	int				pos;
	int				red;
	struct s_pos	*next;
}				t_pos;

typedef struct s_envp
{
	char			*var;
	struct s_envp	*next;
}				t_envp;

typedef struct s_pipes
{
	int				num;
	char			*cmd;
	char			**cmd_argv;
	int				size_cmd_argv;
	t_files			*infile;
	t_files			*outfile;
	t_pos			*pos_outred;
	t_pos			*pos_inred;
	struct s_pipes	*next;
}				t_pipes;

typedef struct s_data
{
	char	*error;
	char	*line;
	int		col_pipes;
	int		get_pid;
	t_envp	*env;
	t_envp	*exp;
	t_pipes	*pipes;
	t_pos	*pos_pipe;
}				t_data;

int		main(int argc, char **argv, char **envp);
void	parser(t_data *data);
//determination of pipe positions
void	search_pipe_position(t_data *data);
t_pos	*list_position(t_pos *list, int pos, int redirect);
//removing spaces in a line
void	remove_spaces_in_line(t_data *data);
//trimming the desired character around the edges
char	*ft_chartrim(char *s1, char set);
//trim line by pipe
void	cut_pipe(t_data *data);
t_pipes	*list_pipe(t_pipes *list, char *str, int num);
//trimming spaces in areas trimmed by pipes
void	remove_spaces_pipe(t_data *data);
//error checking in indirects
void	check_inredirect(t_data *data);
int		help_inred(t_data *data, t_pipes *p, int i, int red);
//error checking in outdirects
void	check_outredirect(t_data *data);
int		help_outred(t_data *data, t_pipes *p, int i, int red);
//checking files with a name from numbers
void	check_space_numfile(t_data *data);
//filling out the sheet with file names
void	make_list(t_data *data, char redrct);
t_files	*list_file(t_files *list, char *str, int redirect);
t_files	*fill_list(t_data *data, t_pipes *pipes, char redrct);
//eof detection
int		end_file(char *str, int start, int len);
//determination of redirect positions
void	search_redirect_position(t_data *data, char redrct);
//case handling "ls|"
void	heredoc_pipe(t_data *data);
int		search_pipe_herdoc(t_data *data);
void	check_heredoc_file(t_data *data);
//case handling "<<"
void	make_heredoc_files(t_data *data);
int		check_heredoc_eof(char *name);
int		check_str(int file, char *str, char *fname, int col);
void	error_heredoc_redrct_ctrl_d(char *name, int col);
int		counter_double_redrct(t_files *infile);
//signals
void	ctrl_c(int status);
void	ctrl_c_redirect_heredoc(int status);
void	ctrl_c_pipe_heredoc(int status);
void	ctrl_slash(int status);
void	ctrl_c_fork(int signal);
//cutting out filenames with redirects
void	cutting_file(t_data *data);
//cutting out redirects
void	cutting_redirect(t_data *data);
//removing spaces in a line cmd
void	remove_spaces_cmd(t_data *data);
//removing spaces in filenames
void	remove_spaces_file(t_data *data);
//handling quotes and dollar
void	quotes_and_dollar(t_data *data);
char	*handling_quotes(t_data *data, char *str);
char	*check_dollar(t_data *data, char *str, int lenght_str, int cmd_len);
char	*help_check_dollar(char *str, char *new);
int		dollar_change(char *str, char **new, int i, t_data *data);
int		help_dollar_change(char *str, char **new, int i, char *dollar);
char	*dollar_and_end_line(char *str, char *new, int i);
int		dollar_question(int i, char *str, char **new);
int		dollar_pid(t_data *data, int i, char *str, char **new);
char	*dollar_and_new_is_null(char *str, char *new, int i);
char	*dollar_and_end_line(char *str, char *new, int i);
char	*search_envp_dollar(t_data *data, char *dollar);
int		compare_dollar_and_exp(char *exp, char *dollar);
//creating an array for transferring to pipex
void	make_cmd_argv(t_data *data);
//process pid determination
void	get_pid(t_data *data, char **envp);
//saving error code in рerdock
void	make_heredoc_question_error(int num);
//skipping quotes
int		skip_quotes(char *str, int quotes, int i);
//error definition
void	ft_error(t_data *data, char *error);
//cleaning
void	clean_position(t_pos *list);
void	cleaning_all(t_pipes *list);
void	clean_exp_env(t_envp *list);
//PIPEX PART
char	**make_new_envp(char **envp);
char	**refresh_envp(t_envp *env, char ***envp);
t_envp	*add_list_envp(t_envp *list, char *str);
t_envp	*make_envp(t_envp *list, char **envp);
void	init_data(t_data *data, char ***envp);
int		is_built_in(t_data *data, char **cmd, char ***envp);
int		check_built_in(char *cmd);
int		ft_cd(t_data *data, char **cmd, char ***envp);
int		ft_echo(char **cmd);
int		ft_env(t_data *data, char **cmd);
int		ft_exit(t_data *data, char **cmd);
int		export_str(t_data *data, char *var, char *value, char ***envp);
char	*find_var_name(char *str);
int		print_export(t_data *data);
int		ft_export(t_data *data, char **cmd, char ***envp);
int		check_is_there_value(char *str);
int		check_existance(t_envp *list, char *var);
char	**make_new_envp(char **envp);
char	**refresh_envp(t_envp *env, char ***envp);
void	free_list_envp(t_envp *list);
int		ft_pwd(char **cmd);
int		ft_unset(t_data *data, char **var, char ***envp);
int		**make_fds(int number_of_pipes);
void	close_fds(int number_of_pipes, int **fd);
void	free_fds(int number_of_pipes, int **fd);
char	**ftt_split(char const *s, char c);
char	**free_args_and_return_null(int temp, char **strs);
int		quote_case(char *s, char c);
int		count_words(char *s, t_parser_info info);
char	**split_args(char *s);
void	free_array(char **arr);
char	**get_exec_arguments(char *envp[], char **cmd_args, \
t_data *data, int num);
int		get_out_fd(t_data *data, int i);
int		get_in_fd(t_data *data, int i);
int		work_without_pipes(t_data *data, char ***envp);
void	check_status_without_pipe(int status, char **cmd);
int		no_pipes_dupping(t_data *data);
void	help_get_path(t_data *data, int num, char **cmd_args);
int		make_here_doc_file(char *limiter, char *name);
char	*make_filename(int i);
int		handle_multipipes(t_data *data, char ***envp, int **fd, int *pid);
int		unlink_here_doc_files(t_data *data);
int		pipex(t_data *data, char ***envp);

#endif