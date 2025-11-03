# UART RX Callback Example

This example demonstrates how to use the UART RX data callback feature to receive data notifications without polling or using an event queue.

## How to use example

### Hardware Required

This example can be run on any ESP32 development board. Connect the UART pins as follows:

- UART TX (GPIO 1) - Can be connected to a USB-to-serial adapter
- UART RX (GPIO 3) - Can be connected to a USB-to-serial adapter

For testing, you can simply use the default UART0 which is typically connected to the USB-serial chip on most development boards.

### Configure the project

This example uses default configuration and doesn't require any special menuconfig settings.

### Build and Flash

Build the project and flash it to the board, then run monitor tool to view serial output:

```
idf.py -p PORT flash monitor
```

(Replace PORT with the name of the serial port to use.)

(To exit the serial monitor, type ``Ctrl-]``.)

See the Getting Started Guide for full steps to configure and use ESP-IDF to build projects.

## Example Output

Type some text in the terminal and press Enter. The example will echo back what you typed:

```
I (xxx) uart_rx_callback: Initializing UART0...
I (xxx) uart_rx_callback: UART RX callback registered successfully
I (xxx) uart_rx_callback: Type something in the terminal and press Enter...
I (xxx) uart_rx_callback: UART RX task started. Waiting for data...
I (xxx) uart_rx_callback: Received 11 bytes: Hello World
```

## How it works

Instead of polling with `uart_read_bytes()` with a timeout or waiting on an event queue:

1. The example registers a callback function using `uart_set_rx_data_callback()`
2. When UART data is received, the callback is invoked from ISR context
3. The callback uses a semaphore to signal a task that data is available
4. The task reads and processes the data

## Key Benefits

- **No polling**: The callback is called immediately when data arrives
- **Lower latency**: Data can be processed as soon as it's received
- **Simpler code**: No need to manage event queues for simple use cases
- **Efficient**: The task can sleep until data arrives

## Important Notes

- The callback runs in ISR context, so it must be kept short
- Do not call blocking UART functions from the callback
- Use FreeRTOS synchronization primitives (semaphores, queues) to signal tasks
- The callback is optional and can coexist with event queues
