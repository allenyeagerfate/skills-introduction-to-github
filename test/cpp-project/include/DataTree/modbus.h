/*
*sangoai
*
*
*/
#ifndef MODBUS_H
#define MODBUS_H

#include <cstring>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

using X_SOCKET = int;
using SOCKADDR = struct sockaddr;
using SOCKADDR_IN = struct sockaddr_in;


/**
 * Modbus Operator Class
 * Providing networking support and mobus operation support.
 */
class modbus
{
public:
    modbus(const string& host = "192.168.1.101", uint16_t port = 502);
    virtual ~modbus();

    bool err{};
    int err_no{};
    string error_msg;

    bool modbus_connect();
    void modbus_close();

    bool modbus_listen(uint16_t port);
    bool accept_connections();

    bool is_connected() const { return _connected; }

    void modbus_set_slave_id(int id);

    int modbus_read_coils(uint16_t address, uint16_t amount, bool *buffer);
    int modbus_read_input_bits(uint16_t address, uint16_t amount, uint8_t *buffer);
    int modbus_read_holding_registers(uint16_t address, uint16_t amount, uint16_t *buffer);
    int modbus_read_input_registers(uint16_t address, uint16_t amount, uint16_t *buffer);

    int modbus_write_coil(uint16_t address, const bool &to_write);
    int modbus_write_register(uint16_t address, const uint16_t &value);
    int modbus_write_coils(uint16_t address, uint16_t amount, const bool *value);
    int modbus_write_registers(uint16_t address, uint16_t amount, const uint16_t *value);


	int get_J1939_1_status();
	int get_J1939_2_status();
	vector<int> &getVJ1939_1(){return m_vJ1939_1;}
	vector<int> &getVJ1939_2(){return m_vJ1939_2;}


	int get_J1939_1_warn();
	vector<uint8_t> &getVJ1939_warn(){return m_vJ1939_warn;}


private:
    bool _connected{false};
    uint16_t PORT{};
    uint32_t _msg_id{};
    int _slaveid{};
    string HOST;
	vector<int> m_vJ1939_1;
	vector<int> m_vJ1939_2;

	vector<uint8_t> m_vJ1939_warn;

	//ofstream m_outfile;


    X_SOCKET _socket{};
    SOCKADDR_IN _server{};

    int m_client_socket = -1;
    bool client_connected = false;
    int server_socket = -1;

    void modbus_build_request(uint8_t *to_send, uint16_t address, int func) const;

    int modbus_read(uint16_t address, uint16_t amount, int func);
    int modbus_write(uint16_t address, uint16_t amount, int func, const uint16_t *value);

    ssize_t modbus_send(uint8_t *to_send, size_t length);
    ssize_t modbus_receive(uint8_t *buffer) const;

    void modbuserror_handle(const uint8_t *msg, int func);

    void set_bad_con();
    void set_bad_input();


	
};

#endif
