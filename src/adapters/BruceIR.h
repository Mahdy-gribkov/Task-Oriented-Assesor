#ifndef VANGUARD_BRUCE_IR_H
#define VANGUARD_BRUCE_IR_H

/**
 * @file BruceIR.h
 * @brief IR adapter for Cardputer (Core: IRremote)
 */

#include <Arduino.h>
#include "../core/VanguardTypes.h"

namespace Vanguard {

class BruceIR {
public:
    static BruceIR& getInstance();

    bool init();
    void tick();

    // Transmission
    void sendRaw(const uint16_t* data, uint16_t len, uint16_t khz = 38);
    void sendTVBGone(); // Power cycle for common TVs

    // Recording
    void startRecording();
    void stopRecording();
    bool hasLastCapture() const { return m_hasLastCapture; }
    void replayLast();

private:
    BruceIR();
    
    bool m_initialized;
    bool m_recording;
    bool m_hasLastCapture;
};

} // namespace Vanguard

#endif // VANGUARD_BRUCE_IR_H
