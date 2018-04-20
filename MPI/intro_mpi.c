#include <stdio.h>

void main (){
	int rank, init, final, i, N, np, chunk, rest;
	N = 10;
	np = 4;
	chunk = N / np;
	rest = N % np;
	

	for(rank = 0; rank < np ; rank++){
		printf("rank %d\n", rank);

		if(rank < rest){
			init = (chunk + 1) * rank;
			final = init + chunk;
		}
		else {
			init = rest * (chunk + 1) + (rank - rest) * chunk; 
			final = init + chunk - 1;
		}
		
		printf("i ");
		for(i = init;i<=final;i++){
			printf("%d", i);
		}
		printf("\n");
	}
}
