#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <cmath>
#include <stdexcept>
#include <json/json.h>

#include "Forecast.h"
#include "AirQuality.h"
#include "TimeMachine.h"
#include "Data.h"


static int failures = 0;
static int checks = 0;

#define CHECK(cond) do { \
    ++checks; \
    if (!(cond)) \
    { \
        ++failures; \
        std::cout << "FAIL " << __FILE__ << ":" << __LINE__ << ": " << #cond << std::endl; \
    } \
} while (0)

static bool almost_equal(double a, double b)
{
    return std::fabs(a - b) < 1e-9;
}

static Json::Value load_json(const std::string & path)
{
    std::ifstream f(path.c_str());
    if (!f)
    {
        throw std::runtime_error("Cannot open " + path);
    }
    std::stringstream ss;
    ss << f.rdbuf();
    std::string content = ss.str();

    Json::CharReaderBuilder builder;
    std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    Json::Value root;
    std::string errs;
    if (!reader->parse(content.data(), content.data() + content.size(), &root, &errs))
    {
        throw std::runtime_error("Cannot parse " + path + ": " + errs);
    }
    return root;
}

static void test_parse_lat_lon()
{
    CHECK(almost_equal(Forecast::parse_lat_lon("51.50853N"), 51.50853));
    CHECK(almost_equal(Forecast::parse_lat_lon("0.12574W"), -0.12574));
    CHECK(almost_equal(Forecast::parse_lat_lon("12.3E"), 12.3));
    CHECK(almost_equal(Forecast::parse_lat_lon("13.2S"), -13.2));

    bool thrown = false;
    try
    {
        Forecast::parse_lat_lon("12.3X");
    }
    catch (const std::invalid_argument &)
    {
        thrown = true;
    }
    CHECK(thrown);
}

static void test_forecast(const std::string & data_dir)
{
    Json::Value data = load_json(data_dir + "/point.json");
    Forecast f(data);

    CHECK(almost_equal(f.lat, 51.50853));
    CHECK(almost_equal(f.lon, -0.12574));
    CHECK(f.elevation == "25");
    CHECK(f.units == "metric");

    CHECK(f.current != nullptr);
    CHECK(f.current->summary == "Sunny");
    CHECK(almost_equal(f.current->temperature, 19.8));

    CHECK(f.minutely != nullptr);
    CHECK(f.minutely->data.size() == 116);
    CHECK(f.minutely->data[0]->date == "2021-09-08T09:45:00");
    CHECK(almost_equal(f.minutely->data[0]->precipitation, 0.0));

    CHECK(f.hourly.size() == 155);
    CHECK(f.hourly[0]->date == "2021-09-08T09:00:00");
    CHECK(almost_equal(f.hourly[0]->temperature, 19.8));
    CHECK(almost_equal(f.hourly[0]->feels_like, 18.8));

    CHECK(f.daily.size() == 30);
    CHECK(f.daily[0]->day == "2021-09-08");
    CHECK(f.daily[0]->weather == "partly_sunny");
    CHECK(f.daily[0]->all_day->weather == "partly_sunny");
    CHECK(f.daily[0]->astro->sun_rise == "2021-09-08T06:24:35");

    CHECK(f.alerts.size() == 4);
    CHECK(f.alerts[0]->event == "Ice");
    CHECK(f.alerts[0]->onset == "2022-03-08T22:00:00");
}

static void test_forecast_missing_sections(const std::string & data_dir)
{
    // A response with only some sections must leave the others empty
    Json::Value data = load_json(data_dir + "/point.json");
    data.removeMember("current");
    data.removeMember("minutely");
    data.removeMember("alerts");
    Forecast f(data);

    CHECK(f.current == nullptr);
    CHECK(f.minutely == nullptr);
    CHECK(f.alerts.size() == 0);
    CHECK(f.hourly.size() == 155);
}

static void test_air_quality(const std::string & data_dir)
{
    Json::Value data = load_json(data_dir + "/air_quality.json");
    AirQuality aq(data);

    CHECK(almost_equal(aq.lat, 51.50853));
    CHECK(almost_equal(aq.lon, -0.12574));
    CHECK(aq.elevation == "25");

    CHECK(aq.data.size() == 6);
    CHECK(aq.data[0]->date == "2026-07-28T00:00:00");
    CHECK(almost_equal(aq.data[0]->air_quality, 2));
    CHECK(almost_equal(aq.data[0]->aerosol_550, 0.19));
    CHECK(almost_equal(aq.data[0]->pm10, 14.8));
    CHECK(almost_equal(aq.data[0]->pm25, 8.9));
    CHECK(almost_equal(aq.data[0]->no2_surface, 11.2));
    CHECK(almost_equal(aq.data[0]->ozone_surface, 31.4));
    CHECK(almost_equal(aq.data[0]->ozone_total, 320.4));
    CHECK(almost_equal(aq.data[0]->co_surface, 155.5));
    CHECK(almost_equal(aq.data[0]->so2_surface, 1.6));
    CHECK(almost_equal(aq.data[0]->dust_550nm, 0.02));
    CHECK(almost_equal(aq.data[0]->dust_mixing_ratio_05, 0.0001));
    CHECK(almost_equal(aq.data[0]->no_surface, 0.5));
    CHECK(almost_equal(aq.data[3]->air_quality, 1));
    CHECK(aq.data[5]->date == "2026-07-28T05:00:00");
}

static void test_nearest_place(const std::string & data_dir)
{
    Json::Value data = load_json(data_dir + "/nearest_place.json");
    Place p(data);

    CHECK(p.name == "London");
    CHECK(p.place_id == "london");
    CHECK(p.adm_area1 == "England");
    CHECK(p.adm_area2 == "Greater London");
    CHECK(p.country == "United Kingdom");
    CHECK(almost_equal(p.lat, 51.50853));
    CHECK(almost_equal(p.lon, -0.12574));
    CHECK(p.timezone == "Europe/London");
    CHECK(p.type == "settlement");
}

static void test_find_places(const std::string & data_dir)
{
    Json::Value data = load_json(data_dir + "/find_places.json");

    std::vector<Place> places;
    places.reserve(data.size());
    for (Json::ArrayIndex i = 0; i < data.size(); ++i)
    {
        places.push_back(Place(data[i]));
    }

    CHECK(places.size() == 5);
    CHECK(places[0].name == "London");
    CHECK(places[0].place_id == "london");
    CHECK(places[1].place_id == "london-6058560");
}

static void test_time_machine(const std::string & data_dir)
{
    Json::Value data = load_json(data_dir + "/time_machine.json");
    TimeMachine tm(data);

    CHECK(almost_equal(tm.lat, 50.08804));
    CHECK(almost_equal(tm.lon, 14.42076));
    CHECK(tm.elevation == "202");
    CHECK(tm.units == "metric");

    CHECK(tm.data.size() == 24);
    CHECK(tm.data[0]->date == "2020-10-01T00:00:00");
    CHECK(tm.data[0]->weather == "partly_sunny");
    CHECK(almost_equal(tm.data[0]->temperature, 7.9));
    CHECK(almost_equal(tm.data[0]->soil_temperature, 9.5));
    CHECK(almost_equal(tm.data[0]->wind_angle, 291.0));
    CHECK(tm.data[0]->wind_dir == "WNW");
    CHECK(almost_equal(tm.data[0]->cloud_cover_total, 34.0));

    CHECK(tm.data[23]->date == "2020-10-01T23:00:00");
    CHECK(almost_equal(tm.data[23]->temperature, 8.6));

    CHECK(almost_equal(tm.daily->temperature, 15.5));
    CHECK(almost_equal(tm.daily->temperature_min, 10.0));
    CHECK(almost_equal(tm.daily->temperature_max, 18.25));
    CHECK(almost_equal(tm.daily->humidity, 68.75));

    CHECK(almost_equal(tm.statistics->temperature_avg, 13.0));
    CHECK(almost_equal(tm.statistics->temperature_avg_min, 8.5));
    CHECK(almost_equal(tm.statistics->temperature_avg_max, 17.5));
    CHECK(almost_equal(tm.statistics->temperature_record_min, 2.0));
    CHECK(almost_equal(tm.statistics->temperature_record_max, 25.2));
    CHECK(tm.statistics->wind_avg_dir == "S");
    CHECK(almost_equal(tm.statistics->precipitation_probability, 31.0));
}

int main(int argc, char ** argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <path to tests/data>" << std::endl;
        return 2;
    }
    const std::string data_dir = argv[1];

    try
    {
        test_parse_lat_lon();
        test_forecast(data_dir);
        test_forecast_missing_sections(data_dir);
        test_air_quality(data_dir);
        test_nearest_place(data_dir);
        test_find_places(data_dir);
        test_time_machine(data_dir);
    }
    catch (const std::exception & ex)
    {
        std::cout << "ERROR: " << ex.what() << std::endl;
        return 2;
    }

    std::cout << checks << " checks, " << failures << " failures" << std::endl;
    return failures == 0 ? 0 : 1;
}
