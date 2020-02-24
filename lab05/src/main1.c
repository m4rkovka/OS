#include <stdio.h>
#include <stdint.h>
#include "extern.h"

int menu() {
    int choice;
    printf("select\n"
           "1-print\n"
           "2-insert\n"
           "3-delete\n"
           "4-clear\n"
           "5-push front\n"
           "6-push back\n"
           "7-pop front\n"
           "8-pop back\n"
           "9-size\n"
           "10-front\n"
           "11-back\n"
           "12-end\n");
    scanf("%d", &choice);
    return choice;
}

int main() {
    Deque deque;
    deque_init(&deque);
    int choice, index;
    int32_t value;
    while ((choice = menu()) != 12) {
        switch (choice) {
            case 1:
                deque_print(&deque);
                break;
            case 2:
                printf("Enter index new value: ");
                Deque_iterator it;
                it.ptr = deque.start;
                scanf("%d", &index);
                if (index < 0 || index > deque_size(&deque)) {
                    printf("Error, invalid index\n");
                    break;
                }
                for (int j = 0; j < index; j++) {
                    it.ptr = it.ptr->next;
                }
                printf("Enter value: ");
                int32_t value;
                scanf("%d", &value);
                deque_insert(it, value, &deque);
                break;
            case 3:
                printf("Enter index to delete: ");
                Deque_iterator it1;
                it1.ptr = deque.start;
                scanf("%d", &index);
                if (index < 0 || index >= deque_size(&deque)) {
                    printf("Error, invalid index\n");
                    break;
                }
                for (int i = 0; i < index + 1; i++) {
                    it1.ptr = it1.ptr->next;
                } 
                deque_erase(it1, &deque);
                break;
            case 4:
                deque_clear(&deque);
                break;
            case 5:
                printf("Enter value: ");
                scanf("%d", &value);
                deque_push_front(value, &deque);
                break;
            case 6:
                printf("Enter value: ");
                scanf("%d", &value);
                deque_push_back(value, &deque);
                break;
            case 7:
                deque_pop_front(&deque);
                break;
            case 8:
                deque_pop_back(&deque);
                break;
            case 9:
                printf("size: %d\n", deque_size(&deque));
                break;
            case 10:
                printf("front elem: %d\n", deque_front(&deque));
                break;
            case 11:
                printf("back elem: %d\n", deque_back(&deque));
                break;
            default:
                printf("Unknown command\n");
        }
    }
    return 0;
}