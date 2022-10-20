#include <iostream>
#include <string>
#include <cmath>
#include <limits>
#include <memory>

#include "Data.h"


CurrentData::CurrentData(const Json::Value & data)
{
    this->icon = data.get("icon", "").asString();
    this->icon_num = data.get("icon_num", -9999).asInt();
    this->summary = data.get("summary", "").asString();
    this->temperature = data.get("temperature", NAN).asDouble();
    this->feels_like = data.get("feels_like", NAN).asDouble();
    this->wind_chill = data.get("wind_chill", NAN).asDouble();
    this->dew_point = data.get("dew_point", NAN).asDouble();
    if (data.isMember("wind"))
    {
        this->wind_speed = data["wind"].get("speed", NAN).asDouble();
        this->wind_gusts = data["wind"].get("gusts", NAN).asDouble();
        this->wind_angle = data["wind"].get("angle", NAN).asDouble();
        this->wind_dir = data["wind"].get("dir", "").asString();
    }
    if (data.isMember("precipitation"))
    {
        this->precipitation_total = data["precipitation"].get("total", NAN).asDouble();
        this->precipitation_type = data["precipitation"].get("type", "").asString();
    }
    this->cloud_cover = data.get("cloud_cover", NAN).asDouble();
    this->irradiance = data.get("irradiance", NAN).asDouble();
    this->ozone = data.get("ozone", NAN).asDouble();
    this->pressure = data.get("pressure", NAN).asDouble();
    this->uv_index = data.get("uv_index", NAN).asDouble();
    this->humidity = data.get("humidity", NAN).asDouble();
    this->visibility = data.get("visibility", NAN).asDouble();
}

std::ostream & operator<<(std::ostream &os, const CurrentData & d)
{
    return os << "<Current data>";
}

void CurrentData::print()
{
    std::cout << "icon: " << this->icon << std::endl;
    std::cout << "icon_num: " << this->icon_num << std::endl;
    std::cout << "summary: " << this->summary << std::endl;
    std::cout << "temperature: " << this->temperature << std::endl;
    std::cout << "feels_like: " << this->feels_like << std::endl;
    std::cout << "wind_chill: " << this->wind_chill << std::endl;
    std::cout << "dew_point: " << this->dew_point << std::endl;
    std::cout << "wind_speed: " << this->wind_speed << std::endl;
    std::cout << "wind_gusts: " << this->wind_gusts << std::endl;
    std::cout << "wind_angle: " << this->wind_angle << std::endl;
    std::cout << "wind_dir: " << this->wind_dir << std::endl;
    std::cout << "precipitation_total: " << this->precipitation_total << std::endl;
    std::cout << "precipitation_type: " << this->precipitation_type << std::endl;
    std::cout << "cloud_cover: " << this->cloud_cover << std::endl;
    std::cout << "irradiance: " << this->irradiance << std::endl;
    std::cout << "ozone: " << this->ozone << std::endl;
    std::cout << "pressure: " << this->pressure << std::endl;
    std::cout << "uv_index: " << this->uv_index << std::endl;
    std::cout << "humidity: " << this->humidity << std::endl;
    std::cout << "visibility: " << this->visibility << std::endl;
}


MinuteData::MinuteData(const Json::Value & data)
{
    this->date = data.get("date", "").asString();
    this->precipitation = data.get("precipitation", NAN).asDouble();
}

std::ostream & operator<<(std::ostream &os, const MinuteData & d)
{
    return os << "<Minutely precipitation for " << d.date << ">";
}

void MinuteData::print()
{
    std::cout << "date: " << this->date << std::endl;
    std::cout << "precipitation: " << this->precipitation << std::endl;
}


MinutelyData::MinutelyData(const Json::Value & data)
{
    this->summary = data.get("summary", "").asString();
    this->data.reserve(data["data"].size());
    for (int i = 0; i < data["data"].size(); ++i)
    {
        this->data.push_back(std::unique_ptr<MinuteData>(new MinuteData(data["data"][i])));
    }
}

std::ostream & operator<<(std::ostream &os, const MinutelyData & d)
{
    size_t sz = d.data.size();
    return os << "<Minutely data from " << d.data[0]->date << " to " << d.data[sz - 1]->date << " (" << sz << " timesteps)>";
}

void MinutelyData::print()
{
    std::cout << "summary: " << this->summary << std::endl;
    std::cout << "data: " << *this << std::endl;
}



HourlyData::HourlyData(const Json::Value & data)
{
    this->date = data.get("date", "").asString();
    this->weather = data.get("weather", "").asString();
    this->icon = data.get("icon", -9999).asInt();
    this->summary = data.get("summary", "").asString();
    this->temperature = data.get("temperature", NAN).asDouble();
    this->feels_like = data.get("feels_like", NAN).asDouble();
    this->soil_temperature = data.get("soil_temperature", NAN).asDouble();
    this->wind_chill = data.get("wind_chill", NAN).asDouble();
    this->dew_point = data.get("dew_point", NAN).asDouble();
    this->surface_temperature = data.get("surface_temperature", NAN).asDouble();
    if (data.isMember("wind"))
    {
        this->wind_speed = data["wind"].get("speed", NAN).asDouble();
        this->wind_gusts = data["wind"].get("gusts", NAN).asDouble();
        this->wind_dir = data["wind"].get("dir", "").asString();
        this->wind_angle = data["wind"].get("angle", NAN).asDouble();
    }
    if (data.isMember("cloud_cover"))
    {
        this->cloud_cover_total = data["cloud_cover"].get("total", NAN).asDouble();
        this->cloud_cover_low = data["cloud_cover"].get("low", NAN).asDouble();
        this->cloud_cover_middle = data["cloud_cover"].get("middle", NAN).asDouble();
        this->cloud_cover_high = data["cloud_cover"].get("high", NAN).asDouble();
    }
    this->pressure = data.get("pressure", NAN).asDouble();
    if (data.isMember("precipitation"))
    {
        this->precipitation_total = data["precipitation"].get("total", NAN).asDouble();
        this->precipitation_type = data["precipitation"].get("type", "").asString();
        this->precipitation_convective = data["precipitation"].get("convective", NAN).asDouble();
        this->precipitation_rainspot = data["precipitation"].get("rainspot", "").asString();
    }
    if (data.isMember("probability"))
    {
        this->probability_precipitation = data["probability"].get("precipitation", NAN).asDouble();
        this->probability_storm = data["probability"].get("storm", NAN).asDouble();
        this->probability_freeze = data["probability"].get("freeze", NAN).asDouble();
    }
    this->cape = data.get("cape", NAN).asDouble();
    this->evaporation = data.get("evaporation", NAN).asDouble();
    this->irradiance = data.get("irradiance", NAN).asDouble();
    this->lftx = data.get("lftx", NAN).asDouble();
    this->ozone = data.get("ozone", NAN).asDouble();
    this->uv_index = data.get("uv_index", NAN).asDouble();
    this->humidity = data.get("humidity", NAN).asDouble();
    this->snow_depth = data.get("snow_depth", NAN).asDouble();
    this->sunshine_duration = data.get("sunshine_duration", NAN).asDouble();
    this->visibility = data.get("visibility", NAN).asDouble();
}

std::ostream & operator<<(std::ostream &os, const HourlyData & d)
{
    return os << "<Hourly data for " << d.date << ">";
}

void HourlyData::print()
{
    std::cout << "date: " << this->date << std::endl;
    std::cout << "weather: " << this->weather << std::endl;
    std::cout << "icon: " << this->icon << std::endl;
    std::cout << "summary: " << this->summary << std::endl;
    std::cout << "temperature: " << this->temperature << std::endl;
    std::cout << "feels_like: " << this->feels_like << std::endl;
    std::cout << "soil_temperature: " << this->soil_temperature << std::endl;
    std::cout << "wind_chill: " << this->wind_chill << std::endl;
    std::cout << "dew_point: " << this->dew_point << std::endl;
    std::cout << "surface_temperature: " << this->surface_temperature << std::endl;
    std::cout << "wind_speed: " << this->wind_speed << std::endl;
    std::cout << "wind_gusts: " << this->wind_gusts << std::endl;
    std::cout << "wind_dir: " << this->wind_dir << std::endl;
    std::cout << "wind_angle: " << this->wind_angle << std::endl;
    std::cout << "cloud_cover_total: " << this->cloud_cover_total << std::endl;
    std::cout << "cloud_cover_low: " << this->cloud_cover_low << std::endl;
    std::cout << "cloud_cover_middle: " << this->cloud_cover_middle << std::endl;
    std::cout << "cloud_cover_high: " << this->cloud_cover_high << std::endl;
    std::cout << "pressure: " << this->pressure << std::endl;
    std::cout << "precipitation_total: " << this->precipitation_total << std::endl;
    std::cout << "precipitation_type: " << this->precipitation_type << std::endl;
    std::cout << "precipitation_convective: " << this->precipitation_convective << std::endl;
    std::cout << "precipitation_rainspot:\n" << this->precipitation_rainspot << std::endl;
    std::cout << "probability_precipitation: " << this->probability_precipitation << std::endl;
    std::cout << "probability_storm: " << this->probability_storm << std::endl;
    std::cout << "probability_freeze: " << this->probability_freeze << std::endl;
    std::cout << "cape: " << this->cape << std::endl;
    std::cout << "evaporation: " << this->evaporation << std::endl;
    std::cout << "irradiance: " << this->irradiance << std::endl;
    std::cout << "lftx: " << this->lftx << std::endl;
    std::cout << "ozone: " << this->ozone << std::endl;
    std::cout << "uv_index: " << this->uv_index << std::endl;
    std::cout << "humidity: " << this->humidity << std::endl;
    std::cout << "snow_depth: " << this->snow_depth << std::endl;
    std::cout << "sunshine_duration: " << this->sunshine_duration << std::endl;
    std::cout << "visibility: " << this->visibility << std::endl;
}


AllDayData::AllDayData(const Json::Value & data)
{
    this->weather = data.get("weather", "").asString();
    this->icon = data.get("icon", -9999).asInt();
    this->temperature = data.get("temperature", NAN).asDouble();
    this->temperature_min = data.get("temperature_min", NAN).asDouble();
    this->temperature_max = data.get("temperature_max", NAN).asDouble();
    this->feels_like = data.get("feels_like", NAN).asDouble();
    this->feels_like_min = data.get("feels_like_min", NAN).asDouble();
    this->feels_like_max = data.get("feels_like_max", NAN).asDouble();
    this->soil_temperature = data.get("soil_temperature", NAN).asDouble();
    this->soil_temperature_min = data.get("soil_temperature_min", NAN).asDouble();
    this->soil_temperature_max = data.get("soil_temperature_max", NAN).asDouble();
    this->wind_chill = data.get("wind_chill", NAN).asDouble();
    this->wind_chill_min = data.get("wind_chill_min", NAN).asDouble();
    this->wind_chill_max = data.get("wind_chill_max", NAN).asDouble();
    this->dew_point = data.get("dew_point", NAN).asDouble();
    this->dew_point_min = data.get("dew_point_min", NAN).asDouble();
    this->dew_point_max = data.get("dew_point_max", NAN).asDouble();
    this->surface_temperature = data.get("surface_temperature", NAN).asDouble();
    this->surface_temperature_min = data.get("surface_temperature_min", NAN).asDouble();
    this->surface_temperature_max = data.get("surface_temperature_max", NAN).asDouble();
    if (data.isMember("wind"))
    {
        this->wind_speed = data["wind"].get("speed", NAN).asDouble();
        this->wind_gusts = data["wind"].get("gusts", NAN).asDouble();
        this->wind_dir = data["wind"].get("dir", "").asString();
        this->wind_angle = data["wind"].get("angle", NAN).asDouble();
    }
    if (data.isMember("cloud_cover"))
    {
        this->cloud_cover_total = data["cloud_cover"].get("total", NAN).asDouble();
        this->cloud_cover_low = data["cloud_cover"].get("low", NAN).asDouble();
        this->cloud_cover_middle = data["cloud_cover"].get("middle", NAN).asDouble();
        this->cloud_cover_high = data["cloud_cover"].get("high", NAN).asDouble();
    }
    this->pressure = data.get("pressure", NAN).asDouble();
    if (data.isMember("precipitation"))
    {
        this->precipitation_total = data["precipitation"].get("total", NAN).asDouble();
        this->precipitation_type = data["precipitation"].get("type", "").asString();
    }
    if (data.isMember("probability"))
    {
        this->probability_precipitation = data["probability"].get("precipitation", NAN).asDouble();
        this->probability_storm = data["probability"].get("storm", NAN).asDouble();
        this->probability_freeze = data["probability"].get("freeze", NAN).asDouble();
    }
    this->ozone = data.get("ozone", NAN).asDouble();
    this->humidity = data.get("humidity", NAN).asDouble();
    this->snow_depth = data.get("snow_depth", NAN).asDouble();
    this->visibility = data.get("visibility", NAN).asDouble();
};

std::ostream & operator<<(std::ostream &os, const AllDayData & d)
{
    return os << "<All day data>";
}

void AllDayData::print()
{
    std::cout << "weather: " << this->weather << std::endl;
    std::cout << "icon: " << this->icon << std::endl;
    std::cout << "temperature: " << this->temperature << std::endl;
    std::cout << "temperature_min: " << this->temperature_min << std::endl;
    std::cout << "temperature_max: " << this->temperature_max << std::endl;
    std::cout << "feels_like: " << this->feels_like << std::endl;
    std::cout << "feels_like_min: " << this->feels_like_min << std::endl;
    std::cout << "feels_like_max: " << this->feels_like_max << std::endl;
    std::cout << "soil_temperature: " << this->soil_temperature << std::endl;
    std::cout << "soil_temperature_min: " << this->soil_temperature_min << std::endl;
    std::cout << "soil_temperature_max: " << this->soil_temperature_max << std::endl;
    std::cout << "wind_chill: " << this->wind_chill << std::endl;
    std::cout << "wind_chill_min: " << this->wind_chill_min << std::endl;
    std::cout << "wind_chill_max: " << this->wind_chill_max << std::endl;
    std::cout << "dew_point: " << this->dew_point << std::endl;
    std::cout << "dew_point_min: " << this->dew_point_min << std::endl;
    std::cout << "dew_point_max: " << this->dew_point_max << std::endl;
    std::cout << "surface_temperature: " << this->surface_temperature << std::endl;
    std::cout << "surface_temperature_min: " << this->surface_temperature_min << std::endl;
    std::cout << "surface_temperature_max: " << this->surface_temperature_max << std::endl;
    std::cout << "wind_speed: " << this->wind_speed << std::endl;
    std::cout << "wind_gusts: " << this->wind_gusts << std::endl;
    std::cout << "wind_dir: " << this->wind_dir << std::endl;
    std::cout << "wind_angle: " << this->wind_angle << std::endl;
    std::cout << "cloud_cover_total: " << this->cloud_cover_total << std::endl;
    std::cout << "cloud_cover_low: " << this->cloud_cover_low << std::endl;
    std::cout << "cloud_cover_middle: " << this->cloud_cover_middle << std::endl;
    std::cout << "cloud_cover_high: " << this->cloud_cover_high << std::endl;
    std::cout << "pressure: " << this->pressure << std::endl;
    std::cout << "precipitation_total: " << this->precipitation_total << std::endl;
    std::cout << "precipitation_type: " << this->precipitation_type << std::endl;
    std::cout << "probability_precipitation: " << this->probability_precipitation << std::endl;
    std::cout << "probability_storm: " << this->probability_storm << std::endl;
    std::cout << "probability_freeze: " << this->probability_freeze << std::endl;
    std::cout << "ozone: " << this->ozone << std::endl;
    std::cout << "humidity: " << this->humidity << std::endl;
    std::cout << "snow_depth: " << this->snow_depth << std::endl;
    std::cout << "visibility: " << this->visibility << std::endl;
}


PartDayData::PartDayData(const Json::Value & data)
{
    this->weather = data.get("weather", "").asString();
    this->icon = data.get("icon", -9999).asInt();
    this->temperature = data.get("temperature", NAN).asDouble();
    this->feels_like = data.get("feels_like", NAN).asDouble();
    this->soil_temperature = data.get("soil_temperature", NAN).asDouble();
    this->wind_chill = data.get("wind_chill", NAN).asDouble();
    this->dew_point = data.get("dew_point", NAN).asDouble();
    this->surface_temperature = data.get("surface_temperature", NAN).asDouble();

    if (data.isMember("wind"))
    {
        this->wind_speed = data["wind"].get("speed", NAN).asDouble();
        this->wind_gusts = data["wind"].get("gusts", NAN).asDouble();
        this->wind_dir = data["wind"].get("dir", "").asString();
        this->wind_angle = data["wind"].get("angle", NAN).asDouble();
    }

    if (data.isMember("cloud_cover"))
    {
        this->cloud_cover_total = data["cloud_cover"].get("total", NAN).asDouble();
        this->cloud_cover_low = data["cloud_cover"].get("low", NAN).asDouble();
        this->cloud_cover_middle = data["cloud_cover"].get("middle", NAN).asDouble();
        this->cloud_cover_high = data["cloud_cover"].get("high", NAN).asDouble();
    }
    this->pressure = data.get("pressure", NAN).asDouble();
    if (data.isMember("precipitation"))
    {
        this->precipitation_total = data["precipitation"].get("total", NAN).asDouble();
        this->precipitation_type = data["precipitation"].get("type", "").asString();
    }
    if (data.isMember("probability"))
    {
        this->probability_precipitation = data["probability"].get("precipitation", NAN).asDouble();
        this->probability_storm = data["probability"].get("storm", NAN).asDouble();
        this->probability_freeze = data["probability"].get("freeze", NAN).asDouble();
    }
    this->ozone = data.get("ozone", NAN).asDouble();
    this->humidity = data.get("humidity", NAN).asDouble();
    this->snow_depth = data.get("snow_depth", NAN).asDouble();
    this->visibility = data.get("visibility", NAN).asDouble();
};

std::ostream & operator<<(std::ostream &os, const PartDayData & d)
{
    return os << "<Part day data>";
}

void PartDayData::print()
{
    std::cout << "weather: " << this->weather << std::endl;
    std::cout << "icon: " << this->icon << std::endl;
    std::cout << "temperature: " << this->temperature << std::endl;
    std::cout << "feels_like: " << this->feels_like << std::endl;
    std::cout << "soil_temperature: " << this->soil_temperature << std::endl;
    std::cout << "wind_chill: " << this->wind_chill << std::endl;
    std::cout << "dew_point: " << this->dew_point << std::endl;
    std::cout << "surface_temperature: " << this->surface_temperature << std::endl;
    std::cout << "wind_speed: " << this->wind_speed << std::endl;
    std::cout << "wind_gusts: " << this->wind_gusts << std::endl;
    std::cout << "wind_dir: " << this->wind_dir << std::endl;
    std::cout << "wind_angle: " << this->wind_angle << std::endl;
    std::cout << "cloud_cover_total: " << this->cloud_cover_total << std::endl;
    std::cout << "cloud_cover_low: " << this->cloud_cover_low << std::endl;
    std::cout << "cloud_cover_middle: " << this->cloud_cover_middle << std::endl;
    std::cout << "cloud_cover_high: " << this->cloud_cover_high << std::endl;
    std::cout << "pressure: " << this->pressure << std::endl;
    std::cout << "precipitation_total: " << this->precipitation_total << std::endl;
    std::cout << "precipitation_type: " << this->precipitation_type << std::endl;
    std::cout << "probability_precipitation: " << this->probability_precipitation << std::endl;
    std::cout << "probability_storm: " << this->probability_storm << std::endl;
    std::cout << "probability_freeze: " << this->probability_freeze << std::endl;
    std::cout << "ozone: " << this->ozone << std::endl;
    std::cout << "humidity: " << this->humidity << std::endl;
    std::cout << "snow_depth: " << this->snow_depth << std::endl;
    std::cout << "visibility: " << this->visibility << std::endl;
}


AstroData::AstroData(const Json::Value & data)
{
    if (data.isMember("sun"))
    {
        this->sun_rise = data["sun"].get("rise", "").asString();
        this->sun_set = data["sun"].get("set", "").asString();
        this->sun_always_up = data["sun"].get("always_up", false).asBool();
        this->sun_always_down = data["sun"].get("always_down", false).asBool();
    }
    if (data.isMember("moon"))
    {
        this->moon_phase = data["moon"].get("phase", "").asString();
        this->moon_rise = data["moon"].get("rise", "").asString();
        this->moon_set = data["moon"].get("set", "").asString();
        this->moon_always_up = data["moon"].get("always_up", false).asBool();
        this->moon_always_down = data["moon"].get("always_down", false).asBool();
    }
}

std::ostream & operator<<(std::ostream &os, const AstroData & d)
{
    return os << "<Astro data>";
}

void AstroData::print()
{
    std::cout << "sun_rise: " << this->sun_rise << std::endl;
    std::cout << "sun_set: " << this->sun_set << std::endl;
    std::cout << "sun_always_up: " << this->sun_always_up << std::endl;
    std::cout << "sun_always_down: " << this->sun_always_down << std::endl;
    std::cout << "moon_phase: " << this->moon_phase << std::endl;
    std::cout << "moon_rise: " << this->moon_rise << std::endl;
    std::cout << "moon_set: " << this->moon_set << std::endl;
    std::cout << "moon_always_up: " << this->moon_always_up << std::endl;
    std::cout << "moon_always_down: " << this->moon_always_down << std::endl;
}


StatisticsData::StatisticsData(const Json::Value & data)
{
    if (data.isMember("temperature"))
    {
        this->temperature_avg = data["temperature"].get("avg", NAN).asDouble();
        this->temperature_avg_min = data["temperature"].get("avg_min", NAN).asDouble();
        this->temperature_avg_max = data["temperature"].get("avg_max", NAN).asDouble();
        this->temperature_record_min = data["temperature"].get("record_min", NAN).asDouble();
        this->temperature_record_max = data["temperature"].get("record_max", NAN).asDouble();
    }
    if (data.isMember("wind"))
    {
        this->wind_avg_speed = data["wind"].get("avg_speed", NAN).asDouble();
        this->wind_avg_angle = data["wind"].get("avg_angle", NAN).asDouble();
        this->wind_avg_dir = data["wind"].get("avg_dir", "").asString();
        this->wind_max_speed = data["wind"].get("max_speed", NAN).asDouble();
        this->wind_max_gust = data["wind"].get("max_gust", NAN).asDouble();
    }
    if (data.isMember("precipitation"))
    {
        this->precipitation_avg = data["precipitation"].get("avg", NAN).asDouble();
        this->precipitation_probability = data["precipitation"].get("probability", NAN).asDouble();
    }
}

std::ostream & operator<<(std::ostream &os, const StatisticsData & d)
{
    return os << "<Statistics data>";
}

void StatisticsData::print()
{
    std::cout << "temperature_avg: " << this->temperature_avg << std::endl;
    std::cout << "temperature_avg_min: " << this->temperature_avg_min << std::endl;
    std::cout << "temperature_avg_max: " << this->temperature_avg_max << std::endl;
    std::cout << "temperature_record_min: " << this->temperature_record_min << std::endl;
    std::cout << "temperature_record_max: " << this->temperature_record_max << std::endl;
    std::cout << "wind_avg_speed: " << this->wind_avg_speed << std::endl;
    std::cout << "wind_avg_angle: " << this->wind_avg_angle << std::endl;
    std::cout << "wind_avg_dir: " << this->wind_avg_dir << std::endl;
    std::cout << "wind_max_speed: " << this->wind_max_speed << std::endl;
    std::cout << "wind_max_gust: " << this->wind_max_gust << std::endl;
    std::cout << "precipitation_avg: " << this->precipitation_avg << std::endl;
    std::cout << "precipitation_probability: " << this->precipitation_probability << std::endl;
}

DailyData::DailyData(const Json::Value & data)
{
    this->day = data.get("day", "").asString();
    this->weather = data.get("weather", "").asString();
    this->icon = data.get("icon", -9999).asInt();
    this->summary = data.get("summary", "").asString();
    this->predictability = data.get("predictability", NAN).asDouble();
    this->all_day = std::unique_ptr<AllDayData>(new AllDayData(data["all_day"]));
    this->morning = std::unique_ptr<PartDayData>(new PartDayData(data["morning"]));
    this->afternoon = std::unique_ptr<PartDayData>(new PartDayData(data["afternoon"]));
    this->evening = std::unique_ptr<PartDayData>(new PartDayData(data["evening"]));
    this->astro = std::unique_ptr<AstroData>(new AstroData(data["astro"]));
    this->statistics = std::unique_ptr<StatisticsData>(new StatisticsData(data["statistics"]));
};

std::ostream & operator<<(std::ostream &os, const DailyData & d)
{
    return os << "<Daily data for " << d.day << ">";
}

void DailyData::print()
{
    std::cout << "day: " << this->day << std::endl;
    std::cout << "weather: " << this->weather << std::endl;
    std::cout << "icon: " << this->icon << std::endl;
    std::cout << "summary: " << this->summary << std::endl;
    std::cout << "predictability: " << this->predictability << std::endl;

    std::cout << "all_day: " << *this->all_day << std::endl;
    std::cout << "morning: " << *this->morning << std::endl;
    std::cout << "afternoon: " << *this->afternoon << std::endl;
    std::cout << "evening: " << *this->evening << std::endl;
    std::cout << "astro: " << *this->astro << std::endl;
    std::cout << "statistics: " << *this->statistics << std::endl;
}

Alert::Alert(const Json::Value & data)
{
    this->event = data.get("event", "").asString();
    this->onset = data.get("onset", "").asString();
    this->expires = data.get("expires", "").asString();
    this->sender = data.get("sender", "").asString();
    this->certainty = data.get("certainty", "").asString();
    this->severity = data.get("severity", "").asString();
    this->headline = data.get("headline", "").asString();
    this->description = data.get("description", "").asString();
}

std::ostream & operator<<(std::ostream &os, const Alert & d)
{
    return os << "<Alert valid from " << d.onset << " to " << d.expires << ">";
}

void Alert::print()
{
    std::cout << "event: " << this->event << std::endl;
    std::cout << "onset: " << this->onset << std::endl;
    std::cout << "expires: " << this->expires << std::endl;
    std::cout << "sender: " << this->sender << std::endl;
    std::cout << "certainty: " << this->certainty << std::endl;
    std::cout << "severity: " << this->severity << std::endl;
    std::cout << "headline: " << this->headline << std::endl;
    std::cout << "description: " << this->description << std::endl;
}
