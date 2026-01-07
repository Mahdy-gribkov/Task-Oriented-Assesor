/**
 * @file ScanSelector.cpp
 * @brief Post-boot scan type selection screen
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

    m_canvas->fillScreen(Theme::COLOR_BACKGROUND);

    // Title
    m_canvas->setTextSize(1);
    m_canvas->setTextColor(Theme::COLOR_ACCENT);
    m_canvas->setTextDatum(TC_DATUM);
    m_canvas->drawString("THE ASSESSOR", Theme::SCREEN_WIDTH / 2, 8);

    // Subtitle with animation
    m_canvas->setTextColor(Theme::COLOR_TEXT_SECONDARY);
    const char* subtitle = (m_animFrame % 2 == 0) ? "Target First. Always." : "Target First. Always._";
    m_canvas->drawString(subtitle, Theme::SCREEN_WIDTH / 2, 22);

    // Separator line
    m_canvas->drawFastHLine(20, 36, Theme::SCREEN_WIDTH - 40, Theme::COLOR_SURFACE_RAISED);

    // Scan options
    int16_t optionY = 48;
    int16_t optionSpacing = 22;

    drawOption(optionY, "R", "Scan WiFi", Theme::COLOR_SUCCESS);
    optionY += optionSpacing;

    drawOption(optionY, "B", "Scan Bluetooth", Theme::COLOR_TYPE_BLE);
    optionY += optionSpacing;

    drawOption(optionY, "ENT", "Scan Both", Theme::COLOR_WARNING);

    // Footer hint
    m_canvas->setTextSize(1);
    m_canvas->setTextColor(Theme::COLOR_TEXT_MUTED);
    m_canvas->setTextDatum(BC_DATUM);
    m_canvas->drawString("[M] Menu", Theme::SCREEN_WIDTH / 2, Theme::SCREEN_HEIGHT - 3);

    m_canvas->pushSprite(0, 0);
}

void ScanSelector::drawOption(int16_t y, const char* key, const char* label, uint16_t keyColor) {
    // Key box
    int16_t keyBoxX = 30;
    int16_t keyBoxW = (strlen(key) > 2) ? 28 : 16;
    int16_t keyBoxH = 14;

    m_canvas->fillRoundRect(keyBoxX, y, keyBoxW, keyBoxH, 2, Theme::COLOR_SURFACE);
    m_canvas->drawRoundRect(keyBoxX, y, keyBoxW, keyBoxH, 2, keyColor);

    // Key text
    m_canvas->setTextSize(1);
    m_canvas->setTextColor(keyColor);
    m_canvas->setTextDatum(MC_DATUM);
    m_canvas->drawString(key, keyBoxX + keyBoxW / 2, y + keyBoxH / 2);

    // Label
    m_canvas->setTextColor(Theme::COLOR_TEXT_PRIMARY);
    m_canvas->setTextDatum(ML_DATUM);
    m_canvas->drawString(label, keyBoxX + keyBoxW + 10, y + keyBoxH / 2);
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
