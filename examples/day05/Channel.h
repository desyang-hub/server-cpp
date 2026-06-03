#pragma once

class Epoll;

class Channel
{
private:
    Epoll* epoll_;
    int fd_;
    int events_;
    int revents_;
    bool isInEpoll_;

public:
    Channel(Epoll* epoll, int fd);
    ~Channel();

    int fd() const;

    int events() const;

    void setEvents(int events);

    int revents() const;

    void setRevents(int revents);

    void update();

    void enableRead();

    void enableET();

    bool isInEpoll() const;

    void setInEpoll();

    void remove();
};
