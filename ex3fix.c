#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#endif

typedef struct LogEntry {
    int id;
    char *message;
    char *timestamp;
} LogEntry;

LogEntry* create_entry(int id, const char *msg, const char *time) {
    LogEntry *entry = (LogEntry*)malloc(sizeof(LogEntry));
    if (!entry) return NULL;

    entry->id = id;
    entry->message = strdup(msg);
    entry->timestamp = strdup(time);

    if (!entry->message || !entry->timestamp) {
        free(entry->message);
        free(entry->timestamp);
        free(entry);
        return NULL;
    }

    // ВИПРАВЛЕННЯ 2 (Memory Leak): видалено другий strdup для timestamp,
    // який перезаписував вказівник і втрачав перший виділений блок.
    return entry;
}

void free_entry(LogEntry *entry) {
    if (!entry) return;
    free(entry->message);
    free(entry->timestamp);
    // ВИПРАВЛЕННЯ 1 (Double Free): видалено повторний free(entry->message)
    free(entry);
}

void format_log_header(const char *title) {
    char header_buffer[16];
    // ВИПРАВЛЕННЯ 3 (Stack Buffer Overflow): обмежуємо копіювання розміром буфера
    snprintf(header_buffer, sizeof(header_buffer), "%s", title);
    printf("Header: %s\n", header_buffer);
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);   // UTF-8 у консолі Windows
#endif
    printf("--- Запуск програми журналу подій ---\n");

    // 1. Тест заголовочного буфера
    format_log_header("Цей заголовок є занадто довгим для 16 байт");

    // 2. Тест створення запису
    LogEntry *entry = create_entry(101, "Системна помилка підключення", "12:45:30");
    if (entry) {
        printf("Запис [%d] [%s]: %s\n", entry->id, entry->timestamp, entry->message);
        free_entry(entry);
    }
    return 0;
}