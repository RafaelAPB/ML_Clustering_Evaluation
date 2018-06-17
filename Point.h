//
// Created by Rafael Belchior on 15/06/2018.
//

#ifndef ML_EVALUATIONMEASURES_POINT_H
#define ML_EVALUATIONMEASURES_POINT_H


#include <array>
#include <vector>
#include <iostream>
#include <cmath>


using namespace std;

class Point {
    vector<double> coordinates;
    string discovered_label;
    int cluster_id;
    int id; //needed for output purposes only

public:

    Point(const vector<double> &coordinates, const string &discovered_label, int cluster_id,
          int point_id);

    friend ostream &operator<<(ostream &os, const Point &point);

    const vector<double> &getCoordinates() const { return coordinates; }

    const string &getDiscovered_label() const { return discovered_label; }

    int getPoint_Cluster_id() { return cluster_id; }

    int getPoint_Id() { return id; };

    double euclideanDistance(Point a);

    double euclideanDistance(vector<double> a);

    double getASilhouette(vector<Point> &points);

    bool operator==(const Point &rhs) const { return id == rhs.id; }

    bool operator!=(const Point &rhs) const { return !(rhs == *this); }

    Point &operator=(const Point &p);


    friend ostream &operator<<(ostream &os, const Point &point) {
        os << "Point " << point.id << endl;
        os << "Belongs to cluster " << point.cluster_id << endl;
        os << "Coordinates: " << endl;
        for (int i = 0; i < point.getCoordinates().size(); i++) {
            os << point.getCoordinates()[i] << endl;
        }
        os << endl << "discovered_label: " << point.discovered_label;
        return os;
    }

};


#endif //ML_EVALUATIONMEASURES_POINT_H
