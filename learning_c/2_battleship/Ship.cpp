#include "Ship.h"
#include <iostream>

Ship::Ship(int size_value) : size(size_value), hits(0) {}

bool Ship::check_size() const
{
    return size == positions.size();
}

void Ship::set_position(const std::vector<std::pair<int, int>> &pos)
{
    positions = pos;
}

bool Ship::occupies_position(std::pair<int, int> target) const
{
    for (const auto &pos : positions)
    {
        if (pos.first == target.first && pos.second == target.second)
        {
            return true;
        }
    }
    return false;
}

bool Ship::is_sunk() const
{
    return hits >= size;
}
