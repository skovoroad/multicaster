#include <sstream>
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include "multicaster.h"

class Dumper : public Multicaster::DataHandler {
  void onReceive(const void *data, size_t nbytes) {
    std::string message(reinterpret_cast<const char *>(data), 
	reinterpret_cast<const char *>(data) + nbytes);
    std::cout << "received bytes: " << nbytes <<", message " << message << std::endl; 
  }
};


int main(int argc, char* argv[]) {
  if (argc != 4)
  {
    std::cerr << "Usage: multicaster <listen_address> <multicast_address> <instance>\n";
    std::cerr << "  For IPv4, try:\n";
    std::cerr << "    sender 239.255.0.1\n";
    std::cerr << "  For IPv6, try:\n";
    std::cerr << "    sender ff31::8000:1234\n";
    return 1;
  }
  
  Multicaster::Config config;
  Dumper dumper;

  config.listenAddress = argv[1];
  config.multicastAddress = argv[2];
  config.multicastPort = 30001;
  config.handler = &dumper;
  const char *instance = argv[3];

  Multicaster::ExchangePoint ep;
  if(auto err = ep.Configure(config)) {
    std::cerr << "Configure error: " << err->description << std::endl;
  }

 if(auto err = ep.Start()) {
    std::cerr << "Start error: " << err->description << std::endl;
  }

  uint32_t counter = 0;
  const int maxMessageCount = 10;

  std::thread send([&](){ 
    std::cout << "send stream started" << std::endl;
  
    while(++counter < maxMessageCount) { 
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(1s);

      std::stringstream message_stream;
      message_stream << instance << " " << counter;
      std::string message_string = message_stream.str();

      Multicaster::MessageBufferPtr message(new Multicaster::MessageBuffer);
      std::copy(message_string.begin(), message_string.end(),
	  std::back_inserter(*message));
      ep.Send(std::move(message));
      std::cout << "send message: " << message_stream.str() << std::endl;  
    }
  });

  send.join();
  ep.Stop();
}
