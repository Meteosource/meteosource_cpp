#include <iostream>
#include <jsoncpp/json/json.h>
#include <memory>

#include "src/Meteosource.h"


int main()
{
    const std::string api_key = "YOUR-API-KEY";
    const std::string tier = "free";
    Meteosource m = Meteosource(api_key, tier);

    const std::string place_id = "london";
    const std::string sections = "all";
    const std::string timezone = "UTC";
    const std::string language = "en";
    const std::string units = "auto";
    auto res = m.get_point_forecast(place_id, sections, timezone, language, units);
    if (!res)
    {
        return -1;
    }

    if (res->current)
    {
        std::cout << "Current weather: " << res->current->summary << std::endl << std::endl;
    }

    if (res->minutely)
    {
        std::cout << "Minutely summary: " << res->minutely->summary << std::endl << std::endl;
        std::cout << "Precipitation for next 5 minutes: " << res->minutely->summary << std::endl << std::endl;

        for (int i = 0; i < 5; ++i)
            std::cout << "  " << res->minutely->data[i]->date << ": precipitation " << res->minutely->data[i]->precipitation << std::endl;
        std::cout << std::endl;
    }

    if (res->hourly.size() > 0)
    {
        std::cout << "Weather for next 5 hours:" << std::endl;
        for (int i = 0; i < 5; ++i)
            std::cout << "  " << res->hourly[i]->date << ": temperature " << res->hourly[i]->temperature << ", wind speed: " << res->hourly[i]->wind_speed << std::endl;
        std::cout << std::endl;
    }

    if (res->daily.size() > 0)
    {
        std::cout << "Daily Weather for next 5 days:" << std::endl;
        for (int i = 0; i < 5; ++i)
            std::cout << "  " << res->daily[i]->day << ": all day weather: '" << res->daily[i]->all_day->weather << "', sunrise: " << res->daily[i]->astro->sun_rise << std::endl;
        std::cout << std::endl;
    }

    if (res->alerts.size() > 0)
    {
        std::cout << "Active alerts:" << std::endl;
        for (unsigned int i = 0; i < res->alerts.size(); ++i)
            std::cout << "  Alert valid from " << res->alerts[i]->onset << " to " << res->alerts[i]->expires << ": " << res->alerts[i]->event << std::endl;
        std::cout << std::endl;
    }

    return 0;
}
