/*
 * worker_thread.h
 *
 *  Created on: 2016年4月12日
 *      Author: chenms
 */
#ifndef WORKER_THREAD_H_
#define WORKER_THREAD_H_
#include<event2/util.h>
#include<event2/event.h>
#include<event2/bufferevent.h>
#include<thread>
#include<memory>
#include<map>
#include<mutex>
#include <fcntl.h>
#include<unistd.h>
#include<event2/buffer.h>
#include<queue>
//增加日志功能
#define ELPP_THREAD_SAFE
#include"easylogging++.h"
/* #include "http_parser.h" */
#include "ThreadPool.h"
#include "ThreadPoolMethod.h"
#include "nedmalloc.h"
/* class WorkerThread; */
/* extern thread_local WorkerThread *pthread_info; */

struct TcpConnItem{
	TcpConnItem(int sock){
		tcp_sock_=sock;
		/* pthread_info_belongs_=pti; */
	}
	int tcp_sock_;
	//TODO add a tcp log for test
	int tcp_log_fd_;
};
struct UdpConnItem{
	UdpConnItem(int sock){
		udp_sock_=sock;
		pudp_read_event_=NULL;
		LOG(TRACE)<<"UdpConnItem";
	}
	~UdpConnItem(){
		LOG(TRACE)<<"~UdpConnItem";
		if(pudp_read_event_)
			event_free(pudp_read_event_);
	}
	int udp_sock_;
	struct event * pudp_read_event_;
};

class WorkerThread
{
public:
	WorkerThread(int tm,int tm_threshold);
	~WorkerThread();
	bool Run();//运行工作线程
	//TCP 处理
	static void HandleTcpConn(WorkerThread* pwt);
	static void TcpConnReadCb(bufferevent * bev,void *ctx);
	static void TcpConnEventCB(bufferevent *bev,short int  events,void * ctx);
	//UDP 处理
	static void HandleUdpConn(WorkerThread* pwt);
	static void UdpConnReadCb(evutil_socket_t fd, short what, void * arg);
	static void UdpConnEventCB(evutil_socket_t fd, short what, void * arg);
	//超时处理
	static void TimingProcessing(evutil_socket_t fd, short what, void * arg);
	//获取通知的处理
	static void HandleConn(evutil_socket_t fd, short what, void * arg);
	//线程缓存数据处理
	void AddTcpConnItem(std::shared_ptr<TcpConnItem> tci){
		map_tcp_conns_.insert(std::make_pair(tci->tcp_sock_,tci));
	}

	void DeleteTcpConnItem(int tcp_sock){
		map_tcp_conns_.erase(tcp_sock);
		/* delete tci; */
	}

	void AddUdpConnItem(std::shared_ptr<UdpConnItem> uci)
	{
		map_udp_conns_.insert(std::make_pair(uci->udp_sock_,uci));
	}

	void DeletUdpConnItem(int udp_sock){
		map_udp_conns_.erase(udp_sock);
		/* delete uci; */
	}

	void PushTcpIntoQueue(int sock){
		std::lock_guard<std::mutex>  lock(mutex_tcp_queue_);
		queue_tcp_socks_.push(sock);
	}

	int PopTcpFromQueue()
	{
		std::lock_guard<std::mutex>  lock(mutex_tcp_queue_);
		int ret=queue_tcp_socks_.front();
		queue_tcp_socks_.pop();
		return ret;
	}

	void PushUdpIntoQueue(sockaddr_in& addr){
		std::lock_guard<std::mutex> lock(mutex_udp_queue_);
		queue_udp_addrs_.push(addr);
	}

	void PopUdpFromQueue(sockaddr_in& addr){
		std::lock_guard<std::mutex> lock(mutex_udp_queue_);
		addr=queue_udp_addrs_.front();
		queue_udp_addrs_.pop();
	}

	bool NotifyWorkerThread(const char* pchar)//由LibeventServer调用
	{
		if(write(notfiy_send_fd_,pchar, 1)!=1)
			return false;
		return true;
	}

	void SetThreadPool(std::shared_ptr<ThreadPool> thread_pool){
		thread_pool_=thread_pool;
	}

	void SetThreadPoolMethod(std::shared_ptr<ThreadPoolMethod> method)
	{
		thread_pool_method_=method;
	}
private:
	bool CreateNotifyFds();//创建主线程和工作线程通信管道
	bool InitEventHandler();//初始化事件处理器
public:
	evutil_socket_t  notfiy_recv_fd_;//工作线程接收端
	evutil_socket_t  notfiy_send_fd_;//监听线程发送端
	std::shared_ptr<std::thread>   shared_ptr_thread_;
	std::map<int,std::shared_ptr<TcpConnItem> > map_tcp_conns_;
	std::map<int,std::shared_ptr<UdpConnItem> > map_udp_conns_;
	std::mutex mutex_tcp_queue_;
	std::queue<int> queue_tcp_socks_;
	std::mutex mutex_udp_queue_;
	std::queue<sockaddr_in> queue_udp_addrs_;
	std::shared_ptr<ThreadPool> thread_pool_;
	std::shared_ptr<ThreadPoolMethod> thread_pool_method_;
private:
	struct event  * pnotify_event_; //主线程通知工作线程连接到来事件
	/* DataHandleProc handle_data_proc_;//数据处理回调函数 */
	struct event_base * pthread_event_base_;
	struct event * ptimeout_event_;
	int overtime_threshold_;
	int timespan_;
};
#endif



