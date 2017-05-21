/**
 *  \file driver.cc
 *  \brief HW 1: OpenMP
 *
 *  This program
 *
 *  - creates an input array of keys to sort, where the caller gives
 *    the array size as a command-line input;
 *
 *  - sorts it sequentially, noting the execution time;
 *
 *  - sorts it using YOUR parallel implementation, also noting the
 *    execution time;
 *
 *  - checks that the two sorts produce the same result;
 *
 *  - outputs the execution times and effective sorting rate (i.e.,
 *    keys per second).
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
typedef unsigned long keytype;

/* ============================================================
 */

void parallelSort (int N, keytype* A);

/* ============================================================
 *  * Some helper routines for managing an array of keys.
 *   */

keytype *
newKeys (int N)
{
  keytype* A = (keytype *)malloc (N * sizeof (keytype));
  return A;
}

/** Returns a new copy of A[0:N-1] */
keytype *
newCopy (int N, const keytype* A)
{
  keytype* A_copy = newKeys (N);
  memcpy (A_copy, A, N * sizeof (keytype));
  return A_copy;
}

int
main (int argc, char* argv[])
{
  int N = -1;

  if (argc == 2) {
    N = atoi (argv[1]);
    if(N <= 0)
        return -1;
  } else {
    fprintf (stderr, "usage: %s <n>\n", argv[0]);
    fprintf (stderr, "where <n> is the length of the list to sort.\n");
    return -1;
  }

  /* Create an input array of length N, initialized to random values */
  keytype* A_in = newKeys (N);
  for (int i = 0; i < N; ++i)
    A_in[i] = lrand48 ();

  printf ("\nN == %d\n\n", N);

  /* Sort in parallel, calling YOUR routine. */
  keytype* A_par = newCopy (N, A_in);
  parallelSort (N, A_par);
  printf ("Done");

  /* Cleanup */
  printf ("\n");
  free (A_par);
  free (A_in);
  return 0;
}

/* eof */
