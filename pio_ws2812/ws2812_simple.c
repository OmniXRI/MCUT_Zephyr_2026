/*
WS2812 RGB LED 依序閃爍間隔 0.5 秒 (500ms)
程式編譯完後，按下板上 Boot + Reset，放開 Reset 出現虛擬磁碟，
將 \build 下 pio_ws2812_simple.uf2 拖入其中，即可重新啟動執行。
*/

#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

#define IS_RGBW false // 定義非 RGBW LED
#define WS2812_PIN 23 // 定義 YD-RP2040 WS2812 Data In 腳位為 GPIO23

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

    PIO pio; // 可程式輸出入腳位
    uint sm; // 有限狀態機
    uint offset; // 偏置量

    // 取得閒置可程式輸出入腳位(Programable I/O, PIO)及有限狀態機(State Machine, SM)
    bool success = pio_claim_free_sm_and_add_program_for_gpio_range(&ws2812_program, &pio, &sm, &offset, WS2812_PIN, 1, true);
    hard_assert(success);

    // 初始化 WS2812 接腳，時脈 800KHz 
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);

    while (1) {
        put_pixel(pio, sm, urgb_u32(0xff, 0, 0)); // 令 WS2812 顯示紅色
        sleep_ms(500); // 暫停 0.5 秒（500 ms）

        put_pixel(pio, sm, urgb_u32(0, 0xff, 0)); // 令 WS2812 顯示紅色
        sleep_ms(500); // 暫停 0.5 秒（500 ms）

        put_pixel(pio, sm, urgb_u32(0, 0, 0xff)); // 令 WS2812 顯示紅色
        sleep_ms(500); // 暫停 0.5 秒（500 ms）
    } 

    // 釋放 PIO 及 SM 資源
    pio_remove_program_and_unclaim_sm(&ws2812_program, pio, sm, offset);
}
