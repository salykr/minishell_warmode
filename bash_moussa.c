/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bash_moussa.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 14:42:47 by skreik            #+#    #+#             */
/*   Updated: 2024/10/01 15:29:03 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

void configure_terminal(struct termios *original) {
    struct termios new_settings;

    // Get the current terminal settings
    tcgetattr(STDIN_FILENO, &new_settings);
    *original = new_settings;

    // Set terminal to canonical mode (icanon)
    //new_settings.c_lflag |= ICANON;
    //new_settings.c_lflag &= ~ECHO; // Turn off echoing
    new_settings.c_cc[VEOF] = 0x04; // Set EOF character to ^D

    // Apply the new settings
    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);
}

void restore_terminal(struct termios *original) {
    // Restore original terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, original);
}

int main() {
    struct termios original_settings;
    char buffer[BUFFER_SIZE];
    int bytes_read;

    configure_terminal(&original_settings);

    printf("Type something (Ctrl+D to end):\n");

    while (1) {
        // Read input from the terminal
        bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);

        // Check for EOF (^D)
        if (bytes_read == 0) {
            printf("\nEOF detected. Exiting...\n");
            break;
        }

        // Null-terminate the string
        buffer[bytes_read] = '\0';

        // Process the input (you can customize this part)
        printf("You typed: %s", buffer);
    }

    restore_terminal(&original_settings);

    return 0;
}
