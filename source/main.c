#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

/*******************************************************************************
* Macros
 ******************************************************************************/

#define CONSOLE_WHITE 0x07
#define CONSOLE_RED 0x0C
#define CONSOLE_GREEN 0x0A
#define CONSOLE_BLUE 0x09

#define MAX_CNT_NUM   10000

#define UNIQUE_NUMBERS   1
#define ORDINARY_NUMBERS   2

/*******************************************************************************
* Private Variables
 ******************************************************************************/

int64_t g_numbers_types;

int64_t g_min_val, g_max_val;

int64_t g_count_numbers;

int64_t g_generated_numbers[MAX_CNT_NUM];

/*******************************************************************************
* Private Function Declarations
 ******************************************************************************/

void number_type_get(void);

void values_of_the_range_get(void);

void min_max_nums_get(void);

void count_of_nums_get(void);   

void unique_numbers_generate(void);

void ordinary_numbers_generate(void);

void buffer_clean(void);

void console_color_set(int color);

int m_sorting_func(const void*, const void*);

/*******************************************************************************
* Main Function
 ******************************************************************************/

int main(void)
{
    srand((unsigned)time(0));

    while (1)
    {
        number_type_get();

        values_of_the_range_get();

        if (UNIQUE_NUMBERS == g_numbers_types)
        {
            unique_numbers_generate();
        }
        else
        {
            ordinary_numbers_generate();
        }

        buffer_clean();
    }
}

/*******************************************************************************
* Private Function Definitions
 ******************************************************************************/

void number_type_get(void)
{
    printf("---------------------------------------------------------------\n");

    while ((UNIQUE_NUMBERS != g_numbers_types) && 
           (ORDINARY_NUMBERS != g_numbers_types))
    {
        printf("[1]: uniq numbers \n");
        printf("[2]: ordinary numbers \n");
        printf("Choose the number types: ");
        scanf("%lld", &g_numbers_types);

        if ((UNIQUE_NUMBERS != g_numbers_types) && 
            (ORDINARY_NUMBERS != g_numbers_types))
        {
            console_color_set(CONSOLE_RED);
            printf("\tWrong number\n");
            console_color_set(CONSOLE_WHITE);
        }
    }
}

void values_of_the_range_get(void)
{
    min_max_nums_get();

    count_of_nums_get();    
}

void min_max_nums_get(void)
{
    printf("\nEnter the minimum value of the range: ");
    scanf("%lld", &g_min_val);

    printf("Enter the maximum value of the range: ");
    scanf("%lld", &g_max_val);

    while ((g_max_val <= g_min_val) ||
           (g_min_val < INT64_MIN) ||
           (g_max_val > INT64_MAX))
    {
        g_min_val = 0;
        g_max_val = 0;

        console_color_set(CONSOLE_RED);
        printf("\tWrong numbers!\n");

        if (g_max_val <= g_min_val)
        {
            printf("\tMinimum number must be less than maximum number\n\n");
        }
        else if (g_min_val < INT64_MIN)
        {
            printf("\tand minimum number can be %lld or up,\n\n", INT64_MIN);
        }
        else if (g_max_val > INT64_MAX)
        {
            printf("\tmaximum number can be %lld or down\n\n", INT64_MAX);
        }
        else
        {
            printf("\tdebug: undefined status\n\n");
        }
        console_color_set(CONSOLE_WHITE);

        printf("Enter the minimum value of the range: ");
        scanf("%lld", &g_min_val);

        printf("Enter the maximum value of the range: ");
        scanf("%lld", &g_max_val);
    }
}

void count_of_nums_get(void)
{
    printf("Enter the count of the random numbers: ");
    scanf("%lld", &g_count_numbers);

    bool m_count_num_out_range = 
        ((g_count_numbers <= 0) || (g_count_numbers > MAX_CNT_NUM));

    bool m_count_num_out_range_for_unique = false;

    if (UNIQUE_NUMBERS == g_numbers_types)
    {
        m_count_num_out_range_for_unique = 
            (g_count_numbers > ((g_max_val + 1) - g_min_val));
    }

    while (true == m_count_num_out_range ||
           true == m_count_num_out_range_for_unique)
    {
        console_color_set(CONSOLE_RED);

        if (true == m_count_num_out_range)
        {
            printf("\tWrong numbers!\n");
            printf("\tCount of the numbers must be greater than 0\n");
            printf("\tand less than %d\n\n", MAX_CNT_NUM);
        }

        if (true == m_count_num_out_range_for_unique)
        {
            printf("\tWrong numbers!\n");
            printf("\tThis program for generate unique numbers.\n");
            printf("\tCount of the numbers can not be greater than range.\n\n");
        }
        console_color_set(CONSOLE_WHITE);

        printf("Enter the count of the random numbers: ");
        scanf("%lld", &g_count_numbers);

        m_count_num_out_range = 
            ((g_count_numbers <= 0) || (g_count_numbers > MAX_CNT_NUM));

        if (UNIQUE_NUMBERS == g_numbers_types)
        {
            m_count_num_out_range_for_unique = 
                (g_count_numbers > ((g_max_val + 1) - g_min_val));
        }
    }

    getchar();
}

void unique_numbers_generate(void)
{
    for (uint32_t idx = 0U; idx < g_count_numbers; idx++)
    {
        int64_t new_val = rand() % ((g_max_val + 1) - g_min_val) + g_min_val;

        g_generated_numbers[idx] = new_val;

        for (uint32_t i = 0; i < idx; i++)
        {
            if (g_generated_numbers[i] == new_val)
            {
                g_generated_numbers[idx--] = 0; //new value already generated
            }
        }
    }

    qsort(g_generated_numbers,
		  g_count_numbers,
		  sizeof (int64_t),
		  m_sorting_func);

    for (uint32_t idx = 0U; idx < g_count_numbers; idx++)
    {
        console_color_set(CONSOLE_GREEN);
        printf("%lld\n", g_generated_numbers[idx]);
        console_color_set(CONSOLE_WHITE);
    }
}

void ordinary_numbers_generate(void)
{
    for (uint32_t idx = 0U; idx < g_count_numbers; idx++)
    {
        int64_t new_val = rand() % ((g_max_val + 1) - g_min_val) + g_min_val;

        g_generated_numbers[idx] = new_val;
    }

    qsort(g_generated_numbers,
		  g_count_numbers,
		  sizeof (int64_t),
		  m_sorting_func);

    for (uint32_t idx = 0U; idx < g_count_numbers; idx++)
    {
        console_color_set(CONSOLE_GREEN);
        printf("%lld\n", g_generated_numbers[idx]);
        console_color_set(CONSOLE_WHITE);
    }
}

void buffer_clean(void)
{
    for (uint32_t idx = 0U; idx < MAX_CNT_NUM; idx++)
    {
        g_generated_numbers[idx] = 0;
    }

    g_min_val = g_max_val = 0; 

    g_count_numbers = 0;

    g_numbers_types = 0;
}

void console_color_set(int color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

int m_sorting_func(const void* first_element, const void* second_element)
{   
    return (*((int64_t*)first_element) - *((int64_t*)second_element));
}

// end of file
