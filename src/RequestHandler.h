#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <string>
#include <curl/curl.h>
#include <jsoncpp/json/json.h>


class RequestHandler
{
    public:
        RequestHandler(const std::string api_key);
        ~RequestHandler();
        Json::Value execute_request(const std::string url);

    private:
        std::string m_api_key;
        CURL * m_handle;
};

#endif //REQUEST_HANDLER_H
