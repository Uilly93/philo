#include <stdio.h>
#include "stdbool.h"
#include "limits.h"

bool only_digit_string(char *s)
{
    int i;

    i = 0;
    while(s[i])
    {
        if(s[i] >= '0' && s[i] <= '9')
            i++;
        else
            return(true);
    }
    return(false);
}

int	ft_atoi(char *s)
{
	int		i;
	int		sign;
	long	nbr;
	
	i = 0;
	sign = 1;
	nbr = 0;
	while ((s[i] >= 9 && s[i] <= 13) || s[i] == ' ')
		i++;
	if (s[i] == '+' || s[i] == '-')
	{
		if(s[i] == '-')
			sign *= -1;
		i++;
	}
	while(s[i] >= '0' && s[i] <= '9')
	{
		nbr = (nbr * 10 + s[i] - '0');
		i++;
	}
	return (nbr * sign);
}

bool overflow(char *s)
{
	int		i;
	int		sign;
	long	nbr;
	
	i = 0;
	sign = 1;
	nbr = 0;
	while ((s[i] >= 9 && s[i] <= 13) || s[i] == ' ' || s[i] == '+')
		i++;
	if (s[i] == '-')
	{
		sign *= -1;
		i++;
	}
    if(only_digit_string(s + i))
        return(true);
	while(s[i] >= '0' && s[i] <= '9')
	{
		nbr = (nbr * 10 + s[i++] - '0');
		if(nbr * sign > INT_MAX || nbr * sign < INT_MIN)
			return (true);
	}
	return (false);
}

int main(void)
{
    printf("%d\n", overflow("-42"));
    printf("%d\n", overflow("42"));
    printf("%d\n", overflow("0"));
    printf("%d\n", overflow("+214748364 q"));
    printf("%d\n", overflow("2147483647"));
    printf("%d\n", overflow("+++   -42"));
    return(0);
}