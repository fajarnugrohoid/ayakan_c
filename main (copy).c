#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdint.h>
#include <mysql/mysql.h>
#include <mysql/my_global.h>

// typedef struct
// {
//   int id;
//   char name[100];
//   int type;
// } status;

typedef struct{
	int id;
	int age;
	char name[50];
	char address[100];
} student_item;

student_item *student_table;
student_item *tmp;
// student_item *student_get_byregistration_id(char *id){
// 	printf("%d \n", id);
// }


int main()
{

	student_item *tmp; /*bkin pointer tmp dari struct student_item */
	student_item sitem; /*bkin variable penampung dri struct tipe data student_item */
	tmp = &sitem; /*memasukan alamat memory sitem ke pointer tmp, jdi si pointer tmp tau alamat memory sitem */
	
	// int assignid = 2;
	// int assignage = 24;
	// char assignname[50] = "fajar";
	// char assignadd[50] = "cigondewah rahayu";

	// tmp->id = assignid; /* mengisi pointer tmp-> dengan nilai 2. */
	// (*tmp).age = assignage; /* mengisi pointer (*tmp).age dengan nilai 24. */
	// strcpy(tmp->name, assignname); /* mengisi pointer (*tmp).age dengan fungsi strcpy karena string. */
	// strcpy((*tmp).address, assignadd);

	// printf("id : %d\n", tmp->id);
	// printf("age : %d\n", (*tmp).age);
	// printf("name : %s\n", tmp->name);
	// printf("address : %s\n", (*tmp).address);


    MYSQL *db;
	db = mysql_init(NULL);

	if(mysql_real_connect(db, "localhost", "root", "password", "ppdb2017", 0, NULL, 0) == NULL){
		abort();
	}
	printf("connect db \n");
	MYSQL_RES *q;
	MYSQL_ROW row;

	char table[15] = "elementary";

	//tmp++;
	char sql_select[550];

	mysql_query(db, "SELECT id,name FROM ppdb_registration_nonacademic where id=1 limit 1");
	q = mysql_store_result(db);

	

  // status record;
  // record.id=1;
  // strcpy(record.name, "Raju");
  //record.type = 86;

	// int eweuh = 0;
	while((row = mysql_fetch_row(q)) != NULL){
    	tmp->id = row[0][0];
    	//strcpy(tmp->name, row[1][0]);

    // record.id = row[0][0];
    // record.type = row[0][1];
    // strcpy(record.name, row[0][2]);
    //printf("%c\n", row[0][1]);
	//	tmp++;
	}
	

	
	printf("xx:%d <->yyy\n", tmp->id);
	//printf("xx:%s <->yyy\n", tmp->name);
	printf("eweuh euy!\n");
  //printf(" Id is: %d \n", tmp.id);
  // printf(" Name is: %s \n", record.name);
  // printf(" Percentage is: %d \n", record.type);
	mysql_free_result(q);



	mysql_close(db);

    printf("Hello world!\n");
    return 0;
}
