# ML_Clustering_Evaluation :notebook_with_decorative_cover: 
Implementation of 3 internal and 3 external evaluation methods are implemented in C++.
<br><br>
External evaluation: <br>
* Purity 
* F-Measure
* Rand-Measure

Internal evaluation: <br>
* Davies–Bouldin Index
* Silhouette Index
* Calinski-Harabasz Index
<br><br>


## Running the program
1. Clone the project
2. (OPTIONAL) If you wish, choose the data sets you want to use as input and place them on /input/ folder (see the notes for the input format).
3.  Go to the directory /exe and run Evaluation.exe as administrator

##Compiling the program
1. Clone the project
2. Run 
*cmake CMakeLists.txt*

## Notes
* The input of the program, for a data set with numerical data and n dimensions is in the following form: <br><br> 
number_of_classes number_of_attributes number_of_points
cluster1_id point1_label coordinate_1 coordinate_2 coordinate_n
cluster1_id point2_label coordinate_1 coordinate_2 coordinate_n
...

## Main reference
 https://en.wikipedia.org/wiki/Cluster_analysis
  