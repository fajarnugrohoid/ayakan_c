#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdint.h>
#include <mysql/mysql.h>
#include <mysql/my_global.h>
#include "cJSON-master/cJSON.h"

// typedef struct
// {
//   int id;
//   char name[100];
//   int type;
// } status;

struct student_item {
  int student_id;
  char student_name[300];

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
  struct student_item datas[1];
  int filtered;
};

struct student_item *student_table;
struct student_item *tmp;

// global
int num_items = 0;

// struct orderSlip data[100];
void add_item_school(struct school_item *p, struct school_item a, int max_items,
                     int *num_items) {
  if (*num_items < max_items) {
    p[*num_items] = a;
    *num_items += 1;
  }
}

void delete_item_school(struct school_item *p, int *num_items, int item) {
  if (*num_items > 0 && item < *num_items && item > -1) {
    int last_index = *num_items - 1;
    for (int i = item; i < last_index; i++) {
      p[i] = p[i + 1];
    }
    *num_items -= 1;
  }
}

// void add_item_student(struct student_item *p, struct student_item a,
//                       int max_items, int *num_items) {
//   if (*num_items < max_items) {
//     p[*num_items] = a;
//     *num_items += 1;
//   }
// }
void add_item_student(struct school_item *s, int option_idx,
                      struct student_item *p, struct student_item a,
                      int max_items, int *num_items) {

  if (*num_items < max_items) {
    printf("dijero>%d <-> %d <-> %d\n", a.student_id, *num_items, max_items);
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

void print(const struct student_item *p) { printf("%s\n", p->student_name); }
void print_all(const struct student_item *p, int num_items) {
  for (int i = 0; i < num_items; i++) {
    printf("%s\n", p->student_name);
    p++;
  }
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

void display_data(struct school_item schools_data[], int max_items) {
  cJSON *root = NULL;
  cJSON *fmt = NULL;
  cJSON *img = NULL;
  cJSON *thm = NULL;
  cJSON *fld = NULL;
  int i = 0;
  root = cJSON_CreateArray();
  for (i = 0; i < max_items; i++) {
    printf("%d\n", schools_data[i].school_id);
    cJSON_AddItemToArray(root, fld = cJSON_CreateObject());
    cJSON_AddNumberToObject(fld, "school_id", schools_data[i].school_id);
    cJSON_AddStringToObject(fld, "school_name", schools_data[i].school_name);
    cJSON_AddNumberToObject(fld, "quota", schools_data[i].quota);
    cJSON_AddNumberToObject(fld, "count_students",
                            schools_data[i].count_students);

    cJSON *branch = NULL;
    cJSON *bfield = NULL;
    branch = cJSON_CreateArray();
    for (int j = 0; j < schools_data[i].count_students; j++) {
      cJSON_AddItemToArray(branch, bfield = cJSON_CreateObject());
      cJSON_AddNumberToObject(bfield, "student_id",
                              schools_data[i].datas[j].student_id);
      cJSON_AddStringToObject(bfield, "student_name",
                              schools_data[i].datas[j].student_name);

      cJSON_AddNumberToObject(bfield, "score_total1",
                              schools_data[i].datas[j].score_total1);
      cJSON_AddNumberToObject(bfield, "score_total2",
                              schools_data[i].datas[j].score_total2);
      cJSON_AddNumberToObject(bfield, "score_bahasa",
                              schools_data[i].datas[j].score_bahasa);
      cJSON_AddNumberToObject(bfield, "score_english",
                              schools_data[i].datas[j].score_english);

      cJSON_AddNumberToObject(bfield, "first_choice",
                              schools_data[i].datas[j].first_choice);
      cJSON_AddNumberToObject(bfield, "second_choice",
                              schools_data[i].datas[j].second_choice);

      cJSON_AddNumberToObject(bfield, "accepted_option",
                              schools_data[i].datas[j].accepted_option);
      cJSON_AddNumberToObject(bfield, "accepted_status",
                              schools_data[i].datas[j].accepted_status);
    }

    cJSON_AddItemToObject(fld, "students", branch);
  }

  if (print_preallocated(root) != 0) {
    cJSON_Delete(root);
    exit(EXIT_FAILURE);
  }
  cJSON_Delete(root);
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
  int idx_target = 0;
  for (int school_idx = 0; school_idx < count_schools; school_idx++) {
    if (schools[school_idx].school_id == target_option) {
      idx_target = school_idx;
      break;
    }
  }
  return idx_target;
}

int cut_option_under_pg(struct school_item *schools, int count_schools,
                        struct student_item *students, int count_students,
                        int quota, int option_idx, int status) {
  struct school_item tempsch;
  struct student_item tempstd;

  int target_count;
  int target_option;
  int idx_target;

  int target_quota = schools[option_idx].quota;
  for (int student_idx = count_students - 1; student_idx > quota - 1;
       student_idx--) {
    /* code */
    // printf("student_idx:%s\n", students[student_idx].student_name);
    // tempsch.datas[0].student_name = students[student_idx].student_name;
    // tempsch.datas[0].student_id = students[student_idx].student_id;
    printf("before:student_id:%d <->sec_choice:%d <-> idx_target:%d <->%s "
           "<->accepted_status:%d\n",
           students[student_idx].student_id, target_option, idx_target,
           students[student_idx].student_name,
           students[student_idx].accepted_status);

    tempstd.student_id = students[student_idx].student_id;
    strcpy(tempstd.student_name, students[student_idx].student_name);
    tempstd.first_choice = students[student_idx].first_choice;
    tempstd.second_choice = students[student_idx].second_choice;
    tempstd.score_total1 = students[student_idx].score_total1;
    tempstd.score_total2 = students[student_idx].score_total2;
    tempstd.score_bahasa = students[student_idx].score_bahasa;

    // jika status sudah lebih dari 2 maka, lempat ke trash
    if (students[student_idx].accepted_status >= 2) {
      target_option = 999999;
      tempstd.accepted_option = target_option;
      tempstd.accepted_status = 3;

    } else {
      target_option = students[student_idx].second_choice;
      tempstd.accepted_option = target_option;
      tempstd.accepted_status = 2;
      schools[idx_target].filtered = 0;
      status = 0;
    }

    idx_target = get_index_school(schools, count_schools, target_option);
    target_count = schools[idx_target].count_students;
    printf("after:student_id:%d <->sec_choice:%d <-> idx_target:%d <->%s "
           "<->accepted_status:%d\n",
           students[student_idx].student_id, target_option, idx_target,
           tempstd.student_name, tempstd.accepted_status);
    add_item_student(schools, idx_target, students, tempstd, target_quota,
                     &target_count);
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
  int status = 1;

  for (int option_idx = 0; option_idx < count_schools; option_idx++) {
    // printf("sizeof: %zu\n", sizeof(schools_data[option_idx].datas));
    // printf("detail: %zu\n", sizeof(schools_data[option_idx].datas[0]));
    if (schools_data[option_idx].filtered == 0) {
      quicksort(schools_data[option_idx].datas,
                schools_data[option_idx].count_students);

      int quota = schools_data[option_idx].quota;
      int count_students = schools_data[option_idx].count_students;

      printf("school_id:%d<->", schools_data[option_idx].school_id);

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

  if (status == 0) {
    do_selection(schools_data, count_schools);
  }
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
                         "ppdb2017-jabar-dev", 0, NULL, 0) == NULL) {
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
          "(substring(s.code,1,1)=4) and o.type='rmp' and o.id in "
          "(5135,4137,4632,4453,4404) ORDER BY s.code, "
          "o.id");
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

  while ((row_schools = mysql_fetch_row(q_schools)) != NULL) {
    schoolitem.school_id = row_schools[0] ? atoi(row_schools[0]) : 0;
    strcpy(schoolitem.school_name, row_schools[8] ? row_schools[8] : "NULL");
    schoolitem.quota = row_schools[4] ? atoi(row_schools[4]) : 0;
    schoolitem.filtered = 0;

    char sql_select[300];
    sprintf(sql_select,
            "SELECT "
            "id,type,registration,name,score_bahasa,score_english,"
            "score_math,score_physics,score_total1,score_total2,range1,range2,"
            "score_range1,score_range2,first_choice,second_choice,status FROM "
            "ppdb_registration_nonacademic o "
            "where "
            "o.type = 'rmp' and o.first_choice=%d and status = "
            "'approved' ",
            atoi(row_schools[0]));
    printf("%s\n", sql_select);
    mysql_query(db, sql_select);
    q_students = mysql_store_result(db);
    int num_fields_students = mysql_num_fields(q_students);
    int max_students = sizeof(struct student_item);
    struct student_item students_data[max_students];

    int num_students = 0;
    while ((row_students = mysql_fetch_row(q_students)) != NULL) {
      // sitem.student_id = row_students[0] ? atoi(row_students[0]) : 0;
      // strcpy(sitem.student_name, row_students[9] ? row_students[9] : "NULL");

      schoolitem.datas[num_students].student_id =
          row_students[0] ? safeAtoi(row_students[0]) : 0;

      strcpy(schoolitem.datas[num_students].student_name,
             row_students[3] ? row_students[3] : "NULL");

      schoolitem.datas[num_students].score_total1 =
          row_students[8] ? safeAtof(row_students[8]) : 0.0;

      schoolitem.datas[num_students].score_total2 =
          row_students[9] ? safeAtof(row_students[9]) : 0;

      schoolitem.datas[num_students].score_bahasa =
          row_students[4] ? safeAtof(row_students[4]) : 0;
      schoolitem.datas[num_students].score_english =
          row_students[5] ? safeAtof(row_students[5]) : 0;
      schoolitem.datas[num_students].score_math =
          row_students[6] ? safeAtof(row_students[6]) : 0;
      schoolitem.datas[num_students].score_physics =
          row_students[7] ? safeAtof(row_students[7]) : 0;

      schoolitem.datas[num_students].score_range1 =
          row_students[10] ? safeAtof(row_students[10]) : 0;

      schoolitem.datas[num_students].score_range2 =
          row_students[11] ? safeAtof(row_students[11]) : 0;

      schoolitem.datas[num_students].range1 =
          row_students[12] ? safeAtof(row_students[12]) : 0;
      schoolitem.datas[num_students].range2 =
          row_students[13] ? safeAtof(row_students[13]) : 0;

      schoolitem.datas[num_students].first_choice =
          row_students[14] ? safeAtoi(row_students[14]) : 0;
      schoolitem.datas[num_students].second_choice =
          row_students[15] ? safeAtoi(row_students[15]) : 0;

      schoolitem.datas[num_students].accepted_option =
          schoolitem.datas[num_students].first_choice;
      schoolitem.datas[num_students].accepted_status = 1;
      num_students++;
      // schoolitem.datas = sitem;
      // add_item_student(students_data, sitem, count_schools, &num_students);
    }
    mysql_free_result(q_students);

    schoolitem.count_students = num_students;

    // printf("%s\n", row_schools[8]);
    add_item_school(schools_data, schoolitem, count_schools, &num_schools);
  }
  mysql_free_result(q_schools);

  // add trash school
  schoolitem.school_id = 999999;
  strcpy(schoolitem.school_name, "trash");
  schoolitem.quota = 0;
  schoolitem.filtered = 1;
  schoolitem.count_students = 0;

  add_item_school(schools_data, schoolitem, count_schools, &num_schools);

  printf("\n");

  mysql_close(db);
  // print_all(data, num_items);
  display_data(schools_data, count_schools);
  // do_selection(schools_data, count_schools);
  // display_data(schools_data, count_schools);

  return 0;
}
