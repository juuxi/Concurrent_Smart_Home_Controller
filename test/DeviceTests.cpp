#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <LightDevice.hpp>
#include <TemperatureDevice.hpp>

class MockDeviceServer {
public:
    MOCK_METHOD(int, giveDeviceId, (DeviceType type));
    MOCK_METHOD(void, receiveData, (const size_t id, DeviceData data));
};

TEST(LightDeviceTest, ReceiveId) {
    auto mockServer = std::make_shared<MockDeviceServer>();
    LightDevice lightDevice(mockServer);

    EXPECT_CALL(*mockServer, giveDeviceId(DeviceType::LIGHT_DEVICE))
        .Times(1)
        .WillOnce(::testing::Return(1));

    lightDevice.receiveId();
    EXPECT_EQ(lightDevice.getId(), 1);
}

TEST(TemperatureDeviceTest, ReceiveId) {
    auto mockServer = std::make_shared<MockDeviceServer>();
    TemperatureDevice temperatureDevice(mockServer);

    EXPECT_CALL(*mockServer, giveDeviceId(DeviceType::TEMPERATURE_DEVICE))
        .Times(1)
        .WillOnce(::testing::Return(1));

    temperatureDevice.receiveId();
    EXPECT_EQ(temperatureDevice.getId(), 1);
}