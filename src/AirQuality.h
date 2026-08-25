#ifndef AIR_QUALITY_H
#define AIR_QUALITY_H

#include <memory>
#include <json/json.h>
#include "Data.h"

class AirQuality
{
    public:
        AirQuality(Json::Value & data);
        ~AirQuality() {};
        friend std::ostream & operator << (std::ostream & stream,
                                           const AirQuality & a);

        double lat;
        double lon;
        std::string elevation;

        std::vector<std::unique_ptr<AirQualityData> > data;
};


#endif //AIR_QUALITY_H
