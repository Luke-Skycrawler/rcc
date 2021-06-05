double a = 3; // test type conversion
int b = 4;
char c = 65; // test type conversion; aka, 'A'
int global_iarray[10]; // default to {0}

int f()
{
    printf("\nIn f():\n");
    printf("Global a = %f, b = %d, c = %c\n", a, b, c);
    for(j: 0 to 9)
    {
        printf("global_iarray[%d] = %d\n", j, global_iarray[j]);
    }
    return 0;
}

int main()
{
    double darray[10];
    int iarray[10];
    double a = 6.66;

    printf("\nIn main():\n");
    printf("Global a = %f, b = %d, c = %c\n", a, b, c);

    for(i: 0 to 9)
    {
        // printf("i = %d\n", i);
        darray[i] = i;
        iarray[i] = i; // Segmentation fault at the end?? -> Now incredibly solved??
        global_iarray[i] = i;
    }

    for(j: 0 to 9)
    {
        printf("darray[%d] = %f\t", j, darray[j]);
        printf("iarray[%d] = %d\t", j, iarray[j]);
        printf("global_iarray[%d] = %d\n", j, global_iarray[j]);
    }
    
    if(b)
    {
        f();
    }

    return 0;
}
