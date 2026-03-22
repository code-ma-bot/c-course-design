#include "info_management.h"

int main() {
    int choice;
    Department *dp;
    Department *tmp_d;
    Position *pp;
    Position *tmp_p;
    Employee *ep;
    Employee *tmp_e;

    printf("==================================================\n");
    printf("         工资管理系统 - 基础信息管理模块\n");
    printf("==================================================\n");

    do {
        printLine();
        printf("基础信息管理 主菜单\n");
        printf("  1 - 部门管理\n");
        printf("  2 - 职位管理\n");
        printf("  3 - 员工信息管理\n");
        printf("  0 - 退出系统\n");
        printLine();
        printf("请输入选择: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1: deptMenu(); break;
            case 2: posMenu();  break;
            case 3: empMenu();  break;
            case 0: printf("感谢使用，再见！\n"); break;
            default: printf("无效输入，请重新选择！\n");
        }
    } while (choice != 0);

    dp = deptHead;
    while (dp != NULL) { tmp_d = dp; dp = dp->next; free(tmp_d); }
    pp = posHead;
    while (pp != NULL) { tmp_p = pp; pp = pp->next; free(tmp_p); }
    ep = empHead;
    while (ep != NULL) { tmp_e = ep; ep = ep->next; free(tmp_e); }

    return 0;
}
