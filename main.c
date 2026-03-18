#include "salary.h"

int main() {
    Node *head = NULL; // 初始化空链表
    int choice;

    while (1) {
        menu();
        if (scanf("%d", &choice) != 1) {
            clearBuffer();
            printf("输入无效，请输入数字！\n");
            continue;
        }

        switch (choice) {
            case 1:
                head = inputSalary(head);
                break;
            case 2:
                printAll(head);
                break;
            case 3:
                modifySalary(head);
                break;
            case 4:
                adminStatistics(head);
                break;
            case 5:
                employeeStatistics(head);
                break;
            case 6:
                sortSalaryDescending(head);
                break;
            case 0:
                printf("感谢使用工资管理系统，再见！\n");
                // 此处可以补充释放链表内存的代码
                return 0;
            default:
                printf("无效选项，请重新选择！\n");
        }
    }
    return 0;
}
