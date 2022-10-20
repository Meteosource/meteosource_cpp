#ifndef FORECAST_H
#define FORECAST_H

#include <memory>
#include <jsoncpp/json/json.h>
#include "Data.h"

class Forecast
{
    public:
        Forecast(Json::Value & data);
        ~Forecast() {};
        friend std::ostream & operator << (std::ostream & stream,
                                           const Forecast & f);

        double parse_lat_lon(const std::string & lat_lon);
        double lat;
        double lon;
        std::string elevation;
        std::string units;

        std::unique_ptr<CurrentData> current = nullptr;
        std::unique_ptr<MinutelyData> minutely = nullptr;
        std::vector<std::unique_ptr<HourlyData> > hourly;
        std::vector<std::unique_ptr<DailyData> > daily;
        std::vector<std::unique_ptr<Alert> > alerts;
};


#endif //FORECAST_H
