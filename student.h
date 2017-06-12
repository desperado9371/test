#ifndef _STUDENT_H_
#define _STUDENT_H_

#define MAX_RECORD_SIZE		120	//id(8) + name(20) + department(20) + year(1) + address(30) + phone(15) +  email(26)
#define PAGE_SIZE		1024	// 1 KBytes
#define HEADER_SIZE		100	// 100 Bytes
#define MAX_RESULT		100
#define DATA_SIZE		924
// �ʿ��� ��� 'define'�� �߰��� �� ����

typedef enum {ID=0, NAME, DEPT, YEAR,  ADDR, PHONE, EMAIL} FIELD;

typedef struct _STUDENT
{
	char id[9];		// �й�
	char name[21];		// �̸�
	char dept[21];		// �а�
	char year[2];		// �г�
	char addr[31];		// �ּ�
	char phone[16];		// ��ȭ��ȣ
	char email[27];		// �̸��� �ּ�
} STUDENT;
STUDENT *s[MAX_RESULT];
int student_index;

int num_of_pages;
int num_of_records;
int total_record_size;
int current_record_num;
int current_page_num;
int record_length;
char *recordbuf;
char* headerbuf;
char* databuf;
char *pagebuf;

char* s_field;
char* s_value;
typedef struct{
	int offset;
	int len;
}PAIR;
PAIR pair[24];

#endif
