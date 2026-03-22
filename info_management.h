#ifndef INFO_MANAGEMENT_H
#define INFO_MANAGEMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN    50
#define MAX_ID_LEN      20
#define MAX_PHONE_LEN   20
#define MAX_ADDR_LEN   100
#define MAX_DATE_LEN    20
#define MAX_ID_CARD_LEN 20
#define MAX_EDU_LEN     30
#define MAX_EMAIL_LEN   50

typedef struct Department {
    char  deptId[MAX_ID_LEN];
    char  deptName[MAX_NAME_LEN];
    char  managerName[MAX_NAME_LEN];
    char  createDate[MAX_DATE_LEN];
    int   empCount;
    struct Department *next;
} Department;

typedef struct Position {
    char   posId[MAX_ID_LEN];
    char   posName[MAX_NAME_LEN];
    double minSalary;
    double maxSalary;
    struct Position *next;
} Position;

typedef struct Employee {
    char  empId[MAX_ID_LEN];
    char  empName[MAX_NAME_LEN];
    char  idCard[MAX_ID_CARD_LEN];
    char  hireDate[MAX_DATE_LEN];
    char  deptId[MAX_ID_LEN];
    char  posId[MAX_ID_LEN];
    char  phone[MAX_PHONE_LEN];
    char  email[MAX_EMAIL_LEN];
    char  address[MAX_ADDR_LEN];
    char  education[MAX_EDU_LEN];
    struct Employee *next;
} Employee;

extern Department *deptHead;
extern Position *posHead;
extern Employee *empHead;

void clearInputBuffer(void);
void printLine(void);

Department* findDeptById(const char *id);
void addDepartment(void);
void modifyDepartment(void);
void deleteDepartment(void);
void queryDepartment(void);

Position* findPosById(const char *id);
void addPosition(void);
void modifyPosition(void);
void deletePosition(void);
void queryPosition(void);

Employee* findEmpById(const char *id);
void addEmployee(void);
void modifyEmployee(void);
void deleteEmployee(void);
void queryEmployee(void);

void deptMenu(void);
void posMenu(void);
void empMenu(void);

#endif
