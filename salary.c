#include "salary.h"

// 打印菜单
void menu() {
    printf("\n================ 工资管理系统 ================\n");
    printf("1. 录入员工工资\n");
    printf("2. 打印所有工资记录\n");
    printf("3. 调整员工工资 (管理员功能)\n");
    printf("4. 部门平均工资统计 (管理员功能)\n");
    printf("5. 员工年度账单统计 (员工功能)\n");
    printf("6. 按实发工资从高到低排序\n");
    printf("0. 退出系统\n");
    printf("==============================================\n");
    printf("请选择操作: ");
}

// 清除输入缓冲区
void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// 判断年月是否相同
int sameMonth(YearMonth ym1, YearMonth ym2) {
    return (ym1.year == ym2.year && ym1.month == ym2.month);
}

// 打印单条记录
void printSalaryRecord(SalaryRecord *record) {
    printf("工号:%d | 姓名:%s | 部门:%s | 职位:%s | %d年%d月 | 应发:%.2f | 实发:%.2f\n",
           record->empId, record->name, record->department, record->position,
           record->ym.year, record->ym.month, record->grossPay, record->netPay);
}

// 计算工资单 (保留你的核心逻辑，这里做基础演示)
void calculateSalary(Node *head, SalaryRecord *record, int calcSS, int calcTax) {
    // 应发 = 基本 + 绩效 + 奖惩
    record->grossPay = record->basicSalary + record->performance + record->penalty;
    
    // 假设简易社保和个税计算 (你可以替换回你原本的累计预扣法逻辑)
    if (calcSS) record->socialSecurity = record->grossPay * 0.1; // 假设社保扣10%
    else record->socialSecurity = 0;

    if (calcTax && (record->grossPay - record->socialSecurity > 5000)) {
        record->tax = (record->grossPay - record->socialSecurity - 5000) * 0.03; // 假设超5000扣3%
    } else {
        record->tax = 0;
    }

    // 实发 = 应发 - 社保 - 个税
    record->netPay = record->grossPay - record->socialSecurity - record->tax;
}

// 1. 录入工资
Node* inputSalary(Node *head) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->next = NULL;

    printf("请输入员工编号: ");
    scanf("%d", &newNode->data.empId);
    clearBuffer();
    printf("请输入姓名: ");
    scanf("%s", newNode->data.name);
    printf("请输入部门: ");
    scanf("%s", newNode->data.department);
    printf("请输入职位: ");
    scanf("%s", newNode->data.position);
    printf("请输入城市: ");
    scanf("%s", newNode->data.city);
    printf("请输入年份和月份 (例如 2023 5): ");
    scanf("%d %d", &newNode->data.ym.year, &newNode->data.ym.month);
    printf("请输入基本工资: ");
    scanf("%lf", &newNode->data.basicSalary);
    printf("请输入绩效工资: ");
    scanf("%lf", &newNode->data.performance);
    printf("请输入奖罚金额 (负数表示罚款): ");
    scanf("%lf", &newNode->data.penalty);

    // 计算应发和实发
    calculateSalary(head, &(newNode->data), 1, 1);

    // 头插法加入链表
    newNode->next = head;
    printf("录入成功！\n");
    return newNode;
}

// 2. 打印所有记录
void printAll(Node *head) {
    Node *p = head;
    if (p == NULL) {
        printf("当前没有工资记录！\n");
        return;
    }
    printf("\n--- 所有工资记录 ---\n");
    while (p != NULL) {
        printSalaryRecord(&(p->data));
        p = p->next;
    }
}

// 3. 调整员工工资 (管理员)
void modifySalary(Node *head) {
    int empId;
    YearMonth ym;
    Node *p = head;
    
    printf("输入要修改的员工编号: ");
    scanf("%d", &empId);
    printf("输入要修改的年份 月份: ");
    scanf("%d %d", &ym.year, &ym.month);

    while (p != NULL) {
        if (p->data.empId == empId && sameMonth(p->data.ym, ym)) {
            printf("\n找到记录！当前基本工资: %.2f, 绩效: %.2f, 奖惩: %.2f\n", 
                   p->data.basicSalary, p->data.performance, p->data.penalty);
            
            printf("输入新的基本工资(不修改填原值): ");
            scanf("%lf", &p->data.basicSalary);
            printf("输入新的绩效工资: ");
            scanf("%lf", &p->data.performance);
            printf("输入新的奖惩金额: ");
            scanf("%lf", &p->data.penalty);

            calculateSalary(head, &(p->data), 1, 1); // 重新计算
            
            printf("修改成功！最新的工资单如下：\n");
            printSalaryRecord(&(p->data));
            return;
        }
        p = p->next;
    }
    printf("未找到该员工该月的工资记录！\n");
}

// 4. 部门平均工资统计 (管理员)
void adminStatistics(Node *head) {
    char targetDept[30];
    YearMonth targetYm;
    int count = 0;
    double totalGross = 0.0;
    Node *p = head;

    printf("请输入要统计的部门名称: ");
    scanf("%s", targetDept);
    printf("请输入统计的年份 月份: ");
    scanf("%d %d", &targetYm.year, &targetYm.month);

    while (p != NULL) {
        if (strcmp(p->data.department, targetDept) == 0 && sameMonth(p->data.ym, targetYm)) {
            totalGross += p->data.grossPay;
            count++;
        }
        p = p->next;
    }

    if (count > 0) {
        printf("\n【统计结果】 %d年%d月 [%s] 部门共有 %d 人记录，平均应发工资为: %.2f 元\n", 
               targetYm.year, targetYm.month, targetDept, count, totalGross / count);
    } else {
        printf("\n未找到该部门该月的相关记录。\n");
    }
}

// 5. 员工年度账单统计 (员工)
void employeeStatistics(Node *head) {
    int empId, targetYear;
    double yearlyIncome = 0.0;
    double yearlyPenalty = 0.0;
    int count = 0;
    Node *p = head;

    printf("请输入你的员工编号: ");
    scanf("%d", &empId);
    printf("请输入要统计的年份: ");
    scanf("%d", &targetYear);

    while (p != NULL) {
        if (p->data.empId == empId && p->data.ym.year == targetYear) {
            yearlyIncome += p->data.netPay; 
            yearlyPenalty += p->data.penalty; 
            count++;
        }
        p = p->next;
    }

    if (count > 0) {
        printf("\n【年度账单】 员工[%d]在 %d 年共发放 %d 个月工资。\n", empId, targetYear, count);
        printf("总实发收入: %.2f 元\n", yearlyIncome);
        printf("累计奖惩额: %.2f 元\n", yearlyPenalty);
    } else {
        printf("\n未查到该年份的工资记录。\n");
    }
}

// 6. 链表排序：按实发工资从高到低排序 (冒泡法)
void sortSalaryDescending(Node *head) {
    if (head == NULL || head->next == NULL) {
        printf("记录太少，无需排序。\n");
        return;
    }
    
    int swapped;
    Node *ptr1;
    Node *lptr = NULL; // 标记已排序的部分

    do {
        swapped = 0;
        ptr1 = head;

        while (ptr1->next != lptr) {
            if (ptr1->data.netPay < ptr1->next->data.netPay) {
                // 交换数据域
                SalaryRecord temp = ptr1->data;
                ptr1->data = ptr1->next->data;
                ptr1->next->data = temp;
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);

    printf("排序完成！请使用菜单选项2(打印)查看从高到低的工资排行。\n");
}
