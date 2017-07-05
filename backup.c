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
	float age;
	char name[50];
	char address[100];
} student_item;

student_item *student_table;
student_item *tmp;
// student_item *student_get_byregistration_id(char *id){
// 	printf("%d \n", id);
// }

struct orderSlip
{
    char source[64];
    char destination[64];
    char item[64];
    int position;
};
//global
int MAX_ITEMS = 4;
int num_items = 0;



//struct orderSlip data[100];



void add_item(struct orderSlip *p,struct orderSlip a,int * num_items)
{
   if ( *num_items < MAX_ITEMS )
   {
      p[*num_items] = a;
      *num_items += 1;
   }
}

void delete_item(struct orderSlip *p,int *num_items, int item)
{
   if (*num_items > 0 && item < *num_items && item > -1)
   {
      int last_index = *num_items - 1;
      for (int i = item; i < last_index;i++)
      {
         p[i] = p[i + 1];
      }
      *num_items -= 1;
   }
}

void print(const struct orderSlip  *p){
	printf("%s\n", p->source);
}
void print_all(const struct orderSlip *p, int num_items){
	for(int i=0;i<num_items;i++){
		printf("%s\n", p->source);
		p++;
	}
}


int main()
{
	int length = 4;
	student_item sitem; /*bkin variable penampung dri struct tipe data student_item */
	student_item *tmp; /*bkin pointer tmp dari struct student_item */
	tmp = (student_item*) malloc(length+1 * sizeof(student_item));
	tmp = &sitem; /*memasukan alamat memory sitem ke pointer tmp, jdi si pointer tmp tau alamat memory sitem */

	student_item sitemx; /*bkin variable penampung dri struct tipe data student_item */
	student_item *tmpx; /*bkin pointer tmp dari struct student_item */
	tmpx = (student_item*) malloc(length+1 * sizeof(student_item));
	tmpx = &sitem; /*memasukan alamat memory sitem ke pointer tmp, jdi si pointer tmp tau alamat memory sitem */

	student_item *ptr;
	ptr = &sitem;

	// int assignid = 2;
	// int assignage = 24.1;
	// char assignname[50] = "fajar";
	// char assignadd[50] = "cigondewah rahayu";
	//
	// (tmp)->id = 5; /* mengisi pointer tmp-> dengan nilai 2. */
	// (*tmp).age = assignage; /* mengisi pointer (*tmp).age dengan nilai 24. */
	// strcpy(tmp->name, assignname); /* mengisi pointer (*tmp).age dengan fungsi strcpy karena string. */
	// strcpy((*tmp).address, assignadd);
	//
	//
	// printf("age : %f\n", (*tmp).age);
	// printf("id : %d\n", tmp->id);
	// printf("name : %s\n", tmp->name);
	// printf("address : %s\n", (*tmp).address);
	// for(int i=0;i<length;i++){
	// 	printf("%d\n", i);
	// 	tmp->id = i;
	// 	tmp++;
	// }
	//
	//
	// for(int i=0;i<length;i++){
	// 	printf("id : %d\n", tmpx->id);
	// 	tmpx++;
	// }


    MYSQL *db;
	db = mysql_init(NULL);

	if(mysql_real_connect(db, "localhost", "root", "password", "ppdb2017", 0, NULL, 0) == NULL){
		abort();
	}
	printf("connect db \n");
	MYSQL_RES *q;
	MYSQL_ROW row;
	MYSQL_FIELD *field;

	char table[15] = "elementary";

	//tmp++;
	char sql_select[550];

	mysql_query(db, "SELECT * FROM ppdb_registration_nonacademic");
	q = mysql_store_result(db);



  // status record;
  // record.id=1;
  // strcpy(record.name, "Raju");
  //record.type = 86;
	int num_fields = mysql_num_fields(q);
	printf("num_fields: %d\n", num_fields);
	int eweuh = 0;
	while((row = mysql_fetch_row(q)) != NULL){
		// for(int i=0;i<num_fields;i++){
		// 	// if (i==0){
		// 	// 	while (field=mysql_fetch_field(q)) {
		// 	// 		/* code */
		// 	// 		printf("name: %s\n", field->name);
		// 	// 	}
		// 	// 	printf("\n");
		// 	// }
		// 	printf("isi data : %s\n", row[i] ? row[i]:"NULL");
		// }
		printf("isi data : %s\n", row[1] ? row[1]:"NULL");
    tmp->id = row[0] ? atoi(row[0]):0;
    strcpy(tmp->name, row[1] ? row[1]:"NULL");
		tmp->age = row[5] ? atof(row[5]):0.0;

    // record.id = row[0][0];
    // record.type = row[0][1];
    // strcpy(record.name, row[0][2]);
    //printf("%c\n", row[0][1]);
		tmp++;
	}

printf("\n");


	//printf("xx:%d <->yyy\n", tmp->id);
	//printf("xx:%s <->yyy\n", tmp->name);
	//printf("eweuh euy!\n");
  //printf(" Id is: %d \n", tmp.id);
  // printf(" Name is: %s \n", record.name);
  // printf(" Percentage is: %d \n", record.type);
	mysql_free_result(q);




	mysql_close(db);
printf("Hello world!\n");

for (int i = 0; i < length; i++) {
	printf("%s\n", ptr->name);
	ptr++;
}


struct orderSlip data[MAX_ITEMS];
int num_items = 0;
struct orderSlip a;

printf("bawah:\n");

strcpy(a.source,"source 1");
strcpy(a.destination,"destination 10");
strcpy(a.item,"item xyz");
a.position = 99;
add_item(data,a,&num_items);

strcpy(a.source,"source 2");
strcpy(a.destination,"destination 20");
strcpy(a.item,"item ijk");
a.position = 100;
add_item(data,a,&num_items);

strcpy(a.source,"source 3");
strcpy(a.destination,"destination 30");
strcpy(a.item,"item ijk");
a.position = 101;
add_item(data,a,&num_items);

printf("%d\n", num_items);
print_all(data, num_items);

delete_item(data,&num_items,1);

print_all(data, num_items);


return 0;

}
