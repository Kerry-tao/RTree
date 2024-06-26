#include "Node.h"
#include "RTree.h"
// #include "RTree2.h"
using namespace std;

int main() {
    cout<<"---------\n";
    RTree rtree2;
    std::vector<DataPoint> data_points = {
        {"id1",{4,1},{1, 0, 0, 1}},
        {"id2",{5,2},{0, 1, 0, 1}},
        {"id3",{4,5},{0, 0, 1, 0}},
        {"id4",{9,10},{1, 0, 0, 0}},
        {"id5",{10,9},{0, 1, 1, 0}},
        {"id6",{9,9},{0, 1, 0, 0}},
        {"id7",{2,4},{1, 0, 0, 1}},
        {"id8",{8,4},{0, 1, 1, 0}},
        {"id9",{4,7},{0, 0, 1, 1}}
    };
    rtree2.calculateDistances(data_points);
    for(const DataPoint& data_point :data_points) {
        rtree2.insert(DataPoint(data_point));
    }
    rtree2.printTree2();
}
