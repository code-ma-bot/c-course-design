
#ifndef SALARY_H
#define SALARY_H

#include <stdio.h>

/* =========================
   年月结构体
   用于表示某条工资所属的年月
   ========================= */
typedef struct {
    int year;
    int month;
} YearMonth;

/* =========================
   工资记录结构体
   每个员工每个月对应一条工资记录
   ========================= */
typedef struct {
    int empId;                  // 员工编号
    char name[50];              // 员工姓名
    char city[30];              // 所在城市
    YearMonth ym;               // 工资所属年月

    /* 原始工资项 */
    double basicSalary;         // 基本工资
    double performance;         // 绩效工资
    double bonus;               // 奖金
    double allowance;           // 补贴
    double penalty;             // 奖惩（可正可负）

    /* 计算结果 */
    double grossPay;            // 应发工资

    /* 社保公积金 */
    double pension;             // 养老保险
    double medical;             // 医疗保险
    double unemployment;        // 失业保险
    double housingFund;         // 住房公积金
    double socialTotal;         // 社保公积金合计

    /* 税后结果 */
    double tax;                 // 个税
    double netPay;              // 实发工资
} SalaryRecord;

/* =========================
   链表结点结构体
   动态链表每个结点存一条工资记录
   ========================= */
typedef struct Node {
    SalaryRecord data;          // 当前结点保存的工资记录
    struct Node *next;          // 指向下一个结点
} Node;


/* ========== 工具函数 ========== */
int sameMonth(YearMonth a, YearMonth b);
int compareMonth(YearMonth a, YearMonth b);
void clearBuffer();

/* ========== 链表操作 ========== */
Node* createNode(SalaryRecord r);
void appendNode(Node **head, SalaryRecord r);
int findRecord(Node *head, int empId, YearMonth ym, SalaryRecord *result);
void printAllRecords(Node *head);
void freeList(Node **head);

/* ========== 工资计算核心 ========== */
void calcSocialByCity(SalaryRecord *r);
double getTaxRate(double taxable, double *quickDeduction);
double calcCurrentMonthTax(Node *head, SalaryRecord *current);
void calculateSalary(Node *head, SalaryRecord *r, int enableSocial, int enableTax);

/* ========== 业务功能 ========== */
void inputSalary(Node **head);
void querySalary(Node *head);

/* ========== 文件读写 ========== */
void saveToFile(Node *head, const char *filename);
void loadFromFile(Node **head, const char *filename);

/* ========== 输出函数 ========== */
void printSalaryRecord(const SalaryRecord *r);

/* ========== 菜单函数 ========== */
void menu();

#endif
