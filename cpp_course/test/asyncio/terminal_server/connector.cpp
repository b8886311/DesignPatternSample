#include "connector.h"
#include <iostream>

Connector::Connector(boost::asio::io_context& ioContext)
        : strand_{ioContext}, timer_{ioContext},
          connectStatus_{ConnectionStatus::STOPPED}
{
}
void Connector::start(boost::asio::io_context& ioContext,
           const boost::asio::ip::tcp::endpoint &endpoint,
           OnConnectedCallback callback) {
    if (connectStatus_ == ConnectionStatus::STOPPED || connectStatus_ == ConnectionStatus::CONNECTED) {
        endpoint_ = endpoint;
        socket_ = std::move(std::make_unique<boost::asio::ip::tcp::socket>(ioContext));
        connectStatus_ = ConnectionStatus::STOPPED;
        onConnectedCallback_ = callback;
        boost::asio::post(strand_, std::bind(&Connector::issueConnect, shared_from_this()));
    }
}
void Connector::stop(OnDisConnectedCallback callback) {
    onDisConnectedCallback_ = callback;
    boost::asio::post(strand_, std::bind(&Connector::issueDisConnect, shared_from_this()));
}
void Connector::issueConnect() {
    if (connectStatus_ == ConnectionStatus::STOPPED) {
        connectStatus_ = ConnectionStatus::CONNECTING;
        socket_->async_connect(endpoint_, std::bind(&Connector::onConnect, shared_from_this(), std::placeholders::_1));
    } else if (connectStatus_ == ConnectionStatus::STOPPING) {
        connectStatus_ = ConnectionStatus::CONNECTING;
    } else {
        //connecting, connected, do nothing
    }
}
void Connector::issueDisConnect() {
    if (connectStatus_ == ConnectionStatus::STOPPED) {
        if (onDisConnectedCallback_) {
            onDisConnectedCallback_();
            onDisConnectedCallback_ = nullptr;
        }
    } else if (connectStatus_ == ConnectionStatus::STOPPING) {
        //do nothing
    }
    else if (connectStatus_ == ConnectionStatus::CONNECTING) {
        connectStatus_ = ConnectionStatus::STOPPING;
    } else {    //connected
        connectStatus_ = ConnectionStatus::STOPPED;
        if (onDisConnectedCallback_) {
            onDisConnectedCallback_();
            onDisConnectedCallback_ = nullptr;
        }
    }
}
void Connector::onConnect(boost::system::error_code error) {
    if (!error) {
        std::cout << "onConnect: " << error.message() << std::endl;
        //std::cout << "onConnect in thread: " << std::this_thread::get_id() << std::endl;
        connectStatus_ = ConnectionStatus::CONNECTED; //connected
        if (onConnectedCallback_) {
            onConnectedCallback_(std::move(socket_));
            onConnectedCallback_ = nullptr;
        }
    } else {
        if (connectStatus_==ConnectionStatus::CONNECTING) {
            timer_.expires_after(std::chrono::seconds(1));
            timer_.async_wait([self = shared_from_this()](const boost::system::error_code &error){
                std::cout << "retry async connect" << std::endl;
                self->socket_->async_connect(self->endpoint_, std::bind(&Connector::onConnect, self, std::placeholders::_1));
            });
        } else if (connectStatus_ == ConnectionStatus::CONNECTED) {
            //do nothing. it is strange to enter this case
        } else {  //STOPPING, STOPPED
            connectStatus_ = ConnectionStatus::STOPPED;
            if (onDisConnectedCallback_) {
                onDisConnectedCallback_();
                onDisConnectedCallback_ = nullptr;
            }
        }
    }
}

