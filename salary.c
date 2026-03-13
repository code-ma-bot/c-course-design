#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "salary.h"

/* =====================================================
   工具函数部分
   ===================================================== */

/*
    判断两个年月是否相同
    相同返回1，不同返回0
*/
int sameMonth(YearMonth a, YearMonth b) {
    return a.year == b.year && a.month == b.month;
}

/*
    比较两个年月的先后
    返回值：
    <0  表示 a 早于 b
    =0  表示 a 与 b 相同
    >0  表示 a 晚于 b
*/
int compareMonth(YearMonth a, YearMonth b) {
    if (a.year != b.year) {
        return a.year - b.year;
    }
    return a.month - b.month;
}

/*
    清空输入缓冲区
    作用：
    避免 scanf 之后残留回车影响 fgets
*/
void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


/* =====================================================
   链表操作部分
   ===================================================== */

/*
    创建一个新的链表结点
    参数 r：要保存的工资记录
    返回值：新结点指针
*/
Node* createNode(SalaryRecord r) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("内存申请失败！\n");
        return NULL;
    }

    newNode->data = r;
    newNode->next = NULL;
    return newNode;
}

/*
    在链表尾部追加一个结点
    head 是头指针的地址，因为可能要修改头指针
*/
void appendNode(Node **head, SalaryRecord r) {
    Node *newNode = createNode(r);
    if (newNode == NULL) {
        return;
    }

    /* 如果当前链表为空，新结点直接作为头结点 */
    if (*head == NULL) {
        *head = newNode;
        return;
    }

    /* 否则遍历到链表末尾，再接上新结点 */
    Node *p = *head;
    while (p->next != NULL) {
        p = p->next;
    }
    p->next = newNode;
}

/*
    查找某员工某月工资记录
    如果找到，把结果拷贝到 result 中，并返回1
    如果找不到，返回0
*/
int findRecord(Node *head, int empId, YearMonth ym, SalaryRecord *result) {
    Node *p = head;

    while (p != NULL) {
        if (p->data.empId == empId && sameMonth(p->data.ym, ym)) {
            if (result != NULL) {
                *result = p->data;
            }
            return 1;
        }
        p = p->next;
    }

    return 0;
}

/*
    打印链表中所有工资记录（简表）
*/
void printAllRecords(Node *head) {
    Node *p = head;

    if (p == NULL) {
        printf("当前没有工资记录！\n");
        return;
    }

    printf("\n================ 所有工资记录 ================\n");
    printf("%-8s %-10s %-8s %-8s %-12s %-12s\n",
           "工号", "姓名", "年份", "月份", "应发工资", "实发工资");

    while (p != NULL) {
        printf("%-8d %-10s %-8d %-8d %-12.2f %-12.2f\n",
               p->data.empId,
               p->data.name,
               p->data.ym.year,
               p->data.ym.month,
               p->data.grossPay,
               p->data.netPay);
        p = p->next;
    }
}

/*
    释放整个链表，防止内存泄漏
*/
void freeList(Node **head) {
    Node *p = *head;
    Node *temp;

    while (p != NULL) {
        temp = p;
        p = p->next;
        free(temp);
    }

    *head = NULL;
}


/* =====================================================
   工资计算核心部分
   ===================================================== */

/*
    按城市计算个人社保公积金
    说明：
    1. 这里只做课设简化版
    2. 使用基本工资作为缴费基数
*/
void calcSocialByCity(SalaryRecord *r) {
    double pensionRate = 0.08;
    double medicalRate = 0.02;
    double unemploymentRate = 0.005;
    double housingRate = 0.07;

    if (strcmp(r->city, "北京") == 0) {
        pensionRate = 0.08;
        medicalRate = 0.02;
        unemploymentRate = 0.002;
        housingRate = 0.12;
    } else if (strcmp(r->city, "上海") == 0) {
        pensionRate = 0.08;
        medicalRate = 0.02;
        unemploymentRate = 0.005;
        housingRate = 0.07;
    } else if (strcmp(r->city, "广州") == 0) {
        pensionRate = 0.08;
        medicalRate = 0.02;
        unemploymentRate = 0.002;
        housingRate = 0.05;
    } else if (strcmp(r->city, "深圳") == 0) {
        pensionRate = 0.08;
        medicalRate = 0.02;
        unemploymentRate = 0.003;
        housingRate = 0.05;
    }

    r->pension = r->basicSalary * pensionRate;
    r->medical = r->basicSalary * medicalRate;
    r->unemployment = r->basicSalary * unemploymentRate;
    r->housingFund = r->basicSalary * housingRate;

    r->socialTotal = r->pension + r->medical + r->unemployment + r->housingFund;
}

/*
    根据累计应纳税所得额返回税率和速算扣除数
*/
double getTaxRate(double taxable, double *quickDeduction) {
    if (taxable <= 36000) {
        *quickDeduction = 0;
        return 0.03;
    } else if (taxable <= 144000) {
        *quickDeduction = 2520;
        return 0.10;
    } else if (taxable <= 300000) {
        *quickDeduction = 16920;
        return 0.20;
    } else if (taxable <= 420000) {
        *quickDeduction = 31920;
        return 0.25;
    } else if (taxable <= 660000) {
        *quickDeduction = 52920;
        return 0.30;
    } else if (taxable <= 960000) {
        *quickDeduction = 85920;
        return 0.35;
    } else {
        *quickDeduction = 181920;
        return 0.45;
    }
}

/*
    计算当前月份个税（累计预扣法）
    逻辑：
    1. 遍历链表
    2. 找出同一员工、同一年、当前月份之前的数据
    3. 累加历史收入、历史社保、历史已缴税
    4. 再加上本月数据
    5. 按累计预扣法求出本月应缴税额
*/
double calcCurrentMonthTax(Node *head, SalaryRecord *current) {
    double cumulativeIncome = 0.0;      // 累计收入
    double cumulativeSocial = 0.0;      // 累计社保公积金
    double cumulativeTaxPaid = 0.0;     // 累计已缴个税
    int monthCount = 0;                 // 已计税月份数

    Node *p = head;

    while (p != NULL) {
        if (p->data.empId == current->empId &&
            p->data.ym.year == current->ym.year &&
            compareMonth(p->data.ym, current->ym) < 0) {

            cumulativeIncome += p->data.grossPay;
            cumulativeSocial += p->data.socialTotal;
            cumulativeTaxPaid += p->data.tax;
            monthCount++;
        }
        p = p->next;
    }

    /* 把当前月也加进去 */
    cumulativeIncome += current->grossPay;
    cumulativeSocial += current->socialTotal;
    monthCount++;

    /* 累计应纳税所得额 */
    double taxable = cumulativeIncome - cumulativeSocial - monthCount * 5000.0;
    if (taxable < 0) {
        taxable = 0;
    }

    double quickDeduction = 0.0;
    double rate = getTaxRate(taxable, &quickDeduction);

    double cumulativeTax = taxable * rate - quickDeduction;
    if (cumulativeTax < 0) {
        cumulativeTax = 0;
    }

    /* 本月个税 = 累计应缴税 - 之前已缴税 */
    double currentTax = cumulativeTax - cumulativeTaxPaid;
    if (currentTax < 0) {
        currentTax = 0;
    }

    return currentTax;
}

/*
    工资计算主函数
    参数：
    head         当前链表头指针，用于累计预扣个税时查找历史数据
    r            当前工资记录
    enableSocial 是否启用社保计算
    enableTax    是否启用个税计算
*/
void calculateSalary(Node *head, SalaryRecord *r, int enableSocial, int enableTax) {
    /* 应发工资 */
    r->grossPay = r->basicSalary + r->performance + r->bonus
                + r->allowance + r->penalty;

    /* 社保公积金 */
    if (enableSocial == 1) {
        calcSocialByCity(r);
    } else {
        r->pension = 0;
        r->medical = 0;
        r->unemployment = 0;
        r->housingFund = 0;
        r->socialTotal = 0;
    }

    /* 个税 */
    if (enableTax == 1) {
        r->tax = calcCurrentMonthTax(head, r);
    } else {
        r->tax = 0;
    }

    /* 实发工资 */
    r->netPay = r->grossPay - r->socialTotal - r->tax;
}


/* =====================================================
   输出函数
   ===================================================== */

/*
    打印一条工资单详细信息
*/
void printSalaryRecord(const SalaryRecord *r) {
    printf("\n========== 工资单信息 ==========\n");
    printf("员工编号: %d\n", r->empId);
    printf("姓名: %s\n", r->name);
    printf("城市: %s\n", r->city);
    printf("年月: %d-%02d\n", r->ym.year, r->ym.month);

    printf("\n----- 原始工资项 -----\n");
    printf("基本工资: %.2f\n", r->basicSalary);
    printf("绩效工资: %.2f\n", r->performance);
    printf("奖金: %.2f\n", r->bonus);
    printf("补贴: %.2f\n", r->allowance);
    printf("奖惩: %.2f\n", r->penalty);

    printf("\n----- 计算结果 -----\n");
    printf("应发工资: %.2f\n", r->grossPay);
    printf("养老保险: %.2f\n", r->pension);
    printf("医疗保险: %.2f\n", r->medical);
    printf("失业保险: %.2f\n", r->unemployment);
    printf("住房公积金: %.2f\n", r->housingFund);
    printf("社保公积金合计: %.2f\n", r->socialTotal);
    printf("个税: %.2f\n", r->tax);
    printf("实发工资: %.2f\n", r->netPay);
}


/* =====================================================
   业务功能部分
   ===================================================== */

/*
    录入工资并计算，再加入链表
*/
void inputSalary(Node **head) {
    SalaryRecord r;
    memset(&r, 0, sizeof(SalaryRecord));

    int enableSocial, enableTax;
    SalaryRecord temp;

    printf("输入员工编号: ");
    scanf("%d", &r.empId);
    clearBuffer();

    printf("输入员工姓名: ");
    fgets(r.name, sizeof(r.name), stdin);
    r.name[strcspn(r.name, "\n")] = '\0';

    printf("输入城市(北京/上海/广州/深圳): ");
    fgets(r.city, sizeof(r.city), stdin);
    r.city[strcspn(r.city, "\n")] = '\0';

    printf("输入年份 月份: ");
    scanf("%d %d", &r.ym.year, &r.ym.month);

    /* 防止重复录入同一个员工同一个月的工资 */
    if (findRecord(*head, r.empId, r.ym, &temp)) {
        printf("该员工该月工资单已存在，不能重复录入！\n");
        return;
    }

    printf("输入基本工资: ");
    scanf("%lf", &r.basicSalary);

    printf("输入绩效工资: ");
    scanf("%lf", &r.performance);

    printf("输入奖金: ");
    scanf("%lf", &r.bonus);

    printf("输入补贴: ");
    scanf("%lf", &r.allowance);

    printf("输入奖惩(可为负数，如 -100): ");
    scanf("%lf", &r.penalty);

    printf("是否自动计算社保公积金？(1是 0否): ");
    scanf("%d", &enableSocial);

    printf("是否自动计算个税？(1是 0否): ");
    scanf("%d", &enableTax);

    /* 先计算工资 */
    calculateSalary(*head, &r, enableSocial, enableTax);

    /* 再加入链表 */
    appendNode(head, r);

    printf("\n工资录入成功！\n");
    printSalaryRecord(&r);
}

/*
    查询某员工某月工资单
*/
void querySalary(Node *head) {
    int empId;
    YearMonth ym;
    SalaryRecord result;

    printf("输入员工编号: ");
    scanf("%d", &empId);

    printf("输入年份 月份: ");
    scanf("%d %d", &ym.year, &ym.month);

    if (findRecord(head, empId, ym, &result)) {
        printSalaryRecord(&result);
    } else {
        printf("未找到该工资单！\n");
    }
}


/* =====================================================
   文件读写部分
   ===================================================== */

/*
    将链表中的所有工资记录保存到二进制文件
    保存思路：
    1. 先统计记录数
    2. 先写入记录数
    3. 再逐条写入 SalaryRecord 结构体
*/
void saveToFile(Node *head, const char *filename) {
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        printf("文件打开失败，无法保存！\n");
        return;
    }

    /* 先统计链表长度 */
    int count = 0;
    Node *p = head;
    while (p != NULL) {
        count++;
        p = p->next;
    }

    /* 先写入记录总数 */
    fwrite(&count, sizeof(int), 1, fp);

    /* 再逐条写入结构体数据 */
    p = head;
    while (p != NULL) {
        fwrite(&(p->data), sizeof(SalaryRecord), 1, fp);
        p = p->next;
    }

    fclose(fp);
    printf("工资记录已保存到文件：%s\n", filename);
}

/*
    从二进制文件读取工资记录并重建链表
    读取思路：
    1. 先释放旧链表
    2. 打开文件
    3. 先读取记录数
    4. 循环读取每条工资记录
    5. 每读到一条就追加到链表中
*/
void loadFromFile(Node **head, const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("未找到历史工资文件，当前从空数据开始。\n");
        return;
    }

    /* 读取新数据前先释放旧链表 */
    freeList(head);

    int count = 0;
    int i;
    SalaryRecord r;

    fread(&count, sizeof(int), 1, fp);

    for (i = 0; i < count; i++) {
        if (fread(&r, sizeof(SalaryRecord), 1, fp) == 1) {
            appendNode(head, r);
        }
    }

    fclose(fp);
    printf("已从文件读取 %d 条工资记录。\n", count);
}


/* =====================================================
   菜单函数
   ===================================================== */

void menu() {
    printf("\n========== 工资管理系统 ==========\n");
    printf("1. 录入工资并自动计算\n");
    printf("2. 查询某员工某月工资单\n");
    printf("3. 查看所有工资记录\n");
    printf("4. 保存工资记录到文件\n");
    printf("5. 从文件读取工资记录\n");
    printf("0. 退出系统\n");
    printf("请选择: ");
}
