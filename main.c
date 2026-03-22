#include "system.h"
#include <stdio.h>
#include <stdlib.h>

// 全局链表头定义
Department *dept_head = NULL;
Position *pos_head = NULL;
Employee *emp_head = NULL;
Transfer *trans_head = NULL;
Salary *salary_head = NULL;

void load_all_data() {
    printf("数据加载功能待实现（由组长完成）\n");
}

void save_all_data() {
    printf("数据保存功能待实现（由组长完成）\n");
}

void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void press_any_key() {
    printf("\n按回车键继续...");
    while (getchar() != '\n');
    getchar();
}

int main() {
    load_all_data();

    int choice;
    do {
        clear_screen();
        printf("\n========== 工资管理系统 ==========\n");
        printf("1. 员工查询\n");
        printf("2. 部门查询\n");
        printf("3. 职位查询\n");
        printf("4. 调动记录查询\n");
        printf("5. 工资记录查询\n");
        printf("6. 员工排序（按姓名）\n");
        printf("7. 统计部门人数\n");
        printf("8. 修改密码\n");
        printf("9. 数据备份\n");
        printf("0. 退出\n");
        printf("请选择：");
        scanf("%d", &choice);
        getchar(); // 吸收回车

        char keyword[50];
        int uid;

        switch (choice) {
            case 1:
                printf("请输入员工姓名关键字：");
                fgets(keyword, sizeof(keyword), stdin);
                keyword[strcspn(keyword, "\n")] = '\0';
                search_employee(keyword);
                press_any_key();
                break;
            case 2:
                printf("请输入部门名称关键字：");
                fgets(keyword, sizeof(keyword), stdin);
                keyword[strcspn(keyword, "\n")] = '\0';
                search_department(keyword);
                press_any_key();
                break;
            case 3:
                printf("请输入职位名称关键字：");
                fgets(keyword, sizeof(keyword), stdin);
                keyword[strcspn(keyword, "\n")] = '\0';
                search_position(keyword);
                press_any_key();
                break;
            case 4:
                printf("请输入员工姓名关键字：");
                fgets(keyword, sizeof(keyword), stdin);
                keyword[strcspn(keyword, "\n")] = '\0';
                search_transfer(keyword);
                press_any_key();
                break;
            case 5:
                printf("请输入员工姓名关键字：");
                fgets(keyword, sizeof(keyword), stdin);
                keyword[strcspn(keyword, "\n")] = '\0';
                search_salary(keyword);
                press_any_key();
                break;
            case 6:
                sort_employee_by_name();
                press_any_key();
                break;
            case 7:
                stat_dept_employee_count();
                press_any_key();
                break;
            case 8:
                printf("请输入用户ID：");
                scanf("%d", &uid);
                getchar();
                change_password(uid, 0);
                press_any_key();
                break;
            case 9:
                backup_data();
                press_any_key();
                break;
            case 0:
                save_all_data();
                printf("谢谢使用！\n");
                break;
            default:
                printf("输入错误，请重新选择！\n");
                press_any_key();
        }
    } while (choice != 0);

    return 0;
}
