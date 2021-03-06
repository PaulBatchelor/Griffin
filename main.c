#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/socket.h>
#include "griffin.h"

static void *listener(void *ud) 
{
    griffin_d *gd = ud;

    gd->fid = open(
        "/dev/input/by-id/"
        "usb-Griffin_Technology__Inc._Griffin_PowerMate-event-if00", 
        O_RDONLY | O_NONBLOCK);
    unsigned char msg[24];
    if(gd->fid == -1) {
        printf("There was a problem reading the Griffin Knob. Exiting gracefully...\n");
        return NULL;
    }
    int rc = 0; 
    while(gd->run) {
        rc = read(gd->fid, msg, 24);
        if(msg[STATE] != 0 && rc != -1) { 
            switch(msg[STATE]) {
                case KNOB:
                    if(msg[DATA] > 128) {
                        gd->trigme = -1;
                    } else {
                        gd->trigme = 1;
                    }
                    break;
                case BUTTON: 
                    if(msg[DATA] == 1) {
                        *gd->gate = 1;
                    } else {
                        *gd->gate = 0;
                    }
                default:
                    break;
            }
        }
        usleep(10);
    }

    fprintf(stderr, "Stopping Griffin Knob...\n");
    close(gd->fid);
    pthread_exit(NULL);
}

int griffin_start(griffin_d *gd)
{
    gd->run = 1;
    fprintf(stderr, "Starting Griffin Knob...\n");
    pthread_create(&gd->thread, NULL, listener, gd);
    return 0;
}
int griffin_stop(griffin_d *gd)
{
    gd->run = 0;
    usleep(1000);
    return 0;
}

