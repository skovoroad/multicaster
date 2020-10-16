#pragma once
#include <memory>

namespace Multicaster {

  class DataHandler {
    public:
      virtual void onReceive(const void *, size_t) = 0;
  };

  class ExchangeLogger {
    public:
       virtual void Log(const char *, size_t) = 0;
  };

  struct Error {
    typedef std::shared_ptr<Error> Ptr;
    enum Code {
      UNKNOWN_ERROR,
      NOT_CONFIGURED,
    };

    std::string description;
    Code code;
  };

  struct Config {
    std::string listen_endpoint_;
    std::string multicast_address_;
    uint16_t multicast_port_ = 0;

    DataHandler * handler_ = nullptr; 
    ExchangeLogger * logger_ = nullptr; 
  };

  class ExchangePoint {
    public:
      ExchangePoint(); 
      Error::Ptr Configure(Config &);

      Error::Ptr Start();
      Error::Ptr Stop();

      Error::Ptr Send(const void *, size_t);
    private:
      class Impl;
      std::unique_ptr<Impl> Impl_; 
  };
}
