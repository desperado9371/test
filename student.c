#include <stdio.h>		// 필요한 header file 추가 가능
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "student.h"

//
// readPage() 함수는 레코드 파일에서 주어진 pagenum(=0, 1, 2, ...)에 해당하는 page를
// 읽어서 pagebuf가 가리키는 곳에 저장하는 역할을 수행한다. 정상적인 수행을 한 경우
// '1'을, 그렇지 않은 경우는 '0'을 리턴한다.
// getNextRecord() 함수는 readPage()를 통해 읽어온 page에서 record를 차례대로 전달하는
// 일을 한다. 만약 page에서 전달할 record가 없으면 '0'을, 그렇지 않으면 '1'을 리턴한다.
// unpack() 함수는 recordbuf에 저장되어 있는 record에서 각 field 값을 추출하여 학생 객체에 저장하는 일을 한다.
//
int readPage(FILE *fp, char *pagebuf, int pagenum);
int getNextRecord(const char *pagebuf, char *recordbuf);
void unpack(const char *recordbuf, STUDENT *s);

//
// writePage() 함수는 프로그램 상의 data page(pagebuf)를 레코드 파일의 주어진 pagenum(=0, 1, 2, ...)의 위치에 
// 저장하는 역할을 수행한다. 정상적인 수행을 한 경우 '1'을, 그렇지 않은 경우는 '0'을 리턴한다.
//
int writePage(FILE *fp, const char *pagebuf, int pagenum);

//
// 주어진 학생 객체를 레코드 파일에 저장할 때 사용되며, 이때 파일을 구성하는 
// page 중에서 맨마지막 page에 해당 record를 저장한다. 만약, 맨마지막 page에 새로운
// 레코드를 저장할 공간이 부족하면 새로운 page를 하나 할당해서 여기에 저장한다.
// writeRecord() 함수의 내부에서는 pack() 함수를 호출하며, pack() 함수는 주어진 학생 객체를 실제로 
// 새로운 record가 page에 저장되었을 때의 형태를 recordbuf에 저장하는 역할을 수행한다.
// writeRecord()는 성공적으로 수행하면 '1'을, 그렇지 않으면 '0'을 리턴한다.
//
int writeRecord(FILE *fp, const STUDENT *s);
void pack(char *recordbuf, const STUDENT *s);

//
// 레코드 파일에서 field의 키값(keyval)을 갖는 레코드를 검색하고 그 결과를 출력한다.
// 검색된 레코드를 출력할 때 반드시 printRecord() 함수를 사용한다.
//
void search(FILE *fp, FIELD f, char *keyval);
void printRecord(const STUDENT *s, int n);

//
// 레코드의 필드명을 enum FIELD 타입의 값으로 변환시켜 준다.
// 예를 들면, 사용자가 수행한 명령어의 인자로 "NAME"이라는 필드명을 사용하였다면 
// 프로그램 내부에서 이를 NAME(=1)으로 변환할 필요성이 있으며, 이때 이 함수를 이용한다.
//
FIELD getField(char *fieldname);



int main(int argc, char *argv[])
{
	FILE *fp;						// 모든 file processing operation은 C library를 사용할 것.	
	s[student_index] = (STUDENT *)malloc(sizeof(STUDENT));
	char filename[20];
	strcpy(filename,argv[2]);
	if(argc<2)
		return 0;
	if(strcmp(argv[1],"-i")==0)
	{
		if( (fp=fopen(filename,"r+"))==NULL)
		{
			printf("new file\n");
			//create new page
			FILE *temp;
			temp=fopen(filename,"w");
			pagebuf=(char*)malloc(PAGE_SIZE);
			memset((void*)pagebuf, 0xFF, PAGE_SIZE);
			pagebuf[0]=0;pagebuf[1]=1;
			num_of_pages=1;
			writePage(temp,pagebuf,0);
			headerbuf=(char*)malloc(HEADER_SIZE);
			databuf=(char*)malloc(DATA_SIZE);
			memset((void*)headerbuf, 0xFF, HEADER_SIZE);
			int i;
			for(i=0 ; i<4; i++)
				headerbuf[i]=0;
			memset((void*)pagebuf, 0xFF, PAGE_SIZE);
			memcpy(pagebuf,headerbuf,HEADER_SIZE);
			writePage(temp,pagebuf,1);
			num_of_records=0;
			total_record_size=0;
			fclose(temp);
			free(headerbuf);
			free(databuf);
			fp=fopen(filename,"r+");
		}
		printf("file exist\n");
		pagebuf=(char*)malloc(PAGE_SIZE);
		if(readPage(fp,pagebuf,0)!=1)
		{
			printf("error reading\n");
			return 0;
		}
		num_of_pages= (pagebuf[0])*127 + pagebuf[1] ;
		printf("pages:%d\n",num_of_pages);
		if(readPage(fp,pagebuf,num_of_pages)!=1)
		{
			printf("error reading\n");
			return 0;
		}
		
		headerbuf=(char*)malloc(HEADER_SIZE);
		databuf=(char*)malloc(DATA_SIZE);
		memcpy(headerbuf,pagebuf,HEADER_SIZE);
		memcpy(databuf,pagebuf+HEADER_SIZE,DATA_SIZE);
		num_of_records=(headerbuf[0])*127 + headerbuf[1] ;
		total_record_size=(headerbuf[2])*127 + headerbuf[3] ;
		printf("#rec = %d  total = %d \n",num_of_records,total_record_size);
		int i;
		for(i=0 ; i<num_of_records ; i++)
		{
			pair[i].offset = (headerbuf[4+4*i])*127 + headerbuf[5+4*i];
			pair[i].len    = (headerbuf[6+4*i])*127 + headerbuf[7+4*i];
			printf("pair%d : %d , %d\n",i,pair[i].offset,pair[i].len);
		}
		
		strcpy(s[student_index]->id,argv[3]);
		strcpy(s[student_index]->name,argv[4]);
		strcpy(s[student_index]->dept,argv[5]);
		strcpy(s[student_index]->year,argv[6]);
		strcpy(s[student_index]->addr,argv[7]);
		strcpy(s[student_index]->phone,argv[8]);
		strcpy(s[student_index]->email,argv[9]);
		record_length=7;
		for(i=3 ; i<10 ; i++)
		{
			record_length+=strlen(argv[i]);
		}
		
		recordbuf=(char *)malloc(MAX_RECORD_SIZE+7);
		pack(recordbuf,s[student_index]);
		writeRecord(fp,s[student_index]);
		return 0;
	}
	else if(strcmp(argv[1],"-s")==0)	// search
	{
		if((fp=fopen(filename,"r+"))==NULL)
		{
			printf("Can't find File %s\n",filename);
			return 0;
		}
		char temp[100];
		strcpy(temp,argv[3]);
		s_field=strtok(temp,"=");
		s_value=strtok(NULL,"=");
		printf("%s  %s\n",s_field,s_value);
		FIELD f=getField(s_field);
		search(fp,f,s_value);
		return 0;
	}
	else
	{
		printf("illegal instruction %s\n",argv[1]);
		return 0;
	}
	
	
}

void printRecord(const STUDENT *s, int n)
{
	int i;

	printf("NumOfRecords = %d\n", n);

	for (i = 0; i<n; i++)
	{
		printf("%s#%s#%s#%s#%s#%s#%s\n", s[i].id, s[i].name, s[i].dept, s[i].year, s[i].addr, s[i].phone, s[i].email);
	}
}

int readPage(FILE *fp, char *pagebuf, int pagenum)
{
	int result;
	fseek(fp, PAGE_SIZE*pagenum, SEEK_SET);
	result = fread((void *)pagebuf, PAGE_SIZE, 1, fp);
	if (result == 1)
	{
		return 1;
	}
	else
		return 0;
}
int getNextRecord(const char *pagebuf, char *recordbuf)
{
	if (current_record_num == num_of_records)
		return 0;
	else
	{
		current_record_num++;
		memcpy(recordbuf, pagebuf + pair[current_record_num].offset, pair[current_record_num].len);
		return 1;
	}

}
void unpack(const char *recordbuf, STUDENT *s)
{
	char * temp;
	char * field;
	temp = (char *)malloc(strlen(recordbuf));
	strcpy(temp, recordbuf);
	field = strtok(temp, "#");
	strcpy(s->id, field);
	field = strtok(NULL, "#");
	strcpy(s->name, field);
	field = strtok(NULL, "#");
	strcpy(s->dept, field);
	field = strtok(NULL, "#");
	strcpy(s->year, field);
	field = strtok(NULL, "#");
	strcpy(s->addr, field);
	field = strtok(NULL, "#");
	strcpy(s->phone, field);
	field = strtok(NULL, "#");
	strcpy(s->email, field);

}

int writePage(FILE *fp, const char *pagebuf, int pagenum)
{
	int result;
	if(fp!=NULL)
		fseek(fp, PAGE_SIZE*pagenum, SEEK_SET);
	else
		printf("NULL FP\n");
	result = fwrite((void *)pagebuf, PAGE_SIZE, 1, fp);
	if (result == 1)
		return 1;
	else
		return 0;
}
int writeRecord(FILE *fp, const STUDENT *s)
{
	if(num_of_records>=24 || ((total_record_size + record_length) >924))
	{
		// add new datapage
		num_of_pages++;
		memset((void*)databuf, 0xFF, DATA_SIZE);
		memset((void*)headerbuf, 0xFF, HEADER_SIZE);
		int i;
		for(i=0 ; i<4; i++)
			headerbuf[i]=0;
		memset((void*)pagebuf, 0xFF, PAGE_SIZE);
		memcpy(pagebuf,headerbuf,HEADER_SIZE);
		writePage(fp,pagebuf,num_of_pages);

		// modify HeaderPage
		if(readPage(fp,pagebuf,0)!=1)
		{
			printf("error reading\n");
			return 0;
		}
		pagebuf[0]=num_of_pages/127; pagebuf[1]=num_of_pages%127;
		writePage(fp,pagebuf,0);
		num_of_records=0;
		total_record_size=0;
	}

	if(num_of_records<24 && ((total_record_size + record_length) <924) )
	{
		printf("writing\n");
		memcpy(databuf+total_record_size , recordbuf, record_length);
		num_of_records++;
		headerbuf[0]=num_of_records/127;
		headerbuf[1]=num_of_records%127;

		total_record_size += record_length;
		headerbuf[2]=total_record_size/127;
		headerbuf[3]=total_record_size%127;

		if(num_of_records==1)
		{
			pair[0].offset=0;
			pair[0].len=record_length;
		}
		else
		{
			pair[num_of_records-1].offset=pair[num_of_records-2].offset+pair[num_of_records-2].len;
			pair[num_of_records-1].len=record_length;
		}
		headerbuf[4+4*(num_of_records-1)]=pair[num_of_records-1].offset/127;
		headerbuf[5+4*(num_of_records-1)]=pair[num_of_records-1].offset%127;
		headerbuf[6+4*(num_of_records-1)]=pair[num_of_records-1].len/127;
		headerbuf[7+4*(num_of_records-1)]=pair[num_of_records-1].len%127;
		memset((void*)pagebuf, 0xFF, PAGE_SIZE);
		memcpy(pagebuf,headerbuf,HEADER_SIZE);
		memcpy(pagebuf+HEADER_SIZE, databuf,DATA_SIZE);
		
		writePage(fp,pagebuf,num_of_pages);
	}

}
void pack(char *recordbuf, const STUDENT *s)
{
	strcat(recordbuf,s->id);strcat(recordbuf,"#");
	strcat(recordbuf,s->name);strcat(recordbuf,"#");
	strcat(recordbuf,s->dept);strcat(recordbuf,"#");
	strcat(recordbuf,s->year);strcat(recordbuf,"#");
	strcat(recordbuf,s->addr);strcat(recordbuf,"#");
	strcat(recordbuf,s->phone);strcat(recordbuf,"#");
	strcat(recordbuf,s->email);strcat(recordbuf,"#");
}
void search(FILE *fp, FIELD f, char *keyval)
{
	//printf("file exist\n");
	pagebuf=(char*)malloc(PAGE_SIZE);
	if(readPage(fp,pagebuf,0)!=1)
	{
		printf("error reading\n");
		return ;
	}
	num_of_pages= (pagebuf[0])*127 + pagebuf[1] ;
	//printf("%d\n",num_of_pages);
	int index;
	for(index=1 ; index<=num_of_pages ; index++)
	{
		current_page_num=index;
		if(readPage(fp,pagebuf,current_page_num)!=1)
		{
			printf("error reading\n");
			return ;
		}
		headerbuf=(char*)malloc(HEADER_SIZE);
		databuf=(char*)malloc(DATA_SIZE);
		memcpy(headerbuf,pagebuf,HEADER_SIZE);
		memcpy(databuf,pagebuf+HEADER_SIZE,DATA_SIZE);
		num_of_records=(headerbuf[0])*127 + headerbuf[1] ;
		total_record_size=(headerbuf[2])*127 + headerbuf[3] ;
		//printf("page= %d #rec = %d  total = %d \n",current_page_num, num_of_records,total_record_size);
		int i;
		for(i=0 ; i<num_of_records ; i++)
		{
			pair[i].offset = (headerbuf[4+4*i])*127 + headerbuf[5+4*i];
			pair[i].len    = (headerbuf[6+4*i])*127 + headerbuf[7+4*i];
			//printf("pair%d : %d , %d\n",i,pair[i].offset,pair[i].len);
		}

		for(i=0 ; i<num_of_records; i++)
		{
			recordbuf=(char*)malloc(pair[i].len+2);
			memcpy(recordbuf,databuf+pair[i].offset,pair[i].len);
			STUDENT *temp_s;
			temp_s=(STUDENT*)malloc(sizeof(STUDENT));
			unpack(recordbuf,temp_s);
			switch(f)
			{
				case 0:
					//printf("comp %s : %s\n",keyval,temp_s->id);
					if(strcmp(keyval,temp_s->id)==0)
					{
						s[student_index]=(STUDENT*)malloc(sizeof(STUDENT));
						memcpy(s[student_index++],temp_s,sizeof(STUDENT));
					}
					break;
				case 1:
					//printf("comp %s : %s\n",keyval,temp_s->name);
					if(strcmp(keyval,temp_s->name)==0)
					{
						s[student_index]=(STUDENT*)malloc(sizeof(STUDENT));
						memcpy(s[student_index++],temp_s,sizeof(STUDENT));
					//	printf("%s %s %s\n",s[student_index-1]->name,s[student_index-1]->id,s[student_index-1]->email);
					}
					break;
				case 2:
					//printf("comp %s : %s\n",keyval,temp_s->dept);
					if(strcmp(keyval,temp_s->dept)==0)
					{
						s[student_index]=(STUDENT*)malloc(sizeof(STUDENT));
						memcpy(s[student_index++],temp_s,sizeof(STUDENT));
					}
					break;
				case 3:
					//printf("comp %s : %s\n",keyval,temp_s->year);
					if(strcmp(keyval,temp_s->year)==0)
					{
						s[student_index]=(STUDENT*)malloc(sizeof(STUDENT));
						memcpy(s[student_index++],temp_s,sizeof(STUDENT));
					}
					break;
				case 4:
					//printf("comp %s : %s\n",keyval,temp_s->addr);
					if(strcmp(keyval,temp_s->addr)==0)
					{
						s[student_index]=(STUDENT*)malloc(sizeof(STUDENT));
						memcpy(s[student_index++],temp_s,sizeof(STUDENT));
					}
					break;
				case 5:
					//printf("comp %s : %s\n",keyval,temp_s->phone);
					if(strcmp(keyval,temp_s->phone)==0)
					{
						s[student_index]=(STUDENT*)malloc(sizeof(STUDENT));
						memcpy(s[student_index++],temp_s,sizeof(STUDENT));
					}
					break;
				case 6:
					//printf("comp %s : %s\n",keyval,temp_s->email);
					if(strcmp(keyval,temp_s->email)==0)
					{
						s[student_index]=(STUDENT*)malloc(sizeof(STUDENT));
						memcpy(s[student_index++],temp_s,sizeof(STUDENT));
					}
					break;
			}
		}
	}
	STUDENT student[student_index];
	int shit;
	for(shit=0 ; shit<student_index ; shit++)
	{
		student[shit]=*s[shit];
	}
	printRecord(student,student_index);
}

FIELD getField(char *fieldname)
{
	if(strcmp(fieldname,"ID")==0)
		return ID;
	else if(strcmp(fieldname,"NAME")==0)
		return NAME;
	else if(strcmp(fieldname,"DEPT")==0)
		return DEPT;
	else if(strcmp(fieldname,"YEAR")==0)
		return YEAR;
	else if(strcmp(fieldname,"ADDR")==0)
		return ADDR;
	else if(strcmp(fieldname,"PHONE")==0)
		return PHONE;
	else if(strcmp(fieldname,"EMAIL")==0)
		return EMAIL;
}
