/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpah <elpah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 00:23:58 by elpah             #+#    #+#             */
/*   Updated: 2025/04/12 00:26:35 by elpah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

void printError(char *str)
{
	while (*str)
		write(2, str++, 1);
}

int cd(char **argv, int i)
{
	if (i != 2)
		return (printError("error: cd: bad arguments\n"), 1);
	if (chdir(argv[1]) == -1)
		return (printError("error: cd: cannot change directory to "),
				printError(argv[1]), printError("\n"), 1);
	return (0);
}

void set_pipe(int has_pipe, int *fd, int end)
{
	if (has_pipe && (dup2(fd[end], end) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1))
	{
		printError("error: cannot duplicate file descriptor\n");
		exit(1);
	}
}

int exec(char **argv, int i, char **envp)
{
	int has_pipe;
	int fd[2];
	int pid;
	int status;
	has_pipe = argv[i] && !strcmp(argv[i], "|");
	if (has_pipe && !strcmp(*argv, "cd"))
		return (cd(argv, i));
	if (has_pipe && pipe(fd) == -1)
	{
		printError("error: cannot create pipe\n");
		exit(1);
	}
	pid = fork();
	if (!pid)
	{
		argv[i] = 0;
		set_pipe(has_pipe, fd, 1);
		if (!strcmp(*argv, "cd"))
			exit(cd(argv, i));
		execve(*argv, argv, envp);
		printError("error: cannot execute ");
		printError(*argv);
		printError("\n");
		exit(1);
	}
	waitpid(pid, &status, 0);
	set_pipe(has_pipe, fd, 0);
	return (WIFEXITED(status) && WEXITSTATUS(status));
}

int main(int argc, char **argv, char **envp)
{
	int i = 0;
	int status = 0;
	(void)argc;
	while (argv[i])
	{
		argv += i + 1;
		i = 0;
		while (argv[i] && strcmp(argv[i], "|") && strcmp(argv[i], ";"))
			i++;
		if (i)
			status = exec(argv, i, envp);
	}
	return (status);
}
