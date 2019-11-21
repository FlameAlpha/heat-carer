///
/// \author Adam Wegrzynek <adam.wegrzynek@cern.ch>
///

#include "InfluxDB/include/InfluxDBFactory.h"
#include <functional>
#include <string>
#include <memory>
#include "InfluxDB/src/UriParser.h"
#include "InfluxDB/src/HTTP.h"
#include <map>

#ifdef INFLUXDB_WITH_BOOST
#include "UDP.h"
#include "UnixSocket.h"
#endif

namespace influxdb 
{

#ifdef INFLUXDB_WITH_BOOST
std::unique_ptr<Transport> withUdpTransport(const http::urlString& uri) {
  return std::make_unique<transports::UDP>(uri.host, uri.port);
}

std::unique_ptr<Transport> withUnixSocketTransport(const http::urlString& uri) {
  return std::make_unique<transports::UnixSocket>(uri.path);
}
#else
std::unique_ptr<Transport> withUdpTransport(const http::url& /*uri*/) {
  throw std::runtime_error("UDP transport requires Boost");
}

std::unique_ptr<Transport> withUnixSocketTransport(const http::url& /*uri*/) {
  throw std::runtime_error("Unix socket transport requires Boost");
}
#endif

std::unique_ptr<Transport> withHttpTransport(const http::url& uri) {
  auto transport = std::make_unique<transports::HTTP>(uri.urlString);
  std::cout<<"Enable Basic Auth !"<<std::endl;
  if (!uri.user.empty()) {
    transport->enableBasicAuth(uri.user + ":" + uri.password);
  }

  if (uri.protocol == "https") {
    transport->enableSsl();
  }
  std::cout<<"Init with Http Transport !"<<std::endl;
  return transport;
}

std::unique_ptr<Transport> InfluxDBFactory::GetTransport(std::string url) {
  static const std::map<std::string, std::function<std::unique_ptr<Transport>(const http::url&)>> map = {
    {"udp", withUdpTransport},
    {"http", withHttpTransport},
    {"https", withHttpTransport},
    {"unix", withUnixSocketTransport},
  };

  http::url parsedUrl = http::ParseHttpUrl(url);

  std::cout<<"host : " <<parsedUrl.host<<std::endl;
  std::cout<<"path : " <<parsedUrl.path<<std::endl;
  std::cout<<"port : " <<parsedUrl.port<<std::endl;
  std::cout<<"user : " <<parsedUrl.user<<std::endl;
  std::cout<<"password : " <<parsedUrl.password<<std::endl;
  std::cout<<"protocol : " <<parsedUrl.protocol<<std::endl;
  std::cout<<"urlString : " <<parsedUrl.urlString<<std::endl;
  std::cout<<"search : " <<parsedUrl.search<<std::endl;

  if (parsedUrl.protocol.empty()) {
    throw std::runtime_error("Ill-formed URI");
  }   

  auto iterator = map.find(parsedUrl.protocol);
  if (iterator == map.end()) {
    throw std::runtime_error("Unrecognized backend " + parsedUrl.protocol);
  }

  return iterator->second(parsedUrl);
}

std::unique_ptr<InfluxDB> InfluxDBFactory::Get(std::string url)
{
  std::cout<<"get Url attributes"<<std::endl;
  return std::make_unique<InfluxDB>(InfluxDBFactory::GetTransport(url));
}

} // namespace influxdb
