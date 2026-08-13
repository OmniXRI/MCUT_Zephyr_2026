#include <stdio.h>
#include "pico/stdlib.h"

#define BUTTON_PIN 24 // 指定 YD-RP2040 板上按鍵接腳編號
#define LED_PIN 25 // 指定 YD-RP2040 板上 LED 接腳編號

int main()
{
    stdio_init_all(); // 初始化所有標準化輸出入

    gpio_init(LED_PIN); // 初始化 LED 接腳
    gpio_set_dir(LED_PIN, GPIO_OUT); // 指定 LED 接腳為輸出模式

    gpio_init(BUTTON_PIN); // 初始化按鍵接腳
    gpio_set_dir(BUTTON_PIN, GPIO_IN); // 指定按鍵接腳為輸入模式
    gpio_pull_up(BUTTON_PIN); // 指定按鍵接腳要使用上拉電阻

    while (true) {
        bool button_pressed = !gpio_get(BUTTON_PIN); // 讀取按鍵狀態，按下時為 LOW

        if(button_pressed){ // 若按鍵被按下
            gpio_put(LED_PIN, 1); // 令 LED 點亮
        }
        else{
            gpio_put(LED_PIN, 0); // 令 LED 熄滅
        }
        
        sleep_ms(10);
    }
}
