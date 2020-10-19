#pragma once
#include <memory>
#include <vector>

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
      SOCKET_ERROR,
    };

    std::string description;
    Code code;
  };

  struct Config {
    std::string listenAddress;
    std::string multicastAddress;
    uint16_t multicastPort = 0;

    DataHandler * handler = nullptr; 
    ExchangeLogger * logger = nullptr; 
  };

  using MessageBuffer = std::vector<char>; 
  using MessageBufferPtr = std::shared_ptr<MessageBuffer>;
     
  class ExchangePoint {
    public:
      ExchangePoint(); 
      ~ExchangePoint(); 
      Error::Ptr Configure(Config &);

      Error::Ptr Start();
      Error::Ptr Stop();

      Error::Ptr Send(MessageBufferPtr&&);
    private:
      class Impl;
      std::shared_ptr<Impl> Impl_;  // shared, not unique! shared_from_this in implementation
  };
}
