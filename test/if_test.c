int main()
{
    double x = 1.0;
    /*if-else dangling problem solved :)*/
    // The correct parser would print "1: No",
    // while the false parser would print NOTHING
    if(1)
    if(0)
        printf("1: Yes\n");
    else
        printf("1: No\n");

    if(1.0 - 1.0)
    {
        printf("2: Yes\n");
    }
    else
    {
        printf("2: No\n");
    }

    if(3 - 3)
    {
        printf("3: Yes\n");
    }

    if(1.0 * 3.0)
    {
        printf("4: Yes\n");
    }

    printf("Hello world!\n");
}