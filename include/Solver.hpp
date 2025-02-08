#pragma once

#include <list>
#include <vector>

#include <Entity.hpp>

// Forward declare the Storage we use for each candidate group
class CandidatesStorage;

namespace solver {

enum SolverResult { SUCCESS, FAIL };

class ProblemSolver {
public:
  explicit ProblemSolver(ListOfEntities &list);
  ~ProblemSolver() = default;

  /**
   * @brief trySolve
   * @param solution
   * @return
   */
  SolverResult trySolve(std::pair<ListOfEntities, ListOfEntities> &solution);

  /**
   * @brief setList
   * @param list
   */
  void setList(ListOfEntities &list) { m_list = std::move(list); }

  /**
   * @brief getList
   * @return
   */
  ListOfEntities getList() const noexcept { return m_list; }

  /**
   * @brief getFallbacksRounds
   * @return
   */
  int getFallbacksRounds() const noexcept { return m_fallback_counter; }

private:
  ListOfEntities m_list{};

  // [!][!] This member has debugging-testing purpose only
  int m_fallback_counter = 0;
};
} // namespace solver
