///
/// \author Adam Wegrzynek <adam.wegrzynek@cern.ch>
///

#include "InfluxDB/include/Point.h"

#include <iostream>
#include <chrono>
#include <memory>
#include <sstream>

namespace influxdb
{

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

Point::Point(const std::string& measurement) :
  mMeasurement(measurement), mTimestamp(Point::getCurrentTimestamp())
{
}

Point&& Point::addField(std::string_view name, std::variant<int,long long int, std::string, double> value)
{
  std::stringstream convert;
  if (!mFields.empty()) convert << " ";
  convert << name.data() << "=";
  std::visit(overloaded {
    [&convert](int value) { convert << value; },
    [&convert](long long int value) { convert << value; },
    [&convert](double value) { convert << value; },
    [&convert](const std::string& value) { convert << value; },
    }, value);
  mFields += convert.str();
  //std::cout << convert.str();
  return std::move(*this);
}

Point&& Point::addTag(std::string_view key, std::string_view value)
{
  mTags += ",";
  mTags += key;
  mTags += "=";
  mTags += value;
  return std::move(*this);
}

auto Point::getCurrentTimestamp() -> decltype(std::chrono::system_clock::now())
{
  return std::chrono::system_clock::now();
}

std::string Point::toLineProtocol() const
{
  return mMeasurement + mTags + "," + mFields + " " + std::to_string(
    std::chrono::duration_cast <std::chrono::nanoseconds>(mTimestamp.time_since_epoch()).count()
  );
}

} // namespace influxdb
