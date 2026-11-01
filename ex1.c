#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#endif

int main(void)
{
#ifdef _WIN32
    SetConsoleOutputCP(65001);   /* UTF-8 у консолі Windows */
#endif
    /* 1. Локальна змінна на стеку */
    int stack_var = 42;

    /* 2. Виділення пам'яті в купі під 3 елементи (calloc обнуляє пам'ять) */
    int *heap_array = (int *)calloc(3, sizeof(int));
    if (heap_array == NULL) {
        printf("Помилка виділення пам'яті (calloc).\n");
        return 1;
    }

    for (int i = 0; i < 3; i++)
        *(heap_array + i) = (i + 1) * 10;

    /* 3. Виведення адрес */
    printf("=== Адреси ===\n");
    printf("Змінна stack_var            (стек): %p\n", (void *)&stack_var);
    printf("Вказівник heap_array        (стек): %p\n", (void *)&heap_array);
    printf("Масив heap_array, 3 елементи (купа): %p\n", (void *)heap_array);

    printf("\nМасив (3 елементи): ");
    for (int i = 0; i < 3; i++)
        printf("%d ", *(heap_array + i));
    printf("\n");

    /* 4. Збільшення розміру до 5 елементів (realloc) */
    int *tmp = (int *)realloc(heap_array, 5 * sizeof(int));
    if (tmp == NULL) {
        printf("Помилка перевиділення пам'яті (realloc).\n");
        free(heap_array);   /* старий блок залишається чинним */
        return 1;
    }
    heap_array = tmp;

    *(heap_array + 3) = 40;
    *(heap_array + 4) = 50;

    printf("\n=== Після realloc ===\n");
    printf("Вказівник heap_array        (стек): %p\n", (void *)&heap_array);
    printf("Масив heap_array, 5 елементів (купа): %p\n", (void *)heap_array);

    printf("\nМасив (5 елементів): ");
    for (int i = 0; i < 5; i++)
        printf("%d ", *(heap_array + i));
    printf("\n");

    /* 5. Звільнення пам'яті */
    free(heap_array);
    heap_array = NULL;

    printf("\nПам'ять звільнено.\n");
    return 0;
}