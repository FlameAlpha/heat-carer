///
/// \author Adam Wegrzynek <adam.wegrzynek@cern.ch>
///

#include "InfluxDB/include/InfluxDB.h"

#include <memory>
#include <string>
#include <iostream>

namespace influxdb
{

InfluxDB::InfluxDB(std::unique_ptr<Transport> transport) :
  mTransport(std::move(transport))
{
}

void InfluxDB::batchOf(const std::size_t size)
{
  mBufferSize = size;
  mBuffering = true;
}

void InfluxDB::flushBuffer() {
  if (!mBuffering) {
    return;
  }
  std::string stringBuffer{};
  int index = 0;
  for (const auto &i : mBuffer) {
    if(index!=mBuffer.size()-1)
        stringBuffer+= i + "\n";
    else {
        stringBuffer+= i;
    }
    index++;
  }
  mBuffer.clear();
  //std::cout<< "flushBuffer : "<<stringBuffer<<std::endl;
  transmit(std::move(stringBuffer));
}

void InfluxDB::addGlobalTag(std::string_view key, std::string_view value)
{
  if (!mGlobalTags.empty()) mGlobalTags += ",";
  mGlobalTags += key;
  mGlobalTags += "=";
  mGlobalTags += value;
}

InfluxDB::~InfluxDB()
{
  if (mBuffering) {
    flushBuffer();
  }
}

void InfluxDB::transmit(std::string&& point)
{
    //std::cout << "measure string : "<<point << std::endl;
    mTransport->send(std::move(point));
}

void InfluxDB::write(Point&& metric)
{
  if (mBuffering) {
    //std::cout << "measure string : "<<metric.toLineProtocol() << std::endl;
    mBuffer.emplace_back(metric.toLineProtocol());
    if (mBuffer.size() >= mBufferSize) {
      flushBuffer();
    }
  } else {
    transmit(metric.toLineProtocol());
  }
}
void InfluxDB::write(std::string command)
{
  if (mBuffering) {
    //std::cout << "measure string : "<<metric.toLineProtocol() << std::endl;
    mBuffer.emplace_back(command);
    if (mBuffer.size() >= mBufferSize) {
      flushBuffer();
    }
  } else {
    transmit(std::move(command));
  }
}

} // namespace influxdb
