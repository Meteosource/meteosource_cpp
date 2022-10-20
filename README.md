meteosource_cpp - Weather API library
==========

C++ wrapper library for [Meteosource weather API](https://www.meteosource.com) that provides detailed hyperlocal weather forecasts for any location on earth.


## Dependencies
The `meteosource_cpp` library needs following software installed:

  - [`libcurl`](https://curl.se/libcurl/c/)
  - [`jsoncpp`](https://github.com/open-source-parsers/jsoncpp)

On Debian platforms, you can install these libraries using:

```bash
sudo apt-get update
sudo apt-get install libcurl4-gnutls-dev
sudo apt-get install libjsoncpp-dev
```

## Get started

To use this library, you need to obtain your Meteosource API key. You can [sign up](https://www.meteosource.com/client/sign-up) or get the API key of existing account in [your dashboard](https://www.meteosource.com/client).


## Library usage

To use `meteosource_cpp`, you only need to copy all `*.cpp` and `*.h` files from `src` folder into your project. Then you can `#include "Meteosource.h"` and fetch the weather data you need.

Example usage is shown in `example.cpp`. You can compile it by simply running `make`, and run it by `./example`. Be sure to change variables `api_key` and `tier` in `example.cpp` to your actual API key and tier before compiling.


### Usage notes

The library uses parameter values and variable names with the same convention as the API itself. One exception are nested variables, such as `wind.speed` in the API. This library uses `_` to separate the levels, so `wind.speed` becomes `wind_speed`. You can see the available variable names for the individual sections in `src/Data.h` file.

The library uses empty string as default value for `std::string` variables, `NAN` for `double` variables and `-9999` for `int` variable (only `icon` variable).


## Contact us

You can contact us [here](https://www.meteosource.com/contact).
