//
// Created by Rafael Belchior on 08/06/2018.
//
#ifndef EDAMI_EVALUATION_H
#define EDAMI_EVALUATION_H

#include "Point.h"
#include "Cluster.h"

double euclideanDistance(vector<double> a, vector<double> b);

void createClusters(vector<Point> &points, vector<Cluster> &clusters);

double calculatePurityMeasure(vector<Point> &points, vector<Cluster> &clusters);

double calculateFMeasure(vector<Point> &points, vector<Cluster> &clusters, int beta);

double calculatePrecision(Cluster &clusters);

double calculateRecall(vector<Point> &points, Cluster &clusters);

double calculateRandMeasure(vector<Point> &points, vector<Cluster> &clusters);

void setCenterPoint(Cluster &cluster);

double calculateDaviesBouldinIndex(vector<Cluster> &clusters, int number_points);

double calculateSilhouetteIndex(vector<Point> &points, vector<Cluster> &clusters, int num_points);

double calculateCalinskiHarabaszIndex(vector<Point> &points, vector<Cluster> &clusters);

Cluster getNearestCluster(vector<Cluster> &clusters, Point p);

vector<Point> getPointsFromCluster(vector<Cluster> &clusters, Point p);


#endif //EDAMI_EVALUATION_H
