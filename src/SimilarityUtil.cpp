#include <SimilarityUtil.hpp>

#include <cassert>
#include <cmath>
#include <numeric>

#define SQUARE(x) ((x) * (x))

namespace similarity {

static void calculateStrengths(const std::vector<ListOfEntities> &groups,
                               const int sum_of_counts,
                               std::vector<float> &strengths) {

  strengths.clear();
  strengths.reserve(groups.size());
  for (const auto &g : groups) {
    float avg_strength =
        std::accumulate(g.begin(), g.end(), 0.f,
                        [](float sum, Entity curr_entity) {
                          sum += (curr_entity.m_count * curr_entity.m_strength);
                          return sum;
                        }) /
        sum_of_counts;

    strengths.push_back(avg_strength);
  }
}

static inline float standardDeviation(const std::vector<float> &strengths) {

  const size_t length = strengths.size();

  const float mean =
      std::accumulate(strengths.begin(), strengths.end(), 0.f) / length;

  const float variance =
      std::accumulate(strengths.begin(), strengths.end(), 0.f,
                      [mean](float sum, float sample) {
                        sum += SQUARE(sample - mean);
                        return sum;
                      }) /
      length;

  const float stddev = std::sqrt(variance);

  return stddev;
}

std::pair<int, int> pickSimilar(const std::vector<float> &strengths,
                                const float stddev) {

  for (size_t i = 0; i < strengths.size() - 1; ++i) {
    for (size_t j = i + 1; j < strengths.size(); ++j) {
      // Convergence when: |Xi - Xj| < σ
      bool similarity_criterion =
          std::abs(strengths[i] - strengths[j]) < stddev;
      if (similarity_criterion) {
        // Usualy the happy path is the final return but in this case it is more
        // convenient
        return std::make_pair(i, j);
      }
    }
  }

  return std::make_pair(kInvalidSz, kInvalidSz);
}

bool similarPairGroupsOnStrength(
    const std::vector<ListOfEntities> &groups, const int sum_of_counts,
    std::pair<ListOfEntities, ListOfEntities> &similar_samples) {

  std::vector<float> strength_of_each_group;
  calculateStrengths(groups, sum_of_counts, strength_of_each_group);
  assert(strength_of_each_group.size() == groups.size());

  auto indexes = pickSimilar(strength_of_each_group,
                             standardDeviation(strength_of_each_group));

  if (indexes.first == kInvalidSz || indexes.second == kInvalidSz) {
    return false;
  }

  const auto first = indexes.first;
  const auto second = indexes.second;
  similar_samples =
      std::make_pair(std::move(groups[first]), std::move(groups[second]));

  return true;
}

} // namespace similarity
