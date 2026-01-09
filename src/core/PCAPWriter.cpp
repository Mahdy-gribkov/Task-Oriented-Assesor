#include "PCAPWriter.h"
#include <M5Cardputer.h>

namespace Vanguard {

PCAPWriter::PCAPWriter(const char* filename) 
    : m_headerWritten(false) 
{
    strncpy(m_filename, filename, sizeof(m_filename) - 1);
}

bool PCAPWriter::open() {
    // If file exists, we assume header is already there
    if (SD.exists(m_filename)) {
        m_headerWritten = true;
        return true;
    }

    File file = SD.open(m_filename, FILE_WRITE);
    if (!file) return false;

    pcap_global_header header;
    header.magic_number = 0xa1b2c3d4;
    header.version_major = 2;
    header.version_minor = 4;
    header.thiszone = 0;
    header.sigfigs = 0;
    header.snaplen = 65535;
    header.network = 105; // IEEE 802.11

    file.write((uint8_t*)&header, sizeof(header));
    file.close();
    
    m_headerWritten = true;
    return true;
}

bool PCAPWriter::writePacket(const uint8_t* data, uint16_t len) {
    if (!m_headerWritten && !open()) return false;

    File file = SD.open(m_filename, FILE_APPEND);
    if (!file) return false;

    pcap_packet_header pktHeader;
    uint32_t now = millis();
    pktHeader.ts_sec = now / 1000;
    pktHeader.ts_usec = (now % 1000) * 1000;
    pktHeader.incl_len = len;
    pktHeader.orig_len = len;

    file.write((uint8_t*)&pktHeader, sizeof(pktHeader));
    file.write(data, len);
    file.close();

    return true;
}

void PCAPWriter::close() {
    m_headerWritten = false;
}

} // namespace Vanguard
