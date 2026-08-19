# PWM 控制範例

請使用 VSCode，安裝完 Pi Pico Extension 後，以 Open Folder 開啟即可進行修改、編譯、上傳。

* pwm_led_fade_original.c VSCode Pi Pico Extension 原始範例  
* pwm_led_fade.c 呼吸燈（開發板上LED漸暗漸亮）範例  
* pwm_SG90.c 利用 PWM 控制舵機(Servo) 切換角度範例  

CMakeLists.txt 已加入 pwm_led_fade 及 pwm_SG90 相關編譯設定。  
pwm_led_fade.c 及 pwm_SG90.c 編譯完成後會產生對應 uf2 執行檔。  
按下板上 Boot 不放，再按下 Reset，放開 Reset後會自動開啟虛擬磁碟機，將 *.uf2 拖進後會自動重啟開發板。  

![YD-RP2040_SG90](https://github.com/OmniXRI/MCUT_Zephyr_2026/blob/main/pwm_led_fade/YD-RP2040_SG90.jpg?raw=true)
