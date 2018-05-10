#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    int fd_mice = open("/dev/input/mice", O_RDONLY);	
    if(fd_mice < 0)
    {
        perror("open mice error");
        return -1;
    }

    int fd_keyb = open("/dev/input/event2", O_RDONLY);

    if(fd_keyb < 0)
    {
            perror("open keyboard error");
    }

    if(fd_keyb < 0)
    {
        perror("open fd_keyb error");
        return -1;
    }


    int fd_epoll = epoll_create(512);
    //
    struct epoll_event ev;
    ev.data.fd = fd_mice;
    ev.events = EPOLLIN;

    epoll_ctl(fd_epoll, EPOLL_CTL_ADD, fd_mice, &ev);

    ev.data.fd = fd_keyb;
    ev.events = EPOLLIN;
    epoll_ctl(fd_epoll, EPOLL_CTL_ADD, fd_keyb, &ev);

    struct epoll_event outevents[2];

    for(;;)
    {
        int ret = epoll_wait(fd_epoll, outevents, 2, 2000);
        printf("ret = %d\n", ret);

        if(ret < 0)
        {
            if(errno == EINTR)
            {
                //    continue;
                continue;
            }
            else
            {
                perror("epoll_wait error");
                exit(-1);
            }


        }

        if(ret == 0)
        {
            printf("Nothing happened\n");

        }
        else 
        {
            int i;
            for(i = 0; i < ret; ++i)
            {
                struct epoll_event *p = &outevents[i];
                if(p->data.fd == fd_mice)
                {

                    printf("mice happened\n");
                }
                else
                {
                    printf("keyboard happened\n");
                }
            }

        }
    }

    return 0;
}

