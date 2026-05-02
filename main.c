#define _CRT_SECURE_NO_WARNINGS 1
#include "bank.h"

int main(void) {
    char username[32];
    char password[32];
    int tryCount = 1;

    wellcome();

    while (tryCount <= MAX_TRY) {
        printf("请输入用户名：");
        scanf("%s", username);
        while (getchar() != '\n');
        printf("请输入密码：");
        scanf("%s", password);
        while (getchar() != '\n');

        if (strcmp(username, USER_NAME) == 0 && strcmp(password, USER_PWD) == 0) {
            printf("\n登录成功！\n");
            mainmenu();   /* 进入主菜单 */
            break;
        }
        else {
            printf("用户名或密码错误，您还有 %d 次机会。\n", MAX_TRY - tryCount);
            tryCount++;
        }
    }

    if (tryCount > MAX_TRY) {
        printf("已超过最大尝试次数，系统退出。\n");
    }

    return 0;
}

void wellcome(void) {
    printf("\n");
    printf("********************************************\n");
    printf("*        欢迎使用储蓄综合业务平台          *\n");
    printf("********************************************\n");
    printf("\n");
}