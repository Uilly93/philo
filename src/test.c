/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnocchi <wnocchi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 14:50:19 by wnocchi           #+#    #+#             */
/*   Updated: 2024/02/29 09:50:36 by wnocchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

struct timeval start, end;
// typedef struct s_values
// {
// 	long seconds;
//     long micros;
//     long millis;
// }	t_val;

void* start_routine(void *arg) {
	// t_val *val;
    // // Simuler un travail en dormant pendant 2 secondes
	// val->seconds = (end.tv_sec - start.tv_sec);
    // val->micros = ((val->seconds * 1000000) + end.tv_usec) - (start.tv_usec);
    // val->millis = val->micros / 1000;
    gettimeofday(&start, NULL); // Temps de début
    usleep(300000);
    gettimeofday(&end, NULL); // Temps de fin
    return NULL;
}

int main() {
    pthread_t thread;
	// t_val val;

    // gettimeofday(&start, NULL); // Temps de début
    pthread_create(&thread, NULL, start_routine, NULL);
    pthread_join(thread, NULL); // Attendre la fin du thread

    long seconds = (end.tv_sec - start.tv_sec);
    long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
    long millis = micros / 1000; // Convertir en millisecondes

    printf("Temps écoulé : %ld millisecondes\n", millis);

    return 0;
}