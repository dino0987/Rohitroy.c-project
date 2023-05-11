#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>

// Struct to hold the weather information
typedef struct {
    double temperature;
    int humidity;
    double windSpeed;
} WeatherData;

// Callback function to handle the API response
size_t response_callback(void *contents, size_t size, size_t nmemb, char *output) {
    strcpy(output, (char *)contents);  // Copy the response to output variable
    return size * nmemb;
}

// Function to retrieve weather data from the API
int retrieve_weather_data(const char *api_key, const char *city, WeatherData *weather) {
    CURL *curl;
    CURLcode res;
    char url[100];
    char response[10000];

    sprintf(url, "http://api.openweathermap.org/data/2.5/weather?q=%s&appid=%s", city, api_key);

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, response_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            printf("Failed to retrieve weather data: %s\n", curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            return -1;
        }

        curl_easy_cleanup(curl);
    } else {
        printf("Failed to initialize CURL.\n");
        return -1;
    }

    // Parse the JSON response and extract the relevant information
    // In this example, we assume the response is in the format: {"main": {"temp": 22.5, "humidity": 70}, "wind": {"speed": 5.5}}
    // You might need to modify the parsing logic based on the API response format

    // Extract temperature
    char *temperature_str = strstr(response, "\"temp\":");
    sscanf(temperature_str, "\"temp\":%lf", &(weather->temperature));

    // Extract humidity
    char *humidity_str = strstr(response, "\"humidity\":");
    sscanf(humidity_str, "\"humidity\":%d", &(weather->humidity));

    // Extract wind speed
    char *wind_speed_str = strstr(response, "\"speed\":");
    sscanf(wind_speed_str, "\"speed\":%lf", &(weather->windSpeed));

    return 0;
}

int main() {
    const char *api_key = "YOUR_API_KEY";  // Replace with your OpenWeatherMap API key
    const char *city = "London";  // Replace with the desired city

    WeatherData weather;

    if (retrieve_weather_data(api_key, city, &weather) == 0) {
        printf("Weather in %s:\n", city);
        printf("Temperature: %.1f°C\n", weather.temperature - 273.15);
        printf("Humidity: %d%%\n", weather.humidity);
        printf("Wind Speed: %.1f m/s\n", weather.windSpeed);
    }

    return 0;
}
