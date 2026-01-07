/**
 * @file main.cpp
 * @brief The Assessor - Target-First Auditing Tool
 *
 * A philosophical fork of Bruce that inverts the UX paradigm:
 * Instead of "pick attack, then target", we do "see targets, pick one, see options".
 *
 * @author The Assessor Contributors
 * @license GPL-3.0
 */

#include <M5Unified.h>
#include "core/AssessorEngine.h"
#include "ui/BootSequence.h"
#include "ui/TargetRadar.h"
#include "ui/TargetDetail.h"
#include "ui/Theme.h"

using namespace Assessor;

// =============================================================================
// GLOBALS
// =============================================================================

static AssessorEngine* g_engine = nullptr;
static BootSequence*   g_boot   = nullptr;
static TargetRadar*    g_radar  = nullptr;
static TargetDetail*   g_detail = nullptr;

enum class AppState {
    BOOTING,
    SCANNING,
    RADAR,
    TARGET_DETAIL,
    ATTACKING,
    ERROR
};

static AppState g_state = AppState::BOOTING;

// =============================================================================
// SETUP
// =============================================================================

void setup() {
    // Feed watchdog early
    yield();

    // Initialize M5Stack hardware with Cardputer-safe config
    auto cfg = M5.config();
    cfg.serial_baudrate = 115200;
    cfg.clear_display = true;
    cfg.internal_imu = false;
    cfg.internal_rtc = false;   // Cardputer doesn't have RTC
    cfg.internal_spk = false;
    cfg.internal_mic = false;
    cfg.external_imu = false;
    cfg.external_rtc = false;
    cfg.led_brightness = 0;
    M5.begin(cfg);

    // Feed watchdog after M5.begin
    yield();

    // Apply theme
    M5.Display.setRotation(1);  // Landscape
    M5.Display.fillScreen(Theme::COLOR_BACKGROUND);
    M5.Display.setTextColor(Theme::COLOR_TEXT_PRIMARY);
    M5.Display.setFont(&fonts::Font0);

    // Show immediate feedback (before heavy init)
    M5.Display.setCursor(10, 60);
    M5.Display.print("Initializing...");

    yield();  // Feed watchdog

    // Initialize components (heavy operations)
    g_engine = &AssessorEngine::getInstance();

    yield();  // Feed watchdog

    g_boot = new BootSequence();
    g_radar = new TargetRadar(*g_engine);

    // Start boot sequence
    g_boot->begin();
    g_state = AppState::BOOTING;

    // Safe serial print (only if CDC is connected)
    if (Serial) {
        Serial.println(F("[Assessor] Boot sequence started"));
    }
}

// =============================================================================
// LOOP
// =============================================================================

void loop() {
    M5.update();  // Read buttons/touch

    switch (g_state) {
        case AppState::BOOTING:
            g_boot->tick();
            if (g_boot->isComplete()) {
                g_engine->beginScan();
                g_state = AppState::SCANNING;
            }
            break;

        case AppState::SCANNING:
            g_engine->tick();
            g_radar->renderScanning();
            if (g_engine->getScanState() == ScanState::COMPLETE) {
                g_state = AppState::RADAR;
            }
            break;

        case AppState::RADAR:
            g_engine->tick();
            g_radar->tick();
            g_radar->render();

            // Handle target selection
            if (g_radar->hasSelection()) {
                const Target* selected = g_radar->getSelectedTarget();
                if (selected) {
                    // Clean up any previous detail view
                    if (g_detail) {
                        delete g_detail;
                    }
                    g_detail = new TargetDetail(*g_engine, *selected);
                    g_radar->clearSelection();
                    g_state = AppState::TARGET_DETAIL;
                }
            }
            break;

        case AppState::TARGET_DETAIL:
            g_engine->tick();
            if (g_detail) {
                g_detail->tick();
                g_detail->render();

                // Check if user wants to go back
                if (g_detail->wantsBack()) {
                    delete g_detail;
                    g_detail = nullptr;
                    g_state = AppState::RADAR;
                }
                // Check if action was confirmed
                else if (g_detail->actionConfirmed()) {
                    ActionType action = g_detail->getConfirmedAction();
                    g_detail->clearActionConfirmation();
                    g_engine->executeAction(action, g_detail->getTarget());
                    g_state = AppState::ATTACKING;
                }
            }
            break;

        case AppState::ATTACKING:
            g_engine->tick();
            if (g_detail) {
                g_detail->tick();
                g_detail->render();

                // Check if attack finished
                if (!g_engine->isActionActive()) {
                    g_state = AppState::TARGET_DETAIL;
                }
                // Check if user cancelled
                if (g_detail->wantsBack()) {
                    g_engine->stopAction();
                    g_state = AppState::TARGET_DETAIL;
                }
            }
            break;

        case AppState::ERROR:
            // Show error screen
            M5.Display.fillScreen(Theme::COLOR_BACKGROUND);
            M5.Display.setTextColor(Theme::COLOR_DANGER);
            M5.Display.setTextDatum(MC_DATUM);
            M5.Display.drawString("ERROR", Theme::SCREEN_WIDTH / 2, Theme::SCREEN_HEIGHT / 2 - 10);
            M5.Display.setTextColor(Theme::COLOR_TEXT_SECONDARY);
            M5.Display.drawString("Press any button to restart", Theme::SCREEN_WIDTH / 2, Theme::SCREEN_HEIGHT / 2 + 10);

            // Any button press restarts scanning
            if (M5.BtnA.wasPressed() || M5.BtnB.wasPressed() || M5.BtnC.wasPressed()) {
                g_engine->beginScan();
                g_state = AppState::SCANNING;
            }
            break;
    }

    // Small yield to prevent watchdog
    yield();
}
