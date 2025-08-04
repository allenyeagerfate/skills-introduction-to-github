#pragma once
#include "utils/SingletonEx.h"
#include "utils/HashRWLock.h"
//#include "DeviceDataLockManager.h"
#include <string>
#include <vector>
#include <memory>


using namespace util;

namespace device_data {

// 针对设备数据的特化锁管理器
class DeviceDataLockManager {
public:
    DeviceDataLockManager() 
        : m_deviceLocks(32),     // 设备锁，32个桶
          m_groupLocks(16),      // 组锁，16个桶  
          m_fieldLocks(64) {     // 字段锁，64个桶
    }
    
    // 设备相关锁操作
    void lockDeviceForRead(int deviceId) {
        m_deviceLocks.lock_shared(deviceId);
    }
    
    void unlockDeviceForRead(int deviceId) {
        m_deviceLocks.unlock_shared(deviceId);
    }
    
    void lockDeviceForWrite(int deviceId) {
        m_deviceLocks.lock(deviceId);
    }
    
    void unlockDeviceForWrite(int deviceId) {
        m_deviceLocks.unlock(deviceId);
    }
    
    // 组相关锁操作
    void lockGroupForRead(const std::string& groupId) {
        m_groupLocks.lock_shared(groupId);
    }
    
    void unlockGroupForRead(const std::string& groupId) {
        m_groupLocks.unlock_shared(groupId);
    }
    
    void lockGroupForWrite(const std::string& groupId) {
        m_groupLocks.lock(groupId);
    }
    
    void unlockGroupForWrite(const std::string& groupId) {
        m_groupLocks.unlock(groupId);
    }
    
    // 字段相关锁操作
    void lockFieldForRead(const std::string& fieldId) {
        m_fieldLocks.lock_shared(fieldId);
    }
    
    void unlockFieldForRead(const std::string& fieldId) {
        m_fieldLocks.unlock_shared(fieldId);
    }
    
    void lockFieldForWrite(const std::string& fieldId) {
        m_fieldLocks.lock(fieldId);
    }
    
    void unlockFieldForWrite(const std::string& fieldId) {
        m_fieldLocks.unlock(fieldId);
    }
    

    
    DeviceReadLock createDeviceReadLock(int deviceId) {
        return DeviceReadLock(m_deviceLocks, deviceId);
    }
    
    DeviceWriteLock createDeviceWriteLock(int deviceId) {
        return DeviceWriteLock(m_deviceLocks, deviceId);
    }
    
    GroupReadLock createGroupReadLock(const std::string& groupId) {
        return GroupReadLock(m_groupLocks, groupId);
    }
    
    GroupWriteLock createGroupWriteLock(const std::string& groupId) {
        return GroupWriteLock(m_groupLocks, groupId);
    }
    
    FieldReadLock createFieldReadLock(const std::string& fieldId) {
        return FieldReadLock(m_fieldLocks, fieldId);
    }
    
    FieldWriteLock createFieldWriteLock(const std::string& fieldId) {
        return FieldWriteLock(m_fieldLocks, fieldId);
    }
    
    // 批量锁定设备组中的所有设备
    template<typename Container>
    auto createMultiDeviceReadLock(const Container& deviceIds) {
        return typename HashBasedRWLock<int>::template MultiKeyLock<Container>(
            m_deviceLocks, deviceIds, false);
    }
    
    template<typename Container>
    auto createMultiDeviceWriteLock(const Container& deviceIds) {
        return typename HashBasedRWLock<int>::template MultiKeyLock<Container>(
            m_deviceLocks, deviceIds, true);
    }
    
    // 获取锁统计信息
    struct AllLockStats {
        HashBasedRWLock<int>::LockStats deviceStats;
        HashBasedRWLock<std::string>::LockStats groupStats;
        HashBasedRWLock<std::string>::LockStats fieldStats;
    };
    
    AllLockStats getAllStats() const {
        AllLockStats stats;
        stats.deviceStats = m_deviceLocks.getStats();
        stats.groupStats = m_groupLocks.getStats();
        stats.fieldStats = m_fieldLocks.getStats();
        return stats;
    }
    
private:
    HashBasedRWLock<int> m_deviceLocks;           // 设备锁
    HashBasedRWLock<std::string> m_groupLocks;    // 组锁
    HashBasedRWLock<std::string> m_fieldLocks;    // 字段锁
};


// 元素类 - 用于开关类型字段的值枚举映射
class Element {
public:
    Element() = default;
    Element(int val, const std::string& label) : value(val), label(label) {}
    
    int getValue() const { return value; }
    void setValue(int val) { value = val; }
    
    const std::string& getLabel() const { return label; }
    void setLabel(const std::string& lbl) { label = lbl; }

private:
    int value = 0;
    std::string label;
};

// 字段信息类
class FieldInfo {
public:
    FieldInfo() = default;
    
    // Getters
    const std::string& getItemType() const { return itemType; }
    const std::string& getItemId() const { return itemId; }
    const std::string& getItemNo() const { return itemNo; }
    const std::vector<Element>& getElements() const { return elements; }
    const std::string& getExAlarm() const { return exAlarm; }
    const std::string& getFieldId() const { return fieldId; }
    const std::string& getFieldAlias() const { return fieldAlias; }
    const std::string& getFieldChineseName() const { return fieldChineseName; }
    const std::string& getFieldKey() const { return fieldKey; }
    const std::string& getFieldName() const { return fieldName; }
    const std::string& getFieldType() const { return fieldType; }
    const std::string& getFieldChannel() const { return fieldChannel; }
    const std::string& getHighAlarm() const { return highAlarm; }
    const std::string& getHighHighAlarm() const { return highHighAlarm; }
    const std::string& getLowAlarm() const { return lowAlarm; }
    const std::string& getLowLowAlarm() const { return lowLowAlarm; }
    double getRangeMin() const { return rangeMin; }
    double getRangeMax() const { return rangeMax; }
    int getSplitNumber() const { return splitNumber; }
    const std::string& getUnit() const { return unit; }
    
    // Setters
    void setItemType(const std::string& type) { itemType = type; }
    void setItemId(const std::string& id) { itemId = id; }
    void setItemNo(const std::string& no) { itemNo = no; }
    void setElements(const std::vector<Element>& elems) { elements = elems; }
    void setExAlarm(const std::string& alarm) { exAlarm = alarm; }
    void setFieldId(const std::string& id) { fieldId = id; }
    void setFieldAlias(const std::string& alias) { fieldAlias = alias; }
    void setFieldChineseName(const std::string& name) { fieldChineseName = name; }
    void setFieldKey(const std::string& key) { fieldKey = key; }
    void setFieldName(const std::string& name) { fieldName = name; }
    void setFieldType(const std::string& type) { fieldType = type; }
    void setFieldChannel(const std::string& channel) { fieldChannel = channel; }
    void setHighAlarm(const std::string& alarm) { highAlarm = alarm; }
    void setHighHighAlarm(const std::string& alarm) { highHighAlarm = alarm; }
    void setLowAlarm(const std::string& alarm) { lowAlarm = alarm; }
    void setLowLowAlarm(const std::string& alarm) { lowLowAlarm = alarm; }
    void setRangeMin(double min) { rangeMin = min; }
    void setRangeMax(double max) { rangeMax = max; }
    void setSplitNumber(int num) { splitNumber = num; }
    void setUnit(const std::string& u) { unit = u; }
    
    // 添加元素
    void addElement(const Element& element) { elements.push_back(element); }
    void addElement(int value, const std::string& label) { 
        elements.emplace_back(value, label); 
    }

private:
    std::string itemType;           // "Analog" 或 "Switch"
    std::string itemId;             // 项目ID
    std::string itemNo;             // 项目编号
    std::vector<Element> elements;  // 开关类型的元素列表
    std::string exAlarm;            // 外部报警
    std::string fieldId;            // 设备字段ID，后续上报的字段ID，****
    std::string fieldAlias;         // 字段别名
    std::string fieldChineseName;   // 字段中文名
    std::string fieldKey;           // 字段键
    std::string fieldName;          // 字段名
    std::string fieldType;          // 字段类型 "DOUBLE", "INT"
    std::string fieldChannel;       // 字段通道
    std::string highAlarm;          // 高报警
    std::string highHighAlarm;      // 高高报警
    std::string lowAlarm;           // 低报警
    std::string lowLowAlarm;        // 低低报警
    double rangeMin = 0.0;          // 范围最小值
    double rangeMax = 200.0;        // 范围最大值
    int splitNumber = 0;            // 分割数
    std::string unit;               // 单位
};

// 设备信息类
class DeviceInfo {
public:
    DeviceInfo() = default;
    
    // Getters
    int getDeviceId() const { return deviceId; }
    int getDeviceGroupId() const { return deviceGroupId; }
    const std::string& getProductCode() const { return productCode; }
    const std::string& getDeviceCode() const { return deviceCode; }
    const std::string& getDeviceName() const { return deviceName; }
    const std::string& getDeviceAlias() const { return deviceAlias; }
    const std::string& getDeviceChineseName() const { return deviceChineseName; }
    int getPriority() const { return priority; }
    const std::vector<FieldInfo>& getFieldInfoList() const { return fieldInfoList; }
    
    // Setters
    void setDeviceId(int id) { deviceId = id; }
    void setDeviceGroupId(int groupId) { deviceGroupId = groupId; }
    void setProductCode(const std::string& code) { productCode = code; }
    void setDeviceCode(const std::string& code) { deviceCode = code; }
    void setDeviceName(const std::string& name) { deviceName = name; }
    void setDeviceAlias(const std::string& alias) { deviceAlias = alias; }
    void setDeviceChineseName(const std::string& name) { deviceChineseName = name; }
    void setPriority(int pri) { priority = pri; }
    void setFieldInfoList(const std::vector<FieldInfo>& fields) { fieldInfoList = fields; }
    
    // 添加字段信息
    void addFieldInfo(const FieldInfo& field) { fieldInfoList.push_back(field); }
    
    // 根据字段键查找字段
    const FieldInfo* findFieldByKey(const std::string& key) const {
        for (const auto& field : fieldInfoList) {
            if (field.getFieldKey() == key) {
                return &field;
            }
        }
        return nullptr;
    }

private:
    int deviceId = 0;                           // 设备ID
    int deviceGroupId = 0;                      // 设备组ID
    std::string productCode;                    // 产品代码
    std::string deviceCode;                     // 设备代码
    std::string deviceName;                     // 设备名称
    std::string deviceAlias;                    // 设备别名
    std::string deviceChineseName;              // 设备中文名称
    int priority = 0;                           // 优先级
    std::vector<FieldInfo> fieldInfoList;      // 字段信息列表
};

// 设备组类
class DeviceGroup {
public:
    DeviceGroup() = default;
    
    // Getters
    const std::string& getGroupAlias() const { return groupAlias; }
    const std::string& getGroupId() const { return groupId; }
    const std::string& getGroupNameEn() const { return groupNameEn; }
    const std::string& getGroupNameZh() const { return groupNameZh; }
    int getPriority() const { return priority; }
    
    // Setters
    void setGroupAlias(const std::string& alias) { groupAlias = alias; }
    void setGroupId(const std::string& id) { groupId = id; }
    void setGroupNameEn(const std::string& name) { groupNameEn = name; }
    void setGroupNameZh(const std::string& name) { groupNameZh = name; }
    void setPriority(int pri) { priority = pri; }

private:
    std::string groupAlias;     // 组别名
    std::string groupId;        // 组IDReadWriteLock*
    std::string groupNameEn;    // 英文组名
    std::string groupNameZh;    // 中文组名
    int priority = 0;           // 优先级
};

// 主数据容器类
class DeviceDataContainer {
public:
    DeviceDataContainer() = default;
    
    // 拷贝构造函数 - 使用移动语义优化
    DeviceDataContainer(const DeviceDataContainer& other) 
        : deviceGroupList(other.deviceGroupList), 
          deviceInfoList(other.deviceInfoList) 
    
    {}
    
    // 移动构造函数
    DeviceDataContainer(DeviceDataContainer&& other) noexcept
        : deviceGroupList(std::move(other.deviceGroupList)),
          deviceInfoList(std::move(other.deviceInfoList)) {}
    
    // 拷贝赋值运算符
    DeviceDataContainer& operator=(const DeviceDataContainer& other) {
        if (this != &other) {
            deviceGroupList = other.deviceGroupList;
            deviceInfoList = other.deviceInfoList;
        }
        return *this;
    }
    
    // 移动赋值运算符
    DeviceDataContainer& operator=(DeviceDataContainer&& other) noexcept {
        if (this != &other) {
            deviceGroupList = std::move(other.deviceGroupList);
            deviceInfoList = std::move(other.deviceInfoList);
        }
        return *this;
    }
    
    // 快速深拷贝 - 预分配内存
    //从json生成的临时变量拷贝, 原有临时变量清空
    void lockDstFastcopyfrom(DeviceDataContainer& other) {
        // 预分配内存以减少重新分配
        std::string groupstr("devicegrouplist");
        {
            auto locker = lockManager.createGroupWriteLock(groupstr);
            deviceGroupList.swap(other.deviceGroupList);
        }
        

        deviceInfoList.clear();
        deviceInfoList.reserve(other.deviceInfoList.size());
        for(auto& device : other.deviceInfoList) 
        {
             auto locker = lockManager.createDeviceWriteLock(device.getDeviceId());
            deviceInfoList.emplace_back(device);
        }       
    }


    // //拷贝到无需保护的临时变量
    void lockSourceFastCopyTo(DeviceDataContainer &dsc)
    {
        dsc.deviceGroupList.clear();
        dsc.deviceGroupList.reserve(deviceGroupList.size());
        std::string groupStr("deviceGroupList");
        {
            auto locker = lockManager.createGroupReadLock(groupStr);
            dsc.deviceGroupList = deviceGroupList;
        }

        for (auto &device : deviceInfoList)
        {
            auto locker = lockManager.createDeviceReadLock(device.getDeviceId());
            dsc.deviceInfoList.push_back(device);
        }
    }

    // 选择性拷贝 - 只拷贝需要的部分
    void copyGroupsOnly(const DeviceDataContainer& other) {
        deviceGroupList.clear();
        deviceGroupList.reserve(other.deviceGroupList.size());
        deviceGroupList = other.deviceGroupList;
    }
    
    void copyDevicesOnly(const DeviceDataContainer& other) {
        deviceInfoList.clear();
        deviceInfoList.reserve(other.deviceInfoList.size());
        deviceInfoList = other.deviceInfoList;
    }
    
    // 增量拷贝 - 只拷贝变化的部分
    void incrementalCopy(const DeviceDataContainer& other, 
                        const std::vector<std::string>& changedGroupIds,
                        const std::vector<int>& changedDeviceIds) {
        // 更新变化的组
        for (const auto& groupId : changedGroupIds) {
            const auto* newGroup = other.findGroupById(groupId);
            if (newGroup) {
                // 查找现有组并更新或添加
                bool found = false;
                for (auto& group : deviceGroupList) {
                    if (group.getGroupId() == groupId) {
                        group = *newGroup;
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    deviceGroupList.push_back(*newGroup);
                }
            }
        }
        
        // 更新变化的设备
        for (const auto& deviceId : changedDeviceIds) {
            const auto* newDevice = other.findDeviceById(deviceId);
            if (newDevice) {
                bool found = false;
                for (auto& device : deviceInfoList) {
                    if (device.getDeviceId() == deviceId) {
                        device = *newDevice;
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    deviceInfoList.push_back(*newDevice);
                }
            }
        }
    }
    
    // 获取容器统计信息
    struct ContainerStats {
        size_t groupCount = 0;
        size_t deviceCount = 0;
        size_t totalFieldCount = 0;
        size_t estimatedMemorySize = 0;
    };
    
    ContainerStats getStats() const {
        ContainerStats stats;
        stats.groupCount = deviceGroupList.size();
        stats.deviceCount = deviceInfoList.size();
        
        for (const auto& device : deviceInfoList) {
            stats.totalFieldCount += device.getFieldInfoList().size();
        }
        
        // 估算内存使用
        stats.estimatedMemorySize = sizeof(DeviceDataContainer) +
                                   stats.groupCount * sizeof(DeviceGroup) +
                                   stats.deviceCount * sizeof(DeviceInfo) +
                                   stats.totalFieldCount * sizeof(FieldInfo);
        
        return stats;
    }
    
    // 清空容器并释放内存
    void clearAndShrink() {
        deviceGroupList.clear();
        deviceGroupList.shrink_to_fit();
        deviceInfoList.clear();
        deviceInfoList.shrink_to_fit();
    }
    
    // 预分配内存
    void reserve(size_t groupCount, size_t deviceCount) {
        deviceGroupList.reserve(groupCount);
        deviceInfoList.reserve(deviceCount);
    }
    
    // Getters
    const std::vector<DeviceGroup>& getDeviceGroupList() const { return deviceGroupList; }
    const std::vector<DeviceInfo>& getDeviceInfoList() const { return deviceInfoList; }
    
    // Setters
    void setDeviceGroupList(const std::vector<DeviceGroup>& groups) { deviceGroupList = groups; }
    void setDeviceInfoList(const std::vector<DeviceInfo>& devices) { deviceInfoList = devices; }
    
    // 添加方法
    void addDeviceGroup(const DeviceGroup& group) { deviceGroupList.push_back(group); }
    void addDeviceInfo(const DeviceInfo& device) { deviceInfoList.push_back(device); }
    
    // 查找方法
    const DeviceGroup* findGroupById(const std::string& groupId) const {
        for (const auto& group : deviceGroupList) {
            if (group.getGroupId() == groupId) {
                return &group;
            }
        }
        return nullptr;
    }
    
    const DeviceInfo* findDeviceById(int deviceId) const {
        for (const auto& device : deviceInfoList) {
            if (device.getDeviceId() == deviceId) {
                return &device;
            }
        }
        return nullptr;
    }
    
    std::vector<DeviceInfo> getDevicesByGroupId(int groupId) const {
        std::vector<DeviceInfo> result;
        for (const auto& device : deviceInfoList) {
            if (device.getDeviceGroupId() == groupId) {
                result.push_back(device);
            }
        }
        return result;
    }

private:
    std::vector<DeviceGroup> deviceGroupList;   // 设备组列表
    std::vector<DeviceInfo> deviceInfoList;     // 设备信息列表

    mutable DeviceDataLockManager lockManager;  // 锁管理器，用于线程安全操作
};

// 类型别名
using DeviceGroupPtr = std::shared_ptr<DeviceGroup>;
using DeviceInfoPtr = std::shared_ptr<DeviceInfo>;
using FieldInfoPtr = std::shared_ptr<FieldInfo>;
using DeviceDataContainerPtr = std::shared_ptr<DeviceDataContainer>;

class DeviceDataManager 
{
public:
    DeviceDataManager() = default;
    ~DeviceDataManager() = default;
    
    // 禁用拷贝构造和赋值（单例模式）
    DeviceDataManager(const DeviceDataManager&) = delete;
    DeviceDataManager& operator=(const DeviceDataManager&) = delete;

    // 静态实例获取方法（单例模式）
    static DeviceDataManager& Instance() {
        return SingletonEx<DeviceDataManager>::Instance();
    }

    // 数据加载和保存
    bool loadFromJsonString(const std::string& jsonContent);
    std::string toJsonString() const;

    // 事件通知接口
    class IDataChangeListener {
    public:
        virtual ~IDataChangeListener() = default;
        virtual void onDeviceAdded(const DeviceInfo& device) = 0;
        virtual void onDeviceUpdated(const DeviceInfo& device) = 0;
        virtual void onDeviceRemoved(int deviceId) = 0;
        virtual void onGroupAdded(const DeviceGroup& group) = 0;
        virtual void onGroupUpdated(const DeviceGroup& group) = 0;
        virtual void onGroupRemoved(const std::string& groupId) = 0;
        virtual void onFieldUpdated(int deviceId, const FieldInfo& field) = 0;  //TBD
    };
    
    void UpdateMyContainer(DeviceDataContainer& container) {
        m_deviceData.lockDstFastcopyfrom(container);
    }

    void SaveToContainer(DeviceDataContainer& container) {
        DeviceDataContainer tmp;
        m_deviceData.lockSourceFastCopyTo(tmp);
        container.lockDstFastcopyfrom(tmp);
    }



private:
    DeviceDataContainer m_deviceData;
};

//Config更新
#define DeviceUpdateConfig(a)   (DeviceDataManager::Instance().UpdateMyContainer(a))

//从Config获取配置
#define GetDeviceForInernal(a)  (DeviceDataManager::Instance().SaveToContainer(a))

} // namespace device_data


