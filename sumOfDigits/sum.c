#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<sys/wait.h> 

int main(int argc, char *argv[]) 
{ 

	int number;

	int fd1[2]; // First pipe 
	int fd2[2]; // Second pipe 
	char firm_str[] = "The sum of digits is:";  
    int fS_size = 21; // Size of firm String
	pid_t p;
    
	if (pipe(fd1)==-1) 
	{ 
		fprintf(stderr, "Pipe Failed" ); 
		return 1; 
	} 
	if (pipe(fd2)==-1) 
	{ 
		fprintf(stderr, "Pipe Failed" ); 
		return 1; 
	} 
	//Scanf********************** 
	printf("Enter your desired Number :\n");
	scanf("%d", &number);
	//Scanf**********************
    
    p = fork(); // Create child process

	if (p < 0) 
	{ 
		fprintf(stderr, "fork Failed" ); 
		return 1; 
	} 

	// Parent process 
	else if (p > 0) 
	{ 
		char concat_str[100]; 

		close(fd1[0]); // Close reading end of first pipe

		// Write input string and close writing end of first pipe. 
		write(fd1[1], &number, sizeof(number));
		close(fd1[1]); 
		
		// Wait for result of child  
		wait(NULL); 

		close(fd2[1]); // Close writing end of second pipe 

		// Read string from child, print it. 
		read(fd2[0], concat_str, 100); 
		printf("%s\n", concat_str); 

		// Close reading end of second pipe
		close(fd2[0]); 
	} 

	// Child process 
	else
	{ 
		close(fd1[1]); // Close writing end of first pipe 

		//close(fd[1]); 

        // define text for concatenation number 
        // & sText for concatenation sum of digits
		char text[120],sText[120];
   		int digit,num,sSize;
        int sum = 0;

        // digits of number & sum of number
   		int digits[100],sDigits[100];
   		int i = 0; // Counter

		char concat_str[100];  
		read(fd1[0], &number, sizeof(number));
		//printf("%d$$$$$$$\n",number);
		num = number;
   		while (number>0){
    	number = number / 10;
   		 i++;
  		}
   		int aSize = i; // Length of number String

		//printf("%d$$$$$$$\n",aSize);

		while (num>0){
    	digits[i-1] = num % 10;
    	text[i-1] = digits[i-1] + '0';
    	//printf("%c",text[i]);
    	num = num / 10;
    	i--;
   	   }

        // Append white space to end of number(txt)
        // And ending String with '\0'
		text[aSize] = ' '; 
		text[aSize+1] = '\0';

        /*printf("%s",text);*/

     	for ( i = 0; i < aSize; i++)
      	{
        	sum +=  digits[i];
      	}

      	i = 0;
        //printf("%d\n",sum);
    	int sum1 = sum;
    	while (sum>0){
    	sum = sum / 10;
    	i++;
   		}
   		sSize = i; // Length of sum String

		while (sum1>0){
    	sDigits[i-1] = sum1 % 10;
    	sText[i-1] = sDigits[i-1] + '0';
    	//printf("%c",text[i]);
    	sum1 = sum1 / 10;
    	i--;
   		}
   		sText[sSize] = '\0';

		// Concatenate string 
		int k = 0; 
		for (i=0; i<aSize+1; i++) 
			concat_str[k++] = text[i]; 

		for (i=0; i<fS_size; i++) 
			concat_str[k++] = firm_str[i];

		for (i=0; i<sSize; i++) 
		concat_str[k++] = sText[i]; 

 

		concat_str[k] = '\0'; // String ends with '\0' 

		// Close both reading ends 
		close(fd1[0]); 
		close(fd2[0]); 

		// Write concatenated string
		write(fd2[1], concat_str, k+1); 
		close(fd2[1]); //Close Wrting end of second pipe

		exit(0); 
	} 
} 
