#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
#include <openssl/sha.h>

/*
compile: mpicc double_sha256_colision_mpi.c -lssl -lcrypto
run: mpirun -np <PROCESS_NUMBER> a.out
   ex: mpirun -np 4 a.out
result will be in result.txt
*/

char * sha256 ( char * s ) {

	unsigned char *d = SHA256( s, strlen(s), 0 );
	int i, j;
   char r [32][2], d_s256[64] = "";

	for (i = 0; i < SHA256_DIGEST_LENGTH; i++){
      sprintf(&r[i][0], "%02x", d[i]);
   }
   for(i = 0; i < SHA256_DIGEST_LENGTH; i++){
      for (j = 0; j < 2; j++){
         d_s256[ i*2 + j ]=r[ i ][ j ] ;
      }
   }

   char *ret = malloc(65);
   strncpy(ret, d_s256, 64);
   ret[64] = '\0';
	return ret;
}

char * double_sha256( char * s ) {
   return sha256( sha256( s ) );
}

char * get_string_per_index( int index, char* outstr ){
   char* p = outstr;
   int len;
   char *baseset = "0123456789abcdefghijklmnopqrstuvwxyABCDEFGHIJKLMNOPQRSTUVWXY";
   len = strlen( baseset );
   while( index ){
      *p++ = baseset[0 + ( ( index % len == 0 )? len : index % len ) - 1];
      index = ( index - 1 ) / len;
   }
   *p = '\0';
   return outstr;
}

int main( int argc , char ** argv ){

	MPI_Init( &argc,&argv );

   FILE *f = fopen( "result.txt", "w" );
   if ( f == NULL) {
       printf( "Error opening file\n" );
       exit( 1 );
   }

	int N = 64, rank, np, i, flag = 0, interation = 1;
   const int max_len = 10; //max len of 10, edit if you want
   char * open_word, * s, * s_word, * o_word;

	MPI_Comm_rank( MPI_COMM_WORLD , &rank );
	MPI_Comm_size( MPI_COMM_WORLD , &np );

	int chunk = N/np, init = chunk * rank, final = init + chunk;

	if ( rank == 0 ){

      printf( "\nwelcome to Double-SHA256 Parallel Brute Force with MPI\n" );
      printf( "Insert the open_word to double_sha256 - max_len: %d\n", max_len );
      scanf( "%s", open_word );
      s = double_sha256( open_word );//sha256_word

      printf( "Double-SHA256 of :%s\n%s\n", open_word, s );
      for( i = 1 ; i < np ; i++ )
			MPI_Send( s, strlen( s ), MPI_CHAR, i, 0, MPI_COMM_WORLD );

      int in;
      MPI_Recv( &in, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, NULL );

      fprintf( f, "answer: %s\n", get_string_per_index( in, s ) );
      fprintf( f, "hash: %s\n", double_sha256( get_string_per_index( in, s ) ) );
      fclose( f );

      MPI_Abort( MPI_COMM_WORLD, 0 );
      MPI_Finalize();

	}
   else {

      MPI_Recv( s , 64, MPI_CHAR, 0, 0, MPI_COMM_WORLD, NULL );
      s[64] = '\0';
      interation = rank;

      while( 1 ){
         o_word = get_string_per_index( interation, o_word );
         printf( "word: %s\n", o_word );
         o_word = double_sha256( o_word );
         o_word[64] = '\0';
         if( strcmp( o_word, s ) == 0 ){
            printf( "HASH FOUND: %s\n", o_word );
            MPI_Send( &interation, 1, MPI_INT, 0, 0, MPI_COMM_WORLD );
            break;
         }
         interation += ( np-1 );
      }

   }
   MPI_Finalize();
   return 0;
}
