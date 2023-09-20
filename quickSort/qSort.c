#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 


void quickSort(int array[], int left, int right){

    int i,j,pivot,temp;
    if(left < right){
        i = left+1;
        j = right;
        pivot = array[left];
        while (i < j)
        {
            while (array[i] < pivot) i++;
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


        quickSort(array, left, j-1);
        quickSort(array, j+1, right);
        
    }else
    {
        return;
    }
    
    
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
    int i;    

    int a[] = {4, 7, 1, 3, 10, 5, 9, 14, 45, 30, 11, 13, 621, 212, 300, 154};
    int b[1000];
    srand(time(NULL));
	addData(b, 100);
    for ( i = 0; i < 100; i++)
    {
        printf("%d ",b[i]);
    }
    quickSort(b, 0, 99);

    isSorted(b,100);
    
    return 0;
}
