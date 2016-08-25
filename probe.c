#include <stdio.h>
#include <unistd.h>

enum{
STATE=16,
DATA=20,
KNOB=2,
TOG=1
};

int main() 
{
    FILE *fp = fopen(
        "/dev/input/by-id/"
        "usb-Griffin_Technology__Inc._Griffin_PowerMate-event-if00", 
        "rb");
    unsigned char msg[24];
    while(1) {
        fread(msg, sizeof(char), 24, fp);
        if(msg[STATE] != 0) {
            switch(msg[STATE]) {
                case KNOB:
                    if(msg[DATA] > 128) {
                        printf("DOWN!\n");   
                    } else {
                        printf("UP!\n");   
                    }
                    break;
                case TOG: 
                    if(msg[DATA] == 1) {
                        printf("ON!\n");
                    } else {
                        printf("OFF!\n");
                    }
                default:
                    break;
            }
            usleep(10);
        }
    }
}
