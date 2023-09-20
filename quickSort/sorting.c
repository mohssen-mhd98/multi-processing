#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


void quickSort(int array[], int left, int right){

    int i,j,pivot,temp,status,len;
    if(left >= right) return;
        len = right - left;
        i = left;
        j = right;
        pivot = array[left];
        while (i < j)
        {
            while (array[i] <= pivot && i < right) i++;
            while (array[j] > pivot) j--;

            if (i < j){
                temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            } 
        }

        if(array[left] > array[j] ){
            
        temp = array[left];
        array[left] = array[j];
        array[j] = temp;
        }
  if(len > 100){
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
		quickSort(array, left, j-1);

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
			quickSort(array, j+1, right);
			_exit(0);
		}
	}


	// Wait for child processes to finish
	waitpid(lpid, &status, 0);
	waitpid(rpid, &status, 0);
  }else
  {
      quickSort(array, left, j-1);
      quickSort(array, j+1, right);
  }
  

        /*quickSort(array, left, j-1);
        quickSort(array, j+1, right);*/
        
    
    
    
}

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

int main(){
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
	quickSort(shm_array,0, length-1);
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

