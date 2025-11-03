/* UART RX Callback Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_log.h"

/**
 * This example demonstrates how to use the UART RX data callback feature.
 * Instead of polling or waiting on an event queue, a callback function is
 * registered that gets called from the ISR when data is received.
 *
 * - Port: UART_NUM_0
 * - Receive (Rx) buffer: on
 * - Transmit (Tx) buffer: off
 * - Flow control: off
 * - Event queue: off (not needed with callback)
 * - RX callback: on
 */

#define UART_PORT_NUM      UART_NUM_0
#define UART_BAUD_RATE     115200
#define UART_TXD           (GPIO_NUM_1)
#define UART_RXD           (GPIO_NUM_3)
#define BUF_SIZE           (1024)

static const char *TAG = "uart_rx_callback";

// Semaphore to signal that data is available
static SemaphoreHandle_t rx_data_semaphore;

// Context structure to pass to callback
typedef struct {
    uart_port_t uart_num;
    volatile size_t bytes_available;
} uart_callback_context_t;

static uart_callback_context_t callback_ctx;

/**
 * @brief UART RX data callback function
 *
 * This function is called from ISR context when UART receives data.
 * It should be kept short and should not block.
 */
static void IRAM_ATTR uart_rx_data_callback(uart_port_t uart_num, size_t size, void *user_data)
{
    uart_callback_context_t *ctx = (uart_callback_context_t *)user_data;
    
    // Update bytes available (this is called from ISR, so keep it simple)
    ctx->bytes_available = size;
    
    // Signal the task that data is available
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(rx_data_semaphore, &xHigherPriorityTaskWoken);
    
    if (xHigherPriorityTaskWoken) {
        portYIELD_FROM_ISR();
    }
}

static void uart_rx_task(void *arg)
{
    uint8_t *data = (uint8_t *) malloc(BUF_SIZE);
    if (data == NULL) {
        ESP_LOGE(TAG, "Failed to allocate memory for RX buffer");
        vTaskDelete(NULL);
        return;
    }
    
    ESP_LOGI(TAG, "UART RX task started. Waiting for data...");
    
    while (1) {
        // Wait for callback to signal that data is available
        if (xSemaphoreTake(rx_data_semaphore, portMAX_DELAY)) {
            // Read data from the UART
            int len = uart_read_bytes(callback_ctx.uart_num, data, BUF_SIZE - 1, 100 / portTICK_PERIOD_MS);
            
            if (len > 0) {
                data[len] = '\0';
                ESP_LOGI(TAG, "Received %d bytes: %s", len, (char *) data);
                
                // Echo the data back
                uart_write_bytes(callback_ctx.uart_num, (const char *) data, len);
            }
        }
    }
}

void app_main(void)
{
    /* Configure parameters of an UART driver */
    uart_config_t uart_config = {
        .baud_rate = UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    
    int intr_alloc_flags = 0;
#if CONFIG_UART_ISR_IN_IRAM
    intr_alloc_flags = ESP_INTR_FLAG_IRAM;
#endif

    ESP_LOGI(TAG, "Initializing UART%d...", UART_PORT_NUM);
    
    // Install UART driver (no event queue needed when using callbacks)
    ESP_ERROR_CHECK(uart_driver_install(UART_PORT_NUM, BUF_SIZE * 2, 0, 0, NULL, intr_alloc_flags));
    ESP_ERROR_CHECK(uart_param_config(UART_PORT_NUM, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_PORT_NUM, UART_TXD, UART_RXD, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    
    // Create semaphore for signaling data availability
    rx_data_semaphore = xSemaphoreCreateBinary();
    if (rx_data_semaphore == NULL) {
        ESP_LOGE(TAG, "Failed to create semaphore");
        return;
    }
    
    // Initialize callback context
    callback_ctx.uart_num = UART_PORT_NUM;
    callback_ctx.bytes_available = 0;
    
    // Register the RX data callback
    ESP_ERROR_CHECK(uart_set_rx_data_callback(UART_PORT_NUM, uart_rx_data_callback, &callback_ctx));
    
    ESP_LOGI(TAG, "UART RX callback registered successfully");
    ESP_LOGI(TAG, "Type something in the terminal and press Enter...");
    
    // Create task to handle received data
    xTaskCreate(uart_rx_task, "uart_rx_task", 2048, NULL, 12, NULL);
}
