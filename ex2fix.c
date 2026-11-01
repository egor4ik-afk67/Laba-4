#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#endif

typedef struct {
    int id;
    char name[32];
} User;

User* create_user_list(int count) {
    // Виділяємо пам'ять під count користувачів
    User *users = (User*)malloc(count * sizeof(User));
    if (!users) return NULL;

    // Заповнюємо дані користувачів
    // ВИПРАВЛЕННЯ 1 (Heap Buffer Overflow): було i <= count, тепер i < count
    for (int i = 0; i < count; i++) {
        users[i].id = i + 1;
        snprintf(users[i].name, sizeof(users[i].name), "User_%d", i + 1);
    }
    return users;
}

void process_users() {
    int count = 5;
    User *users = create_user_list(count);
    if (!users) return;

    printf("Створено користувачів: %d\n", count);

    // ВИПРАВЛЕННЯ 2 (Use-After-Free): використовуємо дані ДО free
    printf("Перший користувач: ID=%d, Name=%s\n", users[0].id, users[0].name);

    free(users);
    users = NULL;
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);   // UTF-8 у консолі Windows
#endif
    process_users();
    return 0;
}