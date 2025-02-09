#include "Solver.hpp"

#include <algorithm>
#include <cassert>
#include <random>
#include <stdexcept>
#include <vector>

#include <SimilarityUtil.hpp>

constexpr int kTargetAccumulatedCounts = 5;

static inline void shuffleList(ListOfEntities &lst) {

  std::vector<ListOfEntities::iterator> iterators;
  for (auto it = lst.begin(); it != lst.end(); ++it) {
    iterators.push_back(it);
  }

  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(iterators.begin(), iterators.end(), g);

  ListOfEntities shuffled_list;
  for (auto it : iterators) {
    shuffled_list.splice(shuffled_list.end(), lst, it);
  }

  lst = std::move(shuffled_list);
}

static inline int
accumulateEntities(const std::vector<ListOfEntities> &groups) {
  int total_entities = 0;
  for (const auto &g : groups) {
    total_entities += g.size();
  }
  return total_entities;
}

class CandidatesStorage {
public:
  explicit CandidatesStorage(const int size) {
    m_candidate_iterators.reserve(size);
  }

  ~CandidatesStorage() = default;
  void pushIterator(const ListOfEntities::const_iterator &it) noexcept {
    m_candidate_iterators.push_back(it);
  };

  bool selectionIsGood() { return m_sum == kTargetAccumulatedCounts; }

  void setSum(const int other_sum) noexcept { m_sum = other_sum; }

  int getSum() const noexcept { return m_sum; }

  void resetStorage() noexcept {
    m_sum = 0;
    m_candidate_iterators.clear();
    m_candidate_iterators.reserve(kTargetAccumulatedCounts);
  }

  std::vector<ListOfEntities::const_iterator> &getIteratorsVec() const {
    return m_candidate_iterators;
  }

private:
  mutable std::vector<ListOfEntities::const_iterator> m_candidate_iterators{};
  int m_sum = 0;
};

static void
runAccumulativeIteration(ListOfEntities &list,
                         const ListOfEntities::const_iterator &init_it,
                         CandidatesStorage &candidates) {

  candidates.resetStorage();
  for (auto accumulate_it = init_it; accumulate_it != list.end();
       ++accumulate_it) {

    if (accumulate_it->m_count <= 0) {
      throw std::runtime_error("Only Positive values are allowed");
    }

    // Early exit on greater than 5
    if (accumulate_it->m_count > kTargetAccumulatedCounts) {
      accumulate_it = list.erase(accumulate_it);
      accumulate_it--;
      continue;
    }

    const auto chk_sum = candidates.getSum() + accumulate_it->m_count;

    if (chk_sum > kTargetAccumulatedCounts) {
      continue;
    }

    candidates.setSum(chk_sum);
    candidates.pushIterator(accumulate_it);

    if (candidates.selectionIsGood()) {
      return;
    }
  }
}

void findCandidatesOnAccumulatedTgtCount(
    ListOfEntities list, std::vector<ListOfEntities> &candidate_groups) {

  candidate_groups.clear();
  // Worst case scenario the list is consisted of only fives
  candidate_groups.reserve(list.size());

  CandidatesStorage candidate_iterators(kTargetAccumulatedCounts);

  while (!list.empty()) {

    // Early exit on greater than 5
    if (list.begin()->m_count > kTargetAccumulatedCounts) {
      list.pop_front();
      continue;
    }

    // Early push on 5
    if (list.begin()->m_count == kTargetAccumulatedCounts) {
      ListOfEntities just_five;
      just_five.push_back(std::move(list.front()));
      list.pop_front();
      candidate_groups.push_back(just_five);
      continue;
    }

    runAccumulativeIteration(list, list.begin(), candidate_iterators);

    if (!candidate_iterators.selectionIsGood()) {
      list.pop_front();
      continue;
    }

    const std::vector<ListOfEntities::const_iterator> &selected_iterators =
        candidate_iterators.getIteratorsVec();

    // Move all the entities that the selected iterators point to member list
    ListOfEntities a_group;
    for (auto it : selected_iterators) {
      a_group.splice(a_group.end(), list, it);
    }

    // So push the selected group to the candidates group
    candidate_groups.push_back(a_group);
  }
}

namespace solver {

ProblemSolver::ProblemSolver(ListOfEntities &list) { m_list = std::move(list); }

SolverResult
ProblemSolver::trySolve(std::pair<ListOfEntities, ListOfEntities> &solution) {
  if (m_list.empty()) {
    throw std::runtime_error("Attempt to solve without list");
  }

  SolverResult solved = SolverResult::FAIL;

  constexpr int kMaxFallbacks = 4;
  // We will consider the sample reliable if its size of the total candidate
  // entities is greater of the 10% of the total entities
  constexpr int kMinimumSamplesSize = 3;
  const int reliable_sample_size = std::max(
      static_cast<int>(std::ceil(0.1f * m_list.size())), kMinimumSamplesSize);

  std::vector<ListOfEntities> candidates;
  for (int fallbax = 0; fallbax < kMaxFallbacks; ++fallbax) {

    // Update member fallback, remember only for testing purpose
    m_fallback_counter = fallbax;

    findCandidatesOnAccumulatedTgtCount(m_list, candidates);

    const bool reliable_selection =
        (accumulateEntities(candidates) >= reliable_sample_size);

    if (reliable_selection) {
      if (similarity::similarPairGroupsOnStrength(
              candidates, kTargetAccumulatedCounts, solution)) {
        solved = SolverResult::SUCCESS;
        break;
      }
    }

    // The fallback strategy is to "shuffle the deck" and retry
    shuffleList(m_list);
  }

  return solved;
}

} // namespace solver
