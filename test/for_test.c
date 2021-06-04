int main()
{
    int k = 3;
    for(i: 3 incto 5)
    {
        printf("i = %d\n", i);
    }
    // printf("(Illegal access) i = %d\n", i); // Uncomment this line and you'll see errors
    
    for(k: k * 3 decto 5)
    {
        printf("k = %d\n", k);
    }
    printf("Original k = %d\n", k);
    
    for(j: 3 incto 2) // won't enter the following loop 
    {
        printf("j = %d\n", j);
    }
    return 0;
}