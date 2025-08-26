#include <bits/pthreadtypes.h>
#include <bits/types/struct_itimerspec.h>
#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include <time.h>
#include <pthread.h>
#include <sys/socket.h>
/* general plan: 
 *  read message descriptors from a json file. Each message descriptor should have 
 *      a periodicity, or 0 if only on change
 *      a total length
 *      a CAN ID
 *      a can interface
 *      a symbolic name
 *      an array of signal descriptors
 *      each signal descriptor has
 *          a starting bit
 *          a length
 *          an offset
 *          a scalar
 *          a symbolic name
 *  then allocate every message descriptor with every signal descriptor
 *  for each message descriptor start a timer with the correct periodicity and pass the message descriptor to it
 *  as user data.
 *  
 *  
 *  */

struct signal_desc {
    uint16_t starting_bit;
    uint16_t length;
    uint16_t offset;
    float scalar;
    char *name;
    uint8_t data;
    uint8_t data_len;
};

struct message_desc {
    uint16_t periodicity;
    uint16_t length;
    uint16_t id;
    int can_interface;
    char *name;
    uint16_t num_signals;
    struct signal_desc *signals;
    uint8_t *data;
    pthread_mutex_t *lock;
};



int main(int argc, char **argv) {
    
}

void send_message(union sigval s) {
    struct message_desc *dsc = s.sival_ptr;
    pthread_mutex_lock(dsc->lock);
    send(dsc->can_interface, dsc->data, dsc->length, 0);
    pthread_mutex_unlock(dsc->lock);
}

