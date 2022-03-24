/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include <file_data.h>

FileData* gFilesDatas = NULL;
int gNumFilesDatas = 0;

int bindFile( const char* filename );

int main(int argc, char** argv) {

	int showHelp = 0;
	if ( argc < 2 )
	{
		showHelp = 1;
	}
	else if ( argc >= 2 && (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")) )
	{
		showHelp = 1;
	}
	if ( showHelp )
	{
		printf("\nUsage: %s [command] [options]\n", argv[0]);
		
		printf("  commands:\n\n");
		printf("    bind\n");
		printf("      options: --input file1.extension file2.extension fileN.extension \n\
      --output outputfile.extension\n");

		printf("    unbind\n");
		printf("      options: --input bindedFile.extension\n\n");
		
		printf("The input files must be in \"input\" folder, and the generated output files will be in \"output\" directory to unbind put the output file into input and use the unbind command. The file name must not have spaces between each character, to space between chars, use \"-\", \"_\", or Upper and Lowercase chars.\n\n");
		return 0;
	}
	if ( !strcmp(argv[1], "bind") )
	{
		if ( argc < 6 )
		{	
			printf("Insufficient number of arguments\n");
			return -1;
		}
		if ( !strcmp(argv[2], "--input") || !strcmp(argv[2], "-i") )
		{
			for ( int i = 5; i < argc; i++ )
			{
				if ( strcmp(argv[i], "--output") || strcmp(argv[i], "-o") )
				{
					gNumFilesDatas++;
				}
			}
			
			char* outputFilePath = (char*)malloc(strlen("../output/") + strlen(argv[argc -1]) + 1);
			if ( !outputFilePath )
			{
				printf("Not enough memory\n");
				return -1;
			}
			outputFilePath[0] = '\0';
			strcat(outputFilePath, "../output/");
			strcat(outputFilePath, argv[argc - 1]);
			
			FILE* outputFile = fopen(outputFilePath, "wb");
			if ( !outputFile )
			{
				printf("Failed to open/create output file\n");
				free(outputFilePath);
				return -1;
			}
			free(outputFilePath);
			
			/*
				The top head of output file.
				
				2 Byte = version major
				2 Byte = version minor
				2 Byte = version patch
			*/
			
			uint16_t vMajor = V_MAJOR;
			uint16_t vMinor = V_MINOR;
			uint16_t vPatch = V_PATCH;
			if ( fwrite(&vMajor, sizeof(uint16_t), 1, outputFile) != 1 )
			{
				printf("Number of items does not match: FILE %s, LINE %d\n", 
				__FILE__, __LINE__);
				return -1;
			}
			if ( fwrite(&vMinor, sizeof(uint16_t), 1, outputFile) != 1 )
			{
				printf("Number of items does not match: FILE %s, LINE %d\n", 
				__FILE__, __LINE__);
				return -1;
			}
			if ( fwrite(&vPatch, sizeof(uint16_t), 1, outputFile) != 1 )
			{
				printf("Number of items does not match: FILE %s, LINE %d\n", 
				__FILE__, __LINE__);
				return -1;
			}
			
			gFilesDatas = (FileData*)malloc(sizeof(FileData) * gNumFilesDatas);
			if ( !gFilesDatas )
			{
				printf("Not enough memory\n");
				return -1;
			}
			for ( int i = 3, j = 0; i < 3 + gNumFilesDatas; i++, j++ )
			{
				gFilesDatas[j] = createFileData(argv[i], 1);
				if ( !gFilesDatas[j].name || !gFilesDatas[j].data )
				{
					printf("Error on creating FileData\n");
					break;
				}
				if ( !bindFileData(&gFilesDatas[j], outputFile) )
				{	
					printf("Unable to bind file data\n");
					break;
				}
			}
			
			
			fclose(outputFile);

			for ( int i = 0; i < gNumFilesDatas; i++ )
			{
				destroyFileData(&gFilesDatas[i]);
			}
			free(gFilesDatas);
			gFilesDatas = NULL;
		}
		else {
			printf("unrecognized option\n");
		}
	}
	else if ( !strcmp(argv[1], "unbind") )
	{
		if ( argc < 4 )
		{	
			printf("Insufficient number of arguments\n");
			return -1;
		}
		if ( !strcmp(argv[2], "--input") || !strcmp(argv[2], "-i") )
		{
			
		}
		else {
			printf("unrecognized option\n");
		}
	}
	else {
		printf("unrecognized command\n");
	}

	return 0;
}

int bindFile( const char* filename ) {
	
	
}





