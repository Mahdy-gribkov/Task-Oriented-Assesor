#include <gtest/gtest.h>
#include "../../test/mocks/Arduino.h"
#include "../../src/core/TargetTable.h"
#include "../../src/core/VanguardTypes.h"

using namespace Vanguard;

class TargetTableTest : public ::testing::Test {
protected:
    TargetTable table;

    void SetUp() override {
        table.clear();
    }
};

TEST_F(TargetTableTest, AddNewTarget) {
    Target t;
    memset(&t, 0, sizeof(Target));
    uint8_t mac[6] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
    memcpy(t.bssid, mac, 6);
    strcpy(t.ssid, "TestNet");
    t.type = TargetType::ACCESS_POINT;
    t.rssi = -50;

    EXPECT_TRUE(table.addOrUpdate(t));
    EXPECT_EQ(table.count(), 1);
    
    const Target* found = table.findByBssid(mac);
    ASSERT_NE(found, nullptr);
    EXPECT_STREQ(found->ssid, "TestNet");
}

TEST_F(TargetTableTest, UpdateExistingTarget) {
    Target t1;
    memset(&t1, 0, sizeof(Target));
    uint8_t mac[6] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
    memcpy(t1.bssid, mac, 6);
    strcpy(t1.ssid, "TestNet");
    
    table.addOrUpdate(t1);
    
    Target t2 = t1;
    t2.rssi = -30;
    
    EXPECT_FALSE(table.addOrUpdate(t2)); // Should return false for update
    EXPECT_EQ(table.count(), 1);
    
    const Target* found = table.findByBssid(mac);
    ASSERT_NE(found, nullptr);
    EXPECT_EQ(found->rssi, -30);
}

TEST_F(TargetTableTest, PruneStale) {
    Target t;
    memset(&t, 0, sizeof(Target));
    uint8_t mac[6] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
    memcpy(t.bssid, mac, 6);
    t.lastSeenMs = 1000;
    
    table.addOrUpdate(t);
    
    // Default timeout is 60000ms
    EXPECT_EQ(table.pruneStale(1000), 0);
    EXPECT_EQ(table.pruneStale(62000), 1);
    EXPECT_EQ(table.count(), 0);
}

TEST_F(TargetTableTest, Association) {
    uint8_t apMac[6] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};
    uint8_t cliMac[6] = {0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC};
    
    Target ap;
    memset(&ap, 0, sizeof(Target));
    memcpy(ap.bssid, apMac, 6);
    table.addOrUpdate(ap);
    
    EXPECT_TRUE(table.addAssociation(cliMac, apMac));
    
    const Target* found = table.findByBssid(apMac);
    EXPECT_EQ(found->clientCount, 1);
    EXPECT_TRUE(found->hasClient(cliMac));
}
