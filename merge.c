#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// employee record
struct employee {
  char gender[12];
  char name[12];
  int id;
  int score;
};

// swap employee records
void swap_employee(struct employee *left, struct employee *right) {
  struct employee tmp = *right;
  *right = *left;
  *left = tmp;
}

// compare employee records
int compare_employee(const struct employee *left,
                     const struct employee *right) {

  // printf("leftgen:%s , rightg:%s\n", left->name, right->name);
  // printf("leftgen:%d , rightg:%d\n", left->id, right->id);
  // int gender = strcmp(left->name, right->name);
  int gender = 0;
  int x;

  if (left->id > right->id) {
    gender = -1;
  } else if (left->id < right->id) {
    gender = 1;
  } else {

    if (left->score > right->score) {
      gender = -1;
    } else if (left->score < right->score) {
      gender = 1;
    }
    printf("return score \n");
    return (gender ? gender : (left->score - right->score));
  }

  // x = gender ? gender : (left->id - right->id);

  // printf("gender:%d\n", gender);
  // printf("leftid:%d - rightid:%d\n", left->id, right->id);

  // printf("x:%d\n", x);
  printf("return id \n");
  return (gender ? gender : (left->id - right->id));
  // return (gender ? gender : (left->id - right->id));
  // return x;
}

// quicksort for employees
static void quicksort_(struct employee *arr, int left, int right) {
  struct employee p = arr[(left + right) / 2]; // as good as any
  int l = left, r = right;                     // movable indicies

  while (l <= r) {
    while (compare_employee(arr + l, &p) < 0)
      ++l;
    while (compare_employee(arr + r, &p) > 0)
      --r;
    if (l <= r) {
      swap_employee(arr + l, arr + r);
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
void quicksort(struct employee *arr, int count) {
  // if (arr && (count > 0))
  //   quicksort_(arr, 0, count - 1, "score");

  // if (arr && (count > 0))

  quicksort_(arr, 0, count - 1);
  // quicksort_(arr, 0, count - 1, "score");
}

/* sample usage */
int main(int argc, char *argv[]) {
  struct employee arr[] = {{"d", "aa", 1, 7},  {"e", "vb", 5, 2},
                           {"j", "cc", 7, 4},  {"a", "dd", 5, 9},
                           {"z", "ee", 5, 12}, {"z", "ee", 5, 8}};

  quicksort(arr, sizeof(arr) / sizeof(arr[0]));
  printf("sizeof: %zu\n", sizeof(arr));
  printf("detail: %zu\n", sizeof(arr[0]));
  for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); ++i)
    printf("%s, %s, %d, %d\n", arr[i].gender, arr[i].name, arr[i].id,
           arr[i].score);

  return EXIT_SUCCESS;
}
