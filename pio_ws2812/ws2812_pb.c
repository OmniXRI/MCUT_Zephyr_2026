/*
當 YD-RP2040 板上按鍵按下後，WS2812 RGB LED 依序切換紅、綠、藍色，並停留 0.1 秒 (100ms)
程式編譯完後，按下板上 Boot + Reset，放開 Reset 出現虛擬磁碟，
將 \build 下 pio_ws2812_pb.uf2 拖入其中，即可重新啟動執行。
*/

#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

#define IS_RGBW false // 定義非 RGBW LED
#define WS2812_PIN 23 // 定義 YD-RP2040 WS2812 Data In 腳位為 GPIO23
#define BUTTON_PIN 24 // 指定 YD-RP2040 板上按鍵接腳編號

// 將單個像素內容(GRBW 32bit)透過 PIO 程式輸出至 WS2812 
static inline void put_pixel(PIO pio, uint sm, uint32_t pixel_grb) {
    pio_sm_put_blocking(pio, sm, pixel_grb << 8u);
}

// 將 RGB 資料重組成 GRBW 32bit 格式
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return
            ((uint32_t) (r) << 8) |
            ((uint32_t) (g) << 16) |
            (uint32_t) (b);
}

// 主程式
int main() {
    stdio_init_all(); // 初始化所有 I/O
    printf("WS2812 Test, using pin %d\n", WS2812_PIN); // 列印字串到虛擬串列埠

    gpio_init(BUTTON_PIN); // 初始化按鍵接腳
    gpio_set_dir(BUTTON_PIN, GPIO_IN); // 指定按鍵接腳為輸入模式
    gpio_pull_up(BUTTON_PIN); // 指定按鍵接腳要使用上拉電阻

    PIO pio; // 可程式輸出入腳位
    uint sm; // 有限狀態機
    uint offset; // 偏置量

    // 取得閒置可程式輸出入腳位(Programable I/O, PIO)及有限狀態機(State Machine, SM)
    bool success = pio_claim_free_sm_and_add_program_for_gpio_range(&ws2812_program, &pio, &sm, &offset, WS2812_PIN, 1, true);
    hard_assert(success);

    // 初始化 WS2812 接腳，時脈 800KHz 
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);

    int count = 0; // 使用者計數器
    
    while (1) {
        if(!gpio_get(BUTTON_PIN)){ // 讀取按鍵狀態，按下時為 LOW
            count++;
        
            // 取消註解可解決按鍵一直按著不放 LED 變成紅綠藍一直閃爍問題
            // while(!gpio_get(BUTTON_PIN)){ // 讀取按鍵狀態，等待按鍵放掉(HIGH)
            //     sleep_ms(10); // 暫停 0.01 秒（10 ms）
            // }
        }

        switch(count %3){
            case 1: 
                put_pixel(pio, sm, urgb_u32(0xff, 0, 0)); // 令 WS2812 顯示紅色
                sleep_ms(100); // 暫停 0.1 秒（100 ms）
                break;
            case 2:
                put_pixel(pio, sm, urgb_u32(0, 0xff, 0)); // 令 WS2812 顯示紅色
                sleep_ms(100); // 暫停 0.1 秒（100 ms）
                break;
            case 0:
            default:
                put_pixel(pio, sm, urgb_u32(0, 0, 0xff)); // 令 WS2812 顯示紅色
                sleep_ms(100); // 暫停 0.1 秒（100 ms）
        }
    } 

    // 釋放 PIO 及 SM 資源
    pio_remove_program_and_unclaim_sm(&ws2812_program, pio, sm, offset);
}
