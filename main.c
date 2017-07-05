#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdint.h>
#include <math.h>
#include <mysql/mysql.h>
#include <mysql/my_global.h>
#include "cJSON-master/cJSON.h"

struct student_item {
  int student_id;
  char student_name[300];
int is_foreigner;

  int first_choice;
  int second_choice;

  int accepted_option;
  int accepted_status;

  float score_bahasa;
  float score_english;
  float score_math;
  float score_physics;

  float score_total1;
  float score_total2;

  float score_range1;
  float range1;

  float score_range2;
  float range2;
};

struct school_item {
  int school_id;
  char school_name[200];
  int quota;
  int count_students;
  struct student_item *datas;
  int filtered;
};

struct student_item *student_table;
struct student_item *tmp;

void add_item_student(struct school_item *s, struct school_item *outerReference,
                      int option_idx, struct student_item *p,
                      struct student_item a, int max_items, int *num_items) {

                      printf("num_items:%d < max_items:%d \n", *num_items, max_items);
  if (*num_items < max_items) {

    // outerReference = s + *num_items;
    // printf("num_items1: %d\n", *num_items);

    outerReference[option_idx].datas =
        realloc(outerReference[option_idx].datas,
                (*num_items + 1) *
                    sizeof(num_items, *outerReference[option_idx].datas));
    printf("num_items1: %d\n", *num_items);
    printf("target:%d <-> dijero>%d <-> %d <-> %d\n", option_idx, a.student_id,
           *num_items, max_items);
    // printf("%d\n", option_idx);
    s[option_idx].datas[*num_items] = a;
    *num_items += 1;
  }
}

void delete_item_student(struct student_item *p, int item, int *num_items) {
  printf("%d\n", *num_items);
  if (*num_items > 0 && item < *num_items && item > -1) {

    int last_index = *num_items;
    printf("item:%d - lastidx:%d\n", item, last_index);
    for (int i = item; i < last_index; i++) {
      p[i] = p[i + 1];
    }
    *num_items -= 1;
  }
}

// swap employee records
void swap_students(struct student_item *left, struct student_item *right) {
  struct student_item tmp = *right;
  *right = *left;
  *left = tmp;
}

// compare employee records
int compare_students(const struct student_item *left,
                     const struct student_item *right) {
  // int gender = strcmp(left->gender, right->gender);
  int x;
  if (left->score_total1 > right->score_total1) {
    x = -1;
  } else if (left->score_total1 < right->score_total1) {
    x = 1;
  } else {
    x = 0;

    if (left->score_bahasa > right->score_bahasa) {
      x = -1;
    } else if (left->score_bahasa < right->score_bahasa) {
      x = 1;
    } else {
      if (left->score_english > right->score_english) {
        x = -1;
      } else if (left->score_english < right->score_english) {
        x = 1;
      }
    }
  }
  return x;
}

// quicksort for student_item
static void quicksort_(struct student_item *arr, int left, int right) {
  struct student_item p = arr[(left + right) / 2]; // as good as any
  int l = left, r = right;                         // movable indicies

  while (l <= r) {
    while (compare_students(arr + l, &p) < 0)
      ++l;
    while (compare_students(arr + r, &p) > 0)
      --r;
    if (l <= r) {
      swap_students(arr + l, arr + r);
      ++l;
      --r;
    }
  }

  if (left < r)
    quicksort_(arr, left, r);
  if (l < right)
    quicksort_(arr, l, right);
}

// exposed API
void quicksort(struct student_item *arr, int count) {
  // count = count / 2;
  // count = 15;
  if (arr && (count > 0))
    quicksort_(arr, 0, count - 1);
}

int get_index_school(struct school_item *schools, int count_schools,
                     int target_option) {
  int idx_target = -1;
  for (int school_idx = 0; school_idx < count_schools; school_idx++) {
    if (schools[school_idx].school_id == target_option) {
      idx_target = school_idx;
      break;
    }
  }
  return idx_target;
}

int cut_students_foreigner(struct school_item *schools, int count_schools,
                        struct student_item *students, int count_students,
                        int quota, int option_idx, int status_foreigner){

  struct school_item tempsch;
  struct student_item tempstd;

  int target_count;
  int target_option;
  int target_quota;
  int idx_target;


  int count_foreigner = 0;
  int quota_foreigner = 0;
  int school_id = schools[option_idx].school_id;

  quota_foreigner = floor(quota * 0.025);
  if (school_id==999999){
    quota_foreigner = 999999;
  }

  printf("school_id:%d <-> count_students:%d <->quota_foreigner: %d\n",school_id, count_students, quota_foreigner);
  for (int student_idx = 0; student_idx < count_students - 1; student_idx++) {

    printf("before:student_id:%d <->is_foreigner:%d <->sec_choice:%d <->%s "
           "<->accepted_status:%d\n", students[student_idx].student_id,students[student_idx].is_foreigner, students[student_idx].second_choice, students[student_idx].student_name, students[student_idx].accepted_status);

         if (students[student_idx].is_foreigner==1){
            count_foreigner++;

           if (count_foreigner > quota_foreigner){

             tempstd.student_id = students[student_idx].student_id;
             strcpy(tempstd.student_name, students[student_idx].student_name);
             tempstd.is_foreigner = students[student_idx].is_foreigner;
             tempstd.first_choice = students[student_idx].first_choice;
             tempstd.second_choice = students[student_idx].second_choice;
             tempstd.score_total1 = students[student_idx].score_total1;
             tempstd.score_total2 = students[student_idx].score_total2;
             tempstd.score_bahasa = students[student_idx].score_bahasa;



             // jika status sudah lebih dari 2 maka, lempat ke trash
             if (students[student_idx].accepted_status >= 2) {
               printf("xif_foreigner\n");
               target_option = 999999;
               tempstd.accepted_option = target_option;
               tempstd.accepted_status = 3;
               idx_target = get_index_school(schools, count_schools, target_option);
             } else {
               printf("xelse_foreigner\n");
               target_option = students[student_idx].second_choice;
               idx_target = get_index_school(schools, count_schools, target_option);
               if (idx_target == -1) {
                 target_option = 999999;
                 tempstd.accepted_option = target_option;
                 tempstd.accepted_status = 3;
                 idx_target = get_index_school(schools, count_schools, target_option);
                 printf("xelse1__foreigner\n");
               } else {
                 tempstd.accepted_option = target_option;
                 tempstd.accepted_status = 2;
                 schools[idx_target].filtered = 0;
                 status_foreigner = 0;
                 printf("xelse2_foreigner\n");
               } //end if idx target
             }//end if accepted_status




             target_count = schools[idx_target].count_students;
             target_quota = floor(schools[idx_target].quota * 0.025);
             if (target_option==999999){
               target_quota = 999999;
             }

             printf("after:student_id:%d <->sec_choice:%d <-> idx_target:%d <-> "
                    "target_count:%d<->%s "
                    "<->accepted_status:%d<->target_quota_foreigner:%d\n",
                    students[student_idx].student_id, target_option, idx_target,
                    target_count, tempstd.student_name, tempstd.accepted_status, target_quota);

             add_item_student(schools, schools, idx_target, students, tempstd,
                              target_quota, &target_count);
             delete_item_student(students, student_idx, &count_students);
             //count_students--;

             schools[idx_target].count_students = target_count;
             student_idx--;
           }//end if check count_foreigner > quota_foreigner





         }//end if is foreigner


  } //end for

  schools[option_idx].count_students = count_students;


	return status_foreigner;
}

int cut_option_under_pg(struct school_item *schools, int count_schools,
                        struct student_item *students, int count_students,
                        int quota, int option_idx, int status) {
  struct school_item tempsch;
  struct student_item tempstd;

  int target_count;
  int target_option;
  int idx_target;

  printf("count_students:%d <->quota: %d\n", count_students, quota);
  int target_quota = schools[option_idx].quota;
  for (int student_idx = count_students - 1; student_idx > quota - 1;
       student_idx--) {
    /* code */
    // printf("student_idx:%s\n", students[student_idx].student_name);
    // tempsch.datas[0].student_name = students[student_idx].student_name;
    // tempsch.datas[0].student_id = students[student_idx].student_id;

    tempstd.student_id = students[student_idx].student_id;
    strcpy(tempstd.student_name, students[student_idx].student_name);
    tempstd.is_foreigner = students[student_idx].is_foreigner;
    tempstd.first_choice = students[student_idx].first_choice;
    tempstd.second_choice = students[student_idx].second_choice;
    tempstd.score_total1 = students[student_idx].score_total1;
    tempstd.score_total2 = students[student_idx].score_total2;
    tempstd.score_bahasa = students[student_idx].score_bahasa;

    // printf("before:student_id:%d <->sec_choice:%d <-> idx_target:%d <->%s "
    //        "<->accepted_status:%d\n",
    //        students[student_idx].student_id,
    //        students[student_idx].second_choice, idx_target,
    //        students[student_idx].student_name,
    //        students[student_idx].accepted_status);

    // jika status sudah lebih dari 2 maka, lempat ke trash
    if (students[student_idx].accepted_status >= 2) {
      printf("if\n");
      target_option = 999999;
      tempstd.accepted_option = target_option;
      tempstd.accepted_status = 3;
      idx_target = get_index_school(schools, count_schools, target_option);
    } else {
      printf("else1\n");
      target_option = students[student_idx].second_choice;
      idx_target = get_index_school(schools, count_schools, target_option);
      if (idx_target == -1) {
        target_option = 999999;
        tempstd.accepted_option = target_option;
        tempstd.accepted_status = 3;
        idx_target = get_index_school(schools, count_schools, target_option);
        printf("else1\n");
      } else {
        tempstd.accepted_option = target_option;
        tempstd.accepted_status = 2;
        schools[idx_target].filtered = 0;
        status = 0;
        printf("else2\n");
      }
    }

    // printf("target_option:%d\n", target_option);

    target_count = schools[idx_target].count_students;



    add_item_student(schools, schools, idx_target, students, tempstd,
                     target_quota, &target_count);
    delete_item_student(students, student_idx, &count_students);
    schools[idx_target].count_students = target_count;

    // printf("target_count:%d\n", target_count);
    // printf("count_students:%d\n", count_students);
  }

  schools[option_idx].count_students = count_students;
  // add_item_school(schools, tempsch, count_schools, &num_schools);

  // delete_item_student(students, 1, &count_students);
  return status;
}

int do_selection(struct school_item *schools_data, int count_schools) {
  int status = 1; int status_foreigner=1;

  for (int option_idx = 0; option_idx < count_schools; option_idx++) {
    // printf("sizeof: %zu\n", sizeof(schools_data[option_idx].datas));
    // printf("detail: %zu\n", sizeof(schools_data[option_idx].datas[0]));
    if (schools_data[option_idx].filtered == 0) {
      quicksort(schools_data[option_idx].datas,
                schools_data[option_idx].count_students);

      int quota = schools_data[option_idx].quota;
      int count_students = schools_data[option_idx].count_students;

      printf("school_id:%d<->", schools_data[option_idx].school_id);
        status_foreigner = cut_students_foreigner(schools_data, count_schools,
                                     schools_data[option_idx].datas,
                                     count_students, quota, option_idx, status_foreigner);
      quota = schools_data[option_idx].quota;
      count_students = schools_data[option_idx].count_students;

      if (count_students > quota) {
        // potong data siswa
        printf("count_students:%d > quota:%d\n", count_students, quota);
        status = cut_option_under_pg(schools_data, count_schools,
                                     schools_data[option_idx].datas,
                                     count_students, quota, option_idx, status);
        schools_data[option_idx].filtered = 1;

      } // end if count student > quota
      printf("\n");
    } // end if filtered
  }   // end for

  if ((status == 0) || (status_foreigner==0)) {
    do_selection(schools_data, count_schools);
  }
}

char* join_strings(char* strings[], char* seperator, int count) {
    char* str = NULL;             /* Pointer to the joined strings  */
    size_t total_length = 0;      /* Total length of joined strings */
    int i = 0;                    /* Loop counter                   */

    /* Find total length of joined strings */
    for (i = 0; i < count; i++) total_length += strlen(strings[i]);
    total_length++;     /* For joined string terminator */
    total_length += strlen(seperator) * (count - 1); // for seperators

    str = (char*) malloc(total_length);  /* Allocate memory for joined strings */
    str[0] = '\0';                      /* Empty string we can append to      */

    /* Append all the strings */
    for (i = 0; i < count; i++) {
        strcat(str, strings[i]);
        if (i < (count - 1)) strcat(str, seperator);
    }

    return str;
}

void finish_with_error(MYSQL *db)
{
  fprintf(stderr, "%s\n", mysql_error(db));
  mysql_close(db);
  exit(1);
}

void insert_filtered(MYSQL *db, struct school_item *outerArray, int max_items){
  if (mysql_query(db, "TRUNCATE ppdb_filtered_academic")) {
    finish_with_error(db);
  }
  for (int i = 0; i < max_items; i++) {
    char *sql_join[1500];
    //outerArray[i].school_id
    for (int j = 0; j < outerArray[i].count_students; j++) {
      //outerArray[i].datas[j].student_id
      char sql_insert_student[300];
      sprintf(sql_insert_student,"INSERT INTO ppdb_filtered_academic (`option`, `registration`, `is_foreigner`) VALUES (%d,%d,%d);", outerArray[i].datas[j].accepted_option, outerArray[i].datas[j].student_id, outerArray[i].datas[j].is_foreigner);
      //mysql_query(db, sql_insert_student);
      //sql_join[j]=sql_insert_student;
      printf("insert_filtered:%s\n", sql_insert_student);
      if (mysql_query(db, sql_insert_student)) {
        finish_with_error(db);
      }

    }//end for students

  }//end for schools
}



/* Create a bunch of objects as demonstration. */
static int print_preallocated(cJSON *root) {
  /* declarations */
  char *out = NULL;
  char *buf = NULL;
  char *buf_fail = NULL;
  size_t len = 0;
  size_t len_fail = 0;

  /* formatted print */
  out = cJSON_Print(root);

  /* create buffer to succeed */
  /* the extra 5 bytes are because of inaccuracies when reserving memory */
  len = strlen(out) + 5;
  buf = (char *)malloc(len);
  if (buf == NULL) {
    printf("Failed to allocate memory.\n");
    exit(1);
  }

  /* create buffer to fail */
  len_fail = strlen(out);
  buf_fail = (char *)malloc(len_fail);
  if (buf_fail == NULL) {
    printf("Failed to allocate memory.\n");
    exit(1);
  }

  /* Print to buffer */
  if (!cJSON_PrintPreallocated(root, buf, (int)len, 1)) {
    printf("cJSON_PrintPreallocated failed!\n");
    if (strcmp(out, buf) != 0) {
      printf("cJSON_PrintPreallocated not the same as cJSON_Print!\n");
      printf("cJSON_Print result:\n%s\n", out);
      printf("cJSON_PrintPreallocated result:\n%s\n", buf);
    }
    free(out);
    free(buf_fail);
    free(buf);
    return -1;
  }

  /* success */
  printf("%s\n", buf);
  FILE *opening;
  opening = fopen("res.json", "w+");
  fprintf(opening, "%s", buf);
  fclose(opening);

  /* force it to fail */
  if (cJSON_PrintPreallocated(root, buf_fail, (int)len_fail, 1)) {
    printf("cJSON_PrintPreallocated failed to show error with insufficient "
           "memory!\n");
    printf("cJSON_Print result:\n%s\n", out);
    printf("cJSON_PrintPreallocated result:\n%s\n", buf_fail);
    free(out);
    free(buf_fail);
    free(buf);
    return -1;
  }

  free(out);
  free(buf_fail);
  free(buf);
  return 0;
}

void display_data(struct school_item *outerArray, int max_items) {

  cJSON *root = NULL;
  cJSON *fmt = NULL;
  cJSON *img = NULL;
  cJSON *thm = NULL;
  cJSON *fld = NULL;
  int i = 0;
  root = cJSON_CreateArray();
  for (i = 0; i < max_items; i++) {
    // printf("%d\n", outerArray[i].school_id);
    cJSON_AddItemToArray(root, fld = cJSON_CreateObject());
    cJSON_AddNumberToObject(fld, "school_id", outerArray[i].school_id);
    cJSON_AddStringToObject(fld, "school_name", outerArray[i].school_name);
    cJSON_AddNumberToObject(fld, "quota", outerArray[i].quota);
    cJSON_AddNumberToObject(fld, "count_students",
                            outerArray[i].count_students);

    cJSON *branch = NULL;
    cJSON *bfield = NULL;
    branch = cJSON_CreateArray();

    for (int j = 0; j < outerArray[i].count_students; j++) {
      cJSON_AddItemToArray(branch, bfield = cJSON_CreateObject());

      cJSON_AddNumberToObject(bfield, "student_id",
                              outerArray[i].datas[j].student_id);
      cJSON_AddStringToObject(bfield, "student_name",
                              outerArray[i].datas[j].student_name);
      cJSON_AddNumberToObject(bfield, "is_foreigner",
                              outerArray[i].datas[j].is_foreigner);

      cJSON_AddNumberToObject(bfield, "score_total1",
                              outerArray[i].datas[j].score_total1);
      cJSON_AddNumberToObject(bfield, "score_total2",
                              outerArray[i].datas[j].score_total2);
      cJSON_AddNumberToObject(bfield, "score_bahasa",
                              outerArray[i].datas[j].score_bahasa);
      cJSON_AddNumberToObject(bfield, "score_english",
                              outerArray[i].datas[j].score_english);

      cJSON_AddNumberToObject(bfield, "first_choice",
                              outerArray[i].datas[j].first_choice);
      cJSON_AddNumberToObject(bfield, "second_choice",
                              outerArray[i].datas[j].second_choice);

      cJSON_AddNumberToObject(bfield, "accepted_option",
                              outerArray[i].datas[j].accepted_option);
      cJSON_AddNumberToObject(bfield, "accepted_status",
                              outerArray[i].datas[j].accepted_status);
    }

    cJSON_AddItemToObject(fld, "students", branch);
  }

  if (print_preallocated(root) != 0) {
    cJSON_Delete(root);
    exit(EXIT_FAILURE);
  }
  cJSON_Delete(root);
}

float safeAtof(char *str) {
  if (str == NULL)
    return 0;

  if (strlen(str) == 0)
    return 0;

  return atof(str);
}

int safeAtoi(char *str) {
  if (str == NULL)
    return 0;

  if (strlen(str) == 0)
    return 0;

  return atoi(str);
}

int main() {
  int length = 4;
  struct student_item
      sitem; /*bkin variable penampung dri struct tipe data student_item */
  struct student_item *tmp; /*bkin pointer tmp dari struct student_item */
  tmp = (struct student_item *)malloc(length + 1 * sizeof(struct student_item));
  tmp = &sitem; /*memasukan alamat memory sitem ke pointer tmp, jdi si pointer
                   tmp tau alamat memory sitem */

  struct student_item *ptr;
  ptr = &sitem;

  MYSQL *db;
  db = mysql_init(NULL);

  if (mysql_real_connect(db, "localhost", "root", "password",
                         "jabar-dev", 0, NULL, 0) == NULL) {
    abort();
  }
  printf("connect db \n");
  MYSQL_RES *q_schools, *q_students;
  MYSQL_ROW row_schools, row_students;
  MYSQL_FIELD *field;
  char sql_select_schools[300];
  sprintf(sql_select_schools,
          "SELECT distinct(o.id),s.code,o.school, "
          "o.type,o.quota,o.old_quota,o.priority, "
          "s.name as school_name,o.name as "
          "option_name,s.is_border,substring(s.code,1,1) as "
          "code,s.foreigner_percentage FROM ppdb_school s INNER JOIN "
          "ppdb_option o ON ( s.id = o.school ) where "
          "(substring(s.code,1,1)=4) and o.type='academic'  and o.id in (1,2,3,4,5)"
          "ORDER BY s.code, "
          "o.id");

  // sprintf(sql_select_schools,
  //         "SELECT distinct(o.id),s.code,o.school, "
  //         "o.type,o.quota,o.old_quota,o.priority, "
  //         "s.name as school_name,o.name as "
  //         "option_name,s.is_border,substring(s.code,1,1) as "
  //         "code,s.foreigner_percentage FROM ppdb_school s INNER JOIN "
  //         "ppdb_option o ON ( s.id = o.school ) where "
  //         "(substring(s.code,1,1)=4) and o.type='academic' and o.id in "
  //         "(1,2,3,4,5) ORDER BY s.code, "
  //         "o.id");
  mysql_query(db, sql_select_schools);

  q_schools = mysql_store_result(db);
  int num_fields_schools = mysql_num_fields(q_schools);

  int count_schools = mysql_num_rows(q_schools);
  count_schools++;

  struct school_item schoolitem;
  struct school_item *pschool;
  struct school_item schools_data[count_schools];
  pschool = &schoolitem;
  int num_schools = 0;
  struct school_item *outerArray;
  struct school_item *outerReference;
  outerArray = malloc(count_schools * sizeof(*outerArray));

  while ((row_schools = mysql_fetch_row(q_schools)) != NULL) {

    char sql_select[300];
    sprintf(sql_select,
            "SELECT "
            "id,type,registration,name,score_bahasa,score_english,"
            "score_math,score_physics,score_total1,score_total2,range1,range2,"
            "score_range1,score_range2,first_choice,second_choice,status,is_foreigner FROM "
            "ppdb_registration_academic o "
            "where "
            "o.type = 'academic' and o.first_choice=%d and status = "
            "'approved' ",
            atoi(row_schools[0]));
    printf("%s\n", sql_select);
    mysql_query(db, sql_select);
    q_students = mysql_store_result(db);
    int num_fields_students = mysql_num_fields(q_students);
    int count_students = mysql_num_rows(q_students);

    outerReference = outerArray + num_schools;
    outerReference->datas =
        malloc((count_students + 1) * sizeof(*outerReference->datas));

    outerArray[num_schools].school_id =
        row_schools[0] ? safeAtoi(row_schools[0]) : 0;
    strcpy(outerArray[num_schools].school_name,
           row_schools[8] ? row_schools[8] : "NULL");
    outerArray[num_schools].quota =
        row_schools[4] ? safeAtoi(row_schools[4]) : 0;
    outerArray[num_schools].count_students = count_students;

    int num_students = 0;
    while ((row_students = mysql_fetch_row(q_students)) != NULL) {

      outerArray[num_schools].datas[num_students].student_id =
          safeAtoi(row_students[0]);
      strcpy(outerArray[num_schools].datas[num_students].student_name,
             row_students[3]);
      outerArray[num_schools].datas[num_students].is_foreigner =
          safeAtoi(row_students[17]);

      outerArray[num_schools].datas[num_students].score_total1 =
          safeAtoi(row_students[8]);
      outerArray[num_schools].datas[num_students].score_total2 =
          safeAtoi(row_students[9]);

      outerArray[num_schools].datas[num_students].score_bahasa =
          safeAtoi(row_students[4]);
      outerArray[num_schools].datas[num_students].score_english =
          safeAtoi(row_students[5]);
      outerArray[num_schools].datas[num_students].score_math =
          safeAtoi(row_students[6]);
      outerArray[num_schools].datas[num_students].score_physics =
          safeAtoi(row_students[7]);

      outerArray[num_schools].datas[num_students].score_range1 =
          safeAtoi(row_students[10]);
      outerArray[num_schools].datas[num_students].score_range2 =
          safeAtoi(row_students[11]);

      outerArray[num_schools].datas[num_students].range1 =
          safeAtoi(row_students[12]);
      outerArray[num_schools].datas[num_students].range2 =
          safeAtoi(row_students[13]);

      outerArray[num_schools].datas[num_students].first_choice =
          safeAtoi(row_students[14]);
      outerArray[num_schools].datas[num_students].second_choice =
          safeAtoi(row_students[15]);

      outerArray[num_schools].datas[num_students].accepted_option =
          outerArray[num_schools].datas[num_students].first_choice;
      outerArray[num_schools].datas[num_students].accepted_status = 1;

      num_students++;
    }
    mysql_free_result(q_students);
    num_schools++;
  }
  mysql_free_result(q_schools);


  // add trash school
  outerReference = outerArray + num_schools;
  outerReference->datas = malloc(0 * sizeof(*outerReference->datas));

  outerArray[num_schools].school_id = 999999;
  strcpy(outerArray[num_schools].school_name, "trash");
  outerArray[num_schools].quota = 999999;
  outerArray[num_schools].count_students = 0;

  printf("\n");

  //display_data(outerArray, count_schools);
  do_selection(outerArray, count_schools);
  insert_filtered(db,outerArray, count_schools);
  mysql_close(db);
  display_data(outerArray, count_schools);

  return 0;
}
