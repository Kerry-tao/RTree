#include "Node.h"
#include "RTree.h"
using namespace std;

int main() {

    // RTree rtree;
    // rtree.insert(Point(4, 1));
    // rtree.insert(Point(5, 2));
    // rtree.insert(Point(4, 5));
    // rtree.insert(Point(9, 10));
    // rtree.insert(Point(10, 9));
    // rtree.insert(Point(9, 9));
    // rtree.insert(Point(2, 4));
    // rtree.insert(Point(8, 4));
    // rtree.insert(Point(4, 7));
    //
    // rtree.printTree();

    cout<<"---------\n";
    RTree rtree2;
    std::vector<DataPoint> data_points = {
        {{4,1},{1, 0, 0, 1}},
        {{5,2},{0, 1, 0, 1}},
        {{4,5},{0, 0, 1, 0}},
        {{9,10},{1, 0, 0, 0}},
        {{10,9},{0, 1, 1, 0}},
        {{9,9},{0, 1, 0, 0}},
        {{2,4},{1, 0, 0, 1}},
        {{8,4},{0, 1, 1, 0}},
        {{4,7},{0, 0, 1, 1}},
    };
    rtree2.calculateDistances(data_points);
    for(const DataPoint& data_point :data_points) {
        rtree2.insert(DataPoint(data_point));
    }
    rtree2.printTree2();
}
