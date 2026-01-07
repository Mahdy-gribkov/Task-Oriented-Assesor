/**
 * @file ScanSelector.cpp
 * @brief Post-boot scan type selection screen - ORANGE THEMED
 */

#include "ScanSelector.h"

namespace Assessor {

ScanSelector::ScanSelector()
    : m_visible(false)
    , m_needsRedraw(true)
    , m_selection(ScanChoice::NONE)
    , m_canvas(nullptr)
    , m_lastRenderMs(0)
    , m_animFrame(0)
{
    m_canvas = new M5Canvas(&M5Cardputer.Display);
    m_canvas->createSprite(Theme::SCREEN_WIDTH, Theme::SCREEN_HEIGHT);
}

ScanSelector::~ScanSelector() {
    if (m_canvas) {
        m_canvas->deleteSprite();
        delete m_canvas;
        m_canvas = nullptr;
    }
}

void ScanSelector::show() {
    m_visible = true;
    m_needsRedraw = true;
    m_selection = ScanChoice::NONE;
    m_animFrame = 0;
}

void ScanSelector::hide() {
    m_visible = false;
}

bool ScanSelector::isVisible() const {
    return m_visible;
}

void ScanSelector::tick() {
    // Animation tick
    uint32_t now = millis();
    if (now - m_lastRenderMs >= ANIM_INTERVAL_MS) {
        m_animFrame++;
        m_needsRedraw = true;
    }
}

void ScanSelector::render() {
    if (!m_visible || !m_canvas) return;

    uint32_t now = millis();
    if (!m_needsRedraw && (now - m_lastRenderMs) < RENDER_INTERVAL_MS) {
        return;
    }
    m_lastRenderMs = now;
    m_needsRedraw = false;

    // Dark background
    m_canvas->fillScreen(Theme::COLOR_BACKGROUND);

    // Orange accent bar at top
    m_canvas->fillRect(0, 0, Theme::SCREEN_WIDTH, 3, Theme::COLOR_ACCENT);

    // Title - BIG and ORANGE
    m_canvas->setTextSize(2);
    m_canvas->setTextColor(Theme::COLOR_ACCENT);
    m_canvas->setTextDatum(TC_DATUM);
    m_canvas->drawString("THE ASSESSOR", Theme::SCREEN_WIDTH / 2, 12);

    // Subtitle with blinking cursor
    m_canvas->setTextSize(1);
    m_canvas->setTextColor(Theme::COLOR_TEXT_SECONDARY);
    const char* subtitle = (m_animFrame % 2 == 0) ? "Target First. Always." : "Target First. Always._";
    m_canvas->drawString(subtitle, Theme::SCREEN_WIDTH / 2, 32);

    // Orange separator line
    m_canvas->drawFastHLine(30, 44, Theme::SCREEN_WIDTH - 60, Theme::COLOR_ACCENT);

    // Scan options - ALL ORANGE THEMED
    int16_t optionY = 54;
    int16_t optionSpacing = 24;

    // WiFi option
    drawOption(optionY, "R", "Scan WiFi Networks", Theme::COLOR_ACCENT);
    optionY += optionSpacing;

    // BLE option
    drawOption(optionY, "B", "Scan Bluetooth", Theme::COLOR_ACCENT);
    optionY += optionSpacing;

    // Both option - highlighted differently
    drawOption(optionY, "OK", "Scan Everything", Theme::COLOR_ACCENT);

    // Footer with orange accent
    m_canvas->fillRect(0, Theme::SCREEN_HEIGHT - 12, Theme::SCREEN_WIDTH, 12, Theme::COLOR_SURFACE);
    m_canvas->setTextSize(1);
    m_canvas->setTextColor(Theme::COLOR_ACCENT);
    m_canvas->setTextDatum(MC_DATUM);
    m_canvas->drawString("[M] Menu", Theme::SCREEN_WIDTH / 2, Theme::SCREEN_HEIGHT - 6);

    m_canvas->pushSprite(0, 0);
}

void ScanSelector::drawOption(int16_t y, const char* key, const char* label, uint16_t keyColor) {
    // Key box - orange border
    int16_t keyBoxX = 20;
    int16_t keyBoxW = (strlen(key) > 1) ? 24 : 16;
    int16_t keyBoxH = 16;

    // Orange bordered box
    m_canvas->fillRoundRect(keyBoxX, y, keyBoxW, keyBoxH, 3, Theme::COLOR_SURFACE);
    m_canvas->drawRoundRect(keyBoxX, y, keyBoxW, keyBoxH, 3, keyColor);

    // Key text in orange
    m_canvas->setTextSize(1);
    m_canvas->setTextColor(keyColor);
    m_canvas->setTextDatum(MC_DATUM);
    m_canvas->drawString(key, keyBoxX + keyBoxW / 2, y + keyBoxH / 2);

    // Label in white
    m_canvas->setTextColor(Theme::COLOR_TEXT_PRIMARY);
    m_canvas->setTextDatum(ML_DATUM);
    m_canvas->drawString(label, keyBoxX + keyBoxW + 12, y + keyBoxH / 2);
}

bool ScanSelector::hasSelection() const {
    return m_selection != ScanChoice::NONE;
}

ScanChoice ScanSelector::getSelection() const {
    return m_selection;
}

void ScanSelector::clearSelection() {
    m_selection = ScanChoice::NONE;
}

void ScanSelector::onKeyR() {
    m_selection = ScanChoice::WIFI_ONLY;
}

void ScanSelector::onKeyB() {
    m_selection = ScanChoice::BLE_ONLY;
}

void ScanSelector::onKeyEnter() {
    m_selection = ScanChoice::COMBINED;
}

} // namespace Assessor
