#define _CRT_SECURE_NO_WARNINGS 1
#include "bank.h"

/* 主菜单 */
void mainmenu(void) {
    int choice;

    maxAccount = readData();   /* 启动时加载数据 */

    while (1) {
        printf("\n");
        printf("*************** 储蓄综合业务平台 ***************\n");
        printf("*  1. 存款                                    *\n");
        printf("*  2. 取款                                    *\n");
        printf("*  3. 查询                                    *\n");
        printf("*  4. 开户                                    *\n");
        printf("*  5. 销户                                    *\n");
        printf("*  6. 显示所有账户                             *\n");
        printf("*  0. 退出                                    *\n");
        printf("************************************************\n");
        printf("请选择操作（0-6）：");
        scanf("%d", &choice);
        while (getchar() != '\n');

        switch (choice) {
        case 0:
            printf("感谢使用，再见！\n");
            exit(0);
        case 1: deposit();      break;
        case 2: withdraw();     break;
        case 3: query();        break;
        case 4: openAnAccount(); break;
        case 5: closeAnAcount(); break;
        case 6: queryall();     break;
        default:
            printf("无效选项，请重新输入！\n");
        }
    }
}

/* 开户 */
void openAnAccount(void) {
    int newIdx;
    char accName[9], accPWD[7], cAmount[20];
    double cash;

    if (maxAccount >= N_MAXREC_NO - 1) {
        printf("系统已满，无法开立新账户！\n");
        return;
    }

    newIdx = maxAccount + 1;
    rs[newIdx].accNumber = createAccNumber();

    getaccName(accName);
    strcpy(rs[newIdx].accName, accName);

    printf("请输入密码（6位以内）：");
    scanf("%6s", accPWD);
    while (getchar() != '\n');
    while (checkInput(1, accPWD)) {
        printf("密码长度不能超过6位，请重新输入：");
        scanf("%6s", accPWD);
        while (getchar() != '\n');
    }
    strcpy(rs[newIdx].accPWD, accPWD);

    printf("请输入存入金额：");
    scanf("%s", cAmount);
    while (getchar() != '\n');
    while (checkInput(2, cAmount)) {
        printf("金额格式不正确，请重新输入：");
        scanf("%s", cAmount);
        while (getchar() != '\n');
    }
    cash = atof(cAmount);
    rs[newIdx].balance = cash;
    strcpy(rs[newIdx].flag, "0");

    maxAccount = newIdx;
    writeData();

    printf("\n开户成功！储户信息如下：\n");
    dispAnAccount(newIdx);
}

/* 存款 */
void deposit(void) {
    int accNumber, pos;
    double cash;
    char cAmount[20];

    printf("请输入存款账号：");
    scanf("%d", &accNumber);
    while (getchar() != '\n');

    pos = findAnAccount(accNumber);
    if (pos == -1) {
        printf("该账户不存在，返回主界面。\n");
        return;
    }
    if (atoi(rs[pos].flag) != 0) {
        printf("该账户状态异常（已销户），无法存款。\n");
        return;
    }

    printf("请输入存款金额：");
    scanf("%s", cAmount);
    while (getchar() != '\n');
    while (checkInput(2, cAmount)) {
        printf("金额格式不正确，请重新输入：");
        scanf("%s", cAmount);
        while (getchar() != '\n');
    }
    cash = atof(cAmount);
    rs[pos].balance += cash;
    writeData();
    printf("存款成功！当前余额：%.2f\n", rs[pos].balance);
}

/* 取款 */
void withdraw(void) {
    int accNumber, pos;
    char pwd[7], cAmount[20];
    double cash;

    printf("请输入取款账号：");
    scanf("%d", &accNumber);
    while (getchar() != '\n');

    pos = findAnAccount(accNumber);
    if (pos == -1) {
        printf("该账户不存在，返回主界面。\n");
        return;
    }

    printf("请输入密码：");
    scanf("%6s", pwd);
    while (getchar() != '\n');
    if (strcmp(rs[pos].accPWD, pwd) != 0) {
        printf("密码错误，返回主界面。\n");
        return;
    }

    if (atoi(rs[pos].flag) != 0) {
        printf("该账户状态异常（已销户），无法取款。\n");
        return;
    }

    printf("请输入取款金额：");
    scanf("%s", cAmount);
    while (getchar() != '\n');
    while (checkInput(2, cAmount)) {
        printf("金额格式不正确，请重新输入：");
        scanf("%s", cAmount);
        while (getchar() != '\n');
    }
    cash = atof(cAmount);

    if (cash > rs[pos].balance) {
        printf("余额不足，无法取款。\n");
        return;
    }

    rs[pos].balance -= cash;
    writeData();
    printf("取款成功！当前余额：%.2f\n", rs[pos].balance);
}

/* 查询单账户 */
void query(void) {
    int accNumber, pos;
    char pwd[7];

    printf("请输入查询账号：");
    scanf("%d", &accNumber);
    while (getchar() != '\n');

    pos = findAnAccount(accNumber);
    if (pos == -1) {
        printf("该账户不存在，返回主界面。\n");
        return;
    }

    dispAnAccount(pos);

    printf("请输入密码：");
    scanf("%6s", pwd);
    while (getchar() != '\n');
    if (strcmp(rs[pos].accPWD, pwd) != 0) {
        printf("密码错误，返回主界面。\n");
        return;
    }

    printf("账户状态：%s\n", atoi(rs[pos].flag) ? "销户" : "正常");
}

/* 销户 */
void closeAnAcount(void) {
    int accNumber, pos;
    char pwd[7], confirm;

    printf("请输入销户账号：");
    scanf("%d", &accNumber);
    while (getchar() != '\n');

    pos = findAnAccount(accNumber);
    if (pos == -1) {
        printf("该账户不存在，返回主界面。\n");
        return;
    }

    dispAnAccount(pos);

    printf("请输入密码：");
    scanf("%6s", pwd);
    while (getchar() != '\n');
    if (strcmp(rs[pos].accPWD, pwd) != 0) {
        printf("密码错误，返回主界面。\n");
        return;
    }

    if (atoi(rs[pos].flag) != 0) {
        printf("该账户已经是销户状态。\n");
        return;
    }

    printf("确认销户吗？(y/n)：");
    scanf("%c", &confirm);
    while (getchar() != '\n');
    if (confirm == 'y' || confirm == 'Y') {
        rs[pos].balance = 0;
        strcpy(rs[pos].flag, "1");
        writeData();
        printf("销户成功！\n");
    }
    else {
        printf("取消销户操作。\n");
    }
}