/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 21:59:29 by nlouis            #+#    #+#             */
/*   Updated: 2025/03/27 10:30:25 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file utils.c
 * @brief Input validation and utility functions for the Minitalk project.
 *
 * This file contains helper functions for validating command-line arguments,
 * displaying the server's PID, and handling system errors gracefully.
 *
 * These functions are used by both the client and server to ensure proper
 * argument formats and reliable error messaging.
 *
 * @author nlouis
 * @date 2024/12/14
 * @ingroup utils
 */
#include "minitalk.h"

/**
 * @brief Displays the server's PID and a waiting message.
 *
 * @param pid The process ID of the server.
 *
 * @ingroup utils
 */
void display_information_server(pid_t pid)
{
	printf("PID: %d\n", pid);
	printf("Waiting for a message...\n");
}

/**
 * @brief Validates the number of arguments passed to the server.
 *
 * @param argc Argument count.
 * @param argv Argument values (unused).
 *
 * Exits with an error message if the argument count is invalid.
 *
 * @ingroup utils
 */
void validate_input_server(int argc, char** argv)
{
	(void) argv;
	if (argc != 1)
	{
		fprintf(stderr, "Error: wrong format\n");
		fprintf(stderr, "Usage: ./server\n");
		exit(EXIT_FAILURE);
	}
}

/**
 * @brief Validates the number of arguments passed to the client.
 *
 * @param argc Argument count.
 *
 * Exits with an error message if the argument count is invalid.
 *
 * @ingroup utils
 */
void validate_input_client(int argc)
{
	if (argc != 3)
	{
		fprintf(stderr, "Error: wrong format\n");
		fprintf(stderr, "Usage: ./client <PID> <\"MESSAGE\">\n");
		exit(EXIT_FAILURE);
	}
}

/**
 * @brief Extracts and validates the server PID from the client's input.
 *
 * @param argv Argument values (from main).
 * @return pid_t The converted and validated PID.
 *
 * Exits with an error if the PID is invalid or non-positive.
 *
 * @ingroup utils
 */
pid_t get_server_pid_from_input(char** argv)
{
	pid_t pid;

	pid = ft_atoi(argv[1]);
	if (pid <= 0)
	{
		fprintf(stderr, "Error: invalid PID.\n");
		exit(EXIT_FAILURE);
	}
	return (pid);
}

/**
 * @brief Prints an error message and exits the program.
 *
 * @param error_message A custom error message to display.
 *
 * Also prints the corresponding system error message via perror.
 *
 * @ingroup utils
 */
void sys_error(char* error_message)
{
	fprintf(stderr, "Error: %s\n", error_message);
	perror("System call error");
	exit(EXIT_FAILURE);
}