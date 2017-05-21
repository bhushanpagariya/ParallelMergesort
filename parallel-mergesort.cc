/**
 *  *  *  *  \file parallel-mergesort.cc
 *   *   *   *
 *    *    *    *  \brief Implement your parallel mergesort in this file.
 *     *     *     */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>

#include "sort.hh"

void serialMerge(keytype * ans, keytype * arr_a, int Na, keytype * arr_b, int Nb);
void merge(keytype* arr, int start, int mid, int end, int numThreads, keytype* temp);
void mergesortRec(keytype* arr, int start, int end, int numThreads, keytype* temp);
void parallelMerge(keytype * result, keytype * arr_a, int Na, keytype * arr_b, int Nb, int numThreads);
int binarySearch(keytype * arr, int len, keytype key);

void parallelSort (int N, keytype* A)
{
    /* Creating temporary array for storing partial results */
    keytype * temp = new keytype[N];
	#pragma omp parallel
	{
		#pragma omp single nowait 
        {
            mergesortRec(A, 0, N-1, omp_get_num_threads(), temp);
        }
	}
    free(temp);
}

/**
Recursive function for MergeSort. 
Divide array into two subarrays and call same function recusrively for 
both parts. Merge the two sorted arrays into one
*/
void mergesortRec(keytype* arr, int start, int end, int numThreads, keytype* temp) {
	if(start>=end)
		return;
	int mid = (start+end)/2;
    /* If number of threads are greater than 1 then calling parallel algorithm */
	if(numThreads > 1) {
    		#pragma omp task shared (arr,temp)
    		{
                mergesortRec(arr, start, mid, numThreads/2, temp);
    		}
            mergesortRec(arr, mid+1, end, numThreads/2, temp);
    		#pragma omp taskwait
	}else {
        /* Sequential MergeSort algorithm */
		mergesortRec(arr, start, mid, numThreads, temp);
		mergesortRec(arr, mid+1, end, numThreads, temp);
	}	
    
    int size_left_subarray = mid - start + 1;
    int size_right_subarray =  end - mid;    
    memcpy(temp+start, arr+start, (size_left_subarray+size_right_subarray)*sizeof(keytype));
    /* Merge two sorted subarrays into one */
    parallelMerge(arr+start, temp+start, size_left_subarray, temp+start+size_left_subarray, size_right_subarray, numThreads);
}

/*
Parallel merge function recursively divides merging task into two parts
And if both parts contains elements less than 2 then merges two subarrys 
into one
*/
void parallelMerge(keytype * result, keytype * arr_a, int Na, keytype * arr_b, int Nb, int numThreads) {
    /* If number of thread is 1 or length of arrays to be merged is less than 2 then calling sequential merge */
	if(numThreads<=1 || (Na<=2 && Nb<=2)) {
		serialMerge(result, arr_a, Na, arr_b, Nb);
		return;
	}
    /* Finding mid element of first array */
    int mid_a = (Na-1)/2;
    /* Searching for mid element of 2nd array using binary search */
    int mid_b = binarySearch(arr_b, Nb, arr_a[mid_a]);
    mid_a++;
    if(Na == 1 && mid_b == Nb) {
        /* mid is greater that entrie arr_b */
        memcpy(result, arr_b, Nb * sizeof(keytype));
        memcpy(result+Nb, arr_a, Na * sizeof(keytype));
        return;
    }
    /* calling parallelMerge function recursively for both parts */
    #pragma omp task shared (arr_a, arr_b, result)
    {
        parallelMerge(result, arr_a, mid_a, arr_b, mid_b, numThreads/2);
    }
    parallelMerge(result+(mid_a+mid_b), arr_a+(mid_a), Na-mid_a, arr_b+(mid_b), Nb-mid_b, numThreads/2);
    #pragma omp taskwait
}

/*
Serial merge
*/
void serialMerge(keytype * ans, keytype * arr_a, int Na, keytype * arr_b, int Nb) {
    int i = 0;
    int j = 0;
    int k = 0;
    while(i<Na && j<Nb) {
        if(arr_a[i] < arr_b[j]) {
            ans[k] = arr_a[i];
            i++;
        } else {
            ans[k] = arr_b[j];
            j++;
        }
        k++;
    }
    while(i<Na) {
        ans[k] = arr_a[i];
        i++;
        k++;
    }
    while(j<Nb) {
        ans[k] = arr_b[j];
        j++;
        k++;
    }
}

/*
Binary search for searching mid element such that all elements on left 
side of mid are less than key.
*/
int binarySearch(keytype * arr, int len, keytype key) {
    if(key > arr[len-1])
        return len;
    int start = 0;
    int end = len-1;
    while(start<end) {
        int mid = (start+end)/2;
        if(arr[mid] < key)
            start = mid+1;
        else
            end = mid;
    }
    return end;
}

/* eof */


