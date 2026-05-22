#include "pch.h"
#include "Vector2.h"
#include <iostream>

namespace engine {

int main() {
    std::cout << "=== Vector2 Test ===\n" << std::endl;
    
    // Test 1: Constructor
    Vector2 v1;
    std::cout << "Test 1 - Constructor: " 
              << ((v1.x == 0 && v1.y == 0) ? "PASSED" : "FAILED") << std::endl;
    
    Vector2 v2(3.5f, 4.5f);
    std::cout << "  v2 = (" << v2.x << ", " << v2.y << ")" << std::endl;
    
    // Test 2: Addition
    Vector2 a(2.5f, 3.5f);
    Vector2 b(1.5f, 2.5f);
    Vector2 c = a + b;
    std::cout << "\nTest 2 - Addition: " 
              << ((c.x == 4.0f && c.y == 6.0f) ? "PASSED" : "FAILED") << std::endl;
    std::cout << "  " << a.x << "+" << b.x << "=" << c.x << std::endl;
    
    // Test 3: Subtraction
    Vector2 d = a - b;
    std::cout << "Test 3 - Subtraction: " 
              << ((d.x == 1.0f && d.y == 1.0f) ? "PASSED" : "FAILED") << std::endl;
    
    // Test 4: Multiplication
    Vector2 e = a * 2.0f;
    std::cout << "Test 4 - Multiplication: " 
              << ((e.x == 5.0f && e.y == 7.0f) ? "PASSED" : "FAILED") << std::endl;
    
    // Test 5: Division
    Vector2 f(6.0f, 8.0f);
    Vector2 g = f / 2.0f;
    std::cout << "Test 5 - Division: " 
              << ((g.x == 3.0f && g.y == 4.0f) ? "PASSED" : "FAILED") << std::endl;
    
    // Test 6: Length
    Vector2 len(3.0f, 4.0f);
    float length = len.Length();
    std::cout << "Test 6 - Length: " 
              << (length == 5.0f ? "PASSED" : "FAILED") 
              << " (length = " << length << ")" << std::endl;
    
    // Test 7: Dot Product
    Vector2 dot1(1.0f, 2.0f);
    Vector2 dot2(3.0f, 4.0f);
    float dot = dot1.Dot(dot2);
    std::cout << "Test 7 - Dot Product: " 
              << (dot == 11.0f ? "PASSED" : "FAILED")
              << " (dot = " << dot << ")" << std::endl;
    
    // Test 8: Normalize
    Vector2 norm(3.0f, 4.0f);
    Vector2 normalized = norm.Normalized();
    std::cout << "Test 8 - Normalize: " 
              << ((normalized.x == 0.6f && normalized.y == 0.8f) ? "PASSED" : "FAILED")
              << " (" << normalized.x << ", " << normalized.y << ")" << std::endl;
    
    // Test 9: Distance
    Vector2 from(0.0f, 0.0f);
    Vector2 to(3.0f, 4.0f);
    float dist = from.Distance(to);
    std::cout << "Test 9 - Distance: " 
              << (dist == 5.0f ? "PASSED" : "FAILED")
              << " (distance = " << dist << ")" << std::endl;
    
    // Test 10: Static constants
    std::cout << "\nTest 10 - Static Constants:" << std::endl;
    std::cout << "  Zero: (" << Vector2::Zero.x << ", " << Vector2::Zero.y << ")" << std::endl;
    std::cout << "  One: (" << Vector2::One.x << ", " << Vector2::One.y << ")" << std::endl;
    std::cout << "  Up: (" << Vector2::Up.x << ", " << Vector2::Up.y << ")" << std::endl;
    std::cout << "  Down: (" << Vector2::Down.x << ", " << Vector2::Down.y << ")" << std::endl;
    std::cout << "  Left: (" << Vector2::Left.x << ", " << Vector2::Left.y << ")" << std::endl;
    std::cout << "  Right: (" << Vector2::Right.x << ", " << Vector2::Right.y << ")" << std::endl;
    
    std::cout << "\n=== ALL TESTS COMPLETED ===" << std::endl;
    
    return 0;
}
}
