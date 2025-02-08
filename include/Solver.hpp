#pragma once

#include <list>
#include <vector>

#include <Entity.hpp>

// Forward declare the Storage we use for each candidate group
class CandidatesStorage;

namespace solver {

enum class SolverResult { SUCCESS, FAIL };

class ProblemSolver {
public:
  explicit ProblemSolver(ListOfEntities &list);
  ~ProblemSolver() = default;

  /**
   * @brief trySolve Try to find a pair of lists of Entitys that the sum of
   * count of each list is equal to five and the average strength of each list
   * is relatively similar
   * @param solution The solution
   * @return Success or Fail
   */
  SolverResult trySolve(std::pair<ListOfEntities, ListOfEntities> &solution);

  /**
   * @brief setList Sets the list if default constructor used
   * @param list
   */
  void setList(ListOfEntities &list) { m_list = std::move(list); }

  /**
   * @brief getList Return the list
   * @return
   */
  ListOfEntities getList() const noexcept { return m_list; }

  /**
   * @brief getFallbacksRounds Returns how many times we fallbacked, only for
   * testing
   * @return
   */
  int getFallbacksRounds() const noexcept { return m_fallback_counter; }

private:
  ListOfEntities m_list{};

  // [!][!] This member has debugging-testing purpose only
  int m_fallback_counter = 0;
};
} // namespace solver
