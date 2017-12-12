/************************************************************************/
/* File Name : lc4.c 													*/
/* Purpose   : This file contains the main() for this project			*/
/*             main() will call the loader and disassembler functions	*/
/*             															*/												*/
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lc4_memory.h"
#include "lc4_loader.h"
#include "lc4_disassembler.h"
/* A utility function to reverse a string  */
void reverse(char str[], int length)
{
    int start = 0;
    int end = length -1;
    while (start < end)
    {
        char t=*(str+start);
        *(str+start)= *(str+end);
        *(str+end)=t;
        start++;
        end--;
    }
}
 
// Implementation of itoa()
char* itoa(int num, char* str, int base)
{
    int i = 0;
    int isNegative = 0;
 
    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
 
    // In standard itoa(), negative numbers are handled only with 
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
        isNegative = 1;
        num = -num;
    }
 
    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }
 
    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';
 
    str[i] = '\0'; // Append string terminator
 
    // Reverse the string
    reverse(str, i);
 
    return str;
}




/* program to mimic pennsim loader and disassemble object files */

int main (int argc, char** argv) {

	/**
	 * main() holds the linked list &
	 * only calls functions in other files
	 */

	/* step 1: create head pointer to linked list: memory 	*/
	row_of_memory* memory = NULL ;


	/* step 2: determine filename, then open it		*/
	/*   TODO: extract filename from argv, pass it to open_file() */
	if(argc<=1 || argv[1]==NULL){
		perror("error1: usage: ./lc4 <object_file.obj>\n");
		return 1;
	}
	char* filename = argv[1] ;	
	FILE *file=open_file(filename);

	/* step 3: call function: parse_file() in lc4_loader.c 	*/
	/*   TODO: call function & check for errors		*/
	
	if(parse_file(file, &memory)!=0){
		perror("parse_file() failed.");
		return -1;
	}
	//printf("entering reverse\n");
	/* step 4: call function: reverse_assemble() in lc4_disassembler.c */
	/*   TODO: call function & check for errors		*/
	if(reverse_assemble(memory)!=0){
		perror("reverse_assemble() failed.");
		return -1;
	}

	/* step 5: call function: print_list() in lc4_memory.c 	*/
	/*   TODO: call function 				*/
	print_list(memory);


	size_t len=strlen(filename);
	filename[len-3]='a';
	filename[len-2]='s';
	filename[len-1]='m';
	FILE *theFile = fopen (filename, "wb");
	//printf("opening file\n");
	row_of_memory* t=memory;
	int data=0;
	while(t!=NULL){
		//printf("first loop\n");
		if(t->address>=0x4000){
			if(data==0){
				data=1;
				char* d=".DATA\n";
				fputs(d,theFile);
				char addr[30];
				char*pre= ".ADDR 0x";
				char taddr[5];
				char* address=itoa (t->address,taddr,16);
				strcpy(addr,pre);
				strcat(addr,address);
				strcat(addr, "\n");
				fputs(addr, theFile);
				if(t->label!=NULL){
					fputs(t->label, theFile);
					fputs("\n", theFile);
				}
				t=t->next;
			}
			else if(t->label==NULL){
				char fill[30];
				strcpy(fill, ".FILL #");
				char* tfill[10];
				char* da=itoa(t->contents, tfill,10);
				strcat(fill,da);
				fputs(fill, theFile);
				fputs("\n", theFile);
				t=t->next;
			}
			else{
				data=0;
			}
		}

		else{
			t=t->next;
		}
	}
	t=memory;
	int code=0;
	short unsigned int a;
	while(t!=NULL){
		//printf("second loop\n");
		if(t->address<0x4000){
			if(code==0){
				//printf("new code\n");
				code=1;
				char* d=".CODE\n";
				fputs(d,theFile);
				char addr[30];
				a=t->address;
				//char* pre=".ADDR 0x";
				char taddr[5];
				char* address=itoa (t->address,taddr,16);
				strcpy(addr,".ADDR 0x");
				//printf("%s\n",addr);
				strcat(addr,address);
				strcat(addr,"\n");
				//printf("%s\n",addr);
				fputs(addr, theFile);
				if(t->label!=NULL){
					fputs(t->label, theFile);
					fputs("\n", theFile);
				}
				//printf("%s\n", t->assembly);
				if(t->assembly!=NULL){
					fputs(t->assembly, theFile);
					fputs("\n", theFile);
				}
				t=t->next;
				a++;
			}
			else{
				//printf("continued code\n");
				if(t->address!=a){
					char* d=".CODE\n";
					fputs(d,theFile);
					char addr[30];
					a=t->address;
					char taddr[5];
					char* address=itoa (t->address,taddr,16);
					strcpy(addr,".ADDR 0x");
					strcat(addr,address);
					strcat(addr,"\n");
					fputs(addr, theFile);
				}
				if(t->label!=NULL){
					fputs(t->label, theFile);
					fputs("\n", theFile);
				}
				//printf("%s\n", t->assembly);
				if(t->assembly!=NULL){
					fputs(t->assembly, theFile);
					fputs("\n", theFile);
				}
				t=t->next;
				a++;
			}
		}
		else{
			t=t->next;
		}

	}
	fclose(theFile);

	/* step 6: call function: delete_list() in lc4_memory.c */
	/*   TODO: call function & check for errors		*/
	if(delete_list(&memory)!=0){
		perror("delete_list() failed.");
		return -1;
	}

	/* only return 0 if everything works properly */
	return 0 ;
}

