/**
 * @file maintenance.c
 * @brief 系统维护模块：密码管理、数据备份与恢复
 * @author 你的名字
 * @date 2026-03-22
 */

#include "system.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 定义数据文件存放路径（可根据实际情况调整）
#define DATA_DIR "."                     // 当前目录
#define BACKUP_DIR "./backup"            // 备份目录
#define ADMIN_PASSWORD_FILE "./admin.dat" // 管理员密码文件（简化示例）

// 辅助函数：复制文件（二进制方式）
static int copy_file(const char *src, const char *dst) {
    FILE *fsrc = fopen(src, "rb");
    if (!fsrc) return 0;                // 源文件不存在或无法打开
    FILE *fdst = fopen(dst, "wb");
    if (!fdst) {
        fclose(fsrc);
        return 0;
    }
    char buffer[4096];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), fsrc)) > 0) {
        if (fwrite(buffer, 1, bytes, fdst) != bytes) {
            fclose(fsrc);
            fclose(fdst);
            return 0;
        }
    }
    fclose(fsrc);
    fclose(fdst);
    return 1;
}

// 辅助函数：创建备份目录（Windows 下使用 system("mkdir ...")）
static void ensure_backup_dir() {
    #ifdef _WIN32
        system("if not exist " BACKUP_DIR " mkdir " BACKUP_DIR);
    #else
        system("mkdir -p " BACKUP_DIR);
    #endif
}

/*-------------------------- 修改密码 --------------------------*/
/**
 * @brief 修改用户密码（支持员工或管理员）
 * @param user_id 用户ID（员工ID或管理员ID，由role决定）
 * @param role    角色：0-员工，1-管理员
 * @note 员工密码存储在 Employee 结构体的 password 字段中；
 *       管理员密码单独存储在 admin.dat 文件中。
 */
void change_password(int user_id, int role) {
    if (role == 0) { // 员工修改密码
        // 根据员工ID查找员工
        Employee *emp = NULL;
        Employee *p = emp_head;
        while (p) {
            if (p->id == user_id) {
                emp = p;
                break;
            }
            p = p->next;
        }
        if (emp == NULL) {
            printf("错误：未找到员工ID为 %d 的员工。\n", user_id);
            return;
        }

        char old_pass[20];
        char new_pass[20];
        char confirm[20];

        printf("请输入原密码：");
        scanf("%s", old_pass);
        if (strcmp(emp->password, old_pass) != 0) {
            printf("原密码错误，密码修改失败。\n");
            return;
        }

        printf("请输入新密码：");
        scanf("%s", new_pass);
        printf("请确认新密码：");
        scanf("%s", confirm);

        if (strcmp(new_pass, confirm) != 0) {
            printf("两次输入的新密码不一致，修改失败。\n");
            return;
        }

        strcpy(emp->password, new_pass);
        printf("密码修改成功！\n");
    }
    else if (role == 1) { // 管理员修改密码
        // 从文件中读取当前管理员密码
        FILE *fp = fopen(ADMIN_PASSWORD_FILE, "rb");
        char stored_pass[20] = "";
        if (fp) {
            fgets(stored_pass, sizeof(stored_pass), fp);
            fclose(fp);
            // 去除可能存在的换行符
            size_t len = strlen(stored_pass);
            if (len > 0 && stored_pass[len-1] == '\n')
                stored_pass[len-1] = '\0';
        }

        char old_pass[20];
        printf("请输入原密码：");
        scanf("%s", old_pass);
        if (strcmp(old_pass, stored_pass) != 0) {
            printf("原密码错误，修改失败。\n");
            return;
        }

        char new_pass[20];
        char confirm[20];
        printf("请输入新密码：");
        scanf("%s", new_pass);
        printf("请确认新密码：");
        scanf("%s", confirm);
        if (strcmp(new_pass, confirm) != 0) {
            printf("两次输入的新密码不一致，修改失败。\n");
            return;
        }

        fp = fopen(ADMIN_PASSWORD_FILE, "w");
        if (fp) {
            fprintf(fp, "%s\n", new_pass);
            fclose(fp);
            printf("管理员密码修改成功！\n");
        } else {
            printf("无法写入密码文件，修改失败。\n");
        }
    }
    else {
        printf("无效角色。\n");
    }
}

/*-------------------------- 重置员工密码 --------------------------*/
/**
 * @brief 管理员重置员工密码为默认值（例如 "123456"）
 * @param emp_id 员工编号
 */
void reset_employee_password(int emp_id) {
    Employee *emp = NULL;
    Employee *p = emp_head;
    while (p) {
        if (p->id == emp_id) {
            emp = p;
            break;
        }
        p = p->next;
    }
    if (emp == NULL) {
        printf("错误：未找到员工ID为 %d 的员工。\n", emp_id);
        return;
    }

    // 默认密码可自定义
    strcpy(emp->password, "123456");
    printf("员工 %s (ID:%d) 的密码已重置为 123456。\n", emp->name, emp_id);
}

/*-------------------------- 数据备份 --------------------------*/
/**
 * @brief 将当前所有数据文件备份到备份目录
 * @note 备份的文件名需与项目中实际使用的文件名一致，可根据需要调整。
 */
void backup_data() {
    ensure_backup_dir();  // 确保备份目录存在

    // 定义需要备份的文件列表（应与 save_all_data 中使用的文件名一致）
    const char *files[] = {
        "departments.dat",
        "positions.dat",
        "employees.dat",
        "transfers.dat",
        "salaries.dat"
    };
    int file_count = sizeof(files) / sizeof(files[0]);

    int success = 1;
    for (int i = 0; i < file_count; i++) {
        char src_path[256];
        char dst_path[256];
        snprintf(src_path, sizeof(src_path), "%s/%s", DATA_DIR, files[i]);
        snprintf(dst_path, sizeof(dst_path), "%s/%s", BACKUP_DIR, files[i]);

        if (copy_file(src_path, dst_path)) {
            printf("备份 %s 成功。\n", files[i]);
        } else {
            // 如果文件不存在，可能尚未生成，不算严重错误
            printf("警告：文件 %s 不存在或备份失败。\n", files[i]);
            success = 0;
        }
    }

    if (success) {
        printf("数据备份完成！备份目录：%s\n", BACKUP_DIR);
    } else {
        printf("部分文件备份失败，请检查。\n");
    }
}

/*-------------------------- 数据恢复 --------------------------*/
/**
 * @brief 从备份目录恢复数据文件到当前目录
 * @note 恢复操作会覆盖当前数据文件，请谨慎使用。
 */
void restore_data() {
    // 定义需要恢复的文件列表（与备份列表一致）
    const char *files[] = {
        "departments.dat",
        "positions.dat",
        "employees.dat",
        "transfers.dat",
        "salaries.dat"
    };
    int file_count = sizeof(files) / sizeof(files[0]);

    int success = 1;
    for (int i = 0; i < file_count; i++) {
        char src_path[256];
        char dst_path[256];
        snprintf(src_path, sizeof(src_path), "%s/%s", BACKUP_DIR, files[i]);
        snprintf(dst_path, sizeof(dst_path), "%s/%s", DATA_DIR, files[i]);

        if (copy_file(src_path, dst_path)) {
            printf("恢复 %s 成功。\n", files[i]);
        } else {
            printf("错误：文件 %s 在备份目录中不存在或恢复失败。\n", files[i]);
            success = 0;
        }
    }

    if (success) {
        printf("数据恢复完成！建议重新启动程序以加载最新数据。\n");
    } else {
        printf("恢复失败，请检查备份目录是否存在且包含完整备份文件。\n");
    }
}
