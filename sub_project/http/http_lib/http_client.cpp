#include <curl/curl.h>
#include <iostream>
#include <QJsonDocument>
#include <QJsonObject>

char buffer[100];

size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    QByteArray j(ptr);
    auto doc = QJsonDocument::fromJson(j);
    auto object = doc.object();
    double value = object["duty_cycle"].toDouble();
    std::cout << value;
}

int main() {
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
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{\"error\": -0.5}");

        res = curl_easy_perform(curl);
    }

    curl_easy_cleanup(curl);
    curl_global_cleanup();
}
