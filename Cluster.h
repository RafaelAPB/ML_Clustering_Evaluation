//
// Created by Rafael Belchior on 15/06/2018.
//

#ifndef EDAMI_CLUSTER_H
#define EDAMI_CLUSTER_H

#include "Point.h"
#include <vector>
#include <map>

class Cluster {
    int cluster_id;
    vector<double> central_point;
    vector<Point> points;

public:

    explicit Cluster(int id);

    int getCluster_id() const { return cluster_id; }

    const vector<double> &getCentral_point() const { return central_point; }

    void setCentral_point(const vector<double> &central_point) { Cluster::central_point = central_point; }

    const vector<Point> &getPoints() const { return points; }

    void addPoint(Point &point) { points.push_back(point); }

    int getNumberOfPoints() const { return int(points.size()); }

    pair<int, string> getMostCommonClass();

    double avgDistToCentroid();

    double whitinClusterVariance();

    double beetwenClusterVariance(vector<Point> &points);

    double euclideanDistance(vector<double> a, vector<double> b);
};

#endif //EDAMI_CLUSTER_H
