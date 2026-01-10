#include "SafeMode.h"
#include <WiFi.h>
#include <SPIFFS.h>
#include <SD.h>
#include "Theme.h"

namespace Vanguard {

void SafeMode::run() {
    auto& lcd = M5Cardputer.Display;
    
    lcd.fillScreen(TFT_RED);
    lcd.setTextColor(TFT_WHITE, TFT_RED);
    lcd.setTextSize(2);
    lcd.setCursor(10, 10);
    lcd.println("!!! SAFE MODE !!!");
    
    lcd.setTextSize(1);
    lcd.setCursor(10, 40);
    lcd.println("System halted to prevent boot loop.");
    lcd.println("");
    lcd.println("Options:");
    lcd.println("  [G0] Reboot");
    lcd.println("  [BS] Wipe WiFi Config");
    
    // Minimal hardware validation
    lcd.println("");
    lcd.print("WiFi: ");
    lcd.println("OFF");
    
    while (true) {
        M5Cardputer.update();
        
        if (M5Cardputer.BtnA.wasPressed()) {
            ESP.restart();
        }
        
        // Backspace to wipe wifi (using M5Cardputer keyboard check if possible, or just BtnA for now)
        // Cardputer BtnA is 'Ok' / G0. 
        // We'll stick to a simple G0 reboot for now.
        
        delay(100);
    }
}

} // namespace Vanguard
