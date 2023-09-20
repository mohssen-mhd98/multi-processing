#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

void mergeSort(int a[], int aux[], int i, int j);
void split(int a[],int aux[], int i, int j);
void isSorted(int arr[], int length);

// making process for left and right halves of array
void split(int a[],int aux[], int i, int j)
{
	int length=j-i+1;
    int status;
	// explains wethere array is Empty or with one element
	if (j<=i)
	{
		return;
	}

    // Saving time by calling less fork()
    // and using recursive func
    if(length > 100){
        pid_t lpid,rpid;
	lpid = fork();
	if (lpid<0)
	{
		// left-child process not created
		perror("Left child Process is Not created!!\n");
		_exit(-1);
	}
	else if (lpid==0)
	{
		split(a,aux,i,(i+j)/2);

		_exit(0);
	}
	else
	{
		rpid = fork();
		if (rpid<0)
		{
			// right-child process not created
			perror("Right child Process is Not created!!\n");
			_exit(-1);
		}
		else if(rpid==0)
		{
			split(a,aux,(i+j)/2+1,j);
			_exit(0);
		}
	}


	// Wait for child processes to finish
	waitpid(lpid, &status, 0);
	waitpid(rpid, &status, 0);



    }else
    {
        split(a,aux,i,(i+j)/2);
        split(a,aux,(i+j)/2+1,j);

    }


	// Merge and sort divided subarrays
	mergeSort(a,aux, i,j);

} //end of split function


// Method to merge sorted subarrays
void mergeSort(int a[], int aux[], int i, int j)
{
        //middle of current Array
	    int mid = (i + j) / 2;

    // left sub-array is a[i .. mid]
    // right sub-array is a[mid + 1 .. j]

    int pointer_left = i;
    int pointer_right = mid + 1;
    int c;      // counter

    // we loop from i to j to fill each element of the final merged array
    for (c = i; c <= j; c++) {

         // left pointer has reached the limit
        if (pointer_left == mid + 1) {
            aux[c] = a[pointer_right];
            pointer_right++;
        }

        // right pointer has reached the limit
        else if (pointer_right == j + 1) {
            aux[c] = a[pointer_left];
            pointer_left++;
        } else if (a[pointer_left] < a[pointer_right]) {
            aux[c] = a[pointer_left];
            pointer_left++;
        } else {
            aux[c] = a[pointer_right];
            pointer_right++;
        }
    }

    // copy back the elements from aux[] to a[]
    for (c = i; c <= j; c++) {
        a[c] = aux[c];
    }

} // end of mergeSort function

// Check Array is Sorted Or Not
void isSorted(int arr[], int length)
{
	if (length==1)
	{
		printf("Sorting Done Successfully\n");
		return;
	}

	int i;
	for (i=1; i<length; i++)
	{
		if (arr[i]<arr[i-1])
		{
			printf("Sorting Not Done\n");
			return;
		}
	}
	printf("\nConratulations\n**Sorting Array was Successfull**\n");
	return;
}

// add Data into Array
void addData(int a[], int len)
{
	// Create random arrays
	int i;
	for (i=0; i<len; i++)
		a[i] = rand();
	return;
}

// main function
int main()
{
	int shmid;
	key_t key = ftok("shmfile",65);
	int *shm_array;
	int i;
	clock_t tStart = clock();
    // auxiliary Array
	int aux[10000];

    //desired length of Array
    //that has to be Sorted
	int length = 10000;

	/* printf("Enter No of elements of Array:");
	scanf("%d",&length); */

	// Calculate segment length
	size_t SHM_SIZE = sizeof(int)*length;

	// required segment.
    // required Shared memory**
	if ((shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0)
	{
		perror("shmget");
		_exit(1);
	}

	//  Attaching segment to our processes
	if ((shm_array = shmat(shmid, NULL, 0)) == (int *) -1)
	{
		perror("shmat");
		_exit(1);
	}

	// fill Array with Random number
	srand(time(NULL));
	addData(shm_array, length);

	// Sort the created array
	split(shm_array,aux, 0, length-1);
	for(i=0;i<length;i++)
	printf("%d ",shm_array[i]);

	// Check if Array is Sorted or Not
	isSorted(shm_array, length);

    // done with memory segment
    //Detaching it
	if (shmdt(shm_array) == -1)
	{
		perror("shmdt");
		_exit(1);
	}

	//delete the Shared Memory
	if (shmctl(shmid, IPC_RMID, NULL) == -1)
	{
		perror("shmctl");
		_exit(1);
	}
	 printf("Time taken: %fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
	return 0;
}
