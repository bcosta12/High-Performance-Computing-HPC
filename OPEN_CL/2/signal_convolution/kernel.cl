kernel void signal_convolution( global float *x, global const float *s, global const float *c ){

   size_t k = get_global_id(0);
   int i = 0;

   for( i=0; i<100; i++ ){
      if ( k - i >= 0 ){
			x[i] += s[k - i] * c[i];
    	}
    	else {
    		return 0;
    	}
   }

}
