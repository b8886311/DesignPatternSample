#include "session.h"

#include <iostream>
#include <boost/lexical_cast.hpp>

class Server
{
public:

    Server(boost::asio::io_context& ioContext, std::uint16_t port)
        : ioContext_(ioContext),
          acceptor_  (ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
    {
        accept();
    }

private:

    void accept() {
        socket_ = std::make_unique<boost::asio::ip::tcp::socket>(ioContext_);

        acceptor_.async_accept(*socket_, [&] (boost::system::error_code error)
        {
            auto s = std::make_shared<Session>(ioContext_, std::move(*socket_));
            s->start([s](const Frame &frame){
                auto frameSize = frame.parseFrameSize();
                frame.print(frameSize, std::cout);
                //todo: check this
                std::array<unsigned char, 4> data{0x3, 0x1, 0x4, 0x1};
                s->write(data.data(), 4);
            },[](){
                std::cerr << "session broken" << std::endl;
            });
            accept();
        });
    }

    boost::asio::io_context& ioContext_;
    boost::asio::ip::tcp::acceptor acceptor_;
    std::unique_ptr<boost::asio::ip::tcp::socket> socket_;
};

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        std::cout << "Usage: server [Port]\n";
        return 0;
    }

    boost::asio::io_context io_context;
    Server srv(io_context, boost::lexical_cast<std::uint16_t>(argv[1]));


    io_context.run();

    return 0;
}
//test to use dynamic buffer
int main_test(int argc, char* argv[])
{
    boost::asio::streambuf streambuf(1024);
    boost::asio::mutable_buffer view = streambuf.prepare(16);
    auto pwrite = static_cast<char*>(view.data());
    std::cout << "size is " << view.size() << std::endl;
    for (size_t i=0; i<view.size(); ++i) {
        pwrite[i] = i;
    }
    streambuf.commit(8);
    view = streambuf.prepare(4);
    pwrite = static_cast<char*>(view.data());
    for (size_t i=0; i<view.size(); ++i) {
        pwrite[i] = 10+i;
    }
    streambuf.commit(4);

    std::cout << "size of streambuf is " << streambuf.size() << std::endl;
    auto pread = streambuf.data();
    auto d = boost::asio::buffers_begin(streambuf.data());
    auto dend = boost::asio::buffers_end(streambuf.data());
    for (; d!=dend; ++d) {
        std::cout << (int)(*d) << " ";
    }
    std::cout << std::endl;

    streambuf.consume(6);
    std::cout << "size of streambuf is " << streambuf.size() << std::endl;
    pread = streambuf.data();
    d = boost::asio::buffers_begin(streambuf.data());
    dend = boost::asio::buffers_end(streambuf.data());
    for (; d!=dend; ++d) {
        std::cout << (int)(*d) << " ";
    }

    return 0;
}