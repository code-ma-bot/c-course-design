#ifndef SALARY_H
#define SALARY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 年月结构体
typedef struct {
    int year;
    int month;
} YearMonth;

// 工资记录结构体
typedef struct {
    int empId;                  // 员工编号
    char name[50];              // 员工姓名
    char department[30];        // 部门 (新增)
    char position[30];          // 职位 (新增)
    char city[30];              // 所在城市
    YearMonth ym;               // 工资所属年月
    double basicSalary;         // 基本工资
    double performance;         // 绩效工资
    double penalty;             // 奖罚金额 (正为奖，负为罚)
    double socialSecurity;      // 社保扣除
    double tax;                 // 个税扣除
    double grossPay;            // 应发工资 (基本+绩效+奖罚)
    double netPay;              // 实发工资 (应发-社保-个税)
} SalaryRecord;

// 链表节点定义
typedef struct Node {
    SalaryRecord data;
    struct Node *next;
} Node;

// ======== 函数声明 ========
void menu();
void clearBuffer();
int sameMonth(YearMonth ym1, YearMonth ym2);
void printSalaryRecord(SalaryRecord *record);
void calculateSalary(Node *head, SalaryRecord *record, int calcSS, int calcTax);

// 核心业务功能
Node* inputSalary(Node *head);             // 1. 录入工资
void printAll(Node *head);                 // 2. 打印所有记录
void modifySalary(Node *head);             // 3. 调整员工工资 (管理员)
void adminStatistics(Node *head);          // 4. 部门平均工资统计 (管理员)
void employeeStatistics(Node *head);       // 5. 员工年度账单统计 (员工)
void sortSalaryDescending(Node *head);     // 6. 按实发工资排序 (分析功能)

#endif
