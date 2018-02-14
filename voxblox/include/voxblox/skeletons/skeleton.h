#ifndef VOXBLOX_SKELETONS_SKELETON_H_
#define VOXBLOX_SKELETONS_SKELETON_H_

#include <map>

#include "voxblox/core/common.h"

namespace voxblox {

struct SkeletonPoint {
  Point point = Point::Zero();
  float distance = 0.0f;
  int num_basis_points = 0;
  AlignedVector<Point> basis_directions;

  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

class Skeleton {
 public:
  Skeleton();

  // Access to all the skeleton points.
  const AlignedVector<SkeletonPoint>& getSkeletonPoints() const {
    return points_;
  }
  const AlignedVector<SkeletonPoint>& getEdgePoints() const { return edges_; }
  const AlignedVector<SkeletonPoint>& getVertexPoints() const {
    return vertices_;
  }

  AlignedVector<SkeletonPoint>& getSkeletonPoints() { return points_; }
  AlignedVector<SkeletonPoint>& getEdgePoints() { return edges_; }
  AlignedVector<SkeletonPoint>& getVertexPoints() { return vertices_; }

  // Converts the points to a pointcloud with no other information, for all
  // points on the GVD.
  void getPointcloud(Pointcloud* pointcloud) const;

  // Also get a vector for the distance information.
  void getPointcloudWithDistances(Pointcloud* pointcloud,
                                  std::vector<float>* distances) const;
  void getEdgePointcloudWithDistances(Pointcloud* pointcloud,
                                      std::vector<float>* distances) const;
  void getVertexPointcloudWithDistances(Pointcloud* pointcloud,
                                        std::vector<float>* distances) const;

 private:
  AlignedVector<SkeletonPoint> points_;

  // Subsets of points (just copies because we don't care)
  AlignedVector<SkeletonPoint> vertices_;
  AlignedVector<SkeletonPoint> edges_;
};

struct SkeletonVertex {
  int64_t vertex_id = -1;
  Point point = Point::Zero();
  float distance = 0.0f;

  std::vector<uint64_t> edge_list;

  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

struct SkeletonEdge {
  int64_t edge_id = -1;
  int64_t start_vertex = -1;
  int64_t end_vertex = -1;
  Point start_point = Point::Zero();
  Point end_point = Point::Zero();
  float start_distance = 0.0f;
  float end_distance = 0.0f;

  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

class SparseSkeletonGraph {
 public:
  SparseSkeletonGraph();

  int64_t addVertex(const SkeletonVertex& vertex);
  // Add Edge does all the heavy lifting: connects the vertices using their
  // internal edge lists, and updates the start and end points within the
  // edge.
  int64_t addEdge(const SkeletonEdge& edge);

  bool hasVertex(int64_t id) const;
  bool hadEdge(int64_t id) const;

  const SkeletonVertex& getVertex(int64_t id) const;
  const SkeletonEdge& getEdge(int64_t id) const;

  SkeletonVertex& getVertex(int64_t id);
  SkeletonEdge& getEdge(int64_t id);

  // Accessors to just get all the vertex and edge IDs.
  void getAllVertexIds(std::vector<int64_t>* vertex_ids) const;
  void getAllEdgeIds(std::vector<int64_t>* edge_ids) const;

  void clear();

 private:
  // Vertex and edge IDs are separate.
  std::map<int64_t, SkeletonVertex> vertex_map_;
  std::map<int64_t, SkeletonEdge> edge_map_;

  int64_t next_vertex_id_;
  int64_t next_edge_id_;
};

}  // namespace voxblox

#endif  // VOXBLOX_SKELETONS_SKELETON_H_