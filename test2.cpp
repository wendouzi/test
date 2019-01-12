#include<thread> 
#include<iostream> 
#include<mutex> 
#include<list> 
#include<condition_variable> 
#include<chrono> 
std::mutex g_mutex; 
std::condition_variable cond; 
//std::list<int> alist; 

enum RENDER_STATE {
    ready = 0,
    rendering ,
    pause ,
    quit
};

RENDER_STATE render_state = ready;

void threadFun1() { 
    std::unique_lock<std::mutex> ul(g_mutex); 
    while (true) {
        std::cout << "do while" << std::endl;
        if (render_state == RENDER_STATE::ready) {
            std::cout << "threadFun1 ready " << std::endl;
            cond.wait(ul); 
            std::cout << "threadFun1 wakeup" << std::endl;
        }
        else if (render_state == RENDER_STATE::rendering) {
            std::cout << "threadFun1 isRendering" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
//        else if (render_state == RENDER_STATE::pause) {
//            std::cout <<  "threadFun1 pause"  << std::endl;
//            cond.wait(ul); 
//        }
        else if (render_state == RENDER_STATE::quit) {
            std::cout << "threadFunc1 quit" << std::endl;
            break;
        }
        else {
            std::cout << "else case " << std::endl;
        }
    } 
    std::cout << "threadFun1 fininal quiet" << std::endl;
}

void threadFun2() { 
    std::this_thread::sleep_for(std::chrono::seconds(5));
    // 准备工作
    //std::lock_guard<std::mutex> lg(g_mutex); 
    render_state = ready;
    std::cout << "threadFun2 ready notify" << std::endl;
    // cond.notify_all(); 

    // 开始渲染
    std::this_thread::sleep_for(std::chrono::seconds(5));
    render_state = rendering;
    std::cout << "threadFun2 render notify" << std::endl;
    cond.notify_all();
    // 暂停
    std::this_thread::sleep_for(std::chrono::seconds(5));
    render_state = ready;
    std::cout << "threadFun2 pause notify" << std::endl;
//    cond.notify_all();

    // 开始渲染
    std::this_thread::sleep_for(std::chrono::seconds(5));
    render_state = rendering;
    std::cout << "threadFun2 rendering notify" << std::endl;
    cond.notify_all();

    // 退出
    std::this_thread::sleep_for(std::chrono::seconds(5));
    render_state = quit;
    std::cout << "threadFun2 quite notify" << std::endl;
   // cond.notify_all();

}

int main() { 
    std::cout << "main begin" << std::endl;
    std::thread th1(threadFun1); 
    std::thread th2(threadFun2); 
    th1.detach(); 
    th2.detach(); 
    while(true) {
        ;
    }
    return 0; 
}
