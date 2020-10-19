#include <memory>

#include <boost/asio.hpp>
#include "boost/bind.hpp"
#include "boost/date_time/posix_time/posix_time_types.hpp"

#include "multicaster.h"

namespace Multicaster {

  class ExchangePoint::Impl 
    : public std::enable_shared_from_this<ExchangePoint::Impl>
  {
    public:
      Impl()
        : sendSocket_(ioService_),
	receiveSocket_(ioService_)
      {}

      Error::Ptr Configure(Config &c) {
	config_ = c;
        receiveInfo_.reset(new ReceiveInfo);

	try {
	// error handling?
	  auto listenAddress = boost::asio::ip::address::from_string(config_.listenAddress);
	  auto multicastAddress = boost::asio::ip::address::from_string(config_.multicastAddress);

	  boost::asio::ip::udp::endpoint listenEndpoint(listenAddress, config_.multicastPort);
	  receiveSocket_.open(listenEndpoint.protocol());
	  receiveSocket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
	  receiveSocket_.bind(listenEndpoint);
          receiveSocket_.set_option(boost::asio::ip::multicast::join_group(multicastAddress));

	  sendEndpoint_ = boost::asio::ip::udp::endpoint(multicastAddress, config_.multicastPort);
          sendSocket_.open(sendEndpoint_.protocol());
	}
	catch (const std::exception& e) {
          return Error::Ptr(new Error {
            e.what(), 
            Error::Code::SOCKET_ERROR
          });
	}
	return Error::Ptr();
      }

      Error::Ptr Start() {
	Receive();
	thread_.reset(new std::thread(
	  [&](){
	    ioService_.run();      
	  }
	));
        return Error::Ptr();
      }

      Error::Ptr Stop() {
      	ioService_.post(
	  boost::bind(&ExchangePoint::Impl::StopService, 
	    shared_from_this())
	);
 
        return Error::Ptr();
      }

      Error::Ptr Send(MessageBufferPtr buffer) {
	ioService_.post(
	  boost::bind(&ExchangePoint::Impl::SendMessage, 
	    shared_from_this(),
	    buffer)
	);
        return Error::Ptr();
      }
    private:
      struct ReceiveInfo {
        ReceiveInfo() {
	  buffer.resize(65535); // udp max
	}

        std::vector<char> buffer;
        boost::asio::ip::udp::endpoint sender;
      };

      void Receive() {
        receiveSocket_.async_receive_from(
          boost::asio::buffer(
	    receiveInfo_->buffer.data(), 
	    receiveInfo_->buffer.size()), 
	  receiveInfo_->sender,
          boost::bind(&ExchangePoint::Impl::onReceive, 
	    shared_from_this(),
	    receiveInfo_,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
      }

      void onReceive(
        std::shared_ptr<ReceiveInfo> rInfo,
        const boost::system::error_code& error, 
	size_t nbytes) 
      {
        if(error) {
	  //	TODO: log it
	}
        else {
          // TODO filter own messages
	  config_.handler->onReceive(rInfo->buffer.data(), nbytes);
	}

        Receive();
      }

      void SendMessage(MessageBufferPtr buffer) 
      {
	sendSocket_.async_send_to(
          boost::asio::buffer(*buffer), 
	  sendEndpoint_,
          boost::bind(
	    &ExchangePoint::Impl::onSent,
	    shared_from_this(),
      	    buffer,
            boost::asio::placeholders::error
	    ));
      }


      void onSent(MessageBufferPtr buffer,
	  const boost::system::error_code& error) {
	if(error) {
	  // TODO : log it
	}
      }

      void StopService() {
        receiveSocket_.close();
        sendSocket_.close();
      }

      Config config_;      
      boost::asio::io_service ioService_;
      std::unique_ptr<std::thread> thread_;
      boost::asio::ip::udp::socket sendSocket_;
      boost::asio::ip::udp::endpoint sendEndpoint_;

      boost::asio::ip::udp::endpoint receiveEndpoint_;
      boost::asio::ip::udp::socket receiveSocket_;

      /*
       * now we use only one buffer to receive
       * if ioservice.Run called in many threads - we will use many buffers
       * */
      std::shared_ptr<ReceiveInfo> receiveInfo_; 
  };

  ExchangePoint::ExchangePoint() {
  }

  ExchangePoint::~ExchangePoint() {
    Stop();
  }
  
  Error::Ptr ExchangePoint::Configure(Config &config) {
    Impl_.reset(new Impl());
    return Impl_->Configure(config);
  }

  Error::Ptr ExchangePoint::Start() {
    if(Impl_)
      return Impl_->Start();
    return Error::Ptr(new Error {
	"Exchange point not configured", 
	Error::Code::NOT_CONFIGURED
    });
  }

  Error::Ptr ExchangePoint::Stop() {
    if(Impl_)
      return Impl_->Stop();
    return Error::Ptr(new Error {
	"Exchange point not configured", 
	Error::Code::NOT_CONFIGURED
    });
  }

  Error::Ptr ExchangePoint::Send(MessageBufferPtr && b) {
    if(Impl_)
      return Impl_->Send(std::move(b));
    return Error::Ptr(new Error {
	"Exchange point not configured", 
	Error::Code::NOT_CONFIGURED
    });
  }
}
