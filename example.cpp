#include <iostream>
#include <json/json.h>
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

    // Search for places by name and get the nearest place for coordinates
    auto places = m.find_places("london", language);
    if (places.size() > 0)
    {
        std::cout << "Places found for 'london':" << std::endl;
        for (unsigned int i = 0; i < places.size(); ++i)
            std::cout << "  " << places[i] << std::endl;
        std::cout << std::endl;
    }

    auto nearest = m.get_nearest_place(51.50853, -0.12574, language);
    if (nearest)
    {
        std::cout << "Nearest place: " << *nearest << std::endl << std::endl;
    }

    // Air quality data (not available in the free tier)
    auto air_quality = m.get_air_quality(place_id, timezone);
    if (air_quality && air_quality->data.size() > 0)
    {
        std::cout << "Air quality for next 5 hours:" << std::endl;
        for (int i = 0; i < 5; ++i)
            std::cout << "  " << air_quality->data[i]->date << ": AQI " << air_quality->data[i]->air_quality << ", PM10 " << air_quality->data[i]->pm10 << std::endl;
        std::cout << std::endl;
    }

    // Historical weather data (not available in the free tier)
    auto time_machine = m.get_time_machine(place_id, "2024-01-01", timezone, units);
    if (time_machine && time_machine->data.size() > 0)
    {
        std::cout << "Historical weather for 2024-01-01, first 5 hours:" << std::endl;
        for (int i = 0; i < 5; ++i)
            std::cout << "  " << time_machine->data[i]->date << ": temperature " << time_machine->data[i]->temperature << std::endl;
        std::cout << std::endl;
    }

    return 0;
}
