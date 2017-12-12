/************************************************************************/
/* File Name : lc4_disassembler.c 										*/
/* Purpose   : This file implements the reverse assembler 				*/
/*             for LC4 assembly.  It will be called by main()			*/
/*             															*/												*/
/************************************************************************/

#include <stdio.h>
#include "lc4_memory.h"

int reverse_assemble (row_of_memory* memory)
{
	//printf("entering reverse_assemble\n");
	row_of_memory* curr=search_opcode(memory, 0x0001);
	while(curr!=NULL){
		
		short unsigned int temp=(curr->contents)<<10;
		int arithOp=(temp)>>13;
		//printf("arithOp %x\n", arithOp);

		temp=(curr->contents)<<4;
		int rd = temp>>13;
		//printf("rd %x\n", rd);

		temp=(curr->contents)<<7;
		int rs = temp>>13;
		//printf("rs %d\n", rs);

		temp=(curr->contents)<<13;
		int rt = temp>>13;
		//printf("rt %d\n", rt);

		temp=(curr->contents)<<11;
		int im = temp>>13;
		if (im > 15) {
			im = im -32;
		}
		//printf("done shifting\n");
		char* assembly=malloc(17*sizeof(char));
		if(assembly==NULL){
			return -1;
		}
		int p=0;

		//printf("switching\n");
		switch(arithOp){
			case 0x0:
				//printf("case0\n");
				assembly[p++]='A';
				assembly[p++]='D';
				assembly[p++]='D';
				assembly[p++]=' ';
				assembly[p++]='R';
				assembly[p++]=rd+48;
				assembly[p++]=',';
				assembly[p++]=' ';
				assembly[p++]='R';
				assembly[p++]=rs+48;
				assembly[p++]=',';
				assembly[p++]=' ';
				assembly[p++]='R';
				assembly[p++]=rt+48;
				assembly[p]='\0';
				//printf("end building assembly str\n");
				curr->assembly=malloc((p+1)*sizeof(char));
				strcpy(curr->assembly, assembly);
				//printf("done case0\n");
				break;

			case 0x01:
				//printf("case1\n");
				assembly[p++]='M';
				assembly[p++]='U';
				assembly[p++]='L';
				assembly[p++]=' ';
				assembly[p++]='R';
				assembly[p++]=rd+48;
				assembly[p++]=',';
				assembly[p++]=' ';
				assembly[p++]='R';
				assembly[p++]=rs+48;
				assembly[p++]=',';
				assembly[p++]=' ';
				assembly[p++]='R';
				assembly[p++]=rt+48;
				assembly[p]='\0';
				curr->assembly=malloc((p+1)*sizeof(char));
				strcpy(curr->assembly, assembly);
				//printf("done case1\n");
				break;

			case 0x10:
				//printf("case2\n");
				assembly[p++]='S';
				assembly[p++]='U';
				assembly[p++]='B';
				assembly[p++]=' ';
				assembly[p++]='R';
				assembly[p++]=rd+48;
				assembly[p++]=',';
				assembly[p++]=' ';
				assembly[p++]='R';
				assembly[p++]=rs+48;
				assembly[p++]=',';
				assembly[p++]=' ';
				assembly[p++]='R';
				assembly[p++]=rt+48;
				assembly[p]='\0';
				curr->assembly=malloc((p+1)*sizeof(char));
				strcpy(curr->assembly, assembly);
				//printf("done case2\n");
				break;

			case 0x11:
				//printf("case3\n");
				assembly[p++]='D';
				assembly[p++]='I';
				assembly[p++]='V';
				assembly[p++]=' ';
				assembly[p++]='R';
				assembly[p++]=rd+48;
				assembly[p++]=',';
				assembly[p++]=' ';
				assembly[p++]='R';
				assembly[p++]=rs+48;
				assembly[p++]=',';
				assembly[p++]=' ';
				assembly[p++]='R';
				assembly[p++]=rt+48;
				assembly[p]='\0';
				curr->assembly=malloc((p+1)*sizeof(char));
				strcpy(curr->assembly, assembly);
				//printf("done case3\n");
				break;

			default:
				//printf("case default\n");
				assembly[p++]='A';
				assembly[p++]='D';
				assembly[p++]='D';
				assembly[p++]=' ';
				assembly[p++]='R';
				assembly[p++]=rd+48;
				assembly[p++]=',';
				assembly[p++]=' ';
				assembly[p++]='R';
				assembly[p++]=rs+48;
				assembly[p++]=',';
				assembly[p++]=' ';
				assembly[p++]='#';
				if(im<0){
					assembly[p++]='-';
				}
				if(im<10&&im>=0){
					assembly[p++]=48+im;
				}
				else if(im>=10){
					assembly[p++]=48+im/10;
					assembly[p++]=48+im%10;
				}
				else if(im<-10){
					assembly[p++]=48-im;
				}
				else {
					assembly[p++]=48-im/10;
					assembly[p++]=48-im%10;
				}
				assembly[p]='\0';
				curr->assembly=malloc((p+1)*sizeof(char));
				strcpy(curr->assembly, assembly);
				//printf("done add im\n");
				break;


		}
		//printf("seach op again\n");
		curr=search_opcode(curr, 0x0001);
	}
	return 0 ;
}
