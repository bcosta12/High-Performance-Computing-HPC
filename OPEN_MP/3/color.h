#include <stdio.h>

#define MAXT 100
void temp2color( double t, int rgb [] ) {
        
        t = (t > MAXT ? MAXT : t);
        rgb[0] = (t*255/MAXT);
        rgb[2] = 255 - (t*255/MAXT);
        // rgb[1] = -t*(t-MAXT)/20;
        rgb[1] = 20;
        

}


void print_colors( double * m, int size ) {
	int i,j, rgb[3];
	for(i=0; i < size; i++ ) {
		for(j=0; j < size; j++ ) {
			temp2color( m[i*size + j], rgb );
			printf("\x1b[48;2;%d;%d;%dm  \x1b[0m", rgb[0], rgb[1], rgb[2] );
	
		}		
		printf("\n");
	
	}
	printf("\n\n");
	int step = MAXT / size >= 1 ? MAXT / size : 1;
	double t = MAXT;
	for( i = 0; i < size; i++ ) {
		temp2color(t, rgb );
		printf("\x1b[48;2;%d;%d;%dm  \x1b[0m", rgb[0], rgb[1], rgb[2] );
		t -= step;
	}
	printf("\n\n");
} 
