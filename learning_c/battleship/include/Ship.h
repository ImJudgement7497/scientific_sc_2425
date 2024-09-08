#ifndef SHIP_H
#define SHIP_H

#include <iostream>
#include <vector>
#include <utility>
#include <unordered_map>
#include <string>

class Ship
{
public:
    int size;
    std::vector<std::pair<int, int>> positions;
    int hits;
    std::string name;

    Ship(int size_value);
    bool check_size() const;
    void set_position(const std::vector<std::pair<int, int>> &pos);
    bool occupies_position(std::pair<int, int> target) const;
    void set_name(std::string &ship_name);
    bool is_sunk() const;
};

#endif // SHIP_H
