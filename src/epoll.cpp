#include <iostream>
#include <sys/epoll.h>
#include <unistd.h>

int main() {
    // 创建 epoll 实例
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        std::cerr << "Failed to create epoll instance" << std::endl;
        return 1;
    }

    // 创建管道用于测试
    int pipe_fds[2];
    if (pipe(pipe_fds) == -1) {
        std::cerr << "Failed to create pipe" << std::endl;
        return 1;
    }

    // 注册读端管道文件描述符到 epoll 实例
    epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = pipe_fds[0];
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, pipe_fds[0], &event) == -1) {
        std::cerr << "Failed to add pipe fd to epoll" << std::endl;
        close(epoll_fd);
        return 1;
    }

    const int MAX_EVENTS = 10;
    epoll_event events[MAX_EVENTS];

    while (true) {
        // 等待事件发生
        int num_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (num_events == -1) {
            std::cerr << "epoll_wait error" << std::endl;
            break;
        }

        // 处理就绪的事件
        for (int i = 0; i < num_events; ++i) {
            if (events[i].data.fd == pipe_fds[0]) {
                // 读取管道数据
                char buffer[256];
                ssize_t num_bytes = read(pipe_fds[0], buffer, sizeof(buffer));
                if (num_bytes == -1) {
                    std::cerr << "Read error" << std::endl;
                    break;
                }

                // 处理读取到的数据
                std::cout << "Received data from pipe: " << std::string(buffer, num_bytes) << std::endl;
            }
        }
    }

    // 清理资源
    close(pipe_fds[0]);
    close(pipe_fds[1]);
    close(epoll_fd);

    return 0;
}
