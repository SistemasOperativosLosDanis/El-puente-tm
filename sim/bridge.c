#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

#include "barrier.h"
#include "bridge.h"
#include "car.h"

extern int local_time;
pthread_mutex_t mtx_time;

void init_bridge()
{
	pthread_mutex_init(&dbridge.mtx, NULL);
	pthread_cond_init(&dbridge.VCs[0], 0);
	pthread_cond_init(&dbridge.VCs[1], 0);

	dbridge.cars_on_bridge = 0;
	dbridge.cur_direction=EMPTY;
	dbridge.cars_waiting[0] = 0;
	dbridge.cars_waiting[1] = 0;

	pthread_mutex_init(&mtx_time, NULL);
}

/*
* Tomamos el mutex primero para acceder con seguridad a las variables
* 
*/
void bridge_in(tcar *dcar) {

	pthread_mutex_lock(&dbridge.mtx);

	
	//Si no puede cruzar, a la cama
	while(dbridge.cars_on_bridge >= 3 && (dbridge.cur_direction == dcar->my_direction || cur_direction == EMPTY)){
		//A dormir (0w(0w(0w0)w0)w0)
		dbridge.cars_waiting[dcar->my_direction]++;
		pthread_cond_wait(&dbridge.VCs[dcar->my_direction], &dbridge.mtx);
	}
	
	//Cuando se le despierte, cruza
	dbridge.cars_on_bridge[dcar->my_direction]--;
	dbridge.cur_direction = dcar->my_direction;
	stat_car_in(dcar);

	unlock(&dbridge.mtx);
}

void bridge_out(tcar *dcar) {
/*
 * cars_on_bridge--
 * call stat_car_out()
 * if there is car waiting in same direction: allow to pass one more* car
 * if not, check if there are cars waiting in the oposite direction
 * else cur_direction=EMPTY
 */

/*Tienes que coger el lock, porque ya sabes que has cruzado y entonces despiertas
a la gente de tu direccion de origen si hay alguien esperando o a la gente de la direccion a
la opuesta si hay más gente allí o lo que sea :3*/
	pthread_mutex_lock(&dbridge.mtx);
	dbridge.cars_on_bridge--;
	
	stat_car_out(dcar);

	//Si hay gente esperando en mi dir
	if(dbrige.cars_waiting[dcar->my_direction] != 0){
	   dbrige.bridge_in(dbridge.VCs[dcar->my_direction].pop);
	}
	else if (dbrige.cars_waiting[(dcar->my_direction+1)%2]){
	cur_direction = dcar->my_direction+1)%2;
	//pA DENTROO
	}
	else{
	cur_direction = EMPTY;
	}

	unlock(mtx)
}

void crossing_bridge(tcar *dcar) {
	sleep(LENGTH);
}


void *cross_bridge(void * arg){
	tcar *car = (tcar*)arg;

	sys_barrier_wait(&mybarrier);
	init_car(car);

	bridge_in(car);
	crossing_bridge(car); // Crossing bridge
	bridge_out(car);

	write_stats(car);
	return 0;
}


