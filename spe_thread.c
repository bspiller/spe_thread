#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>
#include<libspe2.h>
#include"libcell.h"

// Structure to hold arguments passed to the holding thread
struct spe_thread_params {
    spe_context_ptr_t ctx;
    unsigned int *entry;
    unsigned int runflags;
    void *argp;
    void *envp;
    spe_stop_info_t *stopinfo;
};


static void *spe_holding_thread_function( void *p )
{
    struct spe_thread_params *params= (struct spe_thread_params *)p;

    // Run the program. This blocks until the program has finished running
    spe_context_run( params->ctx, params->entry, params->runflags, params->argp, params->envp, NULL );

    free( params );

    return NULL;
}


int spe_thread_run( spe_thread_t *thread, spe_context_ptr_t ctx, unsigned int *entry, unsigned int runflags, void *argp, void *envp )
{
    struct spe_thread_params *params = NULL;
    int err = 0;

    if( thread == NULL || ctx == NULL ) {
        return EINVAL;
    }
   
    *thread = malloc( sizeof(struct _spe_thread_t) );

    if( *thread == NULL ) {
        err = ENOMEM;
        goto end;
    }
    memset( *thread, 0, sizeof(struct _spe_thread_t) );

    (*thread)->ctx = ctx;

    params = malloc( sizeof(struct spe_thread_params) );

    if( params == NULL ) {
        err = ENOMEM;
        goto end;
    }

    params->ctx = ctx;
    params->entry = entry;
    params->runflags = runflags;
    params->argp = argp;
    params->envp = envp;
    params->stopinfo = &((*thread)->stopinfo);

    // Start the thread
    (*thread)->ctx = ctx;
    err = pthread_create( &((*thread)->id), NULL, spe_holding_thread_function, params );

end:
    if( err != 0 ) {
        free( params );
        free( *thread );
    }

    return err;
}


int spe_thread_destroy( spe_thread_t *thread )
{
    free( *thread );
}
