//
// Created by Rafael Belchior on 08/06/2018.
//
#include "Cluster.h"

Cluster::Cluster(int id) : cluster_id(id) {}

//Iterates through the cluster and returns the most common class
pair<int, string> Cluster::getMostCommonClass() {
    int biggest_number = -1;

    pair<int, string> most_common_class;
    map<string, int> classes;

    if (getNumberOfPoints() == 0) {
        return pair<int, string>(-1, "s");
    }

    //Determine the labels and its count
    for (const Point &p : points) {
        const string label = p.getDiscovered_label();
        if (classes.find(p.getDiscovered_label()) == classes.end()) {
            classes.insert(pair<string, int>(p.getDiscovered_label(), 1));
        } else {
            classes[p.getDiscovered_label()] += 1;
        }
    }

    //Determine the most common label (class)
    for (auto it = classes.begin(); it != classes.end(); ++it) {
        if (it->second > biggest_number) {
            biggest_number = it->second;
            most_common_class.first = it->second;
            most_common_class.second = it->first;
        }
    }
    return most_common_class;
}

double Cluster::avgDistToCentroid() {
    double dist = 0.0;
    for (Point p : points) {
        if (p.getCoordinates() == getCentral_point()) {
            continue;
        }
        dist += p.euclideanDistance(getCentral_point());
    }

    return dist / getNumberOfPoints();
}

double Cluster::withinClusterVariance() {
    double sum = 0.0;
    vector<Point> points = getPoints();
    vector<double> center = getCentral_point();
    for (Point p: points) {
        sum += euclideanDistance(p.getCoordinates(), center);
    }
    return sum / points.size();
}

double Cluster::betweenClusterVariance(vector<Point> &points) {
    double sum = 0.0;
    vector<Point> points_cluster = getPoints();
    vector<double> center = getCentral_point();
    int num_points = points.size();
    vector<double> x(num_points);
    //Not the best practise. Used to avoid the function to receive an extra parameter (dimension)
    int dimensions = points[0].getCoordinates().size();

    for (int i = 0; i < dimensions; i++) {
        for (int j = 0; j < num_points; j++) {
            x[i] += points[j].getCoordinates()[i];
        }
    }
    for (double element: x) {
        element /= dimensions;
    }

    for (const Point p: points) {
        sum += euclideanDistance(p.getCoordinates(), x);
    }
    return sum / num_points;
}

double Cluster::euclideanDistance(vector<double> a, vector<double> b) {
    double sum = 0.0;
    int dimensions = a.size();

    for (int i = 0; i < dimensions; i++) {
        sum += pow((a[i] - b[i]), 2);
    }

    return sqrt(sum);
}