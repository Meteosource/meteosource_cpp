meteosource_cpp - Weather API library
==========

C++ wrapper library for [Meteosource weather API](https://www.meteosource.com) that provides detailed hyperlocal weather forecasts for any location on earth.

The library builds on Linux and Windows (MSVC) using CMake.


## Dependencies
The `meteosource_cpp` library needs the following libraries installed:

  - [`libcurl`](https://curl.se/libcurl/c/)
  - [`jsoncpp`](https://github.com/open-source-parsers/jsoncpp)


## Get started

To use this library, you need to obtain your Meteosource API key. You can [sign up](https://www.meteosource.com/client/sign-up) or get the API key of existing account in [your dashboard](https://www.meteosource.com/client).


## Building

### Linux

Install the dependencies and CMake, e.g. on Debian/Ubuntu:

```bash
sudo apt-get update
sudo apt-get install build-essential cmake pkg-config libcurl4-openssl-dev libjsoncpp-dev
```

Then configure and build:

```bash
cmake -B build -S .
cmake --build build
```

This produces the static library `libmeteosource.a` and the `example` executable in the `build` directory.

### Windows (MSVC + vcpkg)

Install the dependencies with [vcpkg](https://github.com/microsoft/vcpkg):

```powershell
vcpkg install curl jsoncpp --triplet x64-windows
```

Then configure and build using the vcpkg toolchain file (adjust the vcpkg path to your installation):

```powershell
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE="C:/vcpkg/scripts/buildsystems/vcpkg.cmake" -DVCPKG_TARGET_TRIPLET=x64-windows
cmake --build build --config Release
```

This produces the static library `meteosource.lib` and `example.exe` in `build/Release`.


## Library usage

You can either link against the `meteosource` static library built above, or simply copy all `*.cpp` and `*.h` files from the `src` folder into your project. Then you can `#include "Meteosource.h"` and fetch the weather data you need.

Example usage is shown in `example.cpp`, which is built automatically as the `example` target. Be sure to change variables `api_key` and `tier` in `example.cpp` to your actual API key and tier before compiling.

### Requesting a forecast

You can request a point forecast either by place identifier:

```cpp
Meteosource m(api_key, tier);
auto forecast = m.get_point_forecast("london", "current,hourly", "UTC", "en", "auto");
```

or by geographic coordinates (latitude and longitude in decimal degrees, north and east positive):

```cpp
Meteosource m(api_key, tier);
auto forecast = m.get_point_forecast(51.50853, -0.12574, "current,hourly", "UTC", "en", "auto");
```

Both variants return a `std::unique_ptr<Forecast>`, or `nullptr` if the request fails.

### Air quality

To get air pollution data for a place, use `get_air_quality`. Like the point forecast, it accepts either a place identifier or coordinates:

```cpp
auto air_quality = m.get_air_quality("london", "UTC");
auto air_quality2 = m.get_air_quality(51.50853, -0.12574, "UTC");

if (air_quality)
{
    for (unsigned int i = 0; i < air_quality->data.size(); ++i)
        std::cout << air_quality->data[i]->date << ": PM10 " << air_quality->data[i]->pm10 << std::endl;
}
```

It returns a `std::unique_ptr<AirQuality>` with hourly `AirQualityData` timesteps, or `nullptr` if the request fails. Note that air quality data are not available in the free tier.

### Finding places

To search for places by name (or ZIP code), use `find_places`. For autocomplete there is `find_places_prefix`, which matches the beginning of the words. To get the nearest named place for GPS coordinates, use `get_nearest_place`:

```cpp
// Search for places by name, returns std::vector<Place>
auto places = m.find_places("london", "en");

// Prefix search, useful for autocomplete
auto suggestions = m.find_places_prefix("lond", "en");

// Nearest named place for coordinates, returns std::unique_ptr<Place>
auto place = m.get_nearest_place(51.50853, -0.12574, "en");
if (place)
{
    std::cout << place->name << " (" << place->place_id << "), " << place->timezone << std::endl;
}
```

The `Place` struct contains `name`, `place_id`, `adm_area1`, `adm_area2`, `country`, `lat`, `lon`, `timezone` and `type`.

### Usage notes

The library uses parameter values and variable names with the same convention as the API itself. One exception are nested variables, such as `wind.speed` in the API. This library uses `_` to separate the levels, so `wind.speed` becomes `wind_speed`. You can see the available variable names for the individual sections in `src/Data.h` file.

The library uses empty string as default value for `std::string` variables, `NAN` for `double` variables and `-9999` for `int` variable (only `icon` variable).


## Tests

The unit tests parse sample API responses from `tests/data` and need no API key or network access. They are built automatically as the `test_meteosource` target and run with:

```bash
ctest --test-dir build --output-on-failure
```


## Contact us

You can contact us [here](https://www.meteosource.com/contact).
