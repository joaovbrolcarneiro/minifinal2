/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* executor_utils_builtins.c                          :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hde-barr <hde-barr@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/04/15 12:15:02 by hde-barr          #+#    #+#             */
/* Updated: 2025/04/20 18:30:00 by hde-barr         ###   ########.fr       */ // (Update date)
/* */
/* ************************************************************************** */

#include "minishell.h"

/*
** Builtin echo command
*/



static bool	is_str_numeric(char *arg_str)
{
	int	i;

	i = 0;
	if (!arg_str)
		return (false);
	if (arg_str[i] == '+' || arg_str[i] == '-')
		i++;
	if (arg_str[i] == '\0')
		return (false);
	while (arg_str[i])
	{
		if (!ft_isdigit(arg_str[i]))
			return (false);
		i++;
	}
	return (true);
}

/*
** Helper for ft_exit: Parses numeric argument
** Returns 0 on success, sets exit_val, returns 1 on error
*/
static int	parse_exit_arg(char *arg_str, int *exit_val)
{
	if (!is_str_numeric(arg_str))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(arg_str, STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		*exit_val = 2;
		return (1);
	}
	*exit_val = ft_atoi(arg_str) % 256;
	if (*exit_val < 0)
		*exit_val += 256;
	return (0);
}

/*
** Handles the exit command with one argument.
** Parses the argument, cleans up, and exits.
*/
static int	handle_exit_one_arg(char *arg_str, t_shell *shell)
{
	int	exit_val_to_use;
	int	parse_result;

	parse_result = parse_exit_arg(arg_str, &exit_val_to_use);
	if (parse_result != 0)
	{
		cleanup_shell(shell);
		exit(exit_val_to_use);
	}
	else
	{
		cleanup_shell(shell);
		exit(exit_val_to_use);
	}
	return (0);
}

/*
** Handles the exit command with no arguments.
** Uses the global exit code, cleans up, and exits.
*/
static int	handle_exit_no_arg(t_shell *shell)
{
	int	exit_val_to_use;

	exit_val_to_use = g_exit_code;
	cleanup_shell(shell);
	exit(exit_val_to_use);
	return (0);
}

/*
** Builtin exit command
*/
int	ft_exit(char **args, t_shell *shell)
{
	ft_putstr_fd("exit\n", STDERR_FILENO);
	if (args[1] && args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		g_exit_code = 1;
		return (1);
	}
	else if (args[1])
	{
		return (handle_exit_one_arg(args[1], shell));
	}
	else
	{
		return (handle_exit_no_arg(shell));
	}
}