#include <bits/pthreadtypes.h>
#include <bits/types/struct_itimerspec.h>
#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include <time.h>
#include <pthread.h>
#include <sys/socket.h>
#include <string.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include "cJSON.h"
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

int can_fds[4];
void init_interfaces(int *fds);
void send_message(union sigval s);

int main(int argc, char **argv) {
    // get the array of message descs and signal descs from the json file.
    // initialize all can interfaces
    // start a timer for each message descriptor
    // start a terminal for accepting commands to change signals
    
}

void send_message(union sigval s) {
    struct message_desc *dsc = s.sival_ptr;
    pthread_mutex_lock(dsc->lock);
    send(dsc->can_interface, dsc->data, dsc->length, 0);
    pthread_mutex_unlock(dsc->lock);
}

void init_interfaces(int *fds) {
    char *can_names[] = {"can0", "can1", "can2", "can3"};
    for (int i = 0; i < 4; i++) {
        fds[i] = socket(PF_CAN, SOCK_RAW, CAN_RAW);
        if (fds[i] < 0) {
            perror("failed to create socket");
            continue;
        }
        struct ifreq ifr;
        strcpy(ifr.ifr_name, can_names[i]);
        ioctl(fds[i], SIOCGIFINDEX, &ifr);
        struct sockaddr_can addr;
        memset(&addr, 0, sizeof(addr));
        addr.can_family = AF_CAN;
        addr.can_ifindex = ifr.ifr_ifindex;
        if (bind(fds[i], (struct sockaddr *)&addr, sizeof(addr)) < 0) {
            perror("failed finding socket");
            fds[i] = -1;
            continue;
        }
    }
}

int parse_can_json(char *filename) {
    
}
