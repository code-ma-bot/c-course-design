#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED



#endif // SYSTEM_H_INCLUDED

#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*========================== 数据结构定义 ==========================*/

typedef struct Department {
    int id;
    char name[50];
    char manager[20];
    char establish_date[11];
    int emp_count;
    struct Department *next;
} Department;

typedef struct Position {
    int id;
    char name[50];
    float min_salary;
    float max_salary;
    struct Position *next;
} Position;

typedef struct Employee {
    int id;
    char name[20];
    char id_card[19];
    char hire_date[11];
    int dept_id;
    int pos_id;
    char phone[12];
    char email[30];
    char address[100];
    char school[50];
    char edu_level[10];
    int status;
    char password[20];
    struct Employee *next;
} Employee;

typedef struct Transfer {
    int record_id;
    int emp_id;
    char emp_name[20];
    int old_dept;
    int old_pos;
    int new_dept;
    int new_pos;
    char change_date[11];
    struct Transfer *next;
} Transfer;

typedef struct Salary {
    int salary_id;
    int emp_id;
    char year_month[7];
    float base_salary;
    float performance;
    float bonus;
    float fine;
    float total_pay;
    float pension;
    float medical;
    float unemployment;
    float housing_fund;
    float tax;
    float net_pay;
    struct Salary *next;
} Salary;

/*========================== 全局链表指针声明 ==========================*/
extern Department *dept_head;
extern Position *pos_head;
extern Employee *emp_head;
extern Transfer *trans_head;
extern Salary *salary_head;

/*========================== 公共函数声明 ==========================*/
void load_all_data();
void save_all_data();

/*========================== 你的模块函数声明 ==========================*/
void search_employee(const char *keyword);
void search_department(const char *keyword);
void search_position(const char *keyword);
void search_transfer(const char *keyword);
void search_salary(const char *keyword);
void sort_employee_by_name();
void stat_dept_employee_count();
void change_password(int user_id, int role);
void backup_data();
void restore_data();

#endif // SYSTEM_H
