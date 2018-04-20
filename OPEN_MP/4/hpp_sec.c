#include <stdlib.h>
#include <stdio.h>
//#include <omp.h>
#include <sys/time.h>

void print_mat( int ** m, int nlines, int ncols ) {
    int i, j;
    for(i=0; i < nlines; i++ ) {
        for(j=0; j < ncols; j++ ) {
            printf("%d ", m[i][j] );
        }
        printf("\n");
    }
    printf("\n");
}

int ** alloc_init_mat( int n ) {
    int i, j;
    double ** mat = (int **) malloc( sizeof( int * ) * n );
    for( i=0; i < n; i++ ) {
        mat[i] = (int *)malloc( sizeof( int ) * n );
        for( j=0; j < n; j++ ) {
            mat[i][j] = 0;
        }
    }
    return mat;
}

int particles_number(int n, int div){
	return n > 0 ? ((n*n)/div): 0;
}

long time_usec( struct timeval t0, struct timeval t1 ){
   // return ( t1.tv_sec - t0.tv_sec ) * 1000 * 1000 + ( t1.tv_usec - t0.tv_usec );
}

int ** init_particles( int ** mat, int n, int n_particles ){
	int i, j, particles_added = 0, p_value;
	for( i=0; i < n; i++ ) {
        for( j=0; j < n; j++ ) {
        	p_value = 0;
        	if(particles_added <= n_particles){
            	p_value = rand_number(4);
            	particles_added++;
        	}
        	if(mat[i][j] == 0 )
        		mat[i][j] = p_value;
        	else
        		i == 0 ? i = -1 : i--;
        }
    }
    return mat;
}

void mat_copy ( int * dest_mat, int * src_mat, int n ) 
{
  memcpy( dest_mat, src_mat, n * n * sizeof(int) );
}

int rand_number( int mod ){
	return ( rand() % mod ) + 1;
}

void hpp_seq( int ** curr, int ** next, int n, int div, int niters ){
	int n_particles = particles_number( n, div );

	int vertice, vertice_r, vertice_d, vertice_l, vertice_u;

	curr = alloc_init_mat( n );
	curr = init_particles( curr, n, n_particles );
	mat_copy( next, curr, n );

	int i, j, k;

	for( k=0; k < niters; k++ ) {
        for( i=0; i < n; i++ ) {
        	for( j=0; j < n; j++ ) {
        		//testa bordas 0 
        		if( (i == 0 && j == 0) ){
        			vertice = curr[i][j];
        			vertice_r = curr[i][j+1];
        			vertice_d = curr[i+1][j];
        			if( vertice == 0 ){
        				if( vertice_r == 1 && vertice_d == 4){
        					next[i][j+1] = 2;
        					next[i+1][j] = 3;
        				}
        				if( vertice_r == 1 && vertice_d == 3){
        					next[i][j] = 1;
        					next[i][j+1] = 0;
        				}
        				if( vertice_r == 2 && vertice_d == 4){
        					next[i][j] = 4;
        					next[i+1][j] = 0;
        				}

        			}
        			break;
        		}

           		//if( i > 0 )

           		//if ( j > 0 )
        	}
    	}
    }
    return ;
}

int main() {

    int n, niters, **curr, **next, div;
    double t0, t1, tt;

    niters = 100; //fixed

    srand( (unsigned)time(0) );

    n = 5;
    div = 1;

    printf("Version             |  | #thre4ads | #Time (s) | Speedup \n");
    printf("------------------------------------------------------------- \n");
    
    //t0 = omp_get_wtime();
    hpp_seq( curr, next, n, div, niters );
    //t1 = omp_get_wtime();
    tt = t1 - t0;
    printf("Sequential          | %6d | %8d | %9.2f | %7.2f \n", niters, 1, tt, 1.0);

	return 0;
}







