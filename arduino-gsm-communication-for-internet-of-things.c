#include "freertos/FreeRTOS.h" // FreeRTOS core
#include "freertos/task.h" // Task APIs
#include "esp_log.h" // Logging
#include "esp_http_client.h" // HTTP client

static const char *TAG = "GSM_IOT"; // Log tag

static void http_task(void *arg) { // Task for HTTP request
  esp_http_client_config_t config = { .url = "http://example.com/iot" }; // Endpoint URL placeholder
  esp_http_client_handle_t client = esp_http_client_init(&config); // Create client
  while (true) { // Loop
    if (esp_http_client_perform(client) == ESP_OK) { // Perform GET
      ESP_LOGI(TAG, "Status = %d", esp_http_client_get_status_code(client)); // Log status
    } else { // Error
      ESP_LOGW(TAG, "HTTP request failed"); // Warn
    } // End if
    vTaskDelay(pdMS_TO_TICKS(5000)); // 5s interval
  } // End loop
} // End task

void app_main(void) { // Entry point
  ESP_LOGI(TAG, "Starting IoT HTTP client"); // Log
  xTaskCreate(http_task, "http", 4096, NULL, 5, NULL); // Create task
} // End app_main

