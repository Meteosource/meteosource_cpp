#ifndef TIME_MACHINE_H
#define TIME_MACHINE_H

#include <memory>
#include <vector>
#include <json/json.h>
#include "Data.h"

class TimeMachine
{
    public:
        TimeMachine(Json::Value & data);
        ~TimeMachine() {};
        friend std::ostream & operator << (std::ostream & stream,
                                           const TimeMachine & t);

        double lat;
        double lon;
        std::string elevation;
        std::string units;

        std::vector<std::unique_ptr<HourlyData> > data;
        std::unique_ptr<AllDayData> daily;
        std::unique_ptr<StatisticsData> statistics;
};


#endif //TIME_MACHINE_H
