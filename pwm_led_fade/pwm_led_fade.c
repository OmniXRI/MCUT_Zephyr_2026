/*
利用 PWM 控制 YD-RP2040 板上 LED 產生呼吸燈（漸暗漸亮）效果
PWM 切片為控制亮暗週期
*/

#include "pico/stdlib.h"
#include <stdio.h>
#include "pico/time.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"

#define LED_PIN 25 // 指定 YD-RP2040 板上 LED 接腳編號

// 當 PWM 時間切片到時對應函數，改變 LED 亮度
void on_pwm_wrap() {
    static int fade = 0; // 亮度初始值
    static bool going_up = true; // 設定遞增為真

    pwm_clear_irq(pwm_gpio_to_slice_num(LED_PIN)); // 清除 PWM 時間中斷

    if (going_up) { // 若遞增
        ++fade; // 亮度加 1
        if (fade > 255) { // 若亮度超過 255
            fade = 255; // 令亮度為 255
            going_up = false; // 改變遞增方向為負（遞減）
        }
    } else { // 若遞減
        --fade; // 亮度減 1
        if (fade < 0) { // 若亮度小於 0
            fade = 0; // 令亮度為 0
            going_up = true; // 改變遞增方向為正（遞增）
        }
    }

    // 設定 PWM 數值（亮度）為 fade * fade
    pwm_set_gpio_level(LED_PIN, fade * fade); 
}

int main() {
    // 宣告 LED 腳位為 PWM 用途
    gpio_set_function(LED_PIN, GPIO_FUNC_PWM);
    
    // 確認 LED 腳位連接到的哪一個切片
    uint slice_num = pwm_gpio_to_slice_num(LED_PIN);

    pwm_clear_irq(slice_num); // 清除 PWM 切片中斷
    pwm_set_irq_enabled(slice_num, true); // 設置 PWM 切片中斷致能
    irq_set_exclusive_handler(PWM_DEFAULT_IRQ_NUM(), on_pwm_wrap); // 指定切片中斷函式
    irq_set_enabled(PWM_DEFAULT_IRQ_NUM(), true); // 啟動 PWM 切片中斷

    // 取得切片合理預設值，預設計數器可在範圍 (0 to 2^16) 循環計數。
    pwm_config config = pwm_get_default_config();
    
    // 設定除頻器，將計數時脈降至系統時脈的 1/此值。
    // PWM 輸出頻率為 125MHZ/65535 = 1907.35 Hz
    // 若除4則為 476.8 Hz，除數越低則頻率越高，閃爍越快。
    // 從最暗到最亮共256階，即 256 / 476.8 = 536.9 ms
    // 即從最暗到最亮再到最暗約 536.9 x 2 = 1.074 秒
    pwm_config_set_clkdiv(&config, 4.f);
    
    // PWM 初始化
    pwm_init(slice_num, &config, true);

    // 後續操作都會在 PWM 中斷程式中處理，就不需人為介入了
    while (1)
        tight_loop_contents();
}
