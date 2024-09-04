#ifndef SHIP_H
#define SHIP_H

#include <vector>
#include <utility>

class Ship
{
public:
    int size;
    std::vector<std::pair<int, int>> positions;
    int hits;

    Ship(int size_value);
    bool check_size() const;
    void set_position(const std::vector<std::pair<int, int>> &pos);
    bool occupies_position(std::pair<int, int> target) const;
    bool is_sunk() const;
};

#endif // SHIP_H
