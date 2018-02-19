#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wiringPi.h>

#define COOLER_PIN 11

int main(void){ 
    wiringPiSetup();
    pinMode(COOLER_PIN, OUTPUT);
    
    FILE *myfile;
    int temp = 0;
    char arr[3];
    while(1){
        myfile = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
        fgets (arr, 3, myfile);
        fclose(myfile);
        //printf("%s", arr);
        //printf("\n");
        temp = atoi(arr);
        //printf("%i\n", temp);
        if (temp > 45){
	    //printf("cooler on\n");
            digitalWrite(COOLER_PIN, 1);
	}
        else{
	    //printf("cooler off\n");
            digitalWrite(COOLER_PIN, 0);
        }
        sleep(60);
    }
}
