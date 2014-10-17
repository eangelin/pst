#include "rand.h"
#include <random>

static std::mt19937 mt;
static std::uniform_int_distribution<int> dist(0, std::numeric_limits<int>::max());

void init_rand()
{
  std::seed_seq seq{1, 2, 3, 4, 5};
  mt.seed(seq);
}

int rand_int()
{
  return dist(mt);
}
