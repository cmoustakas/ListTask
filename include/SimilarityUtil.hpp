#pragma once

#include <Entity.hpp>
#include <functional>
#include <vector>

namespace similarity {
/**
 * @brief similarPairGroupsOnStrength Statistical approach on extraction of two
 * "similar" samples
 * @param samples The samples
 * @param sum_of_counts The summation of counts field of Entity struct
 * @param similar_samples The output of the function
 * @return
 */
bool similarPairGroupsOnStrength(
    const std::vector<ListOfEntities> &samples, const int sum_of_counts,
    std::pair<ListOfEntities, ListOfEntities> &similar_samples);

} // namespace similarity
