#pragma once
#include "utils/SingletonEx.h"
#include "utils/Noncopyable.h"
#include "utils/timeUtil.h"
#include "utils/CallbackQueue.h"
#include "DataTree/modbus.h"
#include "DataTree/DeviceData.h"
#include <memory>
#include <vector>
#include <map>
#include <thread>
#include <list>
#include <set>

using namespace util;

// ip, port
using IP_PORT_PAIR = std::pair<std::string, int>;

// unique key, item name
using ITEM_ID = std::pair<std::string, std::string>;

class ModbusTcpTree;

class INode : Noncopyable
{
public:
    virtual ~INode() = 0;

    bool IsUpdate() const
    {
        return m_isUpdate;
    }

    // 数据更新接口
    virtual void OnDataUpdate(const ITEM_ID &itemId, uint16_t value) = 0;

    // 数据添加接口
    virtual void OnDataAdd(const ITEM_ID &itemId) = 0;

    // 数据删除接口
    virtual void OnDataDelete(const ITEM_ID &itemId) = 0;

    // 数据错误处理接口
    virtual void OnDataError(const ITEM_ID &itemId, const std::string &error) = 0;

private:
    bool m_isUpdate = false;
};

class ModbusItem : public INode
{
public:
    ModbusItem()
    {
        // 构造函数实现
    }

    ~ModbusItem()
    {
        // 析构函数实现
    }

    void OnDataDelete()
    {
    }

    void OnDataUpdate()
    {
    }

    void OnDataAdd()
    {
    }

    void OnDataError()
    {
    }

    bool ItemContentChanged(const ModbusItem &other)
    {
        if (m_itemId != other.m_itemId)
        {
            return true;
        }

        if (m_itemKey != other.m_itemKey)
        {
            return true;
        }

        if (m_ipPortPair != other.m_ipPortPair)
        {
            return true;
        }

        if (m_groupName != other.m_groupName)
        {
            return true;
        }

        if (m_plcAddress != other.m_plcAddress)
        {
            return true;
        }

        if (m_startBit != other.m_startBit)
        {
            return true;
        }

        if (m_endBit != other.m_endBit)
        {
            return true;
        }

        return false;
    }

    void SetPending()
    {
        m_bPending = true;
    }

    void SetDelete()
    {
        m_deleteFlag = true;
    }

    void SetNormal()
    {
        m_bPending = false;
    }

    void SetHold()
    {
        m_deleteFlag = false;
    }

    bool NeedDelete()
    {
        return (!m_bPending) && m_deleteFlag;
    }

private:
    // unique key, item name
    ITEM_ID m_itemId = std::make_pair("", "");

    // item key
    std::string m_itemKey = "";

    // 读取的IP port
    IP_PORT_PAIR m_ipPortPair = std::make_pair("", 0);

    std::string m_groupName = "";

    // 读取的值
    uint16_t m_value = 0;

    // plc读取地址
    uint32_t m_plcAddress = 0;

    // 起始位
    uint16_t m_startBit = 0xffff;

    // 截止位
    uint16_t m_endBit = 0xffff;

    bool m_bPending = false;
    bool m_deleteFlag = false;
};

using ModbusItemPtr = std::unique_ptr<ModbusItem>;

// 设备数据组包
class ModbusItemsGroup
{
public:
    ModbusItemsGroup(std::string groupName)
        : m_groupName(groupName)
    {
        // 构造函数实现
    }

    void init()
    {
        m_runnnigFlag = true;
        m_thread = std::thread(&ModbusItemsGroup::Run, this);
    }

    ~ModbusItemsGroup()
    {
        // 析构函数实现
        m_runnnigFlag = false;
        m_thread.join();
    }

    void SetPending()
    {
        m_bPending = true;
    }

    void SetNormal()
    {
        m_bPending = false;
    }

    void SetHold()
    {
        m_deleteFlag = false;
    }

    void SetDelete()
    {
        m_deleteFlag = true;
    }

    bool NeedDelete()
    {
        return (!m_bPending) && m_deleteFlag;
    }

    // 组包上传最新的数据
    void Run()
    {
    }

    void send2Mqtt()
    {
    }

private:
    std::string m_groupName;

    // 设备组数据获取线程
    std::thread m_thread;
    bool m_runnnigFlag = false;

    bool m_bPending = false;
    bool m_deleteFlag = false;
};

using ModbusItemsGroupPtr = std::unique_ptr<ModbusItemsGroup>;

class IDataHandler
{
public:
    virtual ~IDataHandler() = 0;

    virtual bool Init() = 0;

    virtual void Run() = 0;
};

//using ModbusTcpDataPtr = std::unique_ptr<std::vector<uint16_t>>;
using ModbusTcpData = std::vector<uint16_t>;
// 读取modbus数据的处理类
class ModbusTcpHandle : public IDataHandler
{
public:
    ModbusTcpHandle(IP_PORT_PAIR ipPort) : m_ipPortPair(ipPort),
                                           m_modbus(ipPort.first, ipPort.second),
                                           m_nextTick(getTickCount64()),
                                           m_callbackQueue(1024)
                                           
    {
        // 构造函数实现
    }

    ~ModbusTcpHandle()
    {
         // 析构函数实现
        m_runnnigFlag = false; 
        m_callbackQueue.stop();
        if(m_thread.joinable())
        {
            m_thread.join();
        }
    }

    bool init()
    {
        m_runnnigFlag = true;
        m_thread = std::thread(&ModbusTcpHandle::run, this);

        //DelayCallReadModbusData(100);
        return true;
    }

    //时间管理
    void run()
    {
        // 运行实现
        while(m_runnnigFlag)
        {
            //TBD
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        return;
    }

private:
    // void GetDataLenMax(std::map<uint16_t, std::tuple<uint32_t, uint16_t, uint64_t>> &refAddrList)
    // {
    //     //uint16_t index = refAddrList.rbegin()->first;
        
    // }

//立即处理任务
//  void CallReadModbusData()
//     {
//         m_callbackQueue.call(0, 0, [this]()
//         {
//             ReadModbusDataLoop();
//         });  
//     }
     

    // void DelayCallReadModbusData(uint32_t milsec)
    // {
    //     m_callbackQueue.call(0, 0, [milsec, this]()
    //     {
    //         m_nextTick = getTickCount64() + milsec;
    //         ReadModbusDataLoop();
    //     });  
    // }

    //自激式读取modbus数据
    void ReadModbusDataLoop()
    {
        // if(!m_runnnigFlag)
        // {
        //     return;
        // }

        // uint64_t tick = getTickCount64();
        // if(tick < m_nextTick)
        // {
        //     std::this_thread::sleep_for(std::chrono::milliseconds(10));
        //     CallReadModbusData();
        //     return;
        // }

        // bool readFlag = false;
        // if (!m_modbus.is_connected())
        // {
        //     if (m_modbus.modbus_connect())
        //     {
        //         // TBD: log
        //     }
        //     else // fail
        //     {
        //         //端口异常需要等待
        //         DelayCallReadModbusData(3000);
        //         return;
        //     }
        // }
        // else
        // {
        //     // do nothing
        // }

        // //GetDataLenMax(m_u32DataMap);
        // // for (auto iter = m_u32DataMap.begin(); iter != m_u32DataMap.end(); ++iter)
        // // {
        // //     uint16_t& index = iter->first;
        // //     uint32_t &addr = std::get<0>(iter->second);
        // //     uint16_t &len = std::get<1>(iter->second);
        // //     uint64_t &oldTickTime = std::get<2>(iter->second);
        // //     tick = getTickCount64();
        // //     if (static_cast<uint32_t>(iter->first + 1 + len) <= m_u32DataVec.size())
        // //     {
        // //         if ((tick >= (oldTickTime + m_constMaxReadMillSec)))
        // //         {
        // //             oldTickTime = tick;
        // //             auto ret = m_modbus.modbus_read_holding_registers(addr, len, const_cast<uint16_t*>(m_u32DataVec.data()) + index);
        // //             if (0 != ret) // fail
        // //             {
        // //                 m_modbus.modbus_close();
        // //                 DelayCallReadModbusData(100);
        // //                 return;
        // //             }
        // //             else  //ok
        // //             {
        // //                 readFlag = true;
        // //             }
        // //         }
        // //         else
        // //         {
        // //             // do nothing
        // //         }
        // //     }
        // //     else
        // //     {
        // //         // TBD: log
        // //     }
        // // }

        // if(!readFlag)
        // {
        //     DelayCallReadModbusData(100);
        // }
    }

    
    std::thread m_thread;
    //
    IP_PORT_PAIR m_ipPortPair;
 
    // 
   
    // 数据读取索引信息, <index, <addr, len, tick>>
    std::map<uint16_t, std::tuple<uint32_t, uint16_t, uint64_t>> m_u32DataMap;

    // modbus相关
    modbus m_modbus;
    bool m_runnnigFlag = false;
    const uint16_t m_constMaxReadMillSec = 100;
    //const uint16_t m_constIntervalReadMillSec = 50;
    //uint64_t m_preTick = 0;
    uint64_t m_nextTick = 0;

    // 通过单一队列实现无锁
    CallbackQueue m_callbackQueue;
};

using ModbusTcpHandlePtr = std::shared_ptr<ModbusTcpHandle>;

class ModbusTcpTree
{
public:
    ModbusTcpTree() : m_callbackQueue(1021)
    {
        // 构造函数实现
    }

    ~ModbusTcpTree()
    {
        // 析构函数实现
    }

    //
     

private:
    // 私有成员

    // 所有点位
    std::map<ITEM_ID, ModbusItemPtr> m_itemMap;
    // 设备组
    std::map<std::string, ModbusItemsGroupPtr> m_groupMap;
    // 所有modbus tcp处理
    std::map<IP_PORT_PAIR, ModbusTcpHandlePtr> m_ModbusHandleMap;

    // modbus相关
    modbus m_modbus;

    //callback队列
    CallbackQueue m_callbackQueue;

    // 数据保持， 使用定
    const uint16_t m_dataLen = 0;

    //ModbusTcpDataPtr m_u32DataPtr = nullptr;
    const std::vector<uint16_t> m_u32DataVec;
};


#define ModbusTcpTreeInstance (SingletonEx<ModbusTcpTree>::instance())

