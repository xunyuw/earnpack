#include<ev++.h>
#include<iostream>
#include<unistd.h>

using namespace std;

class IOWatcher
{
public:
    IOWatcher(int fd,unsigned int events)
    {
        m_io.set(fd,events);
        m_io.set<IOWatcher,&IOWatcher::CallBack>(this);
        m_io.start();
    }
    void CallBack(ev::io &w, int revents)
    {
        cout << "In IOWatcher::CallBack" << endl;
        w.stop();
    }
private:
    ev::io m_io;
};

int main()
{
    ev::default_loop loop;
    IOWatcher my_io(STDIN_FILENO,ev::READ);
    loop.run(0);
    return 0;
}
