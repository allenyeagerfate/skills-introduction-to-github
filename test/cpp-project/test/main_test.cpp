#include <iostream>
#include <cassert>

// 前置声明测试函数
void test_device_data();
void test_utils();

int main() {
    std::cout << "=== Running Unit Tests ===" << std::endl;
    
    try {
        std::cout << "Testing device data..." << std::endl;
        test_device_data();
        std::cout << "✓ Device data tests passed" << std::endl;
        
        std::cout << "Testing utils..." << std::endl;
        test_utils();
        std::cout << "✓ Utils tests passed" << std::endl;
        
        std::cout << "All tests passed!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
}
