include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "cJSON.h"

#define API_KEY "your_api_key"
#define CITY_ID "your_city_id"

// callback function for libcurl to handle the HTTP response
size_t curl_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    cJSON *json = cJSON_Parse(ptr);
    cJSON *weather = cJSON_GetObjectItem(json, "weather");
    cJSON *main = cJSON_GetObjectItem(json, "main");
    cJSON *wind = cJSON_GetObjectItem(json, "wind");
    cJSON *name = cJSON_GetObjectItem(json, "name");

    if (weather && main && wind && name) {
        cJSON *desc = cJSON_GetObjectItem(cJSON_GetArrayItem(weather, 0), "description");
        cJSON *temp = cJSON_GetObjectItem(main, "temp");
        cJSON *humidity = cJSON_GetObjectItem(main, "humidity");
        cJSON *speed = cJSON_GetObjectItem(wind, "speed");

        printf("City: %s\n", name->valuestring);
        printf("Description: %s\n", desc->valuestring);
        printf("Temperature: %.1f C\n", temp->valuedouble - 273.15);
        printf("Humidity: %d%%\n", humidity->valueint);
        printf("Wind Speed: %.1f m/s\n", speed->valuedouble);
    } else {
        printf("Error parsing JSON response.\n");
    }

    cJSON_Delete(json);
    return size * nmemb;
}

int main() {
    char url[128];
    CURL *curl = curl_easy_init();

    // build the URL for the API request
    sprintf(url, "http://api.openweathermap.org/data/2.5/weather?id=%s&appid=%s", CITY_ID, API_KEY);

    if (curl) {
        // set the URL for the request
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // set the callback function for the response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_callback);

        // perform the request
        CURLcode res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        // clean up
        curl_easy_cleanup(curl);
    } else {
        fprintf(stderr, "Error initializing libcurl.\n");
    }

    return 0;
}


2	Output
Enter City : Kolkata

Description: scattered clouds
Temperature: 13.6 C
Humidity: 72%
Wind Speed: 2.06 m/s



