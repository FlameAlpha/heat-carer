///
/// \author Adam Wegrzynek
///

#ifndef INFLUXDATA_TRANSPORTS_HTTP_H
#define INFLUXDATA_TRANSPORTS_HTTP_H

#include "InfluxDB/include/Transport.h"
#include <curl/curl.h>
#include <memory>
#include <string>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "wldap32.lib")
namespace influxdb
{
namespace transports
{

/// \brief HTTP transport
class HTTP : public Transport
{
  public:
    /// Constructor
    HTTP(const std::string& url);

    /// Default destructor
    ~HTTP() = default;

    /// Sends point via HTTP POST
    void send(std::string&& post);

    /// Enable Basic Auth
    /// \param auth <username>:<password>
    void enableBasicAuth(const std::string& auth);

    /// Enable SSL
    void enableSsl();
  private:
    /// Custom deleter of CURL object
    static void deleteCurl(CURL * curl);

    /// Initilizes CURL and all common options
    CURL* initCurl(const std::string& url);

    /// CURL smart pointer with custom deleter
    std::unique_ptr<CURL, decltype(&HTTP::deleteCurl)> curlHandle;
};

} // namespace transports
} // namespace influxdb

#endif // INFLUXDATA_TRANSPORTS_HTTP_H
