//
// Created by Rafael Belchior on 08/06/2018.
//
#include "Evaluation.h"
#include <fstream>
#include <chrono>

#include <set>

using namespace std;

class InputParser {
private:
    ifstream fin;
    vector<Point> points;
    int num_classes;
    int num_attributes;
    int num_points;
    string option;
    string dataset;
    string path;
    int beta;

public:
    InputParser() {
        /*
     *
     * INPUT:
     * num_classes, num_attributes, num_Points
     * point_coordinate1 point_coordinate2 point ... _coordinateN label_discovered cluster_id
     *
     *
     * */

        //Receive input
        string absolutePath;
        cout << "Choose the input dataset (1-3) or your own (4)" << endl;
        cout << "1 - Iris Mock Test || few values, few classes, made up classifications" << endl;
        cout << "2 - Iris Test || sample of the DBSCAN output" << endl;
        cout << "3 - Iris || full output" << endl;
        cout << "4 - Absenteeism" << endl;
        cout << "5 - Cardio" << endl;
        cout << "6 - Choose dataset " << endl;

        cin >> option;
        cin.ignore();

        cout << "Choose the value for beta  >= 0 for F-measure" << endl;
        cin >> beta;
        cin.ignore();

        switch (stoi(option)) {
            case 1: {
                absolutePath = "../input/IrisTest";
                break;
            }
            case 2: {
                absolutePath = "../input/Iris0,52";
                break;
            }
            case 3: {
                absolutePath = "../input/iris";
                break;
            }
            case 4: {
                absolutePath = "../input/absenteeism";
                break;
            }
            case 5: {
                absolutePath = "../input/cardio";
                break;
            }
            case 6: {

                cout
                        << "Introduce the directory that contains the input files or press A to load the deafult directory , example: \"../input/\""
                        << endl;
                cin >> absolutePath;
                cin.ignore();

                if (absolutePath == "A") {
                    absolutePath = "../input/";
                }
                cout << "Insert dataset name" << endl;
                cin >> dataset;
                break;
            }
            default: {
                cout << "No proper input selected, try again.";
                exit(EXIT_FAILURE);
            }
        }
        path = absolutePath + dataset;

        fin.open(path);
        if (!fin) {
            cout << path << " Error reading the file\n";
            exit(EXIT_FAILURE);
        }
        parse();
    }

    void parse() {
        fin >> num_classes;
        fin >> num_attributes;
        fin >> num_points;

        string discovered_label;
        int cluster_id = 0;

        //Traverses the file and creates Points
        for (int i = 0; i < num_points; i++) {
            fin >> cluster_id;
            fin >> discovered_label;
            //Stores values for one point
            vector<double> current_point_coordinates;

            for (int j = 0; j < num_attributes; j++) {
                double v = 0;
                fin >> v;
                current_point_coordinates.push_back(v);
            }


            Point p(current_point_coordinates, discovered_label, cluster_id, i);
            points.push_back(p);

        }

        fin.close();

    }

    const vector<Point> &getPoints() const {
        return points;
    }

    int getNum_classes() const {
        return num_classes;
    }


    int getNum_attributes() const {
        return num_attributes;
    }

    int getBeta() const {
        return beta;
    }

    int getNum_points() const {
        return num_points;
    }

};

int main(int argc, char **argv) {
    auto startedPreprocessing = chrono::high_resolution_clock::now();
    vector<Cluster> clusters;
    vector<Point> points;
    set<string> labels;
    int beta = 0;
    int num_points = 0;
    //Not needed, because the number of attributes can be obtained from the number of coordinates of a certain Point
    //int num_attributes = 0;
    int num_classes = 0;

    InputParser InputParser;

    //num_attributes = InputParser.getNum_attributes();
    num_classes = InputParser.getNum_classes();
    num_points = InputParser.getNum_points();
    //Normalizing techniques could be applied
    points = InputParser.getPoints();
    beta = InputParser.getBeta();
    if (beta < 0) {
        beta = 1;
    }

    createClusters(points, clusters);

    for (int i = 0; i < num_classes; i++) {
        setCenterPoint(clusters[i]);
        cout << "";
    }

    auto donePreprocessing = std::chrono::high_resolution_clock::now();
    auto started = chrono::high_resolution_clock::now();

    //External evaluation methods (needs external data):
    double purity = calculatePurityMeasure(points, clusters);
    double f_measure = calculateFMeasure(points, clusters, beta);
    double rand_measure = calculateRandMeasure(points, clusters);

    //Purity = 1 means the cluster has only one class;
    // Purity = 0 means all the classes are different
    cout << "Purity: " << purity << endl;

    //F-measure its a combined metric of precision and recall
    //Precision is the probability that a (randomly selected) retrieved document is relevant
    //Recall is s the probability that a (randomly selected) relevant document is retrieved in a search
    cout << "F-Measure: " << f_measure << endl;

    //Rand index as a measure of the percentage of correct decisions made by the algorithm
    //Rand = 1 means that the clustering is the best possible
    cout << "Rand-Measure: " << rand_measure << endl;

    //Internal evaluation methods

    //If Davies–Bouldin index is low, it means that the clusters have low intra-cluster distances
    // high intra-cluster similarity) and high inter-cluster distances (low inter-cluster similarity)
    //Not normalized
    double davies_bouldin = calculateDaviesBouldinIndex(clusters, num_classes);
    cout << "Davies-Boulding Index: " << davies_bouldin << endl;

    //Value beetwen -1 and 1 where 1 means that the object is assigned to the best possible group,
    // 0 - the object is located between two groups, and -1 - wrong assignment of the object
    double silhouetee = calculateSilhouetteIndex(points, clusters, num_points);
    cout << "Silhouette Index: " << silhouetee << endl;

    //There is no "acceptable" cut-off value. The higher the value, the "better" is the solution.
    //Not normalized
    double chIndex = calculateCalinskiHarabaszIndex(points, clusters);
    cout << "Calinski-Harabasz Index: " << chIndex << endl;

    cout << endl;

    auto done = std::chrono::high_resolution_clock::now();
    //The preprocessing time is reading input, and initializing the structures
    cout << "Preprocessing time: "
         << chrono::duration_cast<chrono::milliseconds>(donePreprocessing - startedPreprocessing).count()
         << " ms to execute." << endl;
    cout << "Execution time: " << chrono::duration_cast<chrono::milliseconds>(done - started).count()
         << " ms to execute." << endl;

    string option;
    cout << "Press ANY KEY to exit";
    cin >> option;

    return EXIT_SUCCESS;
}


void createClusters(vector<Point> &points, vector<Cluster> &clusters) {
    set<int> cluster_ids;

    for (int i = 0; i < points.size(); i++) {
        int current_point_cluster_id = points[i].getPoint_Cluster_id();

        //Todo optimize?
        if (cluster_ids.find(current_point_cluster_id) != cluster_ids.end()) {
            //Cluster already exists, add point to cluster
            for (int j = 0; j < clusters.size(); j++) {

                int current_cluster_id = clusters[j].getCluster_id();
                if (current_cluster_id == current_point_cluster_id) {
                    clusters[j].addPoint(points[i]);
                    break;
                }
            }
        } else {
            cluster_ids.insert(current_point_cluster_id);

            //Creates cluster with the same id as the point
            Cluster c(current_point_cluster_id);
            c.addPoint(points[i]);

            //Adds cluster to the cluster vector
            clusters.push_back(c);

        }
    }
}

//Calculates cluster's centroid
void setCenterPoint(Cluster &cluster) {
    vector<Point> points = cluster.getPoints();
    int dimensions = points[0].getCoordinates().size();
    vector<double> center_coordinates(dimensions);
    int number_points = cluster.getNumberOfPoints();

    for (int i = 0; i < dimensions; i++) {
        for (int j = 0; j < number_points; j++) {
            center_coordinates[i] += points[j].getCoordinates()[i];
        }
    }

    for (int l = 0; l < center_coordinates.size(); l++) {
        center_coordinates[l] /= dimensions;
    }


    cluster.setCentral_point(center_coordinates);

}

//Purity a measure of the extent to which clusters contain a single class.
double calculatePurityMeasure(vector<Point> &points, vector<Cluster> &clusters) {
    double purity = 0.0;
    double total_common_points = 0;
    pair<int, string> result = pair<int, string>(-1, "");


    for (Cluster c : clusters) {
        //IMPORTANT ASSUMPTION: The most common class in a discovered cluster is taken as the correct class
        result = c.getMostCommonClass();
        total_common_points += result.first;
    }

    purity = total_common_points / points.size();

    return purity;
}

// F-measure is the harmonic mean of precision and recall
double calculateFMeasure(vector<Point> &points, vector<Cluster> &clusters, int beta) {
    double f_measure = 0.0;
    double precision = 0.0;
    double recall = 0.0;

    //As beta increases, the recall factor has a higher weight
    //For each cluster we calculate its f-measure. Final result is the average.
    for (Cluster c : clusters) {
        //P = TP / TP + FP
        precision = calculatePrecision(c);

        //R = TP / TP + FN
        recall = calculateRecall(points, c);

        if (precision == 0 && recall == 0) {
            cout << "Precision and recall are 0 on f-measures";
            return EXIT_FAILURE;
        }

        f_measure += ((pow(beta, 2) + 1) * precision * recall) / ((pow(beta, 2)) * precision + recall);

    }

    f_measure /= clusters.size();

    return f_measure;
}

//Precision is calculated as the fraction of pairs correctly put in the same cluster
double calculatePrecision(Cluster &c) {
    double precision = 0.0, TP = 0.0, FP = 0.0;
    pair<int, string> most_common_class;

    //TP are the points that have the same class and are in the same cluster (points that are in most common class in a cluster)
    //FP are the points that have different classes and are in the same cluster (points except the most commmon)

    most_common_class = c.getMostCommonClass();
    TP = most_common_class.first;
    FP = c.getNumberOfPoints() - TP;

    precision = TP / (TP + FP);
    return precision;
}

//Recall is the fraction of actual pairs that were identified
double calculateRecall(vector<Point> &points, Cluster &c) {
    double recall = 0.0, TP = 0.0, FN = 0.0;
    pair<int, string> most_common_class = c.getMostCommonClass();
    //TP are the points that have the same class and are in the same cluster
    //FN are points that have the same class, and are in different clusters
    TP = most_common_class.first;

    for (Point p: points) {
        if (p.getPoint_Cluster_id() != c.getCluster_id()) {
            if (p.getDiscovered_label() == most_common_class.second) {
                FN += 1;
            }
        }
    }

    recall = TP / (TP + FN);
    return recall;
}

//The Rand index computes how similar the clusters are to the benchmark classifications.
double calculateRandMeasure(vector<Point> &points, vector<Cluster> &clusters) {
    double rand = 0.0, TP = 0.0, FP = 0.0;
    pair<int, string> most_common_class;
    //TN are points that have not the same class, and are in different clusters
    //R = TP + TN / TP + TN + FP + FN
    for (Cluster c: clusters) {
        double FN = 0.0, TN = 0.0;
        most_common_class = c.getMostCommonClass();
        TP = most_common_class.first;
        FP = c.getNumberOfPoints() - TP;

        for (Point p: points) {
            if (p.getPoint_Cluster_id() != c.getCluster_id()) {
                if (p.getDiscovered_label() == most_common_class.second) {
                    FN += 1;
                } else {
                    TN += 1;
                }
            }
        }
        rand += (TP + TN) / (TP + TN + FP + FN);
    }

    rand /= clusters.size();
    return rand;
}


double calculateSilhouetteIndex(vector<Point> &points, vector<Cluster> &clusters, int num_points) {
    double silhouette = 0.0;
    //the average distance between x and other objects in a group including x
    double a = 0.0;
    //the minimum average distance between x and the nearest group.
    double b = 0.0;
    vector<Point> cluster_points;
    vector<Point> nearest_cluster_points;

    for (Point p: points) {
        cluster_points = getPointsFromCluster(clusters, p);
        a = p.getASilhouette(cluster_points);
        //Get closest cluster to p
        Cluster nearestCluster = getNearestCluster(clusters, p);
        nearest_cluster_points = nearestCluster.getPoints();
        b = p.getASilhouette(nearest_cluster_points);

        silhouette += (b - a) / (max(b, a));

    }

    return silhouette / num_points;
}

double calculateDaviesBouldinIndex(vector<Cluster> &clusters, int number_clusters) {
    double davies = 0.0;
    int clusters_number = clusters.size();
    for (int i = 0; i < clusters_number; i++) {
        vector<double> v1 = clusters[i].getCentral_point();
        vector<double> v2 = clusters[(i + 1) % number_clusters].getCentral_point();

        //Compare every pair of clusters

        //Exterior combinations
        double teta = clusters[i].avgDistToCentroid() + clusters[(i + 1) % number_clusters].avgDistToCentroid();
        double dist_beetween_centroids = euclideanDistance(v1, v2);

        double parcial_result = teta / dist_beetween_centroids;

        //Interior combinations (all clusters compared to the others)
        for (int j = (i + 2) % number_clusters; j != i; j = (j + 1) % number_clusters) {
            vector<double> v3 = clusters[j].getCentral_point();

            teta = clusters[i].avgDistToCentroid() + clusters[j].avgDistToCentroid();
            dist_beetween_centroids = euclideanDistance(v1, v3);

            //Taking the max
            if (parcial_result < (teta / dist_beetween_centroids)) {
                parcial_result = teta / dist_beetween_centroids;
            }

        }

        davies += parcial_result;
    }

    return davies / number_clusters;
}

double calculateCalinskiHarabaszIndex(vector<Point> &points, vector<Cluster> &clusters) {
    double ch = 0.0;
    int num_points = points.size();
    int num_clusters = clusters.size();

    double b = 0.0, w = 0.0;

    for (Cluster c: clusters) {
        w = c.withinClusterVariance();
        b = c.betweenClusterVariance(points);

        //CH index can show the optimal number of clusters when doing k-means or hierarchical clustering; you would choose the number of clusters k that maximize CH(k). As k increases, B(k) increases, and W(k) decreases.
        //As k increases, B(k) increases, and W(k) decreases.
        ch += (b / w) * ((num_points - num_clusters) / (num_clusters - 1));
    }
    return ch / num_clusters;
}


Cluster getNearestCluster(vector<Cluster> &clusters, Point p) {
    double min_dist = numeric_limits<double>::max();;
    Cluster nearest_cluster(-1);
    for (Cluster c: clusters) {

        double atual_dist = euclideanDistance(p.getCoordinates(), c.getCentral_point());
        if (atual_dist < min_dist) {
            min_dist = atual_dist;
            nearest_cluster = c;
        }
    }
    return nearest_cluster;
}

vector<Point> getPointsFromCluster(vector<Cluster> &clusters, Point p) {
    int point_cluster_id = p.getPoint_Cluster_id();
    vector<Point> points;
    for (Cluster c: clusters) {
        if (c.getCluster_id() == point_cluster_id) {
            return c.getPoints();
        }
    }
    //Shouldn't get here
    return points;
}

double euclideanDistance(vector<double> a, vector<double> b) {
    double sum = 0.0;
    int dimensions = a.size();

    for (int i = 0; i < dimensions; i++) {
        sum += pow((a[i] - b[i]), 2);
    }

    return sqrt(sum);
}