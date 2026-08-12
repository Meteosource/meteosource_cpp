#ifndef METEOSOURCE_H
#define METEOSOURCE_H

#include <string>
#include <vector>
#include <json/json.h>

#include "RequestHandler.h"
#include "Forecast.h"
#include "AirQuality.h"
#include "Data.h"


class Meteosource
{
    public:
        Meteosource(const std::string api_key,
                    const std::string tier,
                    const std::string host="https://www.meteosource.com/api");

        std::unique_ptr<Forecast> get_point_forecast(const std::string place_id,
                                                     const std::string sections="current,hourly",
                                                     const std::string timezone="UTC",
                                                     const std::string language="en",
                                                     const std::string units="auto");

        std::unique_ptr<Forecast> get_point_forecast(const double lat,
                                                     const double lon,
                                                     const std::string sections="current,hourly",
                                                     const std::string timezone="UTC",
                                                     const std::string language="en",
                                                     const std::string units="auto");

        // Note: the air_quality endpoint does not accept a language
        // parameter (its data is numeric only), unlike the other endpoints.
        std::unique_ptr<AirQuality> get_air_quality(const std::string place_id,
                                                    const std::string timezone="UTC");

        std::unique_ptr<AirQuality> get_air_quality(const double lat,
                                                    const double lon,
                                                    const std::string timezone="UTC");

        std::unique_ptr<Place> get_nearest_place(const double lat,
                                                 const double lon,
                                                 const std::string language="en");

        std::vector<Place> find_places(const std::string text,
                                       const std::string language="en");

        std::vector<Place> find_places_prefix(const std::string text,
                                              const std::string language="en");

    private:
        std::string build_url_common(const std::string sections,
                                     const std::string timezone,
                                     const std::string language,
                                     const std::string units);

        std::string build_url(const std::string endpoint,
                              const std::string place_id,
                              const std::string sections,
                              const std::string timezone,
                              const std::string language,
                              const std::string units);

        std::string build_url(const std::string endpoint,
                              const double lat,
                              const double lon,
                              const std::string sections,
                              const std::string timezone,
                              const std::string language,
                              const std::string units);

        std::string url_encode(const std::string & value);

        std::vector<Place> get_places(const std::string url);

        std::unique_ptr<RequestHandler> m_request_handler;
        std::string m_api_key;
        std::string m_tier;
        std::string m_host;
};


#endif //METEOSOURCE_H
