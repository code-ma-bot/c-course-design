#include "info_management.h"

Department *deptHead = NULL;
Position   *posHead  = NULL;
Employee   *empHead  = NULL;

void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void printLine(void) {
    printf("--------------------------------------------------\n");
}

Department* findDeptById(const char *id) {
    Department *p = deptHead;
    while (p != NULL) {
        if (strcmp(p->deptId, id) == 0) return p;
        p = p->next;
    }
    return NULL;
}

void addDepartment(void) {
    char tempId[MAX_ID_LEN];
    Department *newDept;
    Department *p;

    printLine();
    printf("【添加部门】\n");
    printf("请输入部门编号: ");
    scanf("%s", tempId);
    clearInputBuffer();

    if (findDeptById(tempId) != NULL) {
        printf("错误：部门编号 %s 已存在！\n", tempId);
        return;
    }

    newDept = (Department*)malloc(sizeof(Department));
    if (newDept == NULL) {
        printf("内存申请失败！\n");
        return;
    }

    strcpy(newDept->deptId, tempId);
    printf("请输入部门名称: ");
    scanf("%s", newDept->deptName);
    clearInputBuffer();
    printf("请输入部门主管姓名: ");
    scanf("%s", newDept->managerName);
    clearInputBuffer();
    printf("请输入成立时间: ");
    scanf("%s", newDept->createDate);
    clearInputBuffer();
    printf("请输入部门人数: ");
    scanf("%d", &newDept->empCount);
    clearInputBuffer();
    newDept->next = NULL;

    if (deptHead == NULL) {
        deptHead = newDept;
    } else {
        p = deptHead;
        while (p->next != NULL) p = p->next;
        p->next = newDept;
    }
    printf("部门添加成功！\n");
}

void modifyDepartment(void) {
    char id[MAX_ID_LEN];
    Department *dept;
    char buf[MAX_NAME_LEN];
    int newCount;

    printLine();
    printf("修改部门\n");
    printf("请输入要修改的部门编号: ");
    scanf("%s", id);
    clearInputBuffer();

    dept = findDeptById(id);
    if (dept == NULL) {
        printf("未找到编号为 %s 的部门！\n", id);
        return;
    }

    printf("找到部门: %s，请输入新信息（直接回车保留原值）：\n", dept->deptName);

    printf("新部门名称(原:%s): ", dept->deptName);
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = '\0';
    if (strlen(buf) > 0) strcpy(dept->deptName, buf);

    printf("新主管姓名(原:%s): ", dept->managerName);
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = '\0';
    if (strlen(buf) > 0) strcpy(dept->managerName, buf);

    printf("新成立时间(原:%s): ", dept->createDate);
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = '\0';
    if (strlen(buf) > 0) strcpy(dept->createDate, buf);

    printf("新部门人数(原:%d，输入-1保留): ", dept->empCount);
    scanf("%d", &newCount);
    clearInputBuffer();
    if (newCount != -1) dept->empCount = newCount;

    printf("部门信息修改成功！\n");
}

void deleteDepartment(void) {
    char id[MAX_ID_LEN];
    Department *p;
    Department *prev = NULL;

    printLine();
    printf("删除部门\n");
    printf("请输入要删除的部门编号: ");
    scanf("%s", id);
    clearInputBuffer();

    if (deptHead == NULL) {
        printf("当前没有部门数据！\n");
        return;
    }

    p = deptHead;
    while (p != NULL && strcmp(p->deptId, id) != 0) {
        prev = p;
        p = p->next;
    }

    if (p == NULL) {
        printf("未找到编号为 %s 的部门！\n", id);
        return;
    }

    if (prev == NULL) {
        deptHead = p->next;
    } else {
        prev->next = p->next;
    }
    printf("已删除部门: %s\n", p->deptName);
    free(p);
}

void queryDepartment(void) {
    char keyword[MAX_NAME_LEN];
    int found = 0;
    Department *p;

    printLine();
    printf("查询部门\n");
    printf("请输入查询内容（编号/名称，输入ALL查看全部）: ");
    scanf("%s", keyword);
    clearInputBuffer();

    p = deptHead;
    printf("\n%-10s %-15s %-12s %-15s %s\n",
           "部门编号", "部门名称", "主管", "成立时间", "人数");
    printLine();

    while (p != NULL) {
        if (strcmp(keyword, "ALL") == 0 ||
            strstr(p->deptId, keyword) != NULL ||
            strstr(p->deptName, keyword) != NULL) {
            printf("%-10s %-15s %-12s %-15s %d\n",
                   p->deptId, p->deptName, p->managerName,
                   p->createDate, p->empCount);
            found++;
        }
        p = p->next;
    }

    if (found == 0) printf("未查询到相关部门信息。\n");
    else printf("\n共查询到 %d 条记录。\n", found);
}

/* ==================== 职位管理 ==================== */
Position* findPosById(const char *id) {
    Position *p = posHead;
    while (p != NULL) {
        if (strcmp(p->posId, id) == 0) return p;
        p = p->next;
    }
    return NULL;
}

void addPosition(void) {
    char tempId[MAX_ID_LEN];
    Position *newPos;
    Position *p;

    printLine();
    printf("【添加职位】\n");
    printf("请输入职位编号: ");
    scanf("%s", tempId);
    clearInputBuffer();

    if (findPosById(tempId) != NULL) {
        printf("错误：职位编号 %s 已存在！\n", tempId);
        return;
    }

    newPos = (Position*)malloc(sizeof(Position));
    if (newPos == NULL) {
        printf("内存申请失败！\n");
        return;
    }

    strcpy(newPos->posId, tempId);
    printf("请输入职位名称: ");
    scanf("%s", newPos->posName);
    clearInputBuffer();
    printf("请输入最低工资: ");
    scanf("%lf", &newPos->minSalary);
    clearInputBuffer();
    printf("请输入最高工资: ");
    scanf("%lf", &newPos->maxSalary);
    clearInputBuffer();
    newPos->next = NULL;

    if (posHead == NULL) {
        posHead = newPos;
    } else {
        p = posHead;
        while (p->next != NULL) p = p->next;
        p->next = newPos;
    }
    printf("职位添加成功！\n");
}

void modifyPosition(void) {
    char id[MAX_ID_LEN];
    Position *pos;
    char buf[MAX_NAME_LEN];
    double val;

    printLine();
    printf("修改职位\n");
    printf("请输入要修改的职位编号: ");
    scanf("%s", id);
    clearInputBuffer();

    pos = findPosById(id);
    if (pos == NULL) {
        printf("未找到编号为 %s 的职位！\n", id);
        return;
    }

    printf("找到职位: %s，请输入新信息：\n", pos->posName);

    printf("新职位名称(原:%s): ", pos->posName);
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = '\0';
    if (strlen(buf) > 0) strcpy(pos->posName, buf);

    printf("新最低工资(原:%.2f，输入-1保留): ", pos->minSalary);
    scanf("%lf", &val);
    clearInputBuffer();
    if (val != -1) pos->minSalary = val;

    printf("新最高工资(原:%.2f，输入-1保留): ", pos->maxSalary);
    scanf("%lf", &val);
    clearInputBuffer();
    if (val != -1) pos->maxSalary = val;

    printf("职位信息修改成功！\n");
}

void deletePosition(void) {
    char id[MAX_ID_LEN];
    Position *p;
    Position *prev = NULL;

    printLine();
    printf("删除职位\n");
    printf("请输入要删除的职位编号: ");
    scanf("%s", id);
    clearInputBuffer();

    if (posHead == NULL) {
        printf("当前没有职位数据！\n");
        return;
    }

    p = posHead;
    while (p != NULL && strcmp(p->posId, id) != 0) {
        prev = p;
        p = p->next;
    }

    if (p == NULL) {
        printf("未找到编号为 %s 的职位！\n", id);
        return;
    }

    if (prev == NULL) posHead = p->next;
    else prev->next = p->next;

    printf("已删除职位: %s\n", p->posName);
    free(p);
}

void queryPosition(void) {
    char keyword[MAX_NAME_LEN];
    int found = 0;
    Position *p;

    printLine();
    printf("查询职位\n");
    printf("请输入查询内容（编号/名称，输入ALL查看全部）: ");
    scanf("%s", keyword);
    clearInputBuffer();

    p = posHead;
    printf("\n%-10s %-15s %-12s %s\n", "职位编号", "职位名称", "最低工资", "最高工资");
    printLine();

    while (p != NULL) {
        if (strcmp(keyword, "ALL") == 0 ||
            strstr(p->posId, keyword) != NULL ||
            strstr(p->posName, keyword) != NULL) {
            printf("%-10s %-15s %-12.2f %.2f\n",
                   p->posId, p->posName, p->minSalary, p->maxSalary);
            found++;
        }
        p = p->next;
    }

    if (found == 0) printf("未查询到相关职位信息。\n");
    else printf("\n共查询到 %d 条记录。\n", found);
}

/* ==================== 员工管理 ==================== */
Employee* findEmpById(const char *id) {
    Employee *p = empHead;
    while (p != NULL) {
        if (strcmp(p->empId, id) == 0) return p;
        p = p->next;
    }
    return NULL;
}

void addEmployee(void) {
    char tempId[MAX_ID_LEN];
    Employee *newEmp;
    Employee *p;

    printLine();
    printf("添加员工\n");
    printf("请输入员工编号: ");
    scanf("%s", tempId);
    clearInputBuffer();

    if (findEmpById(tempId) != NULL) {
        printf("错误：员工编号 %s 已存在！\n", tempId);
        return;
    }

    newEmp = (Employee*)malloc(sizeof(Employee));
    if (newEmp == NULL) {
        printf("内存申请失败！\n");
        return;
    }

    strcpy(newEmp->empId, tempId);
    printf("请输入员工姓名: ");
    scanf("%s", newEmp->empName);
    clearInputBuffer();
    printf("请输入身份证号码: ");
    scanf("%s", newEmp->idCard);
    clearInputBuffer();
    printf("请输入入职时间(如2022-06-01): ");
    scanf("%s", newEmp->hireDate);
    clearInputBuffer();
    printf("请输入所属部门编号: ");
    scanf("%s", newEmp->deptId);
    clearInputBuffer();
    if (findDeptById(newEmp->deptId) == NULL) {
        printf("警告：部门编号 %s 不存在，请先添加该部门！\n", newEmp->deptId);
        free(newEmp);
        return;
    }

    printf("请输入所属职位编号: ");
    scanf("%s", newEmp->posId);
    clearInputBuffer();
    if (findPosById(newEmp->posId) == NULL) {
        printf("警告：职位编号 %s 不存在，请先添加该职位！\n", newEmp->posId);
        free(newEmp);
        return;
    }

    printf("请输入联系电话: ");
    scanf("%s", newEmp->phone);
    clearInputBuffer();
    printf("请输入电子邮箱: ");
    scanf("%s", newEmp->email);
    clearInputBuffer();
    printf("请输入家庭住址: ");
    fgets(newEmp->address, sizeof(newEmp->address), stdin);
    newEmp->address[strcspn(newEmp->address, "\n")] = '\0';
    printf("请输入最终学历(如本科/硕士): ");
    scanf("%s", newEmp->education);
    clearInputBuffer();
    newEmp->next = NULL;

    if (empHead == NULL) {
        empHead = newEmp;
    } else {
        p = empHead;
        while (p->next != NULL) p = p->next;
        p->next = newEmp;
    }
    printf("员工添加成功！\n");
}

void modifyEmployee(void) {
    char id[MAX_ID_LEN];
    Employee *emp;
    char buf[MAX_ADDR_LEN];

    printLine();
    printf("修改员工信息\n");
    printf("请输入要修改的员工编号: ");
    scanf("%s", id);
    clearInputBuffer();

    emp = findEmpById(id);
    if (emp == NULL) {
        printf("未找到编号为 %s 的员工！\n", id);
        return;
    }

    printf("找到员工: %s，请输入新信息（直接回车保留原值）：\n", emp->empName);

    printf("新姓名(原:%s): ", emp->empName);
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = '\0';
    if (strlen(buf) > 0) strcpy(emp->empName, buf);

    printf("新联系电话(原:%s): ", emp->phone);
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = '\0';
    if (strlen(buf) > 0) strcpy(emp->phone, buf);

    printf("新电子邮箱(原:%s): ", emp->email);
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = '\0';
    if (strlen(buf) > 0) strcpy(emp->email, buf);

    printf("新家庭住址(原:%s): ", emp->address);
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = '\0';
    if (strlen(buf) > 0) strcpy(emp->address, buf);

    printf("新最终学历(原:%s): ", emp->education);
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = '\0';
    if (strlen(buf) > 0) strcpy(emp->education, buf);

    printf("员工信息修改成功！\n");
}

void deleteEmployee(void) {
    char id[MAX_ID_LEN];
    Employee *p;
    Employee *prev = NULL;

    printLine();
    printf("删除员工\n");
    printf("请输入要删除的员工编号: ");
    scanf("%s", id);
    clearInputBuffer();

    if (empHead == NULL) {
        printf("当前没有员工数据！\n");
        return;
    }

    p = empHead;
    while (p != NULL && strcmp(p->empId, id) != 0) {
        prev = p;
        p = p->next;
    }

    if (p == NULL) {
        printf("未找到编号为 %s 的员工！\n", id);
        return;
    }

    if (prev == NULL) empHead = p->next;
    else prev->next = p->next;

    printf("已删除员工: %s\n", p->empName);
    free(p);
}

void queryEmployee(void) {
    int choice;
    char keyword[MAX_NAME_LEN] = "";
    int found = 0;
    Employee *p;
    int match;

    printLine();
    printf("查询员工\n");
    printf("请选择查询方式:\n");
    printf("  1 - 按员工编号查询\n");
    printf("  2 - 按员工姓名查询\n");
    printf("  3 - 按所属部门编号查询\n");
    printf("  4 - 按所属职位编号查询\n");
    printf("  5 - 显示全部员工\n");
    printf("请输入选择: ");
    scanf("%d", &choice);
    clearInputBuffer();

    if (choice >= 1 && choice <= 4) {
        printf("请输入关键词: ");
        scanf("%s", keyword);
        clearInputBuffer();
    }

    p = empHead;
    printf("\n%-8s %-10s %-10s %-12s %-8s %-8s %-12s %s\n",
           "编号", "姓名", "部门编号", "职位编号", "电话", "学历", "入职时间", "邮箱");
    printLine();

    while (p != NULL) {
        match = 0;
        switch (choice) {
            case 1: match = (strstr(p->empId, keyword) != NULL); break;
            case 2: match = (strstr(p->empName, keyword) != NULL); break;
            case 3: match = (strstr(p->deptId, keyword) != NULL); break;
            case 4: match = (strstr(p->posId, keyword) != NULL); break;
            case 5: match = 1; break;
            default: match = 0; break;
        }
        if (match) {
            printf("%-8s %-10s %-10s %-12s %-8s %-8s %-12s %s\n",
                   p->empId, p->empName, p->deptId, p->posId,
                   p->phone, p->education, p->hireDate, p->email);
            found++;
        }
        p = p->next;
    }

    if (found == 0) printf("未查询到相关员工信息。\n");
    else printf("\n共查询到 %d 条记录。\n", found);
}

/* ==================== 菜单函数 ==================== */
void deptMenu(void) {
    int choice;
    do {
        printLine();
        printf("部门管理\n");
        printf("  1 - 添加部门\n");
        printf("  2 - 修改部门\n");
        printf("  3 - 删除部门\n");
        printf("  4 - 查询部门\n");
        printf("  0 - 返回上级菜单\n");
        printLine();
        printf("请输入选择: ");
        scanf("%d", &choice);
        clearInputBuffer();
        switch (choice) {
            case 1: addDepartment();    break;
            case 2: modifyDepartment(); break;
            case 3: deleteDepartment(); break;
            case 4: queryDepartment();  break;
            case 0: printf("返回上级菜单...\n"); break;
            default: printf("无效输入，请重新选择！\n");
        }
    } while (choice != 0);
}

void posMenu(void) {
    int choice;
    do {
        printLine();
        printf("职位管理\n");
        printf("  1 - 添加职位\n");
        printf("  2 - 修改职位\n");
        printf("  3 - 删除职位\n");
        printf("  4 - 查询职位\n");
        printf("  0 - 返回上级菜单\n");
        printLine();
        printf("请输入选择: ");
        scanf("%d", &choice);
        clearInputBuffer();
        switch (choice) {
            case 1: addPosition();    break;
            case 2: modifyPosition(); break;
            case 3: deletePosition(); break;
            case 4: queryPosition();  break;
            case 0: printf("返回上级菜单...\n"); break;
            default: printf("无效输入，请重新选择！\n");
        }
    } while (choice != 0);
}

void empMenu(void) {
    int choice;
    do {
        printLine();
        printf("员工信息管理\n");
        printf("  1 - 添加员工\n");
        printf("  2 - 修改员工信息\n");
        printf("  3 - 删除员工\n");
        printf("  4 - 查询员工\n");
        printf("  0 - 返回上级菜单\n");
        printLine();
        printf("请输入选择: ");
        scanf("%d", &choice);
        clearInputBuffer();
        switch (choice) {
            case 1: addEmployee();    break;
            case 2: modifyEmployee(); break;
            case 3: deleteEmployee(); break;
            case 4: queryEmployee();  break;
            case 0: printf("返回上级菜单...\n"); break;
            default: printf("无效输入，请重新选择！\n");
        }
    } while (choice != 0);
}
