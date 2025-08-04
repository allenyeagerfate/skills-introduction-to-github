#include "DataTree/DeviceData.h"
#include <iostream>

using namespace device_data;

int main() {
    std::cout << "=== Device Data Manager Demo ===" << std::endl;
    
    // 获取DeviceDataManager单例
    DeviceDataManager& manager = DeviceDataManager::Instance();
    
    // 创建一些测试数据
    DeviceDataContainer container;
    
    // 创建设备组
    DeviceGroup group1;
    group1.setGroupId("group_001");
    group1.setGroupNameEn("Temperature Sensors");
    group1.setGroupNameZh("温度传感器组");
    group1.setPriority(1);
    container.addDeviceGroup(group1);
    
    // 创建设备
    DeviceInfo device1;
    device1.setDeviceId(1001);
    device1.setDeviceGroupId(1);
    device1.setDeviceName("Temperature Sensor 1");
    device1.setDeviceChineseName("温度传感器1");
    device1.setPriority(1);
    
    // 创建字段
    FieldInfo field1;
    field1.setFieldId("temp_001");
    field1.setFieldName("temperature");
    field1.setFieldChineseName("温度");
    field1.setFieldType("DOUBLE");
    field1.setUnit("°C");
    field1.setRangeMin(-50.0);
    field1.setRangeMax(150.0);
    
    device1.addFieldInfo(field1);
    container.addDeviceInfo(device1);
    
    // 显示统计信息
    auto stats = container.getStats();
    std::cout << "Container Statistics:" << std::endl;
    std::cout << "  Groups: " << stats.groupCount << std::endl;
    std::cout << "  Devices: " << stats.deviceCount << std::endl;
    std::cout << "  Fields: " << stats.totalFieldCount << std::endl;
    std::cout << "  Estimated Memory: " << stats.estimatedMemorySize << " bytes" << std::endl;
    
    // 测试JSON导出
    std::string jsonStr = manager.toJsonString();
    std::cout << "\nJSON Export:\n" << jsonStr << std::endl;
    
    std::cout << "\nDemo completed successfully!" << std::endl;


    DeviceDataContainer data;
    DeviceUpdateConfig(data);
    GetDeviceForInernal(data);



    return 0;
}
