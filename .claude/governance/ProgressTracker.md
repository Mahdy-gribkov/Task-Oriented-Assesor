# Progress Tracker
> **Last Updated:** Jan 9, 2026 (Session 3)

## 1. Context Summary

**Phase 2 Complete.** Comprehensive bug fixes applied, 5GHz UX improved, About dialog added, firmware uploaded to Cardputer for testing.

---

## 2. Phase 0: Architecture (COMPLETE)

- [x] Initial governance files created
- [x] Misunderstanding corrected (we wrap Bruce, not replace it)
- [x] MasterPlan.md rewritten with correct architecture
- [x] ProjectRules.md expanded with coding standards
- [x] Boot sequence UX designed
- [x] File structure defined
- [x] **PlatformIO project created**
- [x] **All core class headers stubbed:**
  - Types.h (enums, structs, constants)
  - TargetTable.h (state management)
  - ActionResolver.h (context-aware filtering)
  - AssessorEngine.h (orchestrator)
- [x] **All UI class headers stubbed:**
  - Theme.h (visual constants)
  - BootSequence.h (splash/onboarding)
  - TargetRadar.h (main list view)
  - TargetDetail.h (single target + actions)
- [x] **README.md created** (viral-ready)
- [x] **PHILOSOPHY.md created** (target-first manifesto)
- [x] **main.cpp entry point created**

---

## 3. Phase 1: Bruce Integration (COMPLETE)

- [x] Analyze Bruce repository structure
- [x] Map Bruce's attack function signatures
- [x] Document findings in BruceAnalysis.md
- [x] Create BruceWiFi.h adapter interface
- [x] Create BruceBLE.h adapter interface
- [x] Extract raw frame sending logic (minimal, no bloat)
- [x] Implement BruceWiFi.cpp (deauth, beacon, handshake)
- [x] Implement BruceBLE.cpp (spam, sour apple, beacon spoofing)
- [x] Implement TargetTable.cpp
- [x] Implement ActionResolver.cpp
- [x] Implement AssessorEngine.cpp
- [x] Implement BootSequence.cpp
- [x] Implement TargetRadar.cpp
- [x] Implement TargetDetail.cpp
- [x] Implement MainMenu.cpp
- [x] Implement SettingsPanel.cpp
- [x] Implement ScanSelector.cpp

---

## 4. Phase 2: Bug Fixes & Features (IN PROGRESS)

### Critical Bug Fixes (Jan 9, 2026 - Session 2)
- [x] **Fixed blocking WiFi scan** - Changed from synchronous to async scanning
  - Root cause: `WiFi.scanNetworks(false,...)` blocked for 3-5 seconds causing watchdog reset
  - Fix: Changed to `WiFi.scanNetworks(true,...)` with completion checking in `tick()`
- [x] **Fixed WiFi→BLE transition** - Added proper radio shutdown between WiFi and BLE
- [x] **Removed excessive yield() spam** - Cleaned up overzealous watchdog feeding
- [x] **Fixed Evil Twin mode switch** - Reduced blocking delays

### Comprehensive Bug Fixes (Jan 9, 2026 - Session 3)
- [x] **Added yieldDelay() helper** - Watchdog-safe delay function for blocking transitions
- [x] **Fixed BLE/Combined scan freezes** - Added yield() calls throughout BruceBLE::init() with timeout
- [x] **Fixed menu action misfiring** - Added g_consumeNextInput flag to prevent key "bleed-through"
- [x] **Added 5GHz warning popup** - Shows warning before entering 5GHz network detail (ESP32 limitation)
- [x] **Improved 5GHz messaging** - TargetDetail now shows "5GHz - Info Only" with explanation
- [x] **Implemented About dialog** - New AboutPanel.h/cpp with version info and credits

### New Features (Jan 9, 2026 - Session 2)
- [x] **Implemented Evil Portal** - Full captive portal with credential capture
  - EvilPortal.h/cpp with DNS spoofing and web server
  - Generic WiFi login template
  - Google-style login template
  - Credential storage and client tracking
  - Live status display (client count, captured credentials)

### Hardware Testing
- [x] Firmware uploaded to Cardputer (Jan 9, 2026 - Session 2 & 3)
- [ ] Verify WiFi scanning works without crash
- [ ] Verify BLE scanning works after WiFi
- [ ] Verify 5GHz warning popup works
- [ ] Verify About dialog works
- [ ] Test deauth attack
- [ ] Test beacon flood
- [ ] Test Evil Portal credential capture
- [ ] Test BLE spam

### Remaining Tasks
- [ ] Add PCAP/handshake capture (EAPOL detection)
- [ ] Add more Bruce attacks (probe flood, etc.)
- [ ] README with GIFs/screenshots
- [ ] GitHub Actions CI

---

## 5. Blockers & Decisions

| Date | Issue | Resolution |
|------|-------|------------|
| Jan 7 | Initial arch was raw driver build | Pivoted to Bruce wrapper model |
| Jan 9 | Sync WiFi scan caused watchdog reset | Changed to async scanning |
| Jan 9 | yield() spammed everywhere | Cleaned up, only use delay() where needed |

---

## 6. Session Notes

### Session 1 (Jan 7, 2026)
- User clarified: has Cardputer ADV, loads Bruce from SD
- Goal: target-first UI wrapper, viral-quality repo
- Full creative control granted
- Created full project skeleton with PlatformIO
- Stubbed all core and UI classes
- Created README.md and PHILOSOPHY.md
- Analyzed Bruce source code structure
- Created adapter interfaces

### Session 2 (Jan 9, 2026)
- Previous session left project in broken state ("borken. mid plan.")
- User reported: device crashes, WiFi doesn't scan, BLE doesn't work, display freezes
- Root cause identified: WiFi scan was changed to blocking/synchronous in last commit
- Fixed async scanning, WiFi→BLE transition, removed yield spam
- Implemented full Evil Portal with captive portal and credential capture
- Added ESPAsyncWebServer, AsyncTCP, IRremote libraries
- Firmware uploaded to Cardputer for testing

### Session 3 (Jan 9, 2026)
- Context restored from session summary (previous session hit context limit)
- User testing revealed 5 remaining bugs:
  1. 5GHz network click shows "No actions" with no explanation
  2. Pressing B (BLE scan) freezes device
  3. Combined scan (OK) freezes at 45%
  4. About menu does nothing (TODO comment only)
  5. Menu actions misfire (About triggers scan)
- Created comprehensive plan and got user approval
- Fixed all 5 bugs:
  - Added yieldDelay() helper for watchdog-safe delays
  - Added yield() calls and timeout to BruceBLE::init()
  - Added g_consumeNextInput flag to prevent key bleed-through
  - Added 5GHz warning popup to TargetRadar
  - Improved 5GHz messaging in TargetDetail
  - Created AboutPanel.h/cpp and wired up in main.cpp
- Firmware rebuilt and uploaded to Cardputer for testing

### Session 4 (Jan 9, 2026 - Current)
- **Goal**: Phase 3 - Engine Fusion (Bruce + Evil-M5)
- **Governance**: "Cycle of Truth" established. Strict Doc-Work-Doc workflow.
- **Plan**: `FusionPlan.md` created and approved.
- **Next Steps**:
  1. Cleanup/Purge "fake" code.
  2. Implement Promiscuous Mode (Client Discovery).
  3. Implement SD Card Persistence (Evil Portal).

### Phase 3.0: Cleanup & Migration (COMPLETE)
- [x] **Library Audit** - Added M5Unified to `platformio.ini` [antigravity]
- [x] **Purge BruceWiFi** - Added implementation TODOs for Phase 3.1 [antigravity]
- [x] **Purge EvilPortal** - Added implementation TODOs for Phase 3.2 [antigravity]

### Phase 3.1: The Eyes (Visibility) (COMPLETE)
- [x] **Data Frame Parsing** - Implement 802.11 header extraction in `BruceWiFi` [antigravity]
- [x] **Client Tracking** - Update `TargetTable` to associate clients with APs [antigravity]
- [x] **UI Update** - Show client counts in `TargetRadar` and `TargetDetail` [antigravity]

### Phase 3.2: The Memory (Persistence) (COMPLETE)
- [x] **SD Card Manager** - Initialize SD via `M5Unified` and create folder structure [antigravity]
- [x] **Evil Portal Persistence** - Save captured credentials to `credentials.csv` on SD [antigravity]
- [x] **Template Loading** - Load custom HTML from SD card [antigravity]

### Phase 3.3: The Capture (Handshakes) (COMPLETE)
- [x] **EAPOL Sniffing** - Detect EAPOL frames in `promiscuousCallback` [antigravity]
- [x] **PCAP Writer** - Implement libpcap global and packet headers [antigravity]
- [x] **Handshake Capture** - Orchestrate deauth + sniff cycle to capture 4-way handshake [antigravity]
- [x] **Verification** - Basic sanity check of captured EAPOL frames [antigravity]


### Session 5 (Jan 9, 2026 - CURRENT)
- **Goal**: Phase 3.4 - The Voice (IR) & UI Polish
- **Status**: Starting IR implementation.
- **Tasks**:
  1. Implement IR Adapter.
  2. Implement TV-B-Gone / IR Replay.
  3. Finalize UI "Target-First" flow.
  4. **Document AV False Positive**: Windows Defender flagged `EvilPortal.cpp` as a WebShell due to phishing templates. Confirmed as false positive and documented in `FusionPlan.md`.

### Phase 3.4: The Voice (Transmission) (COMPLETE)
- [x] **IR Implementation** - Create `BruceIR` adapter for Cardputer IR TX/RX [antigravity]
- [x] **TV-B-Gone** - Core infrared power cycling functionality [antigravity]
- [x] **UI Action Mapping** - Wire IR actions to the DetailView [antigravity]

### Phase 3.5: Deployment (M5Launcher) (COMPLETE)
- [x] **Incident Response** - Document Windows Defender false positive incident [antigravity]
- [x] **Build Automation** - Create `build_release.ps1` for easy .bin generation [antigravity]
- [x] **Phase 3 Wrap-up** - Finalize Session 5 walkthrough [antigravity]

---
**Phase 3 (Engine Fusion) is now 100% COMPLETE.**
The Assessor has Eyes (Visibility), Memory (SD), Hands (Offense), and Voice (Transmission).
Next: Phase 4.0 - Field Testing & Refinement.
