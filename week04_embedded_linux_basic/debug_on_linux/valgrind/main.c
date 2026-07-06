#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

volatile sig_atomic_t running = 1;
static int *array_ptr2;
int *g_ptr = NULL;

typedef struct
{
    int *p_data;
} my_struct;

void sigint_handler(int sig)
{
    running = 0;
}

int main(void)
{
    signal(SIGINT, sigint_handler);

    /* Still reachable */
    g_ptr = malloc(sizeof(int) * 10);

    /* Definitely lost */
    int *ptr = malloc(sizeof(int) * 5);
    ptr = NULL;

    /* Indirectly lost */
    my_struct *my_struct_ptr = malloc(sizeof(my_struct));
    my_struct_ptr->p_data = malloc(sizeof(int) * 3);
    my_struct_ptr = NULL;

    /* Possibly lost */
    int *array_ptr = malloc(sizeof(int) * 4);
    array_ptr2 = array_ptr + 1;
    printf("%p\n", (void *)array_ptr2);
    array_ptr = NULL;

    while (running)
    {
        // thực thi các tác vụ khác
    }

    return 0;
}
