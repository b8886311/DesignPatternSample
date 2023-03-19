#ifndef ASIO_SERVER_CLIENT_CONNECTOR_H
#define ASIO_SERVER_CLIENT_CONNECTOR_H

#include <memory>
#include <functional>
#include <boost/asio.hpp>

class Connector : public std::enable_shared_from_this<Connector>{
public:
    enum class ConnectionStatus {STOPPED, CONNECTING, CONNECTED, STOPPING};
    Connector(boost::asio::io_context& ioContext);

    using OnConnectedCallback = std::function<void(std::unique_ptr<boost::asio::ip::tcp::socket> socket)>;    //only be called when connected
    using OnBrokenCallback = std::function<void()>;
    void start(boost::asio::io_context& ioContext,
               const boost::asio::ip::tcp::endpoint &endpoint,
               OnConnectedCallback callback);

    using OnDisConnectedCallback = std::function<void()>;
    void stop(OnDisConnectedCallback callback);

private:
    boost::asio::io_context::strand strand_;
    boost::asio::high_resolution_timer timer_;  //used to retry connect
    ConnectionStatus connectStatus_;
    boost::asio::ip::tcp::endpoint endpoint_;
    std::unique_ptr<boost::asio::ip::tcp::socket> socket_;

    OnConnectedCallback onConnectedCallback_;
    OnDisConnectedCallback onDisConnectedCallback_;

    //should be run in io_contex thread
    void issueConnect();

    //should be run in io_contex thread
    void issueDisConnect();

    void onConnect(boost::system::error_code error);
};

#endif //ASIO_SERVER_CLIENT_CONNECTOR_H
