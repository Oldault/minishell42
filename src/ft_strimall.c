/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strimall.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albeninc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 16:10:58 by albeninc          #+#    #+#             */
/*   Updated: 2024/01/20 16:25:28 by albeninc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int count_quotes(char const *s, char quote_char)
{
    int count;

    count = 0;
    while (*s)
    {
        if (*s == quote_char)
		count++;
        s++;
    }
    return (count);
}

char *ft_strtrim_all(char const *s) {
    if (!s)
	    return (NULL);

    int squote_count = count_quotes(s, '\'');
    int dquote_count = count_quotes(s, '\"');
    int keep_squote = squote_count % 2;
    int keep_dquote = dquote_count % 2;

    char *trimmed = malloc(strlen(s) - squote_count + keep_squote - dquote_count + keep_dquote + 1);
    if (!trimmed) return NULL;

    int i = 0, j = 0;
    while (s[i]) {
        if (s[i] == '\'' && (squote_count > keep_squote)) {
            squote_count--;
        } else if (s[i] == '\"' && (dquote_count > keep_dquote)) {
            dquote_count--;
        } else {
            trimmed[j++] = s[i];
        }
        i++;
    }
    trimmed[j] = '\0';
    return trimmed;
}

