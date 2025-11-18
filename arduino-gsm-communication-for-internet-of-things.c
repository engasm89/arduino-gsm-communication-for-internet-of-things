/*
 * Course: Arduino Gsm Communication For Internet Of Things
 * Platform: Arduino
 * Author: Ashraf S A AlMadhoun
 * 
 * Description:
 * This code demonstrates the core concepts taught in the "Arduino Gsm Communication For Internet Of Things" course.
 * It provides a practical implementation that students can study, modify, and
 * use as a foundation for their own projects.
 * 
 * Learning Objectives:
 * - Understand the fundamental principles covered in this course
 * - Practice implementing the concepts with real code
 * - Build a working example that can be extended
 * 
 * Hardware Requirements:
 * - Development board (Arduino)
 * - Components as specified in CIRCUIT.md
 * - USB cable for programming
 * 
 * Pin Connections:
 * Refer to CIRCUIT.md for detailed wiring diagrams and pin assignments.
 * 
 * Usage:
 * 1. Review the code structure and comments
 * 2. Connect hardware according to CIRCUIT.md
 * 3. Upload code to your development board
 * 4. Monitor serial output for debugging
 * 
 * Course Link: https://www.udemy.com/course/arduino-gsm-communication-for-internet-of-things/
 * Repository: https://github.com/engasm89/arduino-gsm-communication-for-internet-of-things
 * 
 * Copyright (c) 2025 Ashraf S A AlMadhoun
 * Licensed under MIT License
 */

#include "freertos/FreeRTOS.h" // FreeRTOS core
#include "freertos/task.h" // Task APIs
#include "esp_log.h" // Logging
#include "esp_http_client.h" // HTTP client

static const char *TAG = "GSM_IOT"; // Log tag
static int backoff_ms = 2000; // Initial retry backoff

static void http_task(void *arg) { // Task for HTTP request
  esp_http_client_config_t config = { .url = "http://example.com/iot" }; // Endpoint URL placeholder
  esp_http_client_handle_t client = esp_http_client_init(&config); // Create client
  int sample = 0; // Sample counter
  while (true) { // Loop
    char payload[128]; // JSON payload buffer
    sample++; // Increment sample
    int temp = 25 + (sample % 5); // Simulated temp
    int hum = 50 + (sample % 10); // Simulated humidity
    snprintf(payload, sizeof(payload), "{\"sample\":%d,\"temp\":%d,\"hum\":%d}", sample, temp, hum); // Build JSON
    esp_http_client_set_url(client, "http://example.com/iot"); // Set URL
    esp_http_client_set_method(client, HTTP_METHOD_POST); // Use POST
    esp_http_client_set_header(client, "Content-Type", "application/json"); // JSON header
    esp_http_client_set_post_field(client, payload, strlen(payload)); // Set body
    if (esp_http_client_perform(client) == ESP_OK) { // Perform request
      int status = esp_http_client_get_status_code(client); // Get status
      ESP_LOGI(TAG, "POST sample=%d status=%d", sample, status); // Log success
      backoff_ms = 2000; // Reset backoff on success
      vTaskDelay(pdMS_TO_TICKS(5000)); // Normal interval
    } else { // Error
      ESP_LOGW(TAG, "HTTP POST failed, backoff=%dms", backoff_ms); // Warn with backoff
      vTaskDelay(pdMS_TO_TICKS(backoff_ms)); // Wait before retry
      backoff_ms = backoff_ms < 60000 ? backoff_ms * 2 : 60000; // Exponential backoff up to 60s
    } // End if
  } // End loop
} // End task

void app_main(void) { // Program entry point point
  ESP_LOGI(TAG, "Starting IoT HTTP client"); // Log
  xTaskCreate(http_task, "http", 4096, NULL, 5, NULL); // Create task
} // End of main function

