#include "barrier.h"
#include <errno.h>

#ifdef POSIX_BARRIER

/* Wrapper functions to use pthread barriers */

int sys_barrier_init(sys_barrier_t* barrier, unsigned int nthreads)
{
	return pthread_barrier_init(barrier,NULL,nthreads);
}

int sys_barrier_destroy(sys_barrier_t* barrier)
{
	return pthread_barrier_destroy(barrier);
}

int sys_barrier_wait(sys_barrier_t *barrier)
{
	return pthread_barrier_wait(barrier);
}

#else


/* Barrier initialization function */
int sys_barrier_init(sys_barrier_t *barrier, unsigned int nr_threads)
{
	//Init de Mutex y cond_var
	pthread_mutex_init(&(barrier->mutex), NULL);
  	pthread_cond_init(&(barrier->cond), NULL);
	
	//Init variables
      	barrier->nr_threads_arrived = 0;
      	barrier->max_threads = nr_threads;
  
	return 0;
}

/* Destroy barrier resources */
//Destruimos primero el mutex y luego la condvar 
int sys_barrier_destroy(sys_barrier_t *barrier)
{
 	 pthread_mutex_destroy(&(barrier->mutex));
  	 pthread_cond_destroy(&(barrier->cond));

	return 0;
}

/* Main synchronization operation */
int sys_barrier_wait(sys_barrier_t *barrier)
{


	pthread_mutex_lock(&(barrier->mutex));

	//Region critica
  	barrier->nr_threads_arrived++;

  	while(barrier->nr_threads_arrived < barrier->max_threads){
    		pthread_cond_wait(&(barrier->cond), &(barrier->mutex));
  	}

  	//---------A partir de aqui ya ha llegado el último------
	//---------     y se despiertan TODOS              -----
        //Ponemos los threads a 0 para el siguiente wait que se quiera hacer.
    	barrier->nr_threads_arrived = 0;  
    	pthread_cond_broadcast(&(barrier->cond));
  	}
	//Fin region critica
    //Unlock antes de salir de la función
  	pthread_mutex_unlock(&(barrier->mutex));
    
	return 0;
}

#endif /* POSIX_BARRIER */
