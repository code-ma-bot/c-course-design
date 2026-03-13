#include <stdio.h>
#include "salary.h"

#define FILE_NAME "salary_records.dat"

int main() {
    Node *head = NULL;   // 动态链表头指针
    int choice;

    /* 程序启动时先尝试读取历史数据 */
    loadFromFile(&head, FILE_NAME);

    do {
        menu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                inputSalary(&head);
                break;
            case 2:
                querySalary(head);
                break;
            case 3:
                printAllRecords(head);
                break;
            case 4:
                saveToFile(head, FILE_NAME);
                break;
            case 5:
                loadFromFile(&head, FILE_NAME);
                break;
            case 0:
                saveToFile(head, FILE_NAME);
                printf("系统退出。\n");
                break;
            default:
                printf("输入无效，请重新选择。\n");
        }
    } while (choice != 0);

    /* 程序结束前释放链表内存 */
    freeList(&head);

    return 0;
}
