#ifndef DATA_H
#define DATA_H

#include <math.h>
#include <jsoncpp/json/json.h>

struct CurrentData
{
    CurrentData(const Json::Value & data);
    friend std::ostream & operator << (std::ostream & stream, const CurrentData & d);
    void print();

    std::string icon = "";
    int icon_num = -9999;
    std::string summary = "";
    double temperature = NAN;
    double feels_like = NAN;
    double wind_chill = NAN;
    double dew_point = NAN;
    double wind_speed = NAN;
    double wind_gusts = NAN;
    double wind_angle = NAN;
    std::string wind_dir = "";
    double precipitation_total = NAN;
    std::string precipitation_type = "";
    double cloud_cover = NAN;
    double irradiance = NAN;
    double ozone = NAN;
    double pressure = NAN;
    double uv_index = NAN;
    double humidity = NAN;
    double visibility = NAN;
};

struct MinuteData
{
    MinuteData(const Json::Value & data);
    friend std::ostream & operator << (std::ostream & stream, const MinuteData & d);
    void print();

    std::string date = "";
    double precipitation = NAN;
};

struct MinutelyData
{
    MinutelyData(const Json::Value & data);
    friend std::ostream & operator << (std::ostream & stream, const MinutelyData & d);
    void print();

    std::string summary = "";
    std::vector<std::unique_ptr<MinuteData> > data;
};


struct HourlyData
{
    HourlyData(const Json::Value & data);
    friend std::ostream & operator << (std::ostream & stream, const HourlyData & d);
    void print();

    std::string date = "";
    std::string weather = "";
    int icon = -9999;
    std::string summary = "";
    double temperature = NAN;
    double feels_like = NAN;
    double soil_temperature = NAN;
    double wind_chill = NAN;
    double dew_point = NAN;
    double surface_temperature = NAN;
    double wind_speed = NAN;
    double wind_gusts = NAN;
    std::string wind_dir = "";
    double wind_angle = NAN;
    double cloud_cover_total = NAN;
    double cloud_cover_low = NAN;
    double cloud_cover_middle = NAN;
    double cloud_cover_high = NAN;
    double pressure = NAN;
    double precipitation_total = NAN;
    std::string precipitation_type = "";
    double precipitation_convective = NAN;
    std::string precipitation_rainspot = "";
    double probability_precipitation = NAN;
    double probability_storm = NAN;
    double probability_freeze = NAN;
    double cape = NAN;
    double evaporation = NAN;
    double irradiance = NAN;
    double lftx = NAN;
    double ozone = NAN;
    double uv_index = NAN;
    double humidity = NAN;
    double snow_depth = NAN;
    double sunshine_duration = NAN;
    double visibility = NAN;
};


struct AllDayData
{
    AllDayData(const Json::Value & data);
    friend std::ostream & operator << (std::ostream & stream, const AllDayData & d);
    void print();

    std::string weather = "";
    int icon = -9999;
    double temperature = NAN;
    double temperature_min = NAN;
    double temperature_max = NAN;
    double feels_like = NAN;
    double feels_like_min = NAN;
    double feels_like_max = NAN;
    double soil_temperature = NAN;
    double soil_temperature_min = NAN;
    double soil_temperature_max = NAN;
    double wind_chill = NAN;
    double wind_chill_min = NAN;
    double wind_chill_max = NAN;
    double dew_point = NAN;
    double dew_point_min = NAN;
    double dew_point_max = NAN;
    double surface_temperature = NAN;
    double surface_temperature_min = NAN;
    double surface_temperature_max = NAN;
    double wind_speed = NAN;
    double wind_gusts = NAN;
    std::string wind_dir = "";
    double wind_angle = NAN;
    double cloud_cover_total = NAN;
    double cloud_cover_low = NAN;
    double cloud_cover_middle = NAN;
    double cloud_cover_high = NAN;
    double pressure = NAN;
    double precipitation_total = NAN;
    std::string precipitation_type = "";
    double probability_precipitation = NAN;
    double probability_storm = NAN;
    double probability_freeze = NAN;
    double ozone = NAN;
    double humidity = NAN;
    double snow_depth = NAN;
    double visibility = NAN;
};


struct PartDayData
{
    PartDayData(const Json::Value & data);
    friend std::ostream & operator << (std::ostream & stream, const PartDayData & d);
    void print();

    std::string weather = "";
    int icon = -9999;
    double temperature = NAN;
    double feels_like = NAN;
    double soil_temperature = NAN;
    double wind_chill = NAN;
    double dew_point = NAN;
    double surface_temperature = NAN;
    double wind_speed = NAN;
    double wind_gusts = NAN;
    std::string wind_dir = "";
    double wind_angle = NAN;
    double cloud_cover_total = NAN;
    double cloud_cover_low = NAN;
    double cloud_cover_middle = NAN;
    double cloud_cover_high = NAN;
    double pressure = NAN;
    double precipitation_total = NAN;
    std::string precipitation_type = "";
    double probability_precipitation = NAN;
    double probability_storm = NAN;
    double probability_freeze = NAN;
    double ozone = NAN;
    double humidity = NAN;
    double snow_depth = NAN;
    double visibility = NAN;
};


struct AstroData
{
    AstroData(const Json::Value & data);
    friend std::ostream & operator << (std::ostream & stream, const AstroData & d);
    void print();

    std::string sun_rise = "";
    std::string sun_set = "";
    bool sun_always_up = false;
    bool sun_always_down = false;
    std::string moon_phase = "";
    std::string moon_rise = "";
    std::string moon_set = "";
    bool moon_always_up = false;
    bool moon_always_down = false;
};


struct StatisticsData
{
    StatisticsData(const Json::Value & data);
    friend std::ostream & operator << (std::ostream & stream, const StatisticsData & d);
    void print();

    double temperature_avg = NAN;
    double temperature_avg_min = NAN;
    double temperature_avg_max = NAN;
    double temperature_record_min = NAN;
    double temperature_record_max = NAN;
    double wind_avg_speed = NAN;
    double wind_avg_angle = NAN;
    std::string wind_avg_dir = "";
    double wind_max_speed = NAN;
    double wind_max_gust = NAN;
    double precipitation_avg = NAN;
    double precipitation_probability = NAN;
};

struct DailyData
{
    DailyData(const Json::Value & data);
    friend std::ostream & operator << (std::ostream & stream, const DailyData & d);
    void print();

    std::string day = "";
    std::string weather = "";
    int icon = -9999;
    std::string summary = "";
    double predictability = NAN;

    std::unique_ptr<AllDayData> all_day;
    std::unique_ptr<PartDayData> morning;
    std::unique_ptr<PartDayData> afternoon;
    std::unique_ptr<PartDayData> evening;
    std::unique_ptr<AstroData> astro;
    std::unique_ptr<StatisticsData> statistics;
};

struct Alert
{
    Alert(const Json::Value & data);
    friend std::ostream & operator << (std::ostream & stream, const Alert & d);
    void print();

    std::string event = "";
    std::string onset = "";
    std::string expires = "";
    std::string sender = "";
    std::string certainty = "";
    std::string severity = "";
    std::string headline = "";
    std::string description = "";
};


#endif //DATA_H
