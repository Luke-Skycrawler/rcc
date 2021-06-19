/* Global Variables */
char courses[50][100]; // courses names
int courses_len[50];
int course_num = 0;
int tot_credit = 0, tot_score = 0, attempt_credit = 0, left_credit = 0;
char prerequisites[50][5][5][100]; // Only 5 types of prerequisite, within which only 5 courses are allowed
int prerequisites_size_l1[50];
int prerequisites_size_l2[50][5]; // Record size info for `prerequisites`
int prerequisites_size_l3[50][5][5]; // Record string length for each course in `prerequisites`
int pass[50]; // Whether the student have pass that course or not

/* Function Definitions */
int printCourse(int i)
{
    // printf("len: %d, ", courses_len[i]);
    for(j: 0 to courses_len[i])
    {
        printf("%c", courses[i][j]);
    }
    printf("\n");
}

// Read a line of input
// @return int: 0 finished, 1 not finished
int read(int cnt)
{
    int i = 0, j = 0, k = 0;
    int credit;
    char c;
    scanf("%c", c);
    if(c == 10)
    {
        return 0; // An empty line means input finish
    }
    // Otherwise the line is valid
    /* Read course name */
    while(c != '|')
    {
        courses[cnt][i] = c;
        courses_len[cnt] = courses_len[cnt] + 1;
        i = i + 1;
        scanf("%c", c);
    }
    /* Read credit */
    scanf("%d", credit);
    tot_credit = credit + tot_credit;
    scanf("%c", c); // Eat the following '|'
    /* Read prerequisites (optinal) */
    i = k = j = 0;
    scanf("%c", c);
    if(c != '|')
    {
        while(c != '|')
        {
            if(c == ';')
            {
                prerequisites_size_l2[cnt][i] = j + 1;
                i = i + 1;
                j = 0;
            }
            else if(c == ',')
            {
                j = j + 1;
                k = 0;
            }
            else
            {
                prerequisites[cnt][i][j][k] = c;
                prerequisites_size_l3[cnt][i][j] = prerequisites_size_l3[cnt][i][j] + 1;
                k = k + 1;
            }
            scanf("%c", c);
        }
        prerequisites_size_l1[cnt] = i + 1;
        prerequisites_size_l2[cnt][i] = j + 1;
    }
    else
    {
        prerequisites_size_l1[cnt] = 0;
    }
    
    /* Read grade (optinal) */
    scanf("%c", c);
    if(c == 10) // If no grade info
    {
        left_credit = left_credit + credit;
        pass[cnt] = 0;
    }
    else if(c == 'A' || c == 'B' || c == 'C' || c == 'D')
    {
        tot_score = tot_score + credit * (4 - (c - 'A'));
        attempt_credit = attempt_credit + credit;
        pass[cnt] = 1;
        scanf("%c", c); // Eat the '\n'
        while(c != 10)
        {
            printf("error: invalid grade info in line %d!\n", cnt);
            scanf("%c", c); // Eat the '\n'
        }
        // return 1; // Uncomment it and the code would fail :(
    }
    else if(c == 'F')
    {
        attempt_credit = attempt_credit + credit;
        left_credit = left_credit + credit;
        pass[cnt] = 0;
        scanf("%c", c); // Eat the '\n'
        while(c != 10)
        {
            printf("error: invalid grade info in line %d!\n", cnt);
            scanf("%c", c); // Eat the '\n'
        }
    }
    else
    {
        printf("error: invalid grade info in line %d!\n", cnt);
        left_credit = left_credit + credit;
        pass[cnt] = 0;
        while(c != 10)
        {
            scanf("%c", c); // Eat the '\n'
        }
    }
    return 1;
}

int main()
{
    int i = 0, j = 0, k = 0, n = 0, u = 0;
    int recommend_flag = 0;

    /* Read */
    while(read(course_num) == 1)
    {
        course_num = course_num + 1;
    }

    /* Statistics */
    if(tot_credit - left_credit == 0) printf("GPA: 0.0\n");
    else
    {
        double dtmp = tot_credit - left_credit + 0.0;
        printf("GPA: %.1f\n", tot_score / dtmp);
    }
    printf("Hours Attempted: %d\n", attempt_credit);
    printf("Hours Completed: %d\n", tot_credit - left_credit);
    printf("Credits Remaining: %d\n\n", left_credit);

    /* Recommend courses */
    printf("Possible Courses to Take Next\n");
    if(left_credit == 0)
    {
        printf("  None - Congratulations!\n");
        return 0;
    }
    for(i: 0 to course_num - 1) // for each course
    {
        if(pass[i] == 0) // that has not taken or failed
        {
            recommend_flag = 0;
            n = prerequisites_size_l1[i]; // number of prerequisite combination
            if(n == 0) // no prerequisite
            {
                int tmp = 0;
                printf("  ");
                while(tmp < courses_len[i])
                {
                    printf("%c", courses[i][tmp]);
                    tmp = tmp + 1;
                }
                printf("\n");
                j = n + 1; // manually end the loop
            }
            for(j: 0 to n - 1) // for each prerequisite combination
            {
                recommend_flag = 1;
                if(prerequisites_size_l2[i][j] > 0)
                {
                    for(k: 0 to prerequisites_size_l2[i][j] - 1) // for each course in the prerequisite combination
                    {
                        int match_flag = 0;
                        int match_index = 0;
                        int count = 0;
                        while(match_index < course_num && match_flag == 0) // match the prerequisite course name
                        {
                            match_flag = 1;
                            count = 0;
                            if(prerequisites_size_l3[i][j][k] != courses_len[match_index]) // if lengths unmatch
                            {
                                match_flag = 0; // mark
                                match_index = match_index + 1; // look next
                            }
                            else
                            {
                                while(count < courses_len[match_index]) // for each character
                                {
                                    if(courses[match_index][count] != prerequisites[i][j][k][count]) // if an unmatched character found
                                    {
                                        // printf("%d: %c & %c\n", count, courses[match_index][count], prerequisites[i][j][k][count]);
                                        count = courses_len[match_index] + 1; // manually break
                                        match_flag = 0; // mark
                                        match_index = match_index + 1; // look next
                                    }
                                    else
                                    {
                                        count = count + 1;
                                        if(count == courses_len[match_index] - 1) // match
                                        {
                                            match_flag = 1;
                                        }
                                    }
                                }
                            }
                        }
                        if(match_flag == 0) // no match found (a possible case!)
                        {
                            // printf("error: no matching course for the prerequisite course...\n");
                            k = prerequisites_size_l2[i][j] + 1;
                        }
                        if(pass[match_index] == 0) // if it's a course not passed
                        {
                            recommend_flag = 0;
                            k = prerequisites_size_l2[i][j] + 1; // manually break
                        }
                    }
                }
                if(recommend_flag == 1) // Recommend!
                {
                    int tmp = 0;
                    printf("  ");
                    while(tmp < courses_len[i])
                    {
                        printf("%c", courses[i][tmp]);
                        tmp = tmp + 1;
                    }
                    printf("\n");
                    j = n + 1; // manually end the loop
                }
            }
        }
    }

    // printf("There are %d courses:\n", course_num);
    // for(i: 0 to course_num - 1)
    // {
    //     printCourse(i);
    // }
    // printf("tot_credit = %d, tot_score = %d, attempt_credit = %d, left_credit = %d\n", tot_credit, tot_score, attempt_credit, left_credit);

    
    return 0;
}