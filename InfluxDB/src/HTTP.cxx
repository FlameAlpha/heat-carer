///
/// \author Adam Wegrzynek <adam.wegrzynek@cern.ch>
///

#include "InfluxDB/src/HTTP.h"
#include <iostream>
namespace influxdb
{
namespace transports
{

HTTP::HTTP(const std::string& url) :
  curlHandle(initCurl(url), &HTTP::deleteCurl)
{
}

CURL* HTTP::initCurl(const std::string& url)
{
  CURLcode globalInitResult = curl_global_init(CURL_GLOBAL_ALL);
  if (globalInitResult != CURLE_OK) {
    std::cout << "curle is not ok!" << std::endl;
    throw std::runtime_error(std::string("cURL init") + curl_easy_strerror(globalInitResult));
  }

  CURL *curl = curl_easy_init();
  if(curl){
      curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
      curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
      curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
      curl_easy_setopt(curl, CURLOPT_POST, 1L);
      curl_easy_setopt(curl, CURLOPT_TCP_KEEPIDLE, 120L);
      curl_easy_setopt(curl, CURLOPT_TCP_KEEPINTVL, 60L);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA,"./temp.txt");
      std::cout<<"perform : "<<curl_easy_perform(curl)<<std::endl;
  }
  return curl;
}

void HTTP::enableBasicAuth(const std::string& auth)
{
  CURL *curl = curlHandle.get();
  curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
  std::cout<<"auth state : "<<curl_easy_setopt(curl, CURLOPT_USERPWD, auth.c_str())<<std::endl;
}

void HTTP::enableSsl()
{
  CURL *curl = curlHandle.get();
  std::cout<<"ssl state : "<<curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L)<<std::endl;
}

void HTTP::deleteCurl(CURL * curl)
{
  curl_easy_cleanup(curl);
  curl_global_cleanup();
}

void HTTP::send(std::string&& post)
{
  CURLcode response;
  long responseCode;
  //std::cout<<"post String : "<<post;
  curl_easy_setopt(curlHandle.get(), CURLOPT_POSTFIELDS, post.c_str());
  curl_easy_setopt(curlHandle.get(), CURLOPT_POSTFIELDSIZE, (long) post.length());
  response = curl_easy_perform(curlHandle.get());
  curl_easy_getinfo(curlHandle.get(), CURLINFO_RESPONSE_CODE, &responseCode);
  if (response != CURLE_OK) {
    std::cout<<"send Error!"<<std::endl;
    throw std::runtime_error(curl_easy_strerror(response));
  }
  if (responseCode < 200 || responseCode > 206) {
    throw std::runtime_error("response code : " + std::to_string(responseCode));
  }
}

} // namespace transports
} // namespace influxdb
