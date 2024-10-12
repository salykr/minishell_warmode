/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 16:01:21 by saoun             #+#    #+#             */
/*   Updated: 2024/10/08 14:35:41 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"mini_shell.h"
# define CMD_NOT_EXECUTABLE 126
# include <sys/stat.h>
// parsing-----------------------------1
t_parser *create_parser(void)
{
    t_parser *new_parser;

    new_parser = (t_parser *)malloc(sizeof(t_parser));
    if (new_parser == NULL)
        return (NULL);
    new_parser->command = NULL;
    new_parser->input = NULL;
    new_parser->args = NULL;
    new_parser->operations = NULL;
    new_parser->redirection = NULL;
    new_parser->delimeter = NULL;
    new_parser->prev = NULL;  // Initialize the prev pointer
    new_parser->next = NULL;
    new_parser->infile = NULL;
    new_parser->outfile = NULL;
    return (new_parser);
}
// parsing-----------------------------5
void add_parser_node(t_parser **head, t_parser *new_node)
{
    t_parser *current;

    if (head == NULL || new_node == NULL)
        return;
    if (*head == NULL)
    {
        *head = new_node;
        return;
    }
    current = *head;
    while (current->next != NULL)
        current = current->next;
    current->next = new_node;
    new_node->prev = current;  // Set the prev pointer to the current last node
}
// parsing-----------------------------4
int count_rows(char **array)
{
    int count;

    count = 0;
    if (!array)
        return 0;

    while (array[count] != NULL)
        count++;
    return (count);
}
// parsing-----------------------------4
char **add_string_to_2d_array(char **array, char *new_string)
{
    int current_rows = 0;

    if (array != NULL)
        current_rows = count_rows(array);

    char **new_array = malloc((current_rows + 2) * sizeof(char *));
    if (!new_array)
    {
        perror("malloc");
        return NULL;
    }

    int i = 0;
    while (i < current_rows)
    {
        new_array[i] = array[i];
        i++;
    }

    new_array[i] = ft_strdup(new_string);  // Duplicate the string to avoid double free
    new_array[i + 1] = NULL;

    free(array);  // Free the old array pointer, not the content it points to
    return new_array;
}


// parsing-----------------------------3
int is_executable(char *cmd)
{
    char *path;
    char *path_copy;
    char *dir;
    char full_path[1024];

    path = getenv("PATH");
    if (path == NULL)
        return (0);
    path_copy = ft_strdup(path); // Taking a copy so we can modify it
    dir = ft_strtok(path_copy, ":");
    
    while (dir != NULL)
    {
        ft_strcpy(full_path, dir);
        ft_strcat(full_path, "/");
        ft_strcat(full_path, cmd);
        if (access(full_path, X_OK) == 0)
        {
            // printf("SUCCESS: %s\n", full_path);
            free(path_copy);
            return (1);
        }
        dir = ft_strtok(NULL, ":");
    }
    free(path_copy);
    //printf("FAIL: %s not found\n", cmd);
    return (0);
}
int is_executable_2(t_env env, char *cmd)
{
    char *cmd_1;

    cmd_1 = get_path_1(env,cmd);    
    if (access(cmd_1, X_OK)==0)
    {
        return 1;
    }
    return 0;
}
// parsing-----------------------------6
void add_to_array(t_parser *parser, int new_redirection)
{
    int i = 0;
    int *new_array;

    if (parser->redirection == NULL)
    {
        // Allocate space for the first integer
        parser->redirection = (int *)malloc(sizeof(int) * 2); // Space for one int + NULL terminator
        if (parser->redirection == NULL)
            return; // Handle malloc failure
        parser->redirection[0] = new_redirection;
        parser->redirection[1] = '\0';
    }
    else
    {
        // Find the current size of the redirection array
        while (parser->redirection[i] != '\0')
            i++;

        // Allocate new space for the expanded array
        new_array = (int *)malloc(sizeof(int) * (i + 2)); // Space for existing ints + new int + NULL terminator
        if (new_array == NULL)
            return; // Handle malloc failure

        // Copy the old array into the new one
        // for (int j = 0; j < i; j++)
        //     new_array[j] = parser->redirection[j];
            int j=0;
            while (j<i)
            {
                new_array[j] = parser->redirection[j];
                j++;
            }
            
        // Add the new integer and NULL terminator
        new_array[i] = new_redirection;
        new_array[i + 1] = '\0';

        // Free the old array and update the pointer
        free(parser->redirection);
        parser->redirection = new_array;
    }
}
int is_all_spaces(const char *str) {
    if (str == NULL) return 0;  // Check for null pointer
    while (*str) {
        if (!isspace((unsigned char)*str)) {
            return 0;  // If any character is not a space, return 0 (false)
        }
        str++;
    }
    return 1;  // If only spaces were found, return 1 (true)
}

int isempty(const char *str) {
    // Check if the string is NULL or the first character is the null terminator
    return (str == NULL || str[0] == '\0');
}

static bool	add_detail_quotes(char *command)
{
	if (ft_strncmp(command, "export", 7) == 0
		|| ft_strncmp(command, "unset", 6) == 0)
		return (true);
	return (false);
}
void	free_ptr(void *ptr)
{
	if (ptr != NULL)
	{
		free(ptr);
		ptr = NULL;
	}
}
char	*join_strs(char *str, char *add)
{
	char	*tmp;

	if (!add)
		return (str);
	if (!str)
		return (ft_strdup(add));
	tmp = str;
	str = ft_strjoin(tmp, add);
	free_ptr(tmp);
	return (str);
}
void	errmsg_cmd(char *command, char *detail, char *error_message)
{
	char	*msg;
	bool	detail_quotes;

	detail_quotes = add_detail_quotes(command);
	msg = ft_strdup("minishell: ");
	if (command != NULL)
	{
		msg = join_strs(msg, command);
		msg = join_strs(msg, ": ");
	}
	if (detail != NULL)
	{
		if (detail_quotes)
			msg = join_strs(msg, "`");
		msg = join_strs(msg, detail);
		if (detail_quotes)
			msg = join_strs(msg, "'");
		msg = join_strs(msg, ": ");
	}
	msg = join_strs(msg, error_message);
	ft_putendl_fd(msg, STDERR_FILENO);
	free_ptr(msg);
	// return (error_nb);
}

bool	cmd_is_dir(char *cmd)
{
	struct stat	cmd_stat;
	ft_memset(&cmd_stat, 0, sizeof(cmd_stat));
	stat(cmd, &cmd_stat);
	return (S_ISDIR(cmd_stat.st_mode));
}

char *remove_quotes(const char *str)
{
    char *result = malloc(strlen(str) + 1);
    int j = 0;
    bool in_single = false, in_double = false;

    for (int i = 0; str[i]; i++) {
        if (str[i] == '\'' && !in_double)
            in_single = !in_single;
        else if (str[i] == '"' && !in_single)
            in_double = !in_double;
        else
            result[j++] = str[i];
    }
    result[j] = '\0';
    return result;
}

bool count_dash(char *str)
{
    int i = 0;
    int count =0;
    while (str[i])
    {
        if (str[i]== '-')
        {
            count++;
        }
        i++;
    }
    return (count == 1);    
}
bool ft_check_n_operation(char *str)
{
    int i;

    i = 1;
    while(str[i])
    {
        if(str[i] != 'n')
            return 1;
        i++;
    }
    return 0;
}

//  parsing-----------------------------2
int parse_tokens(t_parser **parser, t_tokenlist *list, t_env env)
{
    if (parser == NULL || *parser == NULL || list == NULL || list->head == NULL) 
    {
        printf("Error: Null pointer passed to parse_tokens.\n");
        return -1;
    }
    char * value;
    t_parser *curr = *parser;
    t_input *tokens = list->head;

    while (tokens != NULL)
    {
        if (tokens->type == T_IDENTIFIER)
        {
            if (!ft_strncmp(tokens->value,"\\n",2))
            {
                if (curr->command==NULL)
                {
                    printf("%s: command not found\n",(tokens->value)+1);
                    global_var=127;
                    return (-1);
                }
            }
            if (!ft_strcmp(tokens->value,":") || !ft_strcmp(tokens->value,"!"))
            {
                return -1;
            }
            
             if(curr->command == NULL && !ft_strncmp(tokens->value,"&&",2))
            {
                printf("bash: syntax error near unexpected token `&&' \n");
                global_var = 2;
                return -1;
            }
            else if (curr->command == NULL && ft_strchr(tokens->value,'/'))
            {
               if (cmd_is_dir(tokens->value))
               {
                    errmsg_cmd(tokens->value, NULL, "Is a directory");
                    global_var=0;
                    return 1;
               }
                else
                {
                    printf("bash: %s : No such file or directory\n",tokens->value);
                    global_var=127;
                    return -1;
                }
            }
           if (!check_balanced_quotes(tokens->value))
            {
                printf("Error: Unbalanced quotes in argument");
                global_var=1;
                return -1;
            }
            else
            {
                 value = remove_quotes(tokens->value);
            }
            if (value == NULL || value[0] == '\0')  
            {
                tokens = tokens->next;
                continue;
            }
            if ((is_executable(value) || 
                ft_strncmp(value, "cd", 2) == 0 || 
                ft_strncmp(value, "exit", 4) == 0 || 
                ft_strncmp(value, "export", 6) == 0 || 
                ft_strncmp(value, "unset", 5) == 0) &&( (curr->command == NULL)&& !is_all_spaces(value)))
            {
                curr->command = value;
            }
            else if (curr->command == NULL || !tokens->value[0])
            {
                if (is_all_spaces(tokens->value))
                {
                    return (-1);
                }
                printf("%s: command not found\n", value);
                global_var=127;
                return (-1);
            }
            else
            {
                curr->input = add_string_to_2d_array(curr->input, tokens->value);
            }
        }
        else if (tokens->type == T_PATH)
        {
            if (curr->command == NULL && cmd_is_dir(tokens->value))
            {
                errmsg_cmd(tokens->value, NULL, "Is a directory");
                global_var=0;
                return 1;
            }
            else
            {
                if (!ft_strncmp(tokens->value,"./",2))
                {
                    if (curr->command==NULL && (is_executable(tokens->value) || is_executable_2(env, tokens->value)))
                    {
                        curr->command = tokens->value;
                        tokens = tokens->next;
                    }
                    else if(curr->command != NULL && cmd_is_dir(tokens->value))
                    {
                        curr->input = add_string_to_2d_array(curr->input, tokens->value);
                        tokens = tokens->next;
                    }
                    else
                    {
                        printf("bash: %s : No such file or directory\n",tokens->value);
                        global_var=127;
                        return -1;
                    }
                    continue;
                }
            }
            char *p = ft_strrchr(tokens->value, '/');
            if (p != NULL) 
                p++;
            if (*p != '\0')
            {
                if (is_executable(p) || ft_strncmp(p, "cd", 2) == 0 || ft_strncmp(p, "exit", 4) == 0 || ft_strncmp(p, "export", 6) == 0 || ft_strncmp(p, "unset", 5) == 0)
                    curr->command = p;
                else{
                    curr->input = add_string_to_2d_array(curr->input, tokens->value);
                }
            }
            else
            {
                curr->input = add_string_to_2d_array(curr->input, tokens->value);
            }
        }
        else if (tokens->type == T_ARGUMENT)
        {
            if (!ft_strcmp(curr->command,"echo"))
            {
                int  i=0;
                if(count_dash(tokens->value) && i ==0 && !ft_check_n_operation(tokens->value) && curr->input ==NULL)
                {
                    curr->operations = add_string_to_2d_array(curr->operations, tokens->value);
                }
                else
                {
                    curr->input = add_string_to_2d_array(curr->input, tokens->value);
                    i++;             
                }
            }
            else
            {
                curr->operations = add_string_to_2d_array(curr->operations, tokens->value);
            }
        }
        else if (tokens->type == T_PIPE)
        {
            if(curr->command == NULL || tokens->next ==NULL)
            {
                printf("bash: syntax error near unexpected token `|' \n");
                global_var=2;
                return -1;
            }
            add_parser_node(parser, create_parser());
            curr = curr->next;
        }
        else if (tokens->type == T_TILDE)
        {
            curr->input = add_string_to_2d_array(curr->input, tokens->value);
        }
        else if (tokens->type == T_ENV || tokens->type == T_NUMBER || tokens->type == T_PERIODS || tokens->type == T_QUOTE)
        {
            //             if (!strncmp(tokens->value,"$",1))
            // {
            //     char *value = ft_getenv(&env,tokens->value);
            //     printf("%s testttttt\n",value);
            //     char *cmd =ft_strjoin(value,"/");
            //     printf("%s\n",cmd);
            //    if (cmd_is_dir(cmd))
            //    {
            //         errmsg_cmd(tokens->value, NULL, "Is a directory");
            //         global_var=0;
            //         return 1;
            //    }
            //     if (value)
            //         printf("%s", value);
            //     tokens=tokens->next;
            //         continue;
            // }
            if ((tokens->value[0] == '\'' && tokens->value[1] == '\'' &&!tokens->value[2]) || (tokens->value[0] == '\"' && tokens->value[1] == '\"' && !tokens->value[2]))
                {
                    curr->input = add_string_to_2d_array(curr->input, tokens->value);
                    tokens=tokens->next;
                    continue;
                }  
            char * value = remove_quotes(tokens->value);
            if (curr->command == NULL &&(is_executable(value) || ft_strncmp(value, "cd", 2) == 0 || ft_strncmp(value, "exit", 4) == 0 || ft_strncmp(value, "export", 6) == 0 || ft_strncmp(value, "unset", 5) == 0))
            {
                curr->command = value;
                tokens = tokens->next;
                continue;
            }
            if (curr->command!=NULL &&!ft_strcmp (curr->command,"echo"))
            {
                curr->input = add_string_to_2d_array(curr->input, tokens->value);
                tokens =tokens->next;
                continue;
            }      
            if (curr->command == NULL)
            {
                printf("bash:%s: command not found", value);
                global_var=127;
                return -1;
            }

            // echo "-n" Hola (for this case)
            else if (!strncmp(tokens->value,"\"-",2))
            {
                 curr->operations = add_string_to_2d_array(curr->operations, tokens->value);
            }
            else
            {
                curr->input = add_string_to_2d_array(curr->input, value);
            }
        }
        else if (tokens->type == T_APPEND || tokens->type == T_HEREDOC || tokens->type == T_OUTPUT || tokens->type == T_INPUT)
        {
            // if (curr-> == NULL)
            // {
            //     printf("bash: syntax error near unexpected token `%s'\n", tokens->value);
            //     return -1;
            // }
            if (tokens->next == NULL)
            {
                printf("bash: syntax error near unexpected token `newline`\n");
                global_var=2;
                return (-1);
            }
            if (tokens->type == T_APPEND && tokens->next->type == T_IDENTIFIER)
            {
                curr->outfile = add_string_to_2d_array(curr->outfile, tokens->next->value);
                add_to_array(curr, 0);
                tokens = tokens->next;
            }
            else if (tokens->type == T_HEREDOC && (tokens->next->type == T_ENV || tokens->next->type == T_IDENTIFIER))
            {
                curr->delimeter = tokens->next->value;
                add_to_array(curr, 1);
                tokens = tokens->next;
            }
            else if (tokens->type == T_INPUT && tokens->next->type == T_IDENTIFIER)
            {
                curr->infile = tokens->next->value;
                add_to_array(curr, 3);
                tokens = tokens->next;
            }
            else if (tokens->type == T_OUTPUT && tokens->next->type == T_IDENTIFIER)
            {
                curr->outfile = add_string_to_2d_array(curr->outfile, tokens->next->value);
                add_to_array(curr, 2);
                tokens = tokens->next;
            }
        }
        tokens = tokens->next;
    }
     return 0;
}
