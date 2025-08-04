#include "DataTree/DeviceData.h"
#include <iostream>
#include <cassert>

using namespace device_data;

void test_device_data() {
    // 测试Element类
    Element elem(1, "ON");
    assert(elem.getValue() == 1);
    assert(elem.getLabel() == "ON");
    
    // 测试FieldInfo类
    FieldInfo field;
    field.setFieldId("test_field");
    field.setFieldName("test");
    field.setFieldType("DOUBLE");
    field.setRangeMin(0.0);
    field.setRangeMax(100.0);
    
    assert(field.getFieldId() == "test_field");
    assert(field.getFieldName() == "test");
    assert(field.getRangeMin() == 0.0);
    assert(field.getRangeMax() == 100.0);
    
    // 测试DeviceInfo类
    DeviceInfo device;
    device.setDeviceId(123);
    device.setDeviceName("Test Device");
    device.addFieldInfo(field);
    
    assert(device.getDeviceId() == 123);
    assert(device.getDeviceName() == "Test Device");
    assert(device.getFieldInfoList().size() == 1);
    
    // 测试查找字段
    field.setFieldKey("temp");
    device.setFieldInfoList({field});
    const FieldInfo* foundField = device.findFieldByKey("temp");
    assert(foundField != nullptr);
    assert(foundField->getFieldKey() == "temp");
    
    // 测试DeviceGroup类
    DeviceGroup group;
    group.setGroupId("group1");
    group.setGroupNameEn("Test Group");
    group.setPriority(1);
    
    assert(group.getGroupId() == "group1");
    assert(group.getGroupNameEn() == "Test Group");
    assert(group.getPriority() == 1);
    
    // 测试DeviceDataContainer类
    DeviceDataContainer container;
    container.addDeviceGroup(group);
    container.addDeviceInfo(device);
    
    assert(container.getDeviceGroupList().size() == 1);
    assert(container.getDeviceInfoList().size() == 1);
    
    // 测试查找功能
    const DeviceGroup* foundGroup = container.findGroupById("group1");
    assert(foundGroup != nullptr);
    assert(foundGroup->getGroupId() == "group1");
    
    const DeviceInfo* foundDevice = container.findDeviceById(123);
    assert(foundDevice != nullptr);
    assert(foundDevice->getDeviceId() == 123);
    
    // 测试统计信息
    auto stats = container.getStats();
    assert(stats.groupCount == 1);
    assert(stats.deviceCount == 1);
    assert(stats.totalFieldCount == 1);
    
    // 测试拷贝操作
    DeviceDataContainer container2;
    container2.fastCopyFrom(container);
    assert(container2.getDeviceGroupList().size() == 1);
    assert(container2.getDeviceInfoList().size() == 1);
    
    // 测试DeviceDataManager单例
    DeviceDataManager& manager1 = DeviceDataManager::Instance();
    DeviceDataManager& manager2 = DeviceDataManager::Instance();
    assert(&manager1 == &manager2); // 确保是同一个实例
    
    std::cout << "  ✓ All device data tests passed" << std::endl;
}
