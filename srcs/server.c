/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 13:42:43 by nlouis            #+#    #+#             */
/*   Updated: 2025/03/27 10:29:35 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file server.c
 * @brief Minitalk server that receives messages bit-by-bit using UNIX signals.
 *
 * The server listens for `SIGUSR1` and `SIGUSR2` signals representing
 * binary 1 and 0. It reconstructs each character from the received bits and
 * prints the message to standard output. An acknowledgment signal is sent
 * back to the client after each bit.
 * 
 * This server resets its internal state once it receives a null terminator.
 * It supports communication from one client at a time.
 * 
 * @author nlouis
 * @date 2024/12/14
 * @ingroup server
 */
#include "minitalk.h"

/**
 * @brief Stores the PID of the client currently communicating with the server.
 *
 * This variable is declared as volatile because it is modified asynchronously
 * inside a signal handler. It must also be of type sig_atomic_t to ensure that
 * read and write operations are atomic and safe during signal handling.
 *
 * It is initialized to 0 and updated with the sender's PID when a signal is
 * received. This value is used by the server to send acknowledgment signals
 * back to the correct client.
 * 
 * @ingroup server
 */
volatile sig_atomic_t g_client_pid = 0;

/**
 * @brief Processes a single received signal and updates the current character.
 *
 * This function modifies the character `c` by setting or clearing a specific bit
 * depending on the received signal (`SIGUSR1` or `SIGUSR2`). The `bit` index is
 * decremented after each update to prepare for the next incoming bit.
 *
 * @param sig The received signal. `SIGUSR1` represents bit 1, `SIGUSR2` bit 0.
 * @param bit A pointer to the current bit index (starting from 7 to 0).
 * @param c A pointer to the character being constructed bit by bit.
 * 
 * @ingroup server
 */
static void	handle_received_bit(int sig, int *bit, char *c)
{
	if (sig == SIGUSR1)
		*c |= (1 << *bit);		// Set bit to 1
	else
		*c &= ~(1 << *bit);		// Set bit to 0
	(*bit)--;
}

/**
 * @brief Processes a fully received character and resets bit tracking.
 *
 * This function is called when all 8 bits of a character have been received.
 * If the character is a null terminator (`'\0'`), it indicates the end of the
 * message and prints a newline. Otherwise, it prints the character to the
 * standard output. After processing, it resets the bit index and character
 * for the next incoming byte.
 *
 * @param c Pointer to the character that has been constructed from bits.
 * @param bit Pointer to the current bit index; reset to 7 after processing.
 * 
 * @note If `write`, the program exits with an error message using `sys_error()`.
 * 
 * @ingroup server
 */
static void	process_character(char *c, int *bit)
{
	if (*bit < 0)
	{
		if (*c == '\0')
		{
			if (write(1, "\n", 1) == -1)
				sys_error("Server: write failed");
		}
		else
		{
			if (write(1, c, 1) == -1)
				sys_error("Server: write failed");
		}
		*bit = 7;
		*c = 0;
	}
}

/**
 * @brief Signal handler for the server process.
 *
 * This function is called asynchronously when the server receives a signal.
 * It processes the incoming signal, updates the current character being
 * constructed from bits, and sends an acknowledgment signal back to the client.
 *
 * The client's PID is extracted from the `siginfo_t` structure and stored
 * in the global variable `g_client_pid` for acknowledgment purposes.
 *
 * @param sig The received signal (either `SIGUSR1` or `SIGUSR2`).
 * @param info Information about the signal, including the sender's PID.
 * @param context Additional context information (unused).
 * 
 * @note If `kill` fails to send the signal, the program exits with an
 * error message using `sys_error()`.
 * 
 * @ingroup server
 */
void	signal_handler(int sig, siginfo_t *info, void *context)
{
	static int		bit = 7;
	static char		c = 0;

	(void)context;
	g_client_pid = info->si_pid;

	handle_received_bit(sig, &bit, &c);
	process_character(&c, &bit);

	if (kill(g_client_pid, SIGUSR1) == -1)
		sys_error("Server: ACK failed");
}

/**
 * @brief Configures signal handling for SIGUSR1 and SIGUSR2.
 *
 * This function sets up the server to handle incoming signals used for
 * interprocess communication. It assigns the signal handler function
 * `signal_handler` for both SIGUSR1 and SIGUSR2 using `sigaction`.
 *
 * The `SA_SIGINFO` flag allows access to extra information about the
 * signal, including the sender's PID. `SA_RESTART` ensures that certain
 * system calls interrupted by signals are automatically restarted.
 *
 * If the signal registration fails, an error message is printed and
 * the program exits using `sys_error`.
 * 
 * @ingroup server
 */
void	setup_signals(void)
{
	struct sigaction	sa;

	sa.sa_sigaction = signal_handler;
	sa.sa_flags = SA_SIGINFO | SA_RESTART;
	sigemptyset(&sa.sa_mask);

	if (sigaction(SIGUSR1, &sa, NULL) == -1)
		sys_error("Server: SIGUSR1 setup failed");
	if (sigaction(SIGUSR2, &sa, NULL) == -1)
		sys_error("Server: SIGUSR2 setup failed");
}

/**
 * @brief Entry point for the server application.
 *
 * This function sets up the server to receive messages from a client via
 * Unix signals. It performs input validation, retrieves and displays the
 * server's PID, and configures signal handlers for SIGUSR1 and SIGUSR2.
 *
 * The server then enters an infinite loop, waiting for incoming signals
 * using `pause()`, which suspends execution until a signal is received.
 *
 * @param argc Argument count (should be 1 for server).
 * @param argv Argument vector (not used).
 * @return int returns EXIT_SUCCESS (not actually reached).
 * 
 * @ingroup server
 */
int main(int argc, char **argv)
{
	pid_t	pid;
	
	validate_input_server(argc, argv);
	pid = getpid();
	display_information_server(pid);
	setup_signals();

	while (true)
		pause();

	return (EXIT_SUCCESS);
}

