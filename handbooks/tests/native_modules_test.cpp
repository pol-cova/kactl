#define sz(x) int((x).size())
#include "../../content/finals-math/Combinations.h"
#include "../../content/finals-math/DecimalPeriod.h"
#include "../../content/finals-math/EuclidCRT.h"
#include "../../content/finals-math/ExactDecimals.h"
#include "../../content/finals-math/FactorSieve.h"
#include "../../content/finals-math/FuzzyFactorization.h"
#include "../../content/finals-math/IntegerDivision.h"
#include "../../content/finals-math/IntegerGeometry.h"
#include "../../content/finals-math/Inversions.h"
#include "../../content/finals-math/ModularArithmetic.h"
#include "../../content/finals-math/Prelude.h"
#include "../../content/finals-implementations/BFS.h"
#include "../../content/finals-implementations/BinarySearch.h"
#include "../../content/finals-implementations/Components.h"
#include "../../content/finals-implementations/CompressionPrefix.h"
#include "../../content/finals-implementations/DSUKruskal.h"
#include "../../content/finals-implementations/DifferenceArray.h"
#include "../../content/finals-implementations/DigitDP.h"
#include "../../content/finals-implementations/Fenwick.h"
#include "../../content/finals-implementations/GridBFS.h"
#include "../../content/finals-implementations/Knapsack.h"
#include "../../content/finals-implementations/LIS.h"
#include "../../content/finals-implementations/Prelude.h"
#include "../../content/finals-implementations/SegmentTree.h"
#include "../../content/finals-implementations/SlidingWindow.h"
#include "../../content/finals-implementations/TopoSort.h"
#include "../../content/finals-implementations/TreeLCA.h"
#include "../../content/finals-implementations/WeightedPaths.h"
#include "../../content/finals-implementations/IntervalUnion.h"
#include <cassert>
#include <iostream>
int main() {
    assert(finals_math::floor_div(-7,3)==-3);
    assert(finals_math::decimal_shape(1,6).period==1);
    assert(finals_math::crt({2,6},{5,9})->residue==14);
    assert(finals_math::inversions({3,1,2})==2);
    finals_core::Graph g{{1},{0,2},{1},{}};
    auto b=finals_core::bfs(g,{0});assert(b.dist[2]==2);
    assert(finals_core::components(g).count==2);
    finals_core::Fenwick f(3);f.add(1,5);assert(f.sum(0,2)==5);
    assert(*finals_dp::complete_digits("?2",7)=="42");
    std::cout<<"Native KACTL modules: dependency integration passed\n";
}
