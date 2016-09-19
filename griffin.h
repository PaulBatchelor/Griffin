#ifndef SPFLOAT
#define SPFLOAT float
#endif

typedef struct {
    void (*func)(int type, int ctl, int val, void *);
    int run;
    pthread_t thread;
    SPFLOAT *trig, *gate;
    int trigme, okaydone;
    int fid;
} griffin_d;

enum{
STATE=16,
DATA=20,
KNOB=2,
BUTTON=1
};

int griffin_start(griffin_d *gd);
int griffin_stop(griffin_d *gd);
