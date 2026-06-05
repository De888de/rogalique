#include <iostream>

// Проверка что инклюды работают
namespace engine {
    class Test {
    public:
        void hello() { std::cout << "Engine works!\n"; }
    };
}

int main() {
    engine::Test t;
    t.hello();
    return 0;
}
