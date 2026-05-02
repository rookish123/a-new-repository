#define _CRT_SECURE_NO_WARNINGS 1
#include "bank.h"

/* 全局变量定义 */
RECORD rs[N_MAXREC_NO];
int maxAccount = -1;

/* 读文件 */
int readData(void) {
    FILE* fp;
    char cLineStr[256];
    int i = 0, j, k, length;
    char cData[64];
    char cTmp[4];

    fp = fopen(FILE_PATH, "r");
    if (fp == NULL) return -1;

    while (!feof(fp) && fgets(cLineStr, sizeof(cLineStr), fp) != NULL) {
        length = (int)strlen(cLineStr);
        if (length <= 1) continue;
        j = 0;
        cData[0] = '\0';
        for (k = 0; k < length; k++) {
            memset(cTmp, 0, sizeof(cTmp));
            memcpy(cTmp, &cLineStr[k], 1);
            if (cTmp[0] != ',') {
                strcat(cData, cTmp);
            }
            else {
                if (j == 0) {
                    rs[i].accNumber = atoi(cData);
                }
                else if (j == 1) {
                    rightTrim(cData);
                    strcpy(rs[i].accName, cData);
                }
                else if (j == 2) {
                    strcpy(rs[i].accPWD, cData);
                }
                else if (j == 3) {
                    rs[i].balance = atof(cData);
                }
                else if (j == 4) {
                    rightTrim(cData);
                    strcpy(rs[i].flag, cData);
                }
                cData[0] = '\0';
                j++;
            }
        }
        i++;
        if (i >= N_MAXREC_NO) break;
    }
    fclose(fp);
    return (i - 1);
}

/* 写文件 */
int writeData(void) {
    FILE* fp;
    int i;

    fp = fopen(FILE_PATH, "w");
    if (fp == NULL) {
        printf("无法打开文件进行写入！\n");
        return 0;
    }
    for (i = 0; i <= maxAccount; i++) {
        fprintf(fp, "%-5d,%-8s,%-6s,%12.2f,%-1s,\n",
            rs[i].accNumber, rs[i].accName,
            rs[i].accPWD, rs[i].balance, rs[i].flag);
    }
    fclose(fp);
    return 1;
}

/* 生成新账号 */
int createAccNumber(void) {
    if (maxAccount == -1)
        return 10001;
    else
        return rs[maxAccount].accNumber + 1;
}

/* 根据账号查找下标 */
int findAnAccount(int accNumber) {
    int i;
    for (i = 0; i <= maxAccount; i++) {
        if (rs[i].accNumber == accNumber)
            return i;
    }
    return -1;
}

/* 显示单个账户信息 */
void dispAnAccount(int i) {
    printf("+-------+----------+--------+-------------+------+\n");
    printf("| %-5s | %-8s | %-6s | %-12s| %4s |\n",
        "账号", "户名", "密码", "余额", "状态");
    printf("+-------+----------+--------+-------------+------+\n");
    printf("| %5d | %-8s | %-6s | %12.2f | %4s |\n",
        rs[i].accNumber, rs[i].accName, rs[i].accPWD,
        rs[i].balance, (atoi(rs[i].flag) ? "销户" : "正常"));
    printf("+-------+----------+--------+-------------+------+\n");
}

/* 显示所有账户 */
void queryall(void) {
    int i;
    if (maxAccount == -1) {
        printf("当前没有任何储户信息！\n");
        return;
    }
    printf("\n------------------- 全部账户信息 -------------------\n");
    for (i = 0; i <= maxAccount; i++) {
        dispAnAccount(i);
    }
}

/* 输入合法性检查 */
int checkInput(int flag, char* checkStr) {
    int len = (int)strlen(checkStr);
    if (flag == 0) {
        if (len > 8 || len == 0) return 1;
    }
    else if (flag == 1) {
        if (len > 6 || len == 0) return 1;
    }
    else if (flag == 2) {
        int i, hasDot = 0;
        if (len == 0 || len > 11) return 1;
        for (i = 0; i < len; i++) {
            if (checkStr[i] == '.') {
                if (hasDot) return 1;
                hasDot = 1;
            }
            else if (!isdigit(checkStr[i])) {
                return 1;
            }
        }
    }
    return 0;
}

/* 去除尾部空格、回车换行 */
char* rightTrim(char* str) {
    int len = (int)strlen(str);
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\r' || str[len - 1] == '\n')) {
        str[--len] = '\0';
    }
    return str;
}

/* 安全读取户名 */
void getaccName(char accName[]) {
    char caccName[20];
    int ok = 0;
    while (!ok) {
        memset(caccName, 0, sizeof(caccName));
        printf("\n请输入户名：");
        scanf("%s", caccName);
        while (getchar() != '\n');
        if (checkInput(0, caccName)) {
            printf("超过8位的最大户名长度！\n");
        }
        else {
            ok = 1;
        }
    }
    memcpy(accName, caccName, 9);
}