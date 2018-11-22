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

void bridge_in(tcar *dcar) {
/*
 * check if the car can access to the bridge
 *   if not car must wait 
 *   if yes cars_on_bridge++, set cur_direction, call stat_car_in()
 */
	if(cars_on_bridge < 3 && (cur_direction == dcar->my_direction || cur_direction == EMPTY)){
		cars_on_bridge[dcar->my_direction]++;
		cur_direction = dcar->my_direction;
		stat_car_in(dcar);
	}

	else {
	//A dormir (0w(0w(0w0)w0)w0)
		//     greeetings~!
	dbrige.cars_waiting[dcar->my_direction]++;
	dbridge.VCs.add([dcar->my_direction]); //Add?
	}

}

void bridge_out(tcar *dcar) {
/*
 * cars_on_bridge--
 * call stat_car_out()
 * if there is car waiting in same direction: allow to pass one car more
 * if not, check if there are cars waiting in the oposite direction
 * else cur_direction=EMPTY
 */
	cars_on_bridge--;
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


