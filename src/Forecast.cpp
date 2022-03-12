#include <iostream>
#include <string>

#include "Forecast.h"
#include "Data.h"


double Forecast::parse_lat_lon(const std::string & lat_lon)
{
    double res = stod(lat_lon.substr(0, lat_lon.size() - 1));
    if (lat_lon.back() == 'N' or lat_lon.back() == 'E')
    {
        return res;
    }
    else if (lat_lon.back() == 'S' or lat_lon.back() =='W')
    {
        return -1 * res;
    }
    else
    {
        throw std::invalid_argument("Corrupted lat/lon");
    }
}


Forecast::Forecast(Json::Value & data)
{
    this->lat = this->parse_lat_lon(data.get("lat", "").asString());
    this->lon = this->parse_lat_lon(data.get("lon", "").asString());
    this->elevation = data.get("elevation", "").asString();
    this->units = data.get("units", "").asString();
    
    if (data.isMember("current"))
    {
        this->current = std::unique_ptr<CurrentData>(new CurrentData(data["current"]));
    }
    if (data.isMember("minutely"))
    {
        this->minutely = std::unique_ptr<MinutelyData>(new MinutelyData(data["minutely"]));
    }

    if (data.isMember("hourly"))
    {
        if (data["hourly"].isMember("data"))
        {
            size_t sz = data["hourly"]["data"].size();
            this->hourly.reserve(sz);
            for (int i = 0; i < sz; ++i)
            {
                this->hourly.push_back(std::unique_ptr<HourlyData>(new HourlyData(data["hourly"]["data"][i])));
            }
        }
    }
    if (data.isMember("daily"))
    {
        if (data["daily"].isMember("data"))
        {
            size_t sz = data["daily"]["data"].size();
            this->daily.reserve(sz);
            for (int i = 0; i < sz; ++i)
            {
                this->daily.push_back(std::unique_ptr<DailyData>(new DailyData(data["daily"]["data"][i])));
            }
        }
    }
    if (data.isMember("alerts"))
    {
        if (data["alerts"].isMember("data"))
        {
            size_t sz = data["alerts"]["data"].size();
            this->alerts.reserve(sz);
            for (int i = 0; i < sz; ++i)
            {
                this->alerts.push_back(std::unique_ptr<Alert>(new Alert(data["alerts"]["data"][i])));
            }
        }
    }
}


std::ostream & operator<<(std::ostream &os,
                          const Forecast & f)
{
    return os << "<Forecast for " << f.lat << ", " << f.lon << ">";
}
