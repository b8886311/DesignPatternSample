#include "session.h"

Session::Session(boost::asio::io_context& ioContext, boost::asio::ip::tcp::socket &&socket)
    : socket_{std::move(socket)}, writeStrand_{ioContext}, readStrand_{ioContext},
      connectStatus_{ConnectionStatus::CONNECTED},
      isAsyncWriteCompleteHandlerCalled_{1}, isAsyncReadCompleteHandlerCalled_{1}
{
}
Session::~Session() {
    std::cout << "Session destroyed" << std::endl;
}
void Session::start(OnReadCallback onReadCallback, OnBrokenCallback onBrokenCallback) {
    onReadCallback_ = onReadCallback;
    onBrokenCallback_ = onBrokenCallback;

    //start read after connect
    boost::asio::post(readStrand_,std::bind(&Session::read, shared_from_this()));
}
void Session::stop(OnDisConnectedCallback callback) {
    onDisConnectedCallback_ = callback;
    boost::asio::post(writeStrand_,std::bind(&Session::issueDisConnect, shared_from_this()));
}
void Session::write(unsigned char *pStart, size_t dataSize) {
    //rule: we use mtxOutgoingQueue_ to protect outgoingQueue_
    //      async_write and complete handler are all run in io_context thread(given strand)
    std::lock_guard<std::mutex> lock(mtxOutgoingQueue_);
    Frame frame(outgoingQueue_);
    frame.writeFrameContent(pStart, dataSize);
    boost::asio::post(writeStrand_,std::bind(&Session::moveFromQueueAndWrite, shared_from_this()));
}
void Session::issueDisConnect() {
    if (connectStatus_ == ConnectionStatus::STOPPED) {
        //do nothing
        if (onDisConnectedCallback_) {
            onDisConnectedCallback_();
            onDisConnectedCallback_ = nullptr;
        }
    } else if (connectStatus_ == ConnectionStatus::STOPPING) {
        //do nothing
    } else {    //connected
        boost::system::error_code error;
        socket_.shutdown(boost::asio::socket_base::shutdown_both, error);
        std::cout << "shutdown socket get error code: " << error.message() << std::endl;

        socket_.close(error);
        if (!error) {
            std::cerr << "stop socket get error code: " << error.message() << std::endl;
        } else {
            std::cout << "stop socket successfully" << std::endl;
        }
        connectStatus_ = ConnectionStatus::STOPPED;
        if (onDisConnectedCallback_) {
            onDisConnectedCallback_();
            onDisConnectedCallback_ = nullptr;
        }
    }
}
void Session::read() {
    //std::cout << "read in thread: " << std::this_thread::get_id() << std::endl;
    if (isAsyncReadCompleteHandlerCalled_) {
        isAsyncReadCompleteHandlerCalled_ = 0;
        boost::asio::async_read(socket_,
                                incoming_.prepare(PREPARE_READ_BUFFER_SIZE),
                                boost::asio::transfer_at_least(1),
                                boost::asio::bind_executor(readStrand_,std::bind(&Session::onRead, shared_from_this(), std::placeholders::_1, std::placeholders::_2)));
    }
}
void Session::onRead(boost::system::error_code error, std::size_t bytesTransferred) {
    //std::cout << "on_read in thread: " << std::this_thread::get_id() << std::endl;
    isAsyncReadCompleteHandlerCalled_ = 1;
    if(!error) {
        std::cout << "onRead, bytes: " << std::dec << bytesTransferred << std::endl;
        incoming_.commit(bytesTransferred);
        Frame frame(incoming_);
        while (incoming_.size()) {        //we might get several frames at one time, check them all
            if (!frame.trimToLeadingChars()) {
                break;
            }
            auto frameSize = frame.parseFrameSize();
            if (frameSize == std::numeric_limits<size_t>::max() ||   //if frameSize is not reasonable
                incoming_.size() < frameSize) {                      // if not enough data
                break;
            }
            //frame data are fully received, do something
            //frame.doSomethingWithData(frameSize);
            if (onReadCallback_) {
                onReadCallback_(frame);
            }
            incoming_.consume(frameSize);
        }
        //trigger another read
        read();
    } else {
        std::cerr << "onRead got error: " << error.message() << ", bytes transferred: " << std::dec << bytesTransferred << std::endl;
        if (error == boost::asio::error::eof) {
            onBrokenCallback_();
            onBrokenCallback_ = nullptr;
        }
    }
}
void Session::moveFromQueueAndWrite() {
    if (isAsyncWriteCompleteHandlerCalled_) {
        {
            std::lock_guard<std::mutex> lock(mtxOutgoingQueue_);
            if (outgoingQueue_.size()) {
                auto copiedSize = boost::asio::buffer_copy(outgoing_.prepare(outgoingQueue_.size()), outgoingQueue_.data());
                outgoing_.commit(copiedSize);
                outgoingQueue_.consume(copiedSize);
            }
        }
        if (outgoing_.size()) {
            isAsyncWriteCompleteHandlerCalled_ = 0;
            boost::asio::async_write(socket_,
                                     boost::asio::buffer(outgoing_.data(), outgoing_.size()),
                                     boost::asio::bind_executor(writeStrand_, std::bind(&Session::onWrite, shared_from_this(), std::placeholders::_1, std::placeholders::_2)) );
        }
    }
}
void Session::onWrite(boost::system::error_code error, std::size_t bytesTransferred) {
    //std::cout << "on_write in thread: " << std::this_thread::get_id() << std::endl;
    isAsyncWriteCompleteHandlerCalled_ = 1;
    if (!error) {
        std::cout << "onWrite (no error): " << error.message() << ", bytes transferred: " << std::dec << bytesTransferred << std::endl;
        outgoing_.consume(bytesTransferred);
        moveFromQueueAndWrite();
    }
    else {
        std::cerr << "onWrite: (has error): " << error.message() << ", bytes transferred: " << std::dec << bytesTransferred << std::endl;
        if (error == boost::asio::error::eof) {
            if (onBrokenCallback_) {
                onBrokenCallback_();
                onBrokenCallback_ = nullptr;
            }
        }
    }
}
