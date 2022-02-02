#include <memory>
#include <iostream>
#include <string>
#include <sstream>
#include <jsoncpp/json/json.h>

#include "Meteosource.h"
#include "Forecast.h"


Meteosource::Meteosource(const std::string api_key,
                         const std::string tier,
                         const std::string host) : m_api_key(api_key), m_tier(tier), m_host(host)
{
    this->m_request_handler = std::unique_ptr<RequestHandler>(new RequestHandler(api_key));
}

std::string Meteosource::build_url_common(const std::string sections,
                                          const std::string timezone,
                                          const std::string language,
                                          const std::string units)
{
    std::stringstream ss;
    ss << "&sections=" << sections
       << "&timezone=" << timezone
       << "&language=" << language
       << "&units=" << units
       << "&key=" << this->m_api_key;
    return ss.str();
}

std::string Meteosource::build_url(const std::string endpoint,
                                   const std::string place_id,
                                   const std::string sections,
                                   const std::string timezone,
                                   const std::string language,
                                   const std::string units)
{
    std::stringstream ss;
    ss << this->m_host << "/v1/" << this->m_tier << '/' << endpoint
       << "?place_id=" << place_id
       << this->build_url_common(sections, timezone, language, units);
    return ss.str();
}

std::string Meteosource::build_url(const std::string endpoint,
                                   const double lat,
                                   const double lon,
                                   const std::string sections,
                                   const std::string timezone,
                                   const std::string language,
                                   const std::string units)
{
    std::stringstream ss;
    ss << this->m_host <<  "/v1/" << this->m_tier << '/' << endpoint
       << "?lat=" << lat
       << "&lon=" << lon
       << this->build_url_common(sections, timezone, language, units);
    return ss.str();
}

std::unique_ptr<Forecast> Meteosource::get_point_forecast(const std::string place_id,
                                                          const std::string sections,
                                                          const std::string timezone,
                                                          const std::string language,
                                                          const std::string units)
{
    std::string url = build_url("point", place_id, sections, timezone, language, units);
    Json::Value req_res = this->m_request_handler->execute_request(url);
    if (!req_res)
    {
        return nullptr;
    }
    return std::unique_ptr<Forecast>(new Forecast(req_res));
}

std::unique_ptr<Forecast> Meteosource::get_point_forecast(const double lat,
                                                          const double lon,
                                                          const std::string sections,
                                                          const std::string timezone,
                                                          const std::string language,
                                                          const std::string units)
{
    return std::unique_ptr<Forecast>(nullptr);
}
