#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <soundpipe.h>
#include <sporth.h>

#include "griffin.h"

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
