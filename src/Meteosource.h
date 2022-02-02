#ifndef METEOSOURCE_H
#define METEOSOURCE_H

#include <string>
#include <jsoncpp/json/json.h>

#include "RequestHandler.h"
#include "Forecast.h"


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

        std::unique_ptr<RequestHandler> m_request_handler;
        std::string m_api_key;
        std::string m_tier;
        std::string m_host;
};


#endif //METEOSOURCE_H
