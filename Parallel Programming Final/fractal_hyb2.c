#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <sys/time.h>
#include <omp.h>
#include <mpi.h>

#define xMin 0.74395
#define xMax 0.74973
#define yMin 0.11321
#define yMax 0.11899

unsigned char *GPU_Init(int size);
void GPU_Exec(int width, int from, int to, int maxdepth, double dx, double dy, unsigned char *cnt_d);
void GPU_Fini(unsigned char *cnt, unsigned char *cnt_d, int size);

static void WriteBMP(int x, int y, unsigned char *bmp, char * name)
{
  const unsigned char bmphdr[54] = {66, 77, 255, 255, 255, 255, 0, 0, 0, 0, 54, 4, 0, 0, 40, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 1, 0, 8, 0, 0, 0, 0, 0, 255, 255, 255, 255, 196, 14, 0, 0, 196, 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  unsigned char hdr[1078];
  int i, j, c, xcorr, diff;
  FILE *f;

  xcorr = (x+3) >> 2 << 2;  // BMPs have to be a multiple of 4 pixels wide.
  diff = xcorr - x;

  for (i = 0; i < 54; i++) hdr[i] = bmphdr[i];
  *((int*)(&hdr[18])) = xcorr;
  *((int*)(&hdr[22])) = y;
  *((int*)(&hdr[34])) = xcorr*y;
  *((int*)(&hdr[2])) = xcorr*y + 1078;
  for (i = 0; i < 256; i++) {
    j = i*4 + 54;
    hdr[j+0] = i;  // blue
    hdr[j+1] = i;  // green
    hdr[j+2] = i;  // red
    hdr[j+3] = 0;  // dummy
  }

  f = fopen(name, "wb");
  assert(f != NULL);
  c = fwrite(hdr, 1, 1078, f);
  assert(c == 1078);
  if (diff == 0) {
    c = fwrite(bmp, 1, x*y, f);
    assert(c == x*y);
  } else {
    *((int*)(&hdr[0])) = 0;  // need up to three zero bytes
    for (j = 0; j < y; j++) {
      c = fwrite(&bmp[j * x], 1, x, f);
      assert(c == x);
      c = fwrite(hdr, 1, diff, f);
      assert(c == diff);
    }
  }
  fclose(f);
}

int main(int argc, char *argv[])
{
	MPI_Init(NULL, NULL);
	int comm_sz;
	int my_rank;
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	int const COMM_ARGS_SIZE = 4;
	int comm_args[COMM_ARGS_SIZE];
	unsigned char *global_cnt;
	int error_flag;

  int width, maxdepth, percent;
  double dx, dy;
  unsigned char *cnt, *cnt_d;
  struct timeval start, end;
  int idx, row, col, depth;
  double cx, cy, x, y, x2, y2;

  if(my_rank == 0){
	  printf("Fractal v1.2 [Hybrid]\n");
  }

  /* check command line */
  if (argc != 4) {fprintf(stderr, "usage: %s edge_length max_depth GPU_percentage\n", argv[0]); exit(-1);}
  width = atoi(argv[1]);
 /* printf("width = %d \n", width);
  printf("atoi = %d \n", atoi(argv[1]));
  printf("I am process %d and I exist\n", my_rank);*/

  if (width < 10) {fprintf(stderr, "edge_length must be at least 10\n"); exit(-1);}
  maxdepth = atoi(argv[2]);
  if (maxdepth < 10) {fprintf(stderr, "max_depth must be at least 10\n"); exit(-1);}
  percent = atoi(argv[3]);
  if ((percent < 0) || (100 < percent)) {fprintf(stderr, "GPU_percentage out of range\n"); exit(-1);}

  if(my_rank == 0){
	  printf("computing %d by %d fractal with a maximum depth of %d and %d%% on the GPU\n", width, width, maxdepth, percent);
  }

 // printf("I am process %d and I am beginning workload allocation\n", my_rank);

  /* allocate workload */
  assert((width % comm_sz) == 0); // check for integer multiple
  int from = my_rank * width / comm_sz;
  int to = (my_rank + 1) * width / comm_sz;
  int cut = from + percent * (to - from) / 100;


  /* allocate arrays */
  cnt = (unsigned char *)malloc(((width * width ) / comm_sz) * sizeof(unsigned char));
  if (cnt == NULL) {fprintf(stderr, "could not allocate memory\n"); exit(-1);}

 // printf("I am process %d and I am beginning gpu allocating\n", my_rank);

  if(cut - from > 0){
  cnt_d = GPU_Init(width * (cut - from) * sizeof(unsigned char));}

 // printf("I am process %d and I am done with gpu allocating\n", my_rank);

  if(my_rank == 0){
	  global_cnt = (unsigned char *)malloc(width * width * sizeof(unsigned char));
	  if (global_cnt == NULL)
	  	  {
	  		  error_flag = 1;
	  		  MPI_Bcast(&error_flag, 1, MPI_INT, 0, MPI_COMM_WORLD);
	  		  fprintf(stderr, "could not allocate memory\n");
	  		  exit(-1);
	  	  }
	  	  else
	  	  {
	  		  error_flag = 0;
	  		  MPI_Bcast(&error_flag, 1, MPI_INT, 0, MPI_COMM_WORLD); //implies barrier

	  		  /* start time */
	  		  gettimeofday(&start, NULL);
	  	  }
  }
  else
  {
	  global_cnt = 0;
	  MPI_Bcast(&error_flag, 1, MPI_INT, 0, MPI_COMM_WORLD); //implies barrier
	  if(error_flag == 1){printf("recieved errorflag\n"); exit(-1);}
  }

  int size = (width * width ) / comm_sz;
  /*printf("I am process %d and I am beginning computation\n", my_rank);
  printf("I am process %d and my max size is: %d\n", my_rank, size);
  printf("I am process %d and my from, to, and cut are: %d %d %d\n", my_rank, from, to, cut);*/


  /* compute fractal */
  dx = (xMax - xMin) / width;
  dy = (yMax - yMin) / width;

  // the following call should compute rows from through cut - 1
  GPU_Exec(width, from, cut, maxdepth, dx, dy, cnt_d);

 // int counter; //remember to put in private, along with my_rank

  // the following code should compute rows cut through to - 1
  // insert OpenMP parallelized for loop with 16 threads, default(none), and a dynamic schedule with a chunk size of 1
#pragma omp parallel for num_threads(16) default(none) private(row, col, cy, cx, x, y, depth, x2, y2) \
	shared(width, to, cut, dx, dy, maxdepth, cnt, from) schedule(dynamic, 1)
  for (row = cut; row < to; row++) {
    cy = yMin + row * dy;
    for (col = 0; col < width; col++) {
      cx = xMin + col * dx;
      x = -cx;
      y = -cy;
      depth = maxdepth;
      do {
        x2 = x * x;
        y2 = y * y;
        y = 2 * x * y - cy;
        x = x2 - y2 - cx;
        depth--;
      } while ((depth > 0) && ((x2 + y2) <= 5.0));
//counter = row * width + col;
//if(my_rank == 1) {printf("%d, ", counter);}
      cnt[(row - from) * width + col] = depth & 255;
    }
  }

//  printf("I am process %d and I have finished computation\n", my_rank);


  // the following call should copy the GPU's result into the beginning of the CPU's cnt array
  if(cut - from > 0){
  GPU_Fini(cnt, cnt_d, width * (cut - from) * sizeof(unsigned char));
  }

//  printf("I am process %d and I have finished GPU copying\n", my_rank);

    MPI_Gather(cnt, (width * width) / comm_sz, MPI_UNSIGNED_CHAR, global_cnt, (width * width ) / comm_sz,
		  MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);

//  printf("I am process %d and I have finished gathering\n", my_rank);


  if(my_rank == 0){
	  /* end time */
	  gettimeofday(&end, NULL);
	  printf("compute time: %.4f s\n", end.tv_sec + end.tv_usec / 1000000.0 - start.tv_sec - start.tv_usec / 1000000.0);

	  /* verify result by writing it to a file */
	  if (width <= 1024) {
		  WriteBMP(width, width, global_cnt, "fractal.bmp");
	  }
  }

 // printf("I am process %d and I am freeing memory\n", my_rank);

  free(cnt);
  free(global_cnt);
 // printf("I am process %d and I am done freeing memory\n", my_rank);

  MPI_Finalize();



  return 0;
}
