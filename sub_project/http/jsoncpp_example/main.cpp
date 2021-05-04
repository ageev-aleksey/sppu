#include <iostream>
#include <json/json.h>


int main() {
    Json::Value root;
    root["hello"] = 1;
    root["world"] = "!!";
    std::cout << root << std::endl;
   return 0;
}