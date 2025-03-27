/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 13:39:13 by nlouis            #+#    #+#             */
/*   Updated: 2025/03/27 10:28:33 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file client.c
 * @brief Sends a string message to the Minitalk server using UNIX signals.
 *
 * This file implements the client-side logic of the Minitalk project.
 * The client converts each character of a string message into bits
 * and sends them one by one to the server using SIGUSR1 and SIGUSR2.
 * After each bit, the client waits for an acknowledgment from the server.
 *
 * The message is terminated with a null byte ('\0').
 *
 * @author nlouis
 * @date 2024/12/14
 * @ingroup client
 */
#include "minitalk.h"

/**
 * @brief Acknowledgment flag set by the server.
 *
 * This global variable is used by the client to synchronize the sending
 * of bits. After each bit is sent via `kill()`, the client waits until
 * the server responds with a SIGUSR1 signal. This signal sets `g_ack_received`
 * to 1, allowing the client to proceed with sending the next bit.
 *
 * It is declared as `volatile sig_atomic_t` to ensure:
 * - `volatile`: The compiler doesn't optimize out reads/writes due to changes
 *    happening asynchronously from a signal handler.
 * - `sig_atomic_t`: Ensures the variable is accessed atomically and safely
 *    across signal handler and main code context.
 * @ingroup client
 */
volatile sig_atomic_t	g_ack_received = 0;

/**
 * @brief Signal handler for SIGUSR1 sent by the server to acknowledge
 * receipt of a bit.
 *
 * This function is called asynchronously when the client receives the
 * `SIGUSR1` signal from the server, which acts as an acknowledgment that
 * a bit has been successfully received and processed.
 *
 * The signal number (`sig`) is not used here, as the handler only cares
 * that *a signal was received*, not its value — so it is explicitly cast
 * to void to avoid unused parameter warnings.
 *
 * When triggered, the function sets the global variable `g_ack_received` to 1,
 * informing the main sending loop in the client that it can proceed
 * to the next bit.
 *
 * @param sig The signal number received (expected to be SIGUSR1).
 * 
 * @ingroup client
 */
void	ack_handler(int sig)
{
	(void)sig;
	g_ack_received = 1;
}


/**
 * @brief Sets up the signal handler for SIGUSR1 to acknowledge received bits.
 *
 * This function configures the client to listen for the `SIGUSR1` signal,
 * which the server sends to acknowledge receipt of a single bit.
 * 
 * It uses the `sigaction` system call to set the `ack_handler` function
 * as the signal handler. The `SA_RESTART` flag ensures that interrupted
 * system calls (like `pause()` or `read()`) are automatically restarted
 * after the signal handler returns.
 * 
 * The signal mask is initialized to an empty set, meaning no signals are
 * blocked while the handler runs.
 * 
 * @note If `sigaction` fails to set the handler, the program exits with an
 * error message using `sys_error()`.
 * 
 * @ingroup client
 */
void	setup_ack_signal(void)
{
	struct sigaction sa;

	sa.sa_handler = ack_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
		sys_error("Client: sigaction failed");
}

/**
 * @brief Sends a single character to the server process, bit by bit,
 * via UNIX signals.
 *
 * This function encodes the character `c` into binary and sends each
 * bit individually to the server process identified by `pid`. Each bit
 * is sent using either:
 * - `SIGUSR1` for a bit value of 1
 * - `SIGUSR2` for a bit value of 0
 *
 * After each signal is sent, the function waits until it receives an
 * acknowledgment from the server (via the `g_ack_received` global variable),
 * ensuring reliable delivery and synchronization between client and server.
 *
 * A short `usleep` delay is used to avoid overwhelming the server with
 * signals in quick succession, and to account for context switching and
 * signal delivery time.
 *
 * @param pid The process ID of the server to which signals should be sent.
 * @param c The character to send to the server, one bit at a time
 * (most significant bit first).
 * 
 * @note If `kill` fails to send the signal, the program exits with an
 * error message using `sys_error()`.
 * 
 * @ingroup client
 */
static void	send_char_bits(pid_t pid, char c)
{
	int	bit;

	bit = 7;
	while (bit >= 0)
	{
		g_ack_received = 0;
		if ((c >> bit--) & 1)
		{
			if (kill(pid, SIGUSR1) == -1)
				sys_error("Failed to send SIGUSR1");
		}
		else
		{
			if (kill(pid, SIGUSR2) == -1)
				sys_error("Failed to send SIGUSR2");
		}

		while (!g_ack_received)
			usleep(100);
		usleep(100);
	}
}

/**
 * @brief Sends a null-terminated string to the server via signals.
 *
 * Each character of the message is sent one by one using the
 * send_char_bits function, which transmits individual bits to the
 * server process identified by pid.
 *
 * After the entire string is sent, a null character ('\0') is sent
 * to signal the end of transmission to the server.
 *
 * @param pid The PID of the server process to which the message is sent.
 * @param msg The null-terminated message string to transmit.
 * 
 * @ingroup client
 */
static void	send_message(pid_t pid, const char *msg)
{
	while (*msg)
		send_char_bits(pid, *msg++);
	send_char_bits(pid, '\0');
}

/**
 * @brief Entry point of the client program.
 *
 * Validates the command-line arguments, extracts the server PID,
 * sets up the signal handler for acknowledgments, and sends the
 * message string to the server bit by bit. Prints a confirmation
 * message upon successful transmission.
 *
 * Usage: ./client <PID> "<MESSAGE>"
 *
 * @param argc Argument count; should be exactly 3.
 * @param argv Argument vector; expects the server PID and message.
 * @return int EXIT_SUCCESS on success, or exits with error otherwise.
 * 
 * @ingroup client
 */
int	main(int argc, char **argv)
{
	pid_t	pid;

	validate_input_client(argc);
	pid = get_server_pid_from_input(argv);
	setup_ack_signal();
	send_message(pid, argv[2]);
	ft_putstr_fd("Message sent successfully!\n", STDIN_FILENO);
	return (EXIT_SUCCESS);
}
