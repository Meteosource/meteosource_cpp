#include <iostream>
#include <string>
#include <sstream>
#include <iostream>
#include <curl/curl.h>
#include <jsoncpp/json/json.h>
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>

#include "RequestHandler.h"


namespace
{
    std::size_t callback(
            const char* in,
            std::size_t size,
            std::size_t num,
            std::string* out)
    {
        const std::size_t totalBytes(size * num);
        out->append(in, totalBytes);
        return totalBytes;
    }
}


RequestHandler::RequestHandler(const std::string api_key) : m_api_key(api_key)
{
    this->m_handle = curl_easy_init();
    curl_easy_setopt(this->m_handle, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
    curl_easy_setopt(this->m_handle, CURLOPT_TIMEOUT, 10);
    curl_easy_setopt(this->m_handle, CURLOPT_FOLLOWLOCATION, 1L);
}


RequestHandler::~RequestHandler()
{
    curl_easy_cleanup(this->m_handle);
    this->m_handle = NULL;
}


Json::Value RequestHandler::execute_request(std::string url)
{
    curl_easy_setopt(this->m_handle, CURLOPT_URL, url.c_str());
    std::unique_ptr<std::string> httpData(new std::string());
    curl_easy_setopt(this->m_handle, CURLOPT_WRITEFUNCTION, callback);
    curl_easy_setopt(this->m_handle, CURLOPT_WRITEDATA, httpData.get());
    curl_easy_perform(this->m_handle);

    long http_code = 0;
    curl_easy_getinfo (this->m_handle, CURLINFO_RESPONSE_CODE, &http_code);

    Json::Value jsonData;
    Json::Reader jsonReader;

    if (http_code == 200 && jsonReader.parse(*httpData.get(), jsonData))
    {
        return jsonData;
    }
    else
    {
        std::cout << "Invalid request: " << url << std::endl;
        std::cout << "HTTP data was:\n" << *httpData.get() << std::endl;
        return Json::nullValue;
    }
}
