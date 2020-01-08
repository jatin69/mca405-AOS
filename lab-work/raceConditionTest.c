#include<fcntl.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h> 
int main()
{
	// copy contents to make a temp file

	FILE *fptr1, *fptr2; 
    char c; 
  
    // Open one file for reading 
    fptr1 = fopen("original.txt", "r"); 
    fptr2 = fopen("temp.txt", "w"); 
    
    // Read contents from file 
    c = fgetc(fptr1); 
    while (c != EOF) 
    { 
        fputc(c, fptr2); 
        c = fgetc(fptr1); 
    } 
  
    printf("\nSTARTING NOW \n"); 
  
    fclose(fptr1); 
    fclose(fptr2); 

	
	// work on temp file	
	char temp[128];
	if(fork()==0){
		int fd1;
		char buf1[128];
		fd1=open("temp.txt",O_RDONLY);
		printf("\n starting read 1 \n");
		printf("%lir1", read(fd1,buf1,sizeof(buf1)));
		printf("%s",buf1);
		//sleep(1);
		printf("\n starting read 2 \n");
		printf("%lir2", read(fd1,buf1,sizeof(buf1)));
		printf("%s",buf1);	
	}
	else{
		int fd2,i;
		char buf2[128];
		char mychar = 'a';
		for(i=0;i<sizeof(buf2);i++){
			if(i%26==0){
				buf2[i]='\n';			
			}
			else{
				mychar++;
				buf2[i]= mychar;
			}			
		}
		fd2=open("temp.txt",O_WRONLY);
		printf("\n starting write 1 \n");
		printf("%liw1", write(fd2,buf2,sizeof(buf2)));
		//sleep(10);
		printf("\n starting write 2 \n");
		printf("%liw2", write(fd2,buf2,sizeof(buf2)));

		printf("hello");
		wait(NULL);

		printf("ALL DONE");
	}

	return 0;
}
