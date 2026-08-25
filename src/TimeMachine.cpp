#include <iostream>
#include <string>

#include "TimeMachine.h"
#include "Forecast.h"
#include "Data.h"


TimeMachine::TimeMachine(Json::Value & data)
{
    this->lat = Forecast::parse_lat_lon(data.get("lat", "").asString());
    this->lon = Forecast::parse_lat_lon(data.get("lon", "").asString());
    this->elevation = data.get("elevation", "").asString();
    this->units = data.get("units", "").asString();

    if (data.isMember("data"))
    {
        Json::ArrayIndex sz = data["data"].size();
        this->data.reserve(sz);
        for (Json::ArrayIndex i = 0; i < sz; ++i)
        {
            this->data.push_back(std::unique_ptr<HourlyData>(new HourlyData(data["data"][i])));
        }
    }

    this->daily = std::unique_ptr<AllDayData>(new AllDayData(data["daily"]));
    this->statistics = std::unique_ptr<StatisticsData>(new StatisticsData(data["statistics"]));
}


std::ostream & operator<<(std::ostream &os,
                          const TimeMachine & t)
{
    return os << "<TimeMachine for " << t.lat << ", " << t.lon << ">";
}
