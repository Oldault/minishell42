/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svolodin <svolodin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 11:42:21 by svolodin          #+#    #+#             */
/*   Updated: 2024/02/14 11:47:37 by svolodin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_single_quote(t_parse_seg *pdata)
{
	if (pdata->c == '\'' && !pdata->in_double_quote)
	{
		pdata->in_single_quote = !pdata->in_single_quote;
		pdata->should_expand = !pdata->in_single_quote;
		return (1);
	}
	return (0);
}

int	handle_double_quote(t_parse_seg *pdata)
{
	if (pdata->c == '\"' && !pdata->in_single_quote)
	{
		pdata->in_double_quote = !pdata->in_double_quote;
		return (1);
	}
	return (0);
}

int	handle_space(t_parse_seg *pdata)
{
	if (pdata->c == ' ' && !pdata->in_single_quote && !pdata->in_double_quote)
	{
		if (pdata->current_length > 0)
		{
			pdata->current_arg[pdata->current_length] = '\0';
			pdata->args[pdata->arg_count++] = ft_strdup(pdata->current_arg);
			pdata->current_length = 0;
		}
		return (1);
	}
	return (0);
}

void	handle_nonexistent_variable(t_parse_seg *pdata, char *var_name,
		size_t var_len)
{
	pdata->current_arg[pdata->current_length++] = '$';
	strncpy(pdata->current_arg + pdata->current_length, var_name, var_len);
	pdata->current_length += var_len;
	pdata->current_arg[pdata->current_length] = '\0';
}
