/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isalayan <isalayan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 16:01:21 by isalayan          #+#    #+#             */
/*   Updated: 2025/04/21 12:53:04 by isalayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

int	handle_parsing_argument(t_input *tokens, t_parser *curr)
{
	int	i;

	i = 0;
	if (curr->command == NULL)
		return (-1);
	else if (!ft_strcmp(curr->command, "echo"))
	{
		if (count_dash(tokens->value) && i == 0
			&& !ft_check_n_operation(tokens->value) && curr->input == NULL)
			curr->operations = add_string_to_2d_array(curr->operations,
					tokens->value);
		else
		{
			curr->input = add_string_to_2d_array(curr->input, tokens->value);
			curr->args = add_string_to_2d_array(curr->args, tokens->value);
			i++;
		}
	}
	else
	{
		curr->operations = add_string_to_2d_array(curr->operations,
				tokens->value);
		curr->args = add_string_to_2d_array(curr->args, tokens->value);
	}
	return (0);
}

int	isempty(const char *str)
{
	return (str == NULL || str[0] == '\0');
}

char	*expand_variable(const char *input, t_env env)
{
	char	*dollar_sign;
	size_t	prefix_len;
	char	*prefix;
	char	*var_value;
	char	*result;

	dollar_sign = ft_strchr(input, '$');
	if (!dollar_sign)
		return (ft_strdup(input));
	prefix_len = dollar_sign - input;
	prefix = ft_strndup(input, prefix_len);
	dollar_sign++;
	var_value = ft_getenv(&env, dollar_sign);
	if (var_value)
		result = malloc(prefix_len + ft_strlen(var_value) + 1);
	else
		result = malloc(prefix_len + ft_strlen(dollar_sign) + 1);
	ft_strcpy(result, prefix);
	if (var_value)
		ft_strcat(result, var_value);
	else
		ft_strcat(result, dollar_sign);
	free(prefix);
	return (result);
}

int	parse_tokens_helper(t_input **tokens, t_parser *curr, t_env *env)
{
	if ((*tokens)->type == T_IDENTIFIER)
	{
		if (handle_parsing_identifier(*tokens, curr, env) < 0)
			return (-1);
	}
	else if ((*tokens)->type == T_PATH)
		return (handle_parsing_path(*tokens, curr, env));
	else if ((*tokens)->type == T_ARGUMENT)
		return (handle_parsing_argument(*tokens, curr));
	else if ((*tokens)->type == T_TILDE)
		curr->input = add_string_to_2d_array(curr->input, (*tokens)->value);
	else if ((*tokens)->type == T_ENV || (*tokens)->type == T_NUMBER
		|| (*tokens)->type == T_PERIODS || (*tokens)->type == T_QUOTE)
	{
		if (handle_parsing_quotes(*tokens, curr, env) < 0)
			return (-1);
	}
	else if ((*tokens)->type == T_APPEND || (*tokens)->type == T_HEREDOC
		|| (*tokens)->type == T_OUTPUT || (*tokens)->type == T_INPUT)
	{
		if (handle_parsing_redirection(*tokens, curr, env) < 0)
			return (-1);
		*tokens = (*tokens)->next;
	}
	return (0);
}

int	parse_tokens(t_parser **parser, t_tokenlist *list, t_env *env)
{
	t_input		*tokens;
	t_parser	*curr;

	if (parser == NULL || *parser == NULL || list == NULL || list->head == NULL)
		return (printf("Error: Null pointer passed to parse_tokens.\n"), -1);
	curr = *parser;
	tokens = list->head;
	while (tokens != NULL)
	{
		if (tokens->type == T_PIPE)
		{
			if (curr->command == NULL || tokens->next == NULL)
			{
				env->exit_code = 2;
				printf("bash: syntax error near unexpected token `|' \n");
				return (g_v = 0, -1);
			}
			add_parser_node(parser, create_parser());
			curr = curr->next;
		}
		else if (parse_tokens_helper(&tokens, curr, env) < 0)
			return (-1);
		tokens = tokens->next;
	}
	return (0);
}
