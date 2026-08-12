#include <memory>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <stdexcept>
#include <json/json.h>

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
    std::string url = build_url("point", lat, lon, sections, timezone, language, units);
    Json::Value req_res = this->m_request_handler->execute_request(url);
    if (!req_res)
    {
        return nullptr;
    }
    return std::unique_ptr<Forecast>(new Forecast(req_res));
}

std::string Meteosource::url_encode(const std::string & value)
{
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex << std::uppercase;
    for (std::string::size_type i = 0; i < value.size(); ++i)
    {
        unsigned char c = value[i];
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
        {
            escaped << c;
        }
        else
        {
            escaped << '%' << std::setw(2) << int(c);
        }
    }
    return escaped.str();
}

std::unique_ptr<AirQuality> Meteosource::get_air_quality(const std::string place_id,
                                                         const std::string timezone)
{
    std::stringstream ss;
    ss << this->m_host << "/v1/" << this->m_tier << "/air_quality"
       << "?place_id=" << place_id
       << "&timezone=" << timezone
       << "&key=" << this->m_api_key;
    Json::Value req_res = this->m_request_handler->execute_request(ss.str());
    if (!req_res)
    {
        return nullptr;
    }
    return std::unique_ptr<AirQuality>(new AirQuality(req_res));
}

std::unique_ptr<AirQuality> Meteosource::get_air_quality(const double lat,
                                                         const double lon,
                                                         const std::string timezone)
{
    std::stringstream ss;
    ss << this->m_host << "/v1/" << this->m_tier << "/air_quality"
       << "?lat=" << lat
       << "&lon=" << lon
       << "&timezone=" << timezone
       << "&key=" << this->m_api_key;
    Json::Value req_res = this->m_request_handler->execute_request(ss.str());
    if (!req_res)
    {
        return nullptr;
    }
    return std::unique_ptr<AirQuality>(new AirQuality(req_res));
}

void Meteosource::validate_date(const std::string & date)
{
    bool valid = date.size() == 10 && date[4] == '-' && date[7] == '-';
    for (std::string::size_type i = 0; valid && i < date.size(); ++i)
    {
        if (i == 4 || i == 7)
        {
            continue;
        }
        if (!isdigit(static_cast<unsigned char>(date[i])))
        {
            valid = false;
        }
    }
    if (!valid)
    {
        throw std::invalid_argument("date must be in \"YYYY-MM-DD\" format, got \"" + date + "\"");
    }
}

std::unique_ptr<TimeMachine> Meteosource::get_time_machine(const std::string place_id,
                                                           const std::string date,
                                                           const std::string timezone,
                                                           const std::string units)
{
    this->validate_date(date);
    std::stringstream ss;
    ss << this->m_host << "/v1/" << this->m_tier << "/time_machine"
       << "?place_id=" << place_id
       << "&date=" << date
       << "&timezone=" << timezone
       << "&units=" << units
       << "&key=" << this->m_api_key;
    Json::Value req_res = this->m_request_handler->execute_request(ss.str());
    if (!req_res)
    {
        return nullptr;
    }
    return std::unique_ptr<TimeMachine>(new TimeMachine(req_res));
}

std::unique_ptr<TimeMachine> Meteosource::get_time_machine(const double lat,
                                                           const double lon,
                                                           const std::string date,
                                                           const std::string timezone,
                                                           const std::string units)
{
    this->validate_date(date);
    std::stringstream ss;
    ss << this->m_host << "/v1/" << this->m_tier << "/time_machine"
       << "?lat=" << lat
       << "&lon=" << lon
       << "&date=" << date
       << "&timezone=" << timezone
       << "&units=" << units
       << "&key=" << this->m_api_key;
    Json::Value req_res = this->m_request_handler->execute_request(ss.str());
    if (!req_res)
    {
        return nullptr;
    }
    return std::unique_ptr<TimeMachine>(new TimeMachine(req_res));
}

std::unique_ptr<Place> Meteosource::get_nearest_place(const double lat,
                                                      const double lon,
                                                      const std::string language)
{
    std::stringstream ss;
    ss << this->m_host << "/v1/" << this->m_tier << "/nearest_place"
       << "?lat=" << lat
       << "&lon=" << lon
       << "&language=" << language
       << "&key=" << this->m_api_key;
    Json::Value req_res = this->m_request_handler->execute_request(ss.str());
    if (!req_res)
    {
        return nullptr;
    }
    return std::unique_ptr<Place>(new Place(req_res));
}

std::vector<Place> Meteosource::find_places(const std::string text,
                                            const std::string language)
{
    std::stringstream ss;
    ss << this->m_host << "/v1/" << this->m_tier << "/find_places"
       << "?text=" << this->url_encode(text)
       << "&language=" << language
       << "&key=" << this->m_api_key;
    return this->get_places(ss.str());
}

std::vector<Place> Meteosource::find_places_prefix(const std::string text,
                                                   const std::string language)
{
    std::stringstream ss;
    ss << this->m_host << "/v1/" << this->m_tier << "/find_places_prefix"
       << "?text=" << this->url_encode(text)
       << "&language=" << language
       << "&key=" << this->m_api_key;
    return this->get_places(ss.str());
}

std::vector<Place> Meteosource::get_places(const std::string url)
{
    std::vector<Place> res;
    Json::Value req_res = this->m_request_handler->execute_request(url);
    if (!req_res)
    {
        return res;
    }
    res.reserve(req_res.size());
    for (Json::ArrayIndex i = 0; i < req_res.size(); ++i)
    {
        res.push_back(Place(req_res[i]));
    }
    return res;
}
