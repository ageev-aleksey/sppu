#include <curl/curl.h>
#include <iostream>
#include <QJsonDocument>
#include <QJsonObject>
#include <sstream>
#include "http_client.h"

static double data = 0;
size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    QByteArray j(ptr);
    auto doc = QJsonDocument::fromJson(j);
    auto object = doc.object();
    data = object["duty_cycle"].toDouble();
}

double curl_send_post_http(double error_control) {

    CURL *curl;
    CURLcode  res;
    struct curl_slist *list;
    curl_global_init(CURL_GLOBAL_ALL);

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8000/");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        list = curl_slist_append(list, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        std::stringstream msg;
        msg << "{\"error\": "  << error_control << "}";
        std::string postData = msg.str();
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());

        res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            return data;
        } else {
            return 0;
        }
    }

    curl_easy_cleanup(curl);
    curl_global_cleanup();

}
