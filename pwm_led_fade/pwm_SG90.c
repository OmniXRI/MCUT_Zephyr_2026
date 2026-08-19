/*
利用 PWM 控制舵機（伺服馬達）SG90 在 0, 90, 180 度間來回移動
*/

#include "pico/stdlib.h"
#include <stdio.h>
#include "pico/time.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"

#define SERVO_PIN 28 // 指定 SG90 PWM 接腳編號

// 將角度（0 ~ 180）轉換為 PWM 的計數值（Level）
uint angle_to_level(float angle) {
    // 0 度 = 0.5ms -> (0.5ms / 20ms) * 39062 = 976.5
    // 180 度 = 2.5ms -> (2.5ms / 20ms) * 39062 = 4882.75
    float min_level = 976.0f;
    float max_level = 4883.0f;
    
    // 限制角度範圍在 0 到 180 之間
    if (angle < 0.0f) angle = 0.0f;
    if (angle > 180.0f) angle = 180.0f;
    
    return (uint)(min_level + (angle / 180.0f) * (max_level - min_level));
}

int main() {
    stdio_init_all();

    // 1. 設定 GPIO 功能為 PWM
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);

    // 2. 取得該 GPIO 對應的 PWM 切片 (Slice)
    uint slice_num = pwm_gpio_to_slice_num(SERVO_PIN);

    // 3. 設定時脈分頻比（64.0），讓計數速度變慢
    pwm_set_clkdiv(slice_num, 64.0f);

    // 4. 設定計數最大值（Wrap），達到此值後歸零並產生 50Hz 週期
    pwm_set_wrap(slice_num, 39062);

    // 5. 啟動 PWM 輸出
    pwm_set_enabled(slice_num, true);

    // 主迴圈：讓馬達在 0度 -> 90度 -> 180度 之間循環轉動
    while (true) {
        printf("轉動到 0 度\n");
        pwm_set_gpio_level(SERVO_PIN, angle_to_level(0));
        sleep_ms(1000);

        printf("轉動到 90 度\n");
        pwm_set_gpio_level(SERVO_PIN, angle_to_level(90));
        sleep_ms(1000);

        printf("轉動到 180 度\n");
        pwm_set_gpio_level(SERVO_PIN, angle_to_level(180));
        sleep_ms(1000);
    }
}    
