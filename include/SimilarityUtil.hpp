#pragma once

#include <Entity.hpp>
#include <functional>
#include <vector>

namespace similarity {

bool similarPairGroupsOnStrength(
    const std::vector<ListOfEntities> &samples, const int sum_of_counts,
    std::pair<ListOfEntities, ListOfEntities> &similar_samples);

} // namespace similarity
