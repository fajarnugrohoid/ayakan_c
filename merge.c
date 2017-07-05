#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// employee record
struct employee
{
    char gender[12];
    char name[12];
    int id;
};

// swap employee records
void swap_employee(struct employee *left, struct employee *right)
{
    struct employee tmp = *right;
    *right = *left;
    *left = tmp;
}

// compare employee records
int compare_employee(const struct employee* left,
                     const struct employee* right)
{
    int gender = strcmp(left->gender, right->gender);
    return (gender ? gender : (left->id - right->id));
}

// quicksort for employees
static void quicksort_(struct employee *arr, int left, int right)
{
    struct employee p = arr[(left+right)/2];    // as good as any
    int l = left, r = right;   // movable indicies

    while (l <= r)
    {
        while (compare_employee(arr+l, &p) < 0)
            ++l;
        while (compare_employee(arr+r, &p) > 0)
            --r;
        if (l <= r)
        {
            swap_employee(arr+l, arr+r);
            ++l; --r;
        }
    }

    if (left < r)
        quicksort_(arr, left, r);
    if (l < right)
        quicksort_(arr, l, right);
}

// exposed API
void quicksort(struct employee *arr, int count)
{
    if (arr && (count>0))
        quicksort_(arr, 0, count-1);
}

/* sample usage */
int main(int argc, char *argv[])
{
    struct employee arr[]=
    {
        {"male","Matt",1234},
        {"female","Jessica",2345},
        {"male","Josh",1235},
        {"female","Betsy",2344},
        {"male","Roger",1233}
    };

    quicksort(arr, sizeof(arr)/sizeof(arr[0]));

    for (int i=0;i<sizeof(arr)/sizeof(arr[0]);++i)
        printf("%s, %s, %d\n", arr[i].gender,arr[i].name, arr[i].id);

    return EXIT_SUCCESS;
}
