#include <iostream>
#include <thread>
#include <memory>
#include <boost/asio.hpp>
#include "frame.h"
#include "session.h"
#include "connector.h"

class Application : public std::enable_shared_from_this<Application>
{
public:

    Application(boost::asio::io_context& io_context)
            : ioContext_{io_context}, connector_{std::make_shared<Connector>(io_context)},
            isRunning_{false}, isSessionOK_{false}
    {
    }
    ~Application() {
        if (workingThread_.joinable()) {
            workingThread_.join();
        }
    }
    void initialize(const std::string &ipStr, int port) {
        boost::asio::ip::address address = boost::asio::ip::make_address(ipStr);
        endpoint_ = boost::asio::ip::tcp::endpoint(address, port);

    }
    void run() {
        if (!isRunning_) {
            isRunning_ = true;
            workingThread_ = std::thread([this](){
                workingThreadBody();
            });
        }
        connector_->start(ioContext_, endpoint_, [self = shared_from_this()](std::unique_ptr<boost::asio::ip::tcp::socket> socket){
            self->session_ = std::make_shared<Session>(self->ioContext_, std::move(*socket));
            self->isSessionOK_ = true;
            self->session_->start(std::bind(&Application::onReceiveFrame, self, std::placeholders::_1),
                                  [self](){
                                      self->isSessionOK_ = false;
                                      self->session_->stop([self](){
                                          if (self->isRunning_) {
                                              //after stop, trigger another run, so that it will try to reconnect
                                              self->run();
                                          }
                                      });
                                  });
        });
    }
    void stop() {
        session_->stop([this](){
            std::cout << "disconnected" << std::endl;
            isSessionOK_ = false;
            isRunning_ = false;
        });
    }


private:
    boost::asio::io_context &ioContext_;
    boost::asio::ip::tcp::endpoint endpoint_;
    std::shared_ptr<Connector> connector_;
    std::shared_ptr<Session> session_;
    std::thread workingThread_;
    bool isRunning_, isSessionOK_;

    void onReceiveFrame(const Frame &frame) {
        auto frameSize = frame.parseFrameSize();
        frame.print(frameSize, std::cout);
    }
    void workingThreadBody(){
        int sendCount=0;
        while (isRunning_) {
            if (isSessionOK_) {
                writeToServer();
                ++sendCount;
                if (sendCount & 1) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                }
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
    }
    void writeToServer() {
        static unsigned char c = 0;
        std::array<unsigned char, 6> data {'m','n','p','q','r', c};
        if (c < 254) {
            ++c;
        } else {
            c=0;
        }
        session_->write(data.data(), data.size());
    }

};

int main(int argc, char* argv[])
{
    boost::asio::io_context io_context;
    using work_guard_type = boost::asio::executor_work_guard<boost::asio::io_context::executor_type>;
    work_guard_type work_guard(io_context.get_executor());

    auto app = std::make_shared<Application>(io_context);
    app->initialize("127.0.0.1", 15001);
    app->run();
    std::thread stopAction([app, &io_context, &work_guard](){
        std::this_thread::sleep_for(std::chrono::seconds(10));
        app->stop();
        io_context.stop();  //pending work will be ignored
        //work_guard.reset(); //wait pending work to be finished
    });
    io_context.run();

    std::cout << "right after io_context.start" << std::endl;
    stopAction.join();
    return 0;
}