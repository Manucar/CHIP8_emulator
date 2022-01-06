#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
    FILE *fptr;
	long filelen;
	char *bytes_buffer;
    char *rom_path  = argv[1];
    printf("%s\n", rom_path);


    fptr = fopen(rom_path,"r");

    if(fptr == NULL)
    {
		printf("Wrong path!");   	
		exit(1);             
    }

	
	uint16_t buff[1];
	size_t fred_ret = 1;
    
	int bytes_read = 0;

	// Read all binary from file
	for (;;)
	{
		fred_ret = fread(buff, 2, 1, fptr);
		if (fred_ret == 0)
			break;
		printf("%x\n", buff[0]);
		bytes_read++;
	}
	
	
	printf("bytes read = %d",bytes_read);
	
	/*
	fseek(fptr, 0, SEEK_END);          // Jump to the end of the file
	filelen = ftell(fptr);             // Get the current byte offset in the file
	rewind(fptr);                      // Jump back to the beginning of the file

	bytes_buffer = (char *)malloc(filelen * sizeof(char)); // Enough memory for the file
	fread(bytes_buffer, filelen, 1, fptr); // Read in the entire file
	fclose(fptr); // Close the file

	printf("%d", filelen);
	*/
	 

    return 0;
}