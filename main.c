#include <stdio.h>
#include <stdlib.h>
#include "wage.h"
#include "wage.c"

int main() {
    int choice;

    while (1) {
        menu();
        scanf("%d", &choice);

        switch (choice) {
            case 0:
                printf("\n感谢使用企业工资管理系统！\n");
                return 0;
            case 1:
                wageManagement();
                break;
            case 2:
                employeeTransfer();
                break;
            case 3:
                systemMaintenance();
                break;
            case 4:
                fileManagement();
                break;
            default:
                printf("输入错误，请输入0-4！\n");
                system("pause");
        }
    }

    return 0;
}
