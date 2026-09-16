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

TEST(DevicesTest, ReceiveId) {
    auto mockServer = std::make_shared<MockDeviceServer>();
    LightDevice lightDevice(mockServer);
    TemperatureDevice temperatureDevice(mockServer);

    EXPECT_CALL(*mockServer, giveDeviceId(DeviceType::LIGHT_DEVICE))
        .Times(1)
        .WillOnce(::testing::Return(1));

    EXPECT_CALL(*mockServer, giveDeviceId(DeviceType::TEMPERATURE_DEVICE))
        .Times(1)
        .WillOnce(::testing::Return(2));

    lightDevice.receiveId();
    EXPECT_EQ(lightDevice.getId(), 1);

    temperatureDevice.receiveId();
    EXPECT_EQ(temperatureDevice.getId(), 2);
}

TEST(LightDeviceTest, ReceiveData) {
    auto mockServer = std::make_shared<MockDeviceServer>();
    LightDevice lightDevice(mockServer);

    EXPECT_CALL(*mockServer, giveDeviceId(DeviceType::LIGHT_DEVICE))
        .Times(1)
        .WillOnce(::testing::Return(1));

    EXPECT_CALL(*mockServer, receiveData(1, DeviceData("full")))
        .Times(1);

    lightDevice.receiveId();
    lightDevice.receiveData("full");
}

TEST(TemperatureDeviceTest, ReceiveData) {
    auto mockServer = std::make_shared<MockDeviceServer>();
    TemperatureDevice temperatureDevice(mockServer);

    EXPECT_CALL(*mockServer, giveDeviceId(DeviceType::TEMPERATURE_DEVICE))
        .Times(1)
        .WillOnce(::testing::Return(1));

    EXPECT_CALL(*mockServer, receiveData(1, DeviceData(25)))
        .Times(1);

    temperatureDevice.receiveId();
    temperatureDevice.receiveData(25);
}