#include <gtest/gtest.h>

#include <random>

#include <Entity.hpp>
#include <SimilarityUtil.hpp>
#include <Solver.hpp>

static void generateRandomListOfEntities(size_t lists_size,
                                         ListOfEntities &list) {
  std::random_device rd;
  std::mt19937 gen(rd());

  std::uniform_int_distribution<int> count_r(0, 10);
  std::uniform_int_distribution<int> strength_r(0, 300);

  for (int i = 0; i < lists_size; ++i) {
    Entity e = {"A", count_r(gen), strength_r(gen)};
    list.push_back(e);
  }
}

TEST(SolverTests, EmptyList) {
  std::list<Entity> empty_list;
  std::pair<ListOfEntities, ListOfEntities> solution;

  solver::ProblemSolver solver(empty_list);
  EXPECT_THROW(solver.trySolve(solution), std::runtime_error);
}

TEST(SolverTests, NegativeRecord) {
  std::list<Entity> list_with_negative_count = {{"A", 4, 10},  {"B", 3, 2},
                                                {"C", -1, 10}, {"D", 2, 2},
                                                {"E", 4, 2},   {"F", 5, 10}};

  std::pair<ListOfEntities, ListOfEntities> solution;
  solver::ProblemSolver solver(list_with_negative_count);
  EXPECT_THROW(solver.trySolve(solution), std::runtime_error);
}

TEST(SolverTests, GoodCase) {
  // Here the candidate solutions are {0,2} , {1,3} and {5} and {5} is more
  // simillar to {0,2}
  std::list<Entity> list = {{"A", 4, 10}, {"B", 3, 2}, {"C", 1, 10},
                            {"D", 2, 2},  {"E", 4, 2}, {"F", 5, 10}};

  std::pair<ListOfEntities, ListOfEntities> solution;

  solver::ProblemSolver solver(list);
  EXPECT_EQ(solver.trySolve(solution), solver::SolverResult::SUCCESS);

  EXPECT_EQ(solver.getFallbacksRounds(), 0);
}

TEST(SolverTests, FallbackCase) {
  // Here the candidate solutions are {0,2} , {1,3} and {5} and {5} is more
  // simillar to {0,2}
  std::list<Entity> list = {{"B", 3, 2},  {"C", 1, 10}, {"D", 2, 2},
                            {"A", 4, 10}, {"E", 4, 2},  {"F", 5, 10}};

  std::pair<ListOfEntities, ListOfEntities> solution;

  solver::ProblemSolver solver(list);
  EXPECT_NO_THROW(solver.trySolve(solution));
  // We expect here that fallback
  EXPECT_GE(solver.getFallbacksRounds(), 1);
}

TEST(SimilarityTests, NoSimilarityOnSamples) {
  constexpr int kSumOfCounts = 5;

  std::vector<ListOfEntities> groups = {{{"B", kSumOfCounts, 1}},
                                        {{"C", kSumOfCounts, 50}},
                                        {{"D", kSumOfCounts, 100}}};

  // Suppose that the groups are 3 and are equal to the above, we expect the
  // convergence criterion to fail because the 3 samples are spreaded around 50
  // equally
  std::pair<ListOfEntities, ListOfEntities> similar_samples;

  const auto found_similar = similarity::similarPairGroupsOnStrength(
      groups, kSumOfCounts, similar_samples);

  EXPECT_EQ(found_similar, false);
}

TEST(SimilarityTests, SimilarityOnSamples) {
  constexpr int kSumOfCounts = 5;

  std::vector<ListOfEntities> groups = {{{"B", kSumOfCounts, 1}},
                                        {{"C", kSumOfCounts, 50}},
                                        {{"E", kSumOfCounts, 100}},
                                        {{"F", kSumOfCounts, 55}}};
  // Suppose that the groups are 3 and are equal to the above, we expect the
  // convergence criterion to success because the "C" is close to "F"
  std::pair<ListOfEntities, ListOfEntities> similar_samples;

  const auto found_similar = similarity::similarPairGroupsOnStrength(
      groups, kSumOfCounts, similar_samples);

  EXPECT_EQ(found_similar, true);
  Entity first_entity = similar_samples.first.front();
  Entity second_entity = similar_samples.second.front();

  EXPECT_EQ(first_entity.m_strength, 50);
  EXPECT_EQ(second_entity.m_strength, 55);
}

TEST(SolverTests, RealScenarioRun) {
  const size_t list_len = 1e3;
  ListOfEntities list;

  generateRandomListOfEntities(list_len, list);
  std::pair<ListOfEntities, ListOfEntities> solution;

  solver::ProblemSolver solver(list);
  const auto res = solver.trySolve(solution);
  EXPECT_EQ(res, solver::SolverResult::SUCCESS);
}
