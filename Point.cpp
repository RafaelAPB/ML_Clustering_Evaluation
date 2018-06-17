//
// Created by Rafael Belchior on 08/06/2018.
//
#include "Point.h"

Point::Point(const vector<double> &coordinates, const string &discovered_label, int cluster_id,
             int point_id) : coordinates(coordinates), discovered_label(discovered_label),
                             cluster_id(cluster_id), id(point_id) {}


double Point::euclideanDistance(Point a) {
    double sum = 0.0;
    int dimensions = a.getCoordinates().size();

    for (int i = 0; i < dimensions; i++) {
        sum += pow((a.getCoordinates()[i] - coordinates[i]), 2);
    }

    return sqrt(sum);
}

double Point::euclideanDistance(vector<double> a) {
    double sum = 0.0;
    int dimensions = a.size();

    for (int i = 0; i < dimensions; i++) {
        sum += pow((a[i] - coordinates[i]), 2);
    }

    return sqrt(sum);
}


Point &Point::operator=(const Point &p) {
    if (this != &p) {
        coordinates = p.coordinates;
    }

    return *this;
}


double Point::getASilhouette(vector<Point> &points_from_cluster) {
    double dist = 0.0;
    vector<Point> points;

    for (Point p: points_from_cluster) {
        if (p.getPoint_Id() == getPoint_Id()) {
            continue;
        }
        dist += euclideanDistance(p);
    }

    dist /= points_from_cluster.size();

    return dist;
}
