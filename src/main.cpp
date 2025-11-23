#include <iostream>
#include <roll_tables.hpp>
#include <stdexcept>

RollTable parse_roll_table_from_file(const std::string& path);

int main() {
    std::cout << "Hello world" << std::endl;

    try {
        parse_roll_table_from_file("data/scene_adjustment.md");
    }
    catch(const std::runtime_error err){
        std::cout << err.what() << std::endl;
    }

    return 0;
}