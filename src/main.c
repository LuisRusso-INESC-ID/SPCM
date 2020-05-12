/* BSD 2-Clause License */

/* Copyright (c) 2020, Luís M. S. Russo */
/* All rights reserved. */

/* Redistribution and use in source and binary forms, with or without */
/* modification, are permitted provided that the following conditions are met: */

/* 1. Redistributions of source code must retain the above copyright notice, this */
/*    list of conditions and the following disclaimer. */

/* 2. Redistributions in binary form must reproduce the above copyright notice, */
/*    this list of conditions and the following disclaimer in the documentation */
/*    and/or other materials provided with the distribution. */

/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" */
/* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE */
/* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE */
/* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE */
/* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL */
/* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR */
/* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER */
/* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE */
/* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */



#define _ISOC99_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <bsd/stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <time.h>

struct rowCount
{
  int dirtyBit; /* Dirty row, 0 for dirty and 1 for clean */
  int onesCount; /* Count the number of ones for delta computation */
  int i; /* Row number */
};

typedef struct rowCount* rowCount;

int
randomIndex(int M)
{
  int r;

  r = arc4random_uniform(M);

  return r;
}

int
countOnes(int N, /* Number of columns */
	  int M, /* Number of rows */
	  int **S){
  int r = 0;
  int i;
  int j;

  i = 0;
  while(i < M){
    j = 0;
    while(j < N){
      r += S[i][j];
      j++;
    }
    i++;
  }

  return r;
}

void
writeMatrix(char* fileOutName, /* Output filename */
	    int N, /* Number of columns */
	    int M, /* Number of rows */
	    int **S){
  FILE *f;
  int i;
  int j;

  f = fopen(fileOutName, "w");

  fprintf(f, "%d\n", countOnes(N, M, S));
  fprintf(f, "%d %d\n", M, N);

  i = 0;
  while(i < M){
    j = 0;
    while(j < N){
      fprintf(f, "%d ", S[i][j]);
      j++;
    }
    fprintf(f, "\n");
    i++;
  }
  fclose(f);
}


void
checkOrder(rowCount A, int M)
{
#ifndef NDEBUG
  int i;
  i = 1;
  while(i < M){
    assert(A[i-1].dirtyBit <= A[i].dirtyBit);
    i++;
  }
#endif	/* NDEBUG */
}

void
printA(rowCount A, int M)
{
  int i;
  i = 0;
  while(i < M){
    printf("[D: %2d     O: %2d    i: %2d]  ",
	   A[i].dirtyBit, A[i].onesCount, A[i].i);
    i++;
  }
  printf("\n");
}

void
printMatrix(int N, /* Number of columns */
	    int M, /* Number of rows */
	    int **S){
  int i;
  int j;

  i = 0;
  while(i < M){
    j = 0;
    while(j < N){
      printf("%1d ", S[i][j]);
      j++;
    }
    printf("\n");
    i++;
  }
}

void
swapStructs(rowCount a, rowCount b){
  struct rowCount temp;

  memcpy(&temp, a, sizeof(struct rowCount));
  memcpy(a, b, sizeof(struct rowCount));
  memcpy(b, &temp, sizeof(struct rowCount));
}

void
iSwap(int* a, int* b)
{
  int t;

  t = *a;
  *a = *b;
  *b = t;
}

int
main(int argc, char** argv)
{
/* SYNOPSIS */
/* ./project <input.alist> <output.txt> [<steps> [<iter> [<hotPer> [<hotPr> [<coldPer> [<coldPr>]]]]]] */

/* DESCRIPTION */

/*  The <input.alist> is the input parity check matrix in alist format. If */
/*  this string is exactly stdin then standard input is used instead. The */
/*  <output.txt> is the name of the file to store the output matrix. */

/*  The remaining arguments are numbers and are optional. The value of <steps> */
/*  is the number of temperatures to consider between the hot and the cold */
/*  temperatures, the default is 100. The value of <iter> indicates how many */
/*  iterations to execute at a fixed temperature, the default is 100. The */
/*  <hotPer> value indicates the hit percentage to consider for the hot */
/*  temperature, i.e., what is the percentage of the initial number of non */
/*  zero entries that we consider as an increase. The default is 0.05, which */
/*  corresponds to 5%. The value <hotPr> is the probability of accepting the */
/*  <hotPer> increase, the default is 0.05 which corresponds to 5%.  The */
/*  <coldPer> value indicates the hit percentage to consider for the cold */
/*  temperature. The default is 0.05. The value <coldPr> is the probability */
/*  of accepting the <coldPer> increase, the default is 0.01. */

  if(3 >  argc || 9 < argc ){ /* Print usage message */

    printf("SYNOPSIS\n");
    printf("./project <input.alist> <output.txt> [<steps> [<iter> [<hotPer> [<hotPr> [<coldPer> [<coldPr>]]]]]]\n");
    printf("\n");
    printf("DESCRIPTION\n");
    printf("\n");
    printf(" The <input.alist> is the input parity check matrix in alist format. If\n");
    printf(" this string is exactly stdin then standard input is used instead. The\n");
    printf(" <output.txt> is the name of the file to store the output matrix.\n");
    printf("\n");
    printf(" The remaining arguments are numbers and are optional. The value of <steps>\n");
    printf(" is the number of temperatures to consider between the hot and the cold\n");
    printf(" temperatures, the default is 100. The value of <iter> indicates how many\n");
    printf(" iterations to execute at a fixed temperature, the default is 100. The\n");
    printf(" <hotPer> value indicates the hit percentage to consider for the hot\n");
    printf(" temperature, i.e., what is the percentage of the initial number of non\n");
    printf(" zero entries that we consider as an increase. The default is 0.05, which\n");
    printf(" corresponds to 5%%. The value <hotPr> is the probability of accepting the\n");
    printf(" <hotPer> increase, the default is 0.05 which corresponds to 5%%.  The\n");
    printf(" <coldPer> value indicates the hit percentage to consider for the cold\n");
    printf(" temperature. The default is 0.05. The value <coldPr> is the probability\n");
    printf(" of accepting the <coldPer> increase, the default is 0.01.\n");
    printf("\n");
  } else {

#define __Fname argv[1]
#define __FnameNr 1

#define __OutFname argv[2]
#define __OutFnameNr 2

#define __Steps argv[3]
#define __StepsNr 3
#define __Iter argv[4]
#define __IterNr 4
#define __Hothitperc argv[5]
#define __HothitpercNr 5
#define __Hothitprob argv[6]
#define __HothitprobNr 6
#define __Coldhitperc argv[7]
#define __ColdhitpercNr 7
#define __Coldhitprob argv[8]
#define __ColdhitprobNr 8

    FILE *f;

    f = NULL;
    if(0 != strcmp("stdin", __Fname))
      f = fopen(__Fname, "r");
    if(NULL == f)
      f = stdin;

    int t; /* Trash variable */
    int N; /* Number of columns */
    int M; /* Number of rows */

    rowCount A; /* Array for row selection */
    int dirties; /* Number of dirty rows */
    int *P;       /* Permutation array for uniform ii selection. */

    fscanf(f, "%d", &N);
    fscanf(f, "%d", &M);

    A = (rowCount) malloc(M*sizeof(struct rowCount));
    P = (int*) malloc(M*sizeof(int));

    int maxRowSize;
    int maxColSize;

    fscanf(f, "%d", &maxColSize);
    fscanf(f, "%d", &maxRowSize);

    int i;
    int j;

    j = 0;
    while(j < N){
      fscanf(f, "%d", &t);
      j++;
    }

    i = 0;
    while(i < M){
      fscanf(f, "%d", &t);
      i++;
    }

    int** S; /* Current state matrix */
    int cost = 0;

    S = (int**)malloc(M*sizeof(int*));
    i = 0;
    while(i < M){
      S[i] = (int*)calloc(N, sizeof(int));
      i++;
    }

    j = 0;
    while(j < N){
      i = 0;
      while(i < maxColSize){
	fscanf(f, "%d", &t);
	if(t > 0){
	  cost++;
	  S[t-1][j] = 1;
	}
	i++;
      }
      j++;
    }

    i = 0;
    while(i < M){
      P[i] = i;	       /* Increasing permutation */
      A[i].dirtyBit = 0; /* Dirty by default */
      A[i].onesCount = 0;
      j = 0;
      while(j < N){
	A[i].onesCount += S[i][j];
	j++;
      }
      A[i].i = i;
      /* shiftLeft(A, i); */
      i++;
    }
    dirties = M;

    /* printf("First : %d\n", cost); */
    /* checkOrder(A, M); */

    /* printMatrix(N, M, S); */

    /* Simulated anneling */

    int* newRow = malloc(N*sizeof(int));

    /* Annealing parameters */
    long long int iter;    /* Number of iterations per temperature */
    iter = 100; /* A no min decrease over iter steps implies termination */
    if(argc > __IterNr){
      sscanf(__Iter, "%lld", &iter);
    }

    double sc; /* Variable for scanf */

    double temp; /* Initial and current temperature */
    temp = 0.05; /* Default value */
    if(argc > __HothitpercNr){
      sscanf(__Hothitperc, "%lf", &temp);
    }
    /* temp*=cost; */
    /* temp/=M; */
    temp*=N; /* Multiply by line size */
    sc = 0.05;
    if(argc > __HothitprobNr){
      sscanf(__Hothitprob, "%lf", &sc);
    }
    temp/=-log(sc);

    /* temp = (0.05*cost/M)/(-log(0.05)); */

    double cold; /* final temperature */
    cold = 0.05; /* Default value */
    if(argc > __ColdhitpercNr){
      sscanf(__Coldhitperc, "%lf", &cold);
    }
    /* cold*=cost; */
    /* cold/=M; */
    cold*=N; /* Multiply by line size */
    sc = 0.01;
    if(argc > __ColdhitprobNr){
      sscanf(__Coldhitprob, "%lf", &sc);
    }
    cold/=-log(sc);

    /* cold = (0.05*cost/M)/(-log(0.01)); */

    /* printf("Hot Temperature\t %f\n", temp); */
    /* printf("cold Temperature\t %f\n", cold); */

    double alpha; /* temperature decrease rate  */
    alpha = 100.0;
    if(argc > __StepsNr){
      sscanf(__Steps, "%lf", &alpha);
    }
    alpha = 1.0/alpha;
    alpha = pow((cold/temp), alpha);
    /* alpha = pow((cold/temp), (1.0/1000.0)); */

  /* ╔═╗┌┬┐┌─┐┬─┐┌┬┐  ╔═╗┌┐┌┐┌┌─┐┌─┐┬  ┬┌┐┌┌─┐ */
  /* ╚═╗ │ ├─┤├┬┘ │   ╠═╣│││││├┤ ├─┤│  │││││ ┬ */
  /* ╚═╝ ┴ ┴ ┴┴└─ ┴   ╩ ╩┘└┘└┘└─┘┴ ┴┴─┘┴┘└┘└─┘ */

    long long int Total_Ops =0;
    long long int Total_Iterations =0;
    clock_t Clock_Begin = clock();

    /* Print in gnuplot format */
    printf("# \
Cost\t\
MinCost\t\
Total_Time\t\
Total_Ops\t\
Total_Iterations\t\
Dirty\t\
Temperature\n");

    long long int k;  /* counter for same temp iterations */
    int ip; /* Second row index */
    int min; /* Overall min found so far */
    int pmin; /* Previous minimum */
    int ii;   /* Another counter on rows */

    min = cost;
    pmin = min+1;
    while(pmin > min || temp > cold){
      pmin = min;
      /* printf("\n\n\n Temp Drop: %f \n\n\n", temp); */

      printf("%10d\t", cost);
      printf("%10d\t", min);
      printf("%e\t", (double)(clock()-Clock_Begin)/ CLOCKS_PER_SEC);
      printf("%20lld\t", Total_Ops);
      printf("%20lld\t", Total_Iterations);
      printf("%6d\t", dirties);
      printf("%e\t", temp);
      printf("\n");

      k = 1;
      while(k < iter){
	/* Get random state */
	int deltaCost;

	/* if(5 < dirties){ */
	/*   printf("%10d\t", cost); */
	/*   printf("%10d\t", min); */
	/*   printf("%e\t", (double)(clock()-Clock_Begin)/ CLOCKS_PER_SEC); */
	/*   printf("%20lld\t", Total_Ops); */
	/*   printf("%20lld\t", Total_Iterations); */
	/*   printf("%6d\t", dirties); */
	/*   printf("%e\t", temp); */
	/*   printf("\n"); */
	/* } */

	deltaCost = 0;
	if(0 < dirties){ /* Global max or stuck at local */
	  /* Assume i is dirty */
	  i = randomIndex(dirties);
	  ii = 0;
	  while(ii < M && 0 <= deltaCost){
	    iSwap(&P[ii], &P[ii+arc4random_uniform(M-ii)]);
	    if(P[ii] != i){
	      deltaCost = 0;
	      j = 0;
	      while(j < N){
		Total_Ops++;
		newRow[j] = (S[A[i].i][j]+S[A[P[ii]].i][j]) % 2;
		deltaCost += newRow[j];
		j++;
	      }

	      if(A[i].onesCount > A[P[ii]].onesCount){
		deltaCost -= A[i].onesCount;
	      } else {
		deltaCost -= A[P[ii]].onesCount;
	      }

	      if(0 > deltaCost){/* Negative delta */
		if(A[i].onesCount > A[P[ii]].onesCount){ /* commit to i */
		  ip = i;
		  i = P[ii];
		} else {
		  ip = P[ii];
		}
	      }
	    }
	    ii++;
	  }

	  if(0 <= deltaCost){ /* Clean up bit i */
	    A[i].dirtyBit = 1;
	    dirties--;
	    swapStructs(&A[i],&A[dirties]);
	  }
	}

	if(0 <= deltaCost){
	  /* Fancy search fails fall back to vanilla  */
	  i = randomIndex(M);
	  ip = randomIndex(M-1);
	  if(ip >= i)
	    ip++;

	  deltaCost = 0;
	  j = 0;
	  while(j < N){
	    Total_Ops++;
	    newRow[j] = (S[A[i].i][j]+S[A[ip].i][j]) % 2;
	    deltaCost += newRow[j];
	    j++;
	  }
	  deltaCost -= A[ip].onesCount;
	}

	/* if(deltaCost > 0) */
	/* 	printf("."); */

	if(deltaCost <= 0 ||
	   (double)deltaCost <= temp*(30.0*log(2) - log(1+arc4random_uniform(1<<30)))
	   /* (double)deltaCost >= temp*(30.0*log(2) - log(1+arc4random_uniform(1<<30))) */
	   ){ /* Commit change */
	  /* if(deltaCost > 0) */
	  /*   printf(">>>>>>>>>> Hit %2d ", deltaCost); */

	  memcpy(S[A[ip].i], newRow, N*sizeof(int));
	  A[ip].onesCount += deltaCost;
	  cost += deltaCost;
	  assert(countOnes(N, M, S) == cost);

	  if(1 == A[ip].dirtyBit){
	    A[ip].dirtyBit = 0; /* Make ip dirty */
	    swapStructs(&A[ip], &A[dirties]);
	    dirties++;
	  }

	  /* checkOrder(A, M); */
	  /* printf("Adding %d to %d\n", i, ip); */
	  /* printMatrix(N, M, S); */

	  /* printf("%3d [%3d] dirties: %3d \n", */
	  /* 	 cost, min, dirties); */

	  if(cost < min)
	    min = cost;
	} /* Else do nothing */
	k++;
	Total_Iterations++;
      }
      temp *= alpha;     /* Update temperature */
    }

    writeMatrix(__OutFname, N, M, S);

    i = 0;
    while(i < M){
      free(S[i]);
      i++;
    }

    free(newRow);
    free(P);
    free(A);
    free(S);

    if(stdin != f)
      fclose(f);

    printf("\n\n"); /* Gnuplot new index */

  }

  return 0;
}
