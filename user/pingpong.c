#include "kernel/types.h"
#include "user/user.h"

int main()
{
    char b = 'x';
    int p2c[2]; // 父进程传递给子进程数据的管道 parent to child
    int c2p[2]; // 子进程传递给父进程数据的管道 child to parent

    if (pipe(p2c) < 0)
        exit(1);
    if (pipe(c2p) < 0)
        exit(1);

    int c_pid = fork();
    if (c_pid < 0)
    {
        printf("fork error\n");
        exit(1);
    }

    if (c_pid == 0)
    {
        // 子进程会复制一份跟父进程一模一样的内存数据，所以把未用到的pipe关闭
        close(p2c[1]); // 关闭子进程中指向父管道的写入端
        close(c2p[0]); // 关闭子进程中指向子管道的读取端

        read(p2c[0], &b, sizeof(char)); // 从父管道中读，若父进程还未写任何东西，则此处会阻塞
        close(p2c[0]);                  // 关闭子进程中父管道的读取端
        printf("%d: received ping\n", getpid());

        write(c2p[1], &b, sizeof(char)); // 子进程给父进程发消息
        close(c2p[1]);                   // 关闭子进程中子管道的写入端
    }
    else
    {
        close(p2c[0]); // 关闭父进程中父管道的读取端
        close(c2p[1]); // 关闭父进程中子管道的写入端

        write(p2c[1], &b, sizeof(char)); // 父进程给子进程发消息
        close(p2c[1]);                   // 关闭父进程中父管道的写入端

        read(c2p[0], &b, sizeof(char)); // 从子管道中读消息，若子进程还未写任何东西，则此处会阻塞
        close(c2p[0]);                  // 关闭主进程中的子管道的读取端
        printf("%d: received pong\n", getpid());
    }
    exit(0);
}