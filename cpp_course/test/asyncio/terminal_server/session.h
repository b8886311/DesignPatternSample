#ifndef ASIO_SERVER_CLIENT_SESSION_H
#define ASIO_SERVER_CLIENT_SESSION_H

#include <memory>
#include <functional>
#include <boost/asio.hpp>
#include "frame.h"

class Session: public std::enable_shared_from_this<Session>
{
public:
    enum class ConnectionStatus {CONNECTED, STOPPING, STOPPED};

    Session(boost::asio::io_context& ioContext, boost::asio::ip::tcp::socket &&socket);
    ~Session();

    using OnReadCallback = std::function<void(const Frame &)>;
    using OnBrokenCallback = std::function<void()>;
    //note: these two callbacks are called in io_context thread. return as soon as possible
    void start(OnReadCallback onReadCallback, OnBrokenCallback onBrokenCallback);

    using OnDisConnectedCallback = std::function<void()>;
    void stop(OnDisConnectedCallback callback);

    void write(unsigned char *pStart, size_t dataSize);

private:
    static const int PREPARE_READ_BUFFER_SIZE = 1024;
    boost::asio::ip::tcp::socket socket_;
    boost::asio::io_context::strand writeStrand_, readStrand_;
    ConnectionStatus connectStatus_;
    boost::asio::streambuf incoming_;
    std::mutex mtxOutgoingQueue_;
    boost::asio::streambuf outgoing_, outgoingQueue_;
    int isAsyncWriteCompleteHandlerCalled_, isAsyncReadCompleteHandlerCalled_;

    OnReadCallback onReadCallback_;
    OnDisConnectedCallback  onDisConnectedCallback_;
    OnBrokenCallback onBrokenCallback_;

    //should be run in io_contex thread
    void issueDisConnect();

    //should be called in io_context thread
    void read();
    void onRead(boost::system::error_code error, std::size_t bytesTransferred);
    //should be called in io context thread
    void moveFromQueueAndWrite();
    void onWrite(boost::system::error_code error, std::size_t bytesTransferred);
};

#endif