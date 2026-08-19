# WS2812 範例程式

請使用 VSCode，安裝完 Pi Pico Extension 後，以 Open Folder 開啟即可進行修改、編譯、上傳。

- ws2812.c VSCode Pi Pico Extension 原始範例  
- ws2812_parallel.c Pi Pico Extension 原始範例  
- ws2812.pio Pi Pico Extension PIO 原始範例  
- ws2812_sample.c 紅綠藍三色 LED 輪閃範例  
- ws2812_pb.c 按鍵切換紅綠藍三色 LED 範例

ws2812_sample.c 及 ws2812_pb.c 編譯完成後會產生對應 uf2 執行檔。  
按下板上 Boot 不放，再按下 Reset，放開 Reset後會自動開啟虛擬磁碟機，將 *.uf2 拖進後會自動重啟開發板。  
