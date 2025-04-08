/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 13:17:33 by nlouis            #+#    #+#             */
/*   Updated: 2025/03/27 10:35:02 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file minitalk.h
 * @brief Header file for the Minitalk project.
 *
 * Contains declarations for input validation, error handling,
 * and server/client utilities used in the Minitalk signal-based communication.
 *
 * @author nlouis
 * @date 2025/12/14
 */

/**
 * @defgroup client Client Functions
 * @brief Functions related to the Minitalk client.
 *
 * This group includes all functions used in the client part of the Minitalk
 * project, responsible for sending messages to the server via Unix signals.
 */

/**
 * @defgroup server Server Functions
 * @brief Functions related to the Minitalk server.
 *
 * This group includes all functions used in the server part of the Minitalk
 * project, which listens to signals from the client and reconstructs messages.
 */

/**
 * @defgroup utils Utilities
 * @brief Utility and helper functions for Minitalk.
 *
 * This group contains helper functions shared between client and server, such
 * as input validation and error handling utilities.
 */

#ifndef MINITALK_H
#define MINITALK_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "libft.h"
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <time.h>

void  validate_input_server(int argc, char** argv);
void  display_information_server(pid_t pid);
void  validate_input_client(int argc);
pid_t get_server_pid_from_input(char** argv);

void sys_error(char* error_message);

#endif