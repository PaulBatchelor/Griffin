#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <soundpipe.h>
#include <sporth.h>

enum{
STATE=16,
DATA=20,
KNOB=2,
BUTTON=1
};

typedef struct {
    void (*func)(int type, int ctl, int val, void *);
    int run;
    pthread_t thread;
    SPFLOAT *trig, *gate;
    int trigme, okaydone;
} griffin_d;



static void *listen(void *ud) 
{
    griffin_d *gd = ud;

    FILE *fp = fopen(
        "/dev/input/by-id/"
        "usb-Griffin_Technology__Inc._Griffin_PowerMate-event-if00", 
        "rb");
    unsigned char msg[24];
    if(fp == NULL) {
        printf("There was a problem reading the Griffin Knob. Exiting gracefully...\n");
        return NULL;
    }
    while(1) {
        if(gd->run != 1) break;
        fread(msg, sizeof(char), 24, fp);
        if(msg[STATE] != 0) { switch(msg[STATE]) {
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
            usleep(10);
        }
    }

    fprintf(stderr, "Stopping Griffin Knob...\n");
    fclose(fp);
    gd->run = 0;
    pthread_exit(NULL);
}

int griffin_start(griffin_d *gd)
{
    gd->run = 1;
    fprintf(stderr, "Starting Griffin Knob...\n");
    pthread_create(&gd->thread, NULL, listen, gd);
    return 0;
}
int griffin_stop(griffin_d *gd)
{
    gd->run = -1;
    //usleep(5000);
    //sleep(1);
    while(gd->run != 0) {};
    return 0;
}

static int sporth_griffin(plumber_data *pd, sporth_stack *stack, void **ud)
{
    griffin_d *gd;
    char *strig, *sgate;
    switch(pd->mode) {
        case PLUMBER_CREATE:
            if(sporth_check_args(stack, "ss") != SPORTH_OK) {
                fprintf(stderr,"Incorrect arguments for griffin\n");
                stack->error++;
                return PLUMBER_NOTOK;
            }
            /* malloc and assign address to user data */
            sgate = sporth_stack_pop_string(stack);
            strig = sporth_stack_pop_string(stack);
            gd = malloc(sizeof(griffin_d));
            *ud = gd;
            plumber_create_var(pd, sgate, &gd->gate);
            plumber_create_var(pd, strig, &gd->trig);
            *gd->gate = 0;
            *gd->trig = 0;
            gd->trigme = 0;
            gd->okaydone = 0;
            griffin_start(gd);
            break;
        case PLUMBER_INIT:
            sporth_stack_pop_string(stack);
            sporth_stack_pop_string(stack);
            break;

        case PLUMBER_COMPUTE:
            gd = *ud;
            if(gd->trigme == 1) {
                gd->trigme = 0;
                gd->okaydone = 1;
                *gd->trig = 1;
            } else if(gd->trigme == -1) {
                gd->trigme = 0;
                gd->okaydone = 1;
                *gd->trig = -1;
        
            } else if(gd->okaydone) {
                *gd->trig = 0;
            }
            break;

        case PLUMBER_DESTROY:
            gd = *ud;
            griffin_stop(gd);
            free(gd);
            break;
        default:
            fprintf(stderr, "Huion: unknown mode %d\n", pd->mode);
            break;
    }
    return PLUMBER_OK;
}

plumber_dyn_func sporth_return_ugen()
{
    return sporth_griffin;
}
