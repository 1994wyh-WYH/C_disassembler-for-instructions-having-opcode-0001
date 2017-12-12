/************************************************************************/
/* File Name : lc4_loader.c		 										*/
/* Purpose   : This file implements the loader (ld) from PennSim		*/
/*             It will be called by main()								*/
/*             															*/
/* Author(s) : tjf and you												*/
/************************************************************************/

#include <stdio.h>
#include "lc4_memory.h"

/* declarations of functions that must defined in lc4_loader.c */

FILE* open_file(char* file_name)
{
	FILE *file=fopen(file_name, "rb");
	//printf("file opened\n");
	if(file==NULL){
		perror("file opening failed\n");
	}
	return file ;
}

int parse_file (FILE* my_obj_file, row_of_memory** memory)
{
	int arr[2];

	while(1){
		arr[0]=fgetc(my_obj_file);
		//printf("%x\n", arr[0]);
		if(arr[0]==-1){
			break;
		}
		//printf("parsing\n");
		arr[1]=fgetc(my_obj_file);

	unsigned short int twoBytes=arr[0]*256+arr[1];
	//printf("twoBytes %x\n",twoBytes);
	if(twoBytes==0xCADE || twoBytes==0xDADA){
		//printf("parsing %x\n", twoBytes);
	arr[0]=fgetc(my_obj_file);
	arr[1]=fgetc(my_obj_file);
	twoBytes=arr[0]*256+arr[1];
	unsigned short int addr=twoBytes;

	arr[0]=fgetc(my_obj_file);
	arr[1]=fgetc(my_obj_file);
	twoBytes=arr[0]*256+arr[1];
	int count=twoBytes;

	while(count>0){
		arr[0]=fgetc(my_obj_file);
		arr[1]=fgetc(my_obj_file);
		twoBytes=arr[0]*256+arr[1];
		//printf("%x\n", twoBytes);
		if(add_to_list(memory,addr,twoBytes)!=0){
			return -1;
		}
		addr++;
		count--;
	}
	}
	
	else if(twoBytes==0xC3B7){
		//printf("parsing C3B7\n");
	arr[0]=fgetc(my_obj_file);
	arr[1]=fgetc(my_obj_file);
	twoBytes=arr[0]*256+arr[1];
	row_of_memory* temp=search_address(*memory, twoBytes);
	if(temp==NULL){
		//printf("temp is null\n");
		if(add_to_list(memory,twoBytes,0x0)!=0){
			perror("adding to list failed\n");
			return -1;
		}
		temp=search_address(*memory, twoBytes);
		//printf("%s\n", temp->label);
	}
	//printf("searching address ends\n");
	arr[0]=fgetc(my_obj_file);
	arr[1]=fgetc(my_obj_file);
	twoBytes=arr[0]*256+arr[1];
	//printf("building label str\n");
	char* str=malloc((twoBytes+1)*sizeof(char));
	for(int i=0;i<twoBytes;i++){
		str[i]=fgetc(my_obj_file);
	}
	str[twoBytes]='\0';
	//printf("copying str %s to label\n", str);
	//printf("%d\n", temp==NULL);
	temp->label=malloc((twoBytes+1)*sizeof(char));
	
	//printf("%d\n", temp==NULL);

	if(temp->label==NULL){
		perror("can't allocate enough memory for label\n");
		return -1;
	}
	//printf("copying str %s to label\n", str);
	strcpy(temp->label,str);
	free(str);
	//printf("ends copying\n");
	}

	/*else{
		printf("else case\n");
		if(feof(my_obj_file)){
			break;
		}
		arr[0]=fgetc(my_obj_file);
		arr[1]=fgetc(my_obj_file);
		twoBytes=arr[0]*256+arr[1];
		while(twoBytes!=0xCADE||twoBytes!=0xDADA||twoBytes!=0xCADE){
			arr[0]=fgetc(my_obj_file);
			arr[1]=fgetc(my_obj_file);
			twoBytes=arr[0]*256+arr[1];
		}
	}*/

}
	//printf("closing file\n");
	if(fclose(my_obj_file)!=0){
		//printf("fail closing file, deleting list\n");
		int delete=delete_list(memory);
		if(delete!=0){
			return delete;
		}
		return 0;
	}
	return 0 ;
}
