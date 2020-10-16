#include "multicaster.h"

namespace Multicaster {

  class ExchangePoint::Impl {
    public:
      Error::Ptr Configure(Config &) {
        return Error::Ptr();
      
      }

      Error::Ptr Start() {
        return Error::Ptr();
      }

      Error::Ptr Stop() {
        return Error::Ptr();
      }

      Error::Ptr Send(const void *, size_t) {
        return Error::Ptr();
      }
    private:
      Config config_;      
  };

  ExchangePoint::ExchangePoint() {
  }
  
  Error::Ptr ExchangePoint::Configure(Config &) {
    Impl_.reset(new Impl());
    return Error::Ptr();
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

  Error::Ptr ExchangePoint::Send(const void *data, size_t nbytes) {
    if(Impl_)
      return Impl_->Send(data, nbytes);
    return Error::Ptr(new Error {
	"Exchange point not configured", 
	Error::Code::NOT_CONFIGURED
    });
  }
}
