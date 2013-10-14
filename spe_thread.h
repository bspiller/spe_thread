#ifndef _SPE_THREAD_H
#define _SPE_THREAD_H

#include<stdint.h>
#include<libspe2.h>
#include<pthread.h>

struct _spe_thread_t {
    spe_context_ptr_t ctx;
    spe_stop_info_t stopinfo;
    pthread_t id;
};

typedef struct _spe_thread_t* spe_thread_t;

// Creates a holding thread an executes an SPE context
//
// thread:      Pointer to a spe_thread_t object
// ctx:         A libspe2 SPE context
// entry:       entry parameter for the libspe2 spe_context_run function
// runflags:    runflags parameter of the libspe2 spe_context_run function
// argp:        (optional) argp parameter of the spe_context_run function
// envp:        (optional) Third parameter of the SPE program
//
// RETURN VALUE
//      0 on success, error number on failure
int spe_thread_run( spe_thread_t *thread, spe_context_ptr_t ctx, unsigned int *entry, unsigned int runflags, void *argp, void *envp );


// Destroys resources associated with an SPE thread. The SPE thread must finish
// running before calling this function
//
// thread:      Pointer to a spe_thread_t object
void spe_thread_destroy( spe_thread_t *thread );

#endif
