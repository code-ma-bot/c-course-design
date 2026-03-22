/**
 * @file query.c
 * @brief 查询、排序、统计模块
 */

#include "system.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 辅助宏，用于输出分隔线
#define LINE "========================================\n"

/*-------------------------- 查询模块 --------------------------*/

/**
 * @brief 模糊查询员工（按姓名）
 * @param keyword 关键字
 */
void search_employee(const char *keyword) {
    if (keyword == NULL || strlen(keyword) == 0) {
        printf("查询关键字不能为空！\n");
        return;
    }

    Employee *p = emp_head;
    int found = 0;
    printf(LINE);
    printf("员工模糊查询结果（关键字：%s）\n", keyword);
    printf(LINE);

    while (p) {
        if (strstr(p->name, keyword) != NULL) {
            printf("工号：%d | 姓名：%s | 部门编号：%d | 职位编号：%d\n",
                   p->id, p->name, p->dept_id, p->pos_id);
            found = 1;
        }
        p = p->next;
    }
    if (!found) printf("未找到包含“%s”的员工。\n", keyword);
}

/**
 * @brief 模糊查询部门（按名称）
 * @param keyword 关键字
 */
void search_department(const char *keyword) {
    if (keyword == NULL || strlen(keyword) == 0) {
        printf("查询关键字不能为空！\n");
        return;
    }

    Department *p = dept_head;
    int found = 0;
    printf(LINE);
    printf("部门模糊查询结果（关键字：%s）\n", keyword);
    printf(LINE);

    while (p) {
        if (strstr(p->name, keyword) != NULL) {
            printf("部门编号：%d | 名称：%s | 主管：%s | 人数：%d\n",
                   p->id, p->name, p->manager, p->emp_count);
            found = 1;
        }
        p = p->next;
    }
    if (!found) printf("未找到包含“%s”的部门。\n", keyword);
}

/**
 * @brief 模糊查询职位（按名称）
 * @param keyword 关键字
 */
void search_position(const char *keyword) {
    if (keyword == NULL || strlen(keyword) == 0) {
        printf("查询关键字不能为空！\n");
        return;
    }

    Position *p = pos_head;
    int found = 0;
    printf(LINE);
    printf("职位模糊查询结果（关键字：%s）\n", keyword);
    printf(LINE);

    while (p) {
        if (strstr(p->name, keyword) != NULL) {
            printf("职位编号：%d | 名称：%s | 最低工资：%.2f | 最高工资：%.2f\n",
                   p->id, p->name, p->min_salary, p->max_salary);
            found = 1;
        }
        p = p->next;
    }
    if (!found) printf("未找到包含“%s”的职位。\n", keyword);
}

/**
 * @brief 模糊查询调动记录（按员工姓名）
 * @param keyword 关键字
 */
void search_transfer(const char *keyword) {
    if (keyword == NULL || strlen(keyword) == 0) {
        printf("查询关键字不能为空！\n");
        return;
    }

    Transfer *p = trans_head;
    int found = 0;
    printf(LINE);
    printf("调动记录模糊查询结果（员工姓名：%s）\n", keyword);
    printf(LINE);

    while (p) {
        if (strstr(p->emp_name, keyword) != NULL) {
            printf("记录ID：%d | 员工：%s | 原部门：%d | 原职位：%d | 新部门：%d | 新职位：%d | 调整时间：%s\n",
                   p->record_id, p->emp_name, p->old_dept, p->old_pos,
                   p->new_dept, p->new_pos, p->change_date);
            found = 1;
        }
        p = p->next;
    }
    if (!found) printf("未找到员工姓名包含“%s”的调动记录。\n", keyword);
}

/**
 * @brief 模糊查询工资记录（按员工姓名）
 * @param keyword 关键字
 */
void search_salary(const char *keyword) {
    if (keyword == NULL || strlen(keyword) == 0) {
        printf("查询关键字不能为空！\n");
        return;
    }

    Salary *s = salary_head;
    int found = 0;
    printf(LINE);
    printf("工资记录模糊查询结果（员工姓名：%s）\n", keyword);
    printf(LINE);

    while (s) {
        // 通过员工ID获取员工姓名
        char *emp_name = NULL;
        Employee *e = emp_head;
        while (e) {
            if (e->id == s->emp_id) {
                emp_name = e->name;
                break;
            }
            e = e->next;
        }
        if (emp_name && strstr(emp_name, keyword)) {
            printf("工资单ID：%d | 员工ID：%d | 姓名：%s | 年月：%s | 应发：%.2f | 实发：%.2f\n",
                   s->salary_id, s->emp_id, emp_name, s->year_month,
                   s->total_pay, s->net_pay);
            found = 1;
        }
        s = s->next;
    }
    if (!found) printf("未找到员工姓名包含“%s”的工资记录。\n", keyword);
}

/*-------------------------- 排序模块 --------------------------*/

// 比较函数：按姓名升序
static int compare_emp_by_name(const void *a, const void *b) {
    Employee *ea = *(Employee**)a;
    Employee *eb = *(Employee**)b;
    return strcmp(ea->name, eb->name);
}

// 比较函数：按入职时间升序（字符串直接比较即可，因为格式 YYYY-MM-DD）
static int compare_emp_by_hire_date(const void *a, const void *b) {
    Employee *ea = *(Employee**)a;
    Employee *eb = *(Employee**)b;
    return strcmp(ea->hire_date, eb->hire_date);
}

// 比较函数：按应发工资降序（从高到低）
static int compare_salary_by_amount(const void *a, const void *b) {
    Salary *sa = *(Salary**)a;
    Salary *sb = *(Salary**)b;
    // 降序：如果 sa 的应发 > sb 的应发，返回负数
    if (sa->total_pay > sb->total_pay) return -1;
    if (sa->total_pay < sb->total_pay) return 1;
    return 0;
}

/**
 * @brief 按姓名排序员工并输出
 */
void sort_employee_by_name() {
    int count = 0;
    Employee *p = emp_head;
    while (p) { count++; p = p->next; }
    if (count == 0) {
        printf("没有员工数据。\n");
        return;
    }

    Employee **arr = (Employee**)malloc(count * sizeof(Employee*));
    p = emp_head;
    for (int i = 0; i < count; i++) {
        arr[i] = p;
        p = p->next;
    }

    qsort(arr, count, sizeof(Employee*), compare_emp_by_name);

    printf(LINE);
    printf("按姓名排序员工（升序）：\n");
    printf(LINE);
    for (int i = 0; i < count; i++) {
        printf("工号：%d | 姓名：%s | 部门：%d | 入职日期：%s\n",
               arr[i]->id, arr[i]->name, arr[i]->dept_id, arr[i]->hire_date);
    }
    free(arr);
}

/**
 * @brief 按入职时间排序员工并输出
 */
void sort_employee_by_hire_date() {
    int count = 0;
    Employee *p = emp_head;
    while (p) { count++; p = p->next; }
    if (count == 0) {
        printf("没有员工数据。\n");
        return;
    }

    Employee **arr = (Employee**)malloc(count * sizeof(Employee*));
    p = emp_head;
    for (int i = 0; i < count; i++) {
        arr[i] = p;
        p = p->next;
    }

    qsort(arr, count, sizeof(Employee*), compare_emp_by_hire_date);

    printf(LINE);
    printf("按入职时间排序员工（升序）：\n");
    printf(LINE);
    for (int i = 0; i < count; i++) {
        printf("工号：%d | 姓名：%s | 入职日期：%s\n",
               arr[i]->id, arr[i]->name, arr[i]->hire_date);
    }
    free(arr);
}

/**
 * @brief 按工资金额（应发工资）排序并输出工资记录
 * @note 这里对所有工资记录按应发工资降序排列
 */
void sort_salary_by_amount() {
    int count = 0;
    Salary *p = salary_head;
    while (p) { count++; p = p->next; }
    if (count == 0) {
        printf("没有工资记录。\n");
        return;
    }

    Salary **arr = (Salary**)malloc(count * sizeof(Salary*));
    p = salary_head;
    for (int i = 0; i < count; i++) {
        arr[i] = p;
        p = p->next;
    }

    qsort(arr, count, sizeof(Salary*), compare_salary_by_amount);

    printf(LINE);
    printf("按应发工资排序工资记录（降序）：\n");
    printf(LINE);
    for (int i = 0; i < count; i++) {
        // 查找员工姓名
        char *emp_name = NULL;
        Employee *e = emp_head;
        while (e) {
            if (e->id == arr[i]->emp_id) {
                emp_name = e->name;
                break;
            }
            e = e->next;
        }
        printf("员工：%s | 年月：%s | 应发：%.2f | 实发：%.2f\n",
               emp_name ? emp_name : "未知", arr[i]->year_month,
               arr[i]->total_pay, arr[i]->net_pay);
    }
    free(arr);
}

/*-------------------------- 统计模块 --------------------------*/

/**
 * @brief 统计各部门员工人数
 */
void stat_dept_employee_count() {
    Department *d = dept_head;
    printf(LINE);
    printf("各部门人数统计：\n");
    printf(LINE);
    while (d) {
        // 如果部门结构体中已有 emp_count 字段，可以直接使用
        printf("部门：%s | 人数：%d\n", d->name, d->emp_count);
        d = d->next;
    }
    // 如果部门结构体中没有 emp_count 字段，则需要遍历员工链表统计
    // 此处假设有 emp_count，所以直接输出
}

/**
 * @brief 统计某年入职的员工人数
 * @param year 年份，例如 2023
 */
void stat_hire_by_year(int year) {
    int count = 0;
    Employee *e = emp_head;
    while (e) {
        // 假设 hire_date 格式为 "YYYY-MM-DD"，提取前4位
        char year_str[5];
        strncpy(year_str, e->hire_date, 4);
        year_str[4] = '\0';
        if (atoi(year_str) == year) {
            count++;
        }
        e = e->next;
    }
    printf("%d年入职员工人数：%d\n", year, count);
}

/**
 * @brief 统计某部门平均工资（取所有月份的平均）
 * @param dept_id 部门编号
 */
void stat_avg_salary_by_dept(int dept_id) {
    // 先获取该部门所有员工ID
    int emp_ids[1000];
    int emp_count = 0;
    Employee *e = emp_head;
    while (e) {
        if (e->dept_id == dept_id && e->status == 0) { // 只统计在职员工
            emp_ids[emp_count++] = e->id;
        }
        e = e->next;
    }
    if (emp_count == 0) {
        printf("部门 %d 没有在职员工。\n", dept_id);
        return;
    }

    // 遍历工资记录，累加这些员工的应发工资
    float total = 0;
    int salary_count = 0;
    Salary *s = salary_head;
    while (s) {
        for (int i = 0; i < emp_count; i++) {
            if (s->emp_id == emp_ids[i]) {
                total += s->total_pay;
                salary_count++;
                break;
            }
        }
        s = s->next;
    }
    if (salary_count == 0) {
        printf("部门 %d 没有工资记录。\n", dept_id);
        return;
    }
    printf("部门 %d 平均工资（所有月份）：%.2f\n", dept_id, total / salary_count);
}

/**
 * @brief 统计某职位平均工资（取所有月份的平均）
 * @param pos_id 职位编号
 */
void stat_avg_salary_by_pos(int pos_id) {
    // 先获取该职位所有员工ID
    int emp_ids[1000];
    int emp_count = 0;
    Employee *e = emp_head;
    while (e) {
        if (e->pos_id == pos_id && e->status == 0) {
            emp_ids[emp_count++] = e->id;
        }
        e = e->next;
    }
    if (emp_count == 0) {
        printf("职位 %d 没有在职员工。\n", pos_id);
        return;
    }

    float total = 0;
    int salary_count = 0;
    Salary *s = salary_head;
    while (s) {
        for (int i = 0; i < emp_count; i++) {
            if (s->emp_id == emp_ids[i]) {
                total += s->total_pay;
                salary_count++;
                break;
            }
        }
        s = s->next;
    }
    if (salary_count == 0) {
        printf("职位 %d 没有工资记录。\n", pos_id);
        return;
    }
    printf("职位 %d 平均工资（所有月份）：%.2f\n", pos_id, total / salary_count);
}
