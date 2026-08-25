#include <iostream>
#include <string>

#include "AirQuality.h"
#include "Forecast.h"
#include "Data.h"


AirQuality::AirQuality(Json::Value & data)
{
    this->lat = Forecast::parse_lat_lon(data.get("lat", "").asString());
    this->lon = Forecast::parse_lat_lon(data.get("lon", "").asString());
    this->elevation = data.get("elevation", "").asString();

    if (data.isMember("data"))
    {
        Json::ArrayIndex sz = data["data"].size();
        this->data.reserve(sz);
        for (Json::ArrayIndex i = 0; i < sz; ++i)
        {
            this->data.push_back(std::unique_ptr<AirQualityData>(new AirQualityData(data["data"][i])));
        }
    }
}


std::ostream & operator<<(std::ostream &os,
                          const AirQuality & a)
{
    return os << "<AirQuality for " << a.lat << ", " << a.lon << ">";
}
