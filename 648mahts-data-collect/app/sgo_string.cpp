#include "sgo_func.h"
#include <math.h>
#include <sstream>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <memory>

using namespace std;

static float f_sw1ps[] = { 0.0, 3.4, 7.0, 10.8, 14.8, 18.9, 23.1, 27.5, 32.1, 36.7,
						41.6, 46.6, 51.9, 57.3, 62.8, 68.4, 74.2, 80.1, 86.2, 92.8,
                   		99.6, 106.6, 113.7, 121.0, 128.5, 136.2, 144.0, 152.0, 160.1, 168.2,
                   		176.4, 184.8, 193.4, 202.2, 211.2, 220.3, 229.5, 238.7, 248.0, 257.2,
                  		266.4, 275.6, 284.8, 294.1, 303.3, 312.5, 321.8, 331.0, 340.2, 349.5,
                   		358.7, 367.9, 377.2, 386.4, 395.6, 404.9, 414.1, 423.3, 432.6, 441.8,
                  	 	451.0, 460.3};

static float f_sw2ps[] = {0.0,17.6,35.2,52.7,70.3,87.9,105.5,123.1,140.6,158.2,
						175.8,193.4,211.0,228.6,246.1,263.7};

static float f_sw3ps[] = {0.0,0.0,0.2,0.4,0.8,1.3,1.8,2.5,3.3,4.1,
						5.1,6.2,7.4,8.7,10.1,12.0,14.0,16.2,18.4,20.8,
						23.3,25.9,28.5,31.4,34.3,37.4,40.8,46.5,52.8,59.2,
						65.6,72.1,78.6,85.1,91.7,98.3,104.9,111.5,118.0,124.6,
						131.2,137.8,144.4,151.0,157.6,164.1,170.7,177.3,183.9,190.5,
						197.1,203.7,210.2,216.8,223.4,230.0,236.6,243.2,244.3};

static float f_fwps[] = {0.0,7.7,15.7,23.9,32.2,40.6,49.1,57.6,66.2,74.9,
						83.5,92.1,100.8,109.4,118.1,126.7,135.4,144.0,152.7,161.3,169.9,
						178.6,187.2,195.9,204.5,213.2,221.8,230.4,239.1,247.7,256.4};

static float f_fops[] = {0.0,6.6,13.5,20.5,27.6,34.8,42.1,49.4,56.8,64.2,
						71.6,79.0,86.4,93.8,101.2,108.6,112.3,116.0,119.7,123.6,
						127.6,131.5,135.4,139.3,143.2,147.1,150.0,152.9,155.8,158.7,
						161.7,164.6,167.5,170.4,173.3,176.2,179.1,182.0,184.9,187.9,
						190.8,193.7,196.6,199.5,202.4,205.3};

static float f_fodtps[] = {0.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,
						10.0,11.0,12.0,13.0,14.0,15.0,16.0,17.0,18.0,19.0,20.0};



//截取字符串尾部空格
void Rtrim(char *string)   
{   
	size_t len = 0;   
	if(string == NULL)   
		return;   

	len = strlen(string);   
	while(len > 0 && string[len-1] == ' ')   //位置换一下   
		string[--len] = 0;   
	return;   
}

//截取字符串首部空格
void Ltrim(char *string)
{
	size_t len = 0;
	len = strlen(string);   
	char *p_tmp = string;
	if( (*p_tmp) != ' ') //不是以空格开头
		return;
	//找第一个不为空格的
	while((*p_tmp) != '\0')
	{
		if( (*p_tmp) == ' ')
			p_tmp++;
		else
			break;
	}
	if((*p_tmp) == '\0') //全是空格
	{
		*string = '\0';
		return;
	}
	char *p_tmp2 = string; 
	while((*p_tmp) != '\0')
	{
		(*p_tmp2) = (*p_tmp);
		p_tmp++;
		p_tmp2++;
	}
	(*p_tmp2) = '\0';
    return;
}


// 使用字符分割
void Stringsplit(const string& str, char split, vector<string>& res)
{
	if (str == "")		return;
	//在字符串末尾也加入分隔符，方便截取最后一段
	string strs = str + split;
	size_t pos = strs.find(split);
 
	// 若找不到内容则字符串搜索函数返回 npos
	while (pos != strs.npos)
	{
		string temp = strs.substr(0, pos);
		res.push_back(temp);
		//去掉已分割的字符串,在剩下的字符串中进行分割
		strs = strs.substr(pos + 1, strs.size());
		pos = strs.find(split);
	}
}

long curDatetime =  1703030400000;//20231220 8:00 


long getSystemTime() { //UTC

#if 1
    struct timeval tv;
    gettimeofday(&tv, NULL);
    
    return (long)tv.tv_sec * 1000 + (long)tv.tv_usec / 1000;
#else

	return curDatetime;
#endif
}


#if 0
// 使用字符串分割
void Stringsplit(const string& str, const string& splits, vector<string>& res)
{
	if (str == "")		return;
	//在字符串末尾也加入分隔符，方便截取最后一段
	string strs = str + splits;
	size_t pos = strs.find(splits);
	int step = splits.size();
 
	// 若找不到内容则字符串搜索函数返回 npos
	while (pos != strs.npos)
	{
		string temp = strs.substr(0, pos);
		res.push_back(temp);
		//去掉已分割的字符串,在剩下的字符串中进行分割
		strs = strs.substr(pos + step, strs.size());
		pos = strs.find(splits);
	}
}
#endif



float cur2float(float cur,int first,int last,int tempStart,int tempEnd)
{
	//return round((tempStart + (cur - first) * (tempEnd - tempStart) / (last - first))*10)/10;
	return tempStart + (cur - first) * (tempEnd - tempStart) / (last - first);
}

float cur2float_f(float cur,int first,int last,float tempStart,float tempEnd)
{
	//return round((tempStart + (cur - first) * (tempEnd - tempStart) / (last - first))*10)/10.0;
	return tempStart + (cur - first) * (tempEnd - tempStart) / (last - first);
}

int getCronSecond(const string& cronStr)
{
	stringstream ss(cronStr);
	string field;
	while(getline(ss, field, ' '))
	{
		size_t sp = field.find('/');
		if(sp != string::npos)
		{
			string str = field.substr(sp+1);
			return atoi(str.c_str());
        }
		else
			return 1;
	}
	return -1;
}



void parseGensProp(const char *sdata,vector<string>& vRes)
{
    //创建解析对象
    rapidjson::Document doc;
	vRes.clear();
    //首先进行解析，没有解析错误才能进行具体字段的解析
    if(!doc.Parse(sdata).HasParseError())
    {
       	//3. 解析字符串
        if(doc.HasMember("cron") && doc["cron"].IsString())
        {
			vRes.push_back(doc["cron"].GetString());
        }
        if(doc.HasMember("readType") && doc["readType"].IsString())
        {
			vRes.push_back(doc["readType"].GetString());
        }

        if(doc.HasMember("msgType") && doc["msgType"].IsString())
        {
			vRes.push_back(doc["msgType"].GetString());
        }


        if(doc.HasMember("msgId") && doc["msgId"].IsString())
        {
			vRes.push_back(doc["msgId"].GetString());
        }

        if(doc.HasMember("transferProtocol") && doc["transferProtocol"].IsString())
        {
			vRes.push_back(doc["transferProtocol"].GetString());
        }

        if(doc.HasMember("protocolType") && doc["protocolType"].IsString())
        {
			vRes.push_back(doc["protocolType"].GetString());
        }
        if(doc.HasMember("timestamp") && doc["timestamp"].IsInt64())
        {
			vRes.push_back(to_string(doc["timestamp"].GetInt64()));	
        }


        //4. 解析结构体
        if(doc.HasMember("headers") && doc["headers"].IsObject())
        {
            const rapidjson::Value& object = doc["headers"];
            if(object.HasMember("productId") && object["productId"].IsInt())
            {
				vRes.push_back(to_string(object["productId"].GetInt()));
            }
            if(object.HasMember("deviceCode") && object["deviceCode"].IsString())
            {
				vRes.push_back(object["deviceCode"].GetString());
            }
            if(object.HasMember("productCode") && object["productCode"].IsString())
            {
                vRes.push_back(object["productCode"].GetString());
            }
            if(object.HasMember("deviceId") && object["deviceId"].IsInt())
            {
				vRes.push_back(to_string(object["deviceId"].GetInt()));
            }
			
			if(object.HasMember("isInvokeByRule") && object["isInvokeByRule"].IsBool())
            {
				vRes.push_back(to_string(object["isInvokeByRule"].GetBool()));
            }
        }
        //5. 解析数组类型
        //5.1 整型数组类型
        if(doc.HasMember("properties") && doc["properties"].IsArray())
        {
            //5.1.1 将字段转换成为rapidjson::Value类型
            const rapidjson::Value& array = doc["properties"];
            //5.1.2 获取数组长度
            size_t len = array.Size();
            //5.1.3 根据下标遍历，注意将元素转换为相应类型，即需要调用GetInt()
            for(size_t i = 0; i < len; i++)
            {
				vRes.push_back(array[i].GetString());
            }
        }
 
    }
	else
		cout<<"json error"<<endl;

}

// 将度分格式的坐标转换为度
double convertDegrees(double degreesMinutes)
{
    double degrees = std::floor(degreesMinutes / 100.0);  // 提取度部分
    double minutes = degreesMinutes - (degrees * 100.0);  // 提取分部分
    double decimalDegrees = degrees + (minutes / 60.0);   // 转换为度
    return decimalDegrees;
}

//liquid heigh->volume
/*
typedef enum{
	SW1PS,
	SW2PS,
	SW3PS,
	FWPS,
	FOPS,
	FODTPS
}LIQUIDTYPE;
*/
float _calcVolum(float height,float *arr)
{
	if(height < 0)
		return 0;
	int xHeight = (int)height;
	return arr[xHeight/100] + (arr[xHeight/100 + 1] - arr[xHeight/100]) * (xHeight%100) /100.0;
}

float calcVolumByHeight(float height,LIQUIDTYPE type)
{
	switch (type){
		case SW1PS:
		{
			if(height > 6100)
				return 0;
			return _calcVolum(height,f_sw1ps);
		}
		case SW2PS:
		{
			if(height > 1500)
				return 0;		
			return _calcVolum(height,f_sw2ps);
		}
		case SW3PS:
		{
			if(height == 5720)
				return 244.3;
			if(height > 5720)
				return 0;
			return _calcVolum(height,f_sw3ps);
		}
		case FWPS:
		{
			if(height > 3000)
				return 0;		
			return _calcVolum(height,f_fwps);
		}
		case FOPS:
		{
			if(height > 4500)
				return 0;		
			return _calcVolum(height,f_fops);
		}
		case FODTPS:
		{
			if(height > 2000)
				return 0;
			return _calcVolum(height,f_fodtps);
		}

		default:
		break;
	}
	return 0;
}


bool isDouble(const string& str)
{
	std::istringstream iss(str);
	double num;
	if (iss >> num && iss.eof()) {
		  return true;
	}
	return false;
}

string getCurrentTime(char flag)
{
    time_t timep;
    time (&timep);
    char tmp[64];
	if(0 == flag )
		strftime(tmp, sizeof(tmp), "%Y-%m-%d-%H%M%S",localtime(&timep));
	else if(1 == flag)
    	strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S",localtime(&timep));
	else
		strftime(tmp, sizeof(tmp), "%Y%m%d",localtime(&timep));
    return tmp;
}

bool isFloat(const std::string& str) {
    try {
        size_t pos = 0;
        std::stof(str, &pos);
        return pos == str.size(); // 如果整个字符串都被转换，则为浮点数
    } catch (const std::invalid_argument&) {
        return false; // 转换失败，不是浮点数
    } catch (const std::out_of_range&) {
        return false; // 转换溢出，不是浮点数
    }
}

void parseRangeString(const string& rangeStr,vector<float>& result) 
{
    stringstream ss(rangeStr);
    string token;
    while (getline(ss, token, ',')) {
		string blanks("\f\v\r\t\n ");
		token.erase(0,token.find_first_not_of(blanks));
  		token.erase(token.find_last_not_of(blanks) + 1);
		if(isFloat(token))
			result.push_back(stof(token));
    }
}



string getHostIp(const char* ifname) 
{
    struct ifaddrs* ifaddr;
    struct ifaddrs* ifa;
    int family, s;
    char host[NI_MAXHOST] = {0};
	string strIp;
	
    if (getifaddrs(&ifaddr) == -1) {
        return strIp;
    }

    // 遍历网络接口
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL)
            continue;

        family = ifa->ifa_addr->sa_family;

        // 只处理 IPv4 地址
        if (family == AF_INET) {
            s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in),
                           host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
            if (s != 0) {
            	break;
            }

            // 检查是否为 eno1 接口
            if (strcmp(ifa->ifa_name,ifname) == 0) {
				break;
            }
        }
    }
    freeifaddrs(ifaddr);
    if(host[0] != '\0')
		strIp.append(host);
	return strIp;
}


bool isNetworkInterfaceUp(const string& interfaceName)
{
    // 创建命令字符串
    string command = "ip link show " + interfaceName;
    
    // 使用 popen 执行命令并获取输出
    array<char, 128> buffer;
    string result;
    
    // 使用 unique_ptr 和 popen
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
    
    if (!pipe) {
        cerr << "Failed to open pipe!" << endl;
        return false;
    }

    // 读取命令输出
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    // 检查输出是否包含 "state UP"
    return result.find("state UP") != string::npos;
}


