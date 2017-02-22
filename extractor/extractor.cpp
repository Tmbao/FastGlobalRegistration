//
//  main.cpp
//  FPFHExtractor
//
//  Created by Bao Truong on 2/5/17.
//  Copyright © 2017 Bao Truong. All rights reserved.
//

#include <iostream>
#include <string>
using namespace std;

#include <pcl/point_types.h>
#include <pcl/features/fpfh.h>
#include <pcl/features/normal_3d.h>
#include <pcl/io/obj_io.h>
using namespace pcl;

int main(int argc, const char * argv[]) {
  
  cerr << "Reading " + string(argv[1]) << endl;
  
  PointCloud<PointXYZ>::Ptr cloud(new PointCloud<PointXYZ>());
  io::loadOBJFile(argv[1], *cloud);
  cerr << "Cloud points: " << cloud->points.size() << endl;

  NormalEstimation<PointXYZ, Normal> normalEstimation;
  normalEstimation.setInputCloud(cloud);

  search::KdTree<PointXYZ>::Ptr tree (new search::KdTree<PointXYZ>);
  normalEstimation.setSearchMethod(tree);

  PointCloud<Normal>::Ptr cloudWithNormals(new PointCloud<Normal>);
  normalEstimation.setRadiusSearch(0.03);
  normalEstimation.compute(*cloudWithNormals);
  
  FPFHEstimation<PointXYZ, Normal, FPFHSignature33> fest;
  fest.setRadiusSearch(0.2);
  fest.setInputCloud(cloud);
  fest.setInputNormals(cloudWithNormals);
  fest.setSearchMethod(tree);

  PointCloud<FPFHSignature33>::Ptr object_features(new PointCloud<FPFHSignature33>());
  fest.compute(*object_features);

  cout << "Output points: " << object_features->points.size() << endl;
  cout << object_features->points[0] << endl;
  
  FILE* fid = fopen(argv[2], "wb");
  int nV = cloud->size(), nDim = 33;
  fwrite(&nV, sizeof(int), 1, fid);
  fwrite(&nDim, sizeof(int), 1, fid);
  for (int v = 0; v < nV; v++) {
    const PointXYZ &pt = cloud->points[v];
    float xyz[3] = {pt.x, pt.y, pt.z};
    fwrite(xyz, sizeof(float), 3, fid);
    const FPFHSignature33 &feature = object_features->points[v];
    fwrite(feature.histogram, sizeof(float), nDim, fid);
  }
  fclose(fid);
  
  return 0;
}
