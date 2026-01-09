# The Assessor - Fusion Plan (Phase 3)
> **Status:** ACTIVE | **Phase:** 3 (Engine Fusion) | **Target:** v0.1-Alpha

## 1. The Fusion Vision

**The Assessor** is not just a UI for Bruce. It is the **Unification** of two distinct philosophies:
1.  **Bruce (The Muscle):** Raw frame injection, massive deauths, BLE spam. The "Verb".
2.  **Evil-M5Project (The Brain):** Captive portals, credential logging, persistence. The "Memory".

Our goal is to create a **Target-First Platform** that wields both. The user sees a target, and the engine decides: "Do we hit it with Bruce (Deauth), or trap it with Evil-M5 (Portal)?"

### The Current Reality (The "Hollow Shell")
As of Jan 9, 2026, the project is a beautiful UI shell with **no functional engines**.
- **Bruce Engine:** Disabled/Not linked. "Ported" code is stripped of logic.
- **Evil-M5 Engine:** "Ported" code is RAM-only (amnesia on reboot).

We must build the **Unified Assessor Engine** to bridge this gap.

---

## 2. Governance & Workflow (CRITICAL)

**Rule #1: The Cycle of Truth**
Every task MUST follow this cycle:
1.  **Update Docs**: Mark task as "In Progress" in `.claude/governance/ProgressTracker.md`.
2.  **Execute**: Write code, build, test.
3.  **Commit**: Git commit with detailed explanation (e.g., `feat: implement promiscuous callback [antigravity]`).
4.  **Update Docs**: Mark task as "Complete" in `ProgressTracker.md` and update `FusionPlan.md` if design changed.

**Rule #2: Context Preservation**
- The `.claude` folder is the **Single Source of Truth**.
- Never implement a feature without a corresponding entry in `FusionPlan.md`.

---

## 3. Architecture 2.0: The Unified Engine

```
┌─────────────────────────────────────────────────────────────┐
│                      THE ASSESSOR                           │
│              "Target First. Always."                        │
├─────────────────────────────────────────────────────────────┤
│  ┌─────────────┐   ┌───────────────────────────────┐        │
│  │     UI      │   │       ASSESSOR ENGINE         │        │
│  │   (Ready)   │   │     (The Fusion Layer)        │        │
│  │             │   │                               │        │
│  │ TargetRadar │◄──┤  State: TargetTable + Clients │        │
│  │ DetailView  │   │  Logic: ActionResolver        │        │
│  └─────────────┘   │  Memory: SD Card Manager      │        │
│                    └───────────────┬───────────────┘        │
│                                    │                        │
│             ┌──────────────────────┴──────────────────────┐ │
│             ▼                                             ▼ │
│  ┌──────────────────────┐                  ┌──────────────────────┐
│  │   BRUCE ADAPTER      │                  │   EVIL-M5 ADAPTER    │
│  │      (The Muscle)    │                  │      (The Trap)      │
│  │                      │                  │                      │
│  │ • Raw Frame Injection│                  │ • Captive Portal     │
│  │ • EAPOL Sniffing     │                  │ • DNS Spoofing       │
│  │ • BLE Spamming       │                  │ • Credential Logging │
│  └──────────────────────┘                  └──────────────────────┘
│             │                                             │
└─────────────┼─────────────────────────────────────────────┼─┘
              ▼                                             ▼
       [ESP32 RADIO]                                  [SD CARD]
```

---

## 4. Implementation Phases (Step-by-Step)

### Phase 3.0: Cleanup & Migration (The "Purge")
*Goal: Remove broken stubs and prepare clean foundation.*
- [ ] **Purge "Fake" Ported Code**:
    - Remove gutted logic in `BruceWiFi.cpp` that pretends to scan but doesn't.
    - Remove RAM-only credential storage in `EvilPortal.cpp`.
- [ ] **Library Audit (Jan 2026)**:
    - Update `platformio.ini` to use latest `M5Unified` and `M5Cardputer`.
    - Ensure `NimBLE-Arduino` is pinned to a stable 1.4.x or 2.x release compatible with ESP32-S3.

### Phase 3.1: The Eyes (Visibility)
*Goal: See clients. Without clients, we cannot Target-First.*

- [ ] **Dependency Fix**: Ensure `SD` and `M5Unified` are correctly linked in `platformio.ini`.
- [ ] **Promiscuous Mode Implementation (`BruceWiFi.cpp`)**:
    - **Parse Data Frames**: Extract Source MAC (Client) and BSSID (AP) from 802.11 headers.
    - **Filter**: Ignore control frames to save CPU.
    - **Feed Engine**: Push `AssociationEvent(client, ap)` to `AssessorEngine`.
- [ ] **Target Table Upgrade (`TargetTable.cpp`)**:
    - **Client List**: Add `std::vector<Client>` to `Target` struct.
    - **Association Logic**: `addClient(mac, bssid)` method.
    - **Pruning**: Remove clients that haven't been seen in X seconds.

### Phase 3.2: The Memory (Persistence)
*Goal: Remember what we stole. Evil-M5 core requirement.*

- [ ] **SD Card Manager**:
    - Initialize SD via M5Unified.
    - Create directory structure: `/captures/`, `/evil_portal/`, `/logs/`.
- [ ] **Evil Portal Persistence**:
    - Update `handleLogin` in `EvilPortal.cpp`.
    - **Action**: Append `timestamp,ssid,user,pass,mac` to `/evil_portal/credentials.csv`.
- [ ] **Portal Templates**:
    - Load `index.html` from `/evil_portal/templates/` if present.

### Phase 3.3: The Hand (Offense)
*Goal: Execute attacks that require 3.1 and 3.2.*

- [ ] **Precision Deauth**:
    - `ActionResolver`: Enable "Deauth Single" only if `target.clients.size() > 0`.
    - `BruceWiFi`: Send deauth frames to specific client MAC (from 3.1).
- [ ] **Handshake Capture**:
    - **EAPOL Detection**: In `promiscuousCallback`, detect EtherType `0x888E`.
    - **State Machine**: Track 4-way handshake (M1..M4).
    - **PCAP Dump**: Write raw frames to `/captures/{ssid}_{mac}.pcap` on SD.

### Phase 3.4: The Voice (Transmission)
*Goal: Control the physical environment via IR and Sub-GHz (if capable).*

- [ ] **IR Adapter Implementation**:
    - Port Bruce's IR record/replay logic to a clean `BruceIR` adapter.
    - Support TV-B-Gone (power cycle) and custom code replay.
- [ ] **UI Polish**:
    - Finalize "Target-First" behavior: IR targets appear in Radar as "Physical Devices".
    - Action labels for IR: "Replay", "Spam Power", "Identify".

### Phase 3.5: The Deployment (M5Launcher)
*Goal: Easy SD card loading and release management.*
- [ ] **Build Workflow**:
    - Use `pio run` to generate `.bin`.
    - Automated renaming to `Assessor_vX.X_Alpha.bin`.
    - Create `walkthrough.md` for final alpha release.

---

## 5. Technical Specifications

### Data Frame Parsing (Simulated)
```cpp
// 802.11 Frame Header (Simplified)
struct FrameHeader {
    uint16_t frame_control;
    uint16_t duration;
    uint8_t  addr1[6]; // Destination
    uint8_t  addr2[6]; // Source
    uint8_t  addr3[6]; // BSSID
};
// We need addr2 (Client) and addr3 (BSSID/AP)
```

### EAPOL Detection
```cpp
// Ethernet II Header inside 802.11 Data Frame (after LLC/SNAP)
// EtherType for EAPOL is 0x888E
if (ether_type == 0x888E) {
    // This is a handshake packet
    savePcap(packet);
}
```

---

## 7. Security & False Positives (KNOWLEDGE BASE)

### Windows Defender / AV Alerts
- **Incident (Jan 9, 2026)**: Windows Defender flagged the project as `Backdoor:Java/WebShell.D!dha`.
- **Reason**: The raw HTML/JS templates in `EvilPortal.cpp` (Google/WiFi login clones) contain byte patterns that match phishing signatures used in actual malware.
- **Resolution**: This is a **False Positive**. The project contains no Java code. Users should exclude the project directory from AV scanning to prevent build failures or file quarantine.

---

## 8. Verification Matrix

| Feature | Test Case | Success Criteria |
| :--- | :--- | :--- |
| **Client Discovery** | Scan with active phone connected to WiFi | UI shows "Clients: 1" and phone's MAC in details. |
| **SD Init** | Boot device | "SD OK" appears in Serial log. Folders created. |
| **Deauth Single** | Select phone target -> Deauth | Phone disconnects within 5 seconds. |
| **Portal Log** | Connect to Evil Portal -> Submit "test/test" | `/evil_portal/credentials.csv` contains "test,test". |
| **Handshake** | Deauth -> Reconnect phone | UI notifies "Handshake Captured". .pcap file exists. |
