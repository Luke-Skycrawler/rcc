struct A{
    int x;
    double y;
    char z;
    int array[10];
};

int main()
{
    struct A instance[10];
    for(i: 0 to 9)
    {
        instance[i].x = i;
        for(k: 0 to 9)
        {
            instance[i].array[k] = 10 * i + k;
        }
    }

    for(i: 0 to 9)
    {
        printf("instance[%d].x = %d\n", i, instance[i].x);
        for(k: 0 to 9)
        {
            printf("instance[%d].array[%d] = %d\n", i, k, instance[i].array[k]);
        }
    }
}