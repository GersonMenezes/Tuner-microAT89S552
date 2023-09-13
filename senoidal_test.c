#include <stdio.h>
#include <math.h>

#define NUM_POINTS 1000
#define MAX_AMPLITUDE 255

int main() {
    double signal[NUM_POINTS];
    double frequency;

    // Generate the discrete sinusoidal signal
    for (int i = 0; i < NUM_POINTS; i++) {
        frequency = 2 * M_PI * (i / (double)NUM_POINTS) * 10.0; // Change the frequency as needed
        signal[i] = MAX_AMPLITUDE * sin(frequency);
    }

    // Output the signal (for demonstration purposes)
    for (int i = 0; i < NUM_POINTS; i++) {
        printf("%lf\n", signal[i]);
    }

    return 0;
}

void senoidal(){

    for(int i = 0; i < 256; ++i){
        printf("%d\n");
    }
}