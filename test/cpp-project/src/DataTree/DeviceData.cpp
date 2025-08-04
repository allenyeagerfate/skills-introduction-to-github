#include "DataTree/DeviceData.h"
#include <iostream>
#include <sstream>

namespace device_data {

// DeviceDataManager的实现

bool DeviceDataManager::loadFromJsonString(const std::string& jsonContent) {
    // TODO: 实现JSON解析
    // 这里先放一个简单的占位实现
    std::cout << "Loading from JSON: " << jsonContent.substr(0, 100) << "..." << std::endl;
    return true;
}

std::string DeviceDataManager::toJsonString() const {
    // TODO: 实现JSON序列化
    // 这里先返回一个简单的JSON字符串
    std::stringstream ss;
    ss << "{\n";
    ss << "  \"deviceGroups\": " << m_deviceData.getDeviceGroupList().size() << ",\n";
    ss << "  \"devices\": " << m_deviceData.getDeviceInfoList().size() << "\n";
    ss << "}";
    return ss.str();
}

} // namespace device_data
