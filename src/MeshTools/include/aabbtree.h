#pragma once
#include "boundingbox.h"
#include "plane.h"
#include "basicmath.h"
#include <functional>

namespace mesh_tools
{


struct AABBTriangle
{
  struct cmp_x
  {
    bool operator()(const AABBTriangle& a, const AABBTriangle& b)const
    {
      return a[0][0] < b[0][0];
    }
  };
  struct cmp_y
  {
    bool operator()(const AABBTriangle& a, const AABBTriangle& b)const
    {
      return a[0][1] < b[0][1];
    }
  };
  struct cmp_z
  {
    bool operator()(const AABBTriangle& a, const AABBTriangle& b)const
    {
      return a[0][2] < b[0][2];
    }
  };
  AABBTriangle(const Point3 &point_a,const Point3 &point_b,const Point3 &point_c, unsigned int id):
    points{point_a,point_b,point_c}, id(id)
  {
    normal = TriangleNormal(point_a,point_b,point_c);
  }
  Point3& operator[](unsigned int i)
  {
    return points[i];
  }
  const Point3& operator[](unsigned int i) const
  {
    return points[i];
  }
 
  Point3 points[3];
  Point3 normal;
  unsigned int id;
}; 
class AABBTriangleTree
{
public:
  class AABBTriangleTreeNode
  {
  public:
    AABBTriangleTreeNode(unsigned int size):
      size_(size){}
    virtual ~AABBTriangleTreeNode()=default;
    virtual BoundingBox GetBoundingBox()=0;
    virtual unsigned int size()const
    {
      return size_;
    }
  protected:
    unsigned int size_;
  };

  class AABBTriangleTreeBranchNode:public AABBTriangleTreeNode
  {
  public:
    AABBTriangleTreeBranchNode(AABBTriangleTreeNode *left, AABBTriangleTreeNode *right):
      AABBTriangleTreeNode(left->size()+right->size()+1),left_(left),right_(right)
    {
      box.AddBoundingBox(left_->GetBoundingBox());
      box.AddBoundingBox(right_->GetBoundingBox());
    }
    AABBTriangleTreeNode *left()const
    {
      return left_;
    }
    AABBTriangleTreeNode *right()const
    {
      return right_;
    }
    BoundingBox GetBoundingBox() override
    {
      return box;
    }
    double SquaredDistanceToPoint(const Point3 &point) const
    {
      double distance = 0.;
      if(point[0]>box.x_max())
      {
        distance += square(point[0] - box.x_max());
      }
      if(point[0]<box.x_min())
      {
        distance += square(point[0] - box.x_min());
      }
      if(point[1]>box.y_max())
      {
        distance += square(point[1] - box.y_max());
      }
      if(point[1]<box.y_min())
      {
        distance += square(point[1] - box.y_min());
      }
      if(point[2]>box.z_max())
      {
        distance += square(point[2] - box.z_max());
      }
      if(point[2]<box.z_min())
      {
        distance += square(point[2] - box.z_min());
      }
      return distance;
    }
  private:
    BoundingBox box;
    AABBTriangleTreeNode *left_;
    AABBTriangleTreeNode *right_;
  };

  class AABBTriangleTreeLeafNode:public AABBTriangleTreeNode
  {
  public:
    AABBTriangleTreeLeafNode(const Point3& point_a, const Point3& point_b,const Point3& point_c, unsigned int id):
      AABBTriangleTreeNode(1),triangle_(point_a,point_b,point_c,id){}
    auto Triangle()const
    {
      return triangle_;
    }
    BoundingBox GetBoundingBox() override
    {
      BoundingBox box;
      for(unsigned int i = 0;i<3;i++)
      {
        box.AddPoint3(triangle_[i]);
      }
      return box;
    }
    void FindClosestPoint(const Point3 &point, Point3 &closest_point)
    {
      const Point3 &normal = triangle_.normal;
      int side=0;
      for(unsigned int i = 0;i<3;i++)
      {
        const Point3 a = triangle_[i], b = triangle_[(i+1)%3];
        Plane plane(a,(b-a).outer_product(normal));
        const auto cur_side = plane.GetSideOfPoint(point);
        if(cur_side == Plane::FRONT_OF_PLANE || cur_side == Plane::ON_PLANE)
          side |= (1<<i);
      }
      if(side == 0)
      {
        const double distance_to_plane = normal.inner_product(point - triangle_[0]);
        closest_point = point - normal * distance_to_plane;
      }
      else if(side == 5)
      {
        closest_point = triangle_[0];
      }
      else if(side == 3)
      {
        closest_point = triangle_[1];
      }
      else if(side == 6)
      {
        closest_point = triangle_[2];
      }
      else if(side == 1)
      {
        const double length01=(triangle_[1] - triangle_[0]).length();
        const auto vec01=(triangle_[1] - triangle_[0]).normalize();
        closest_point = triangle_[0] + vec01 * clamp((point - triangle_[0]).inner_product(vec01),0.,length01);
      }
      else if(side == 2)
      {
        const double length12=(triangle_[2] - triangle_[1]).length();
        const auto vec12=(triangle_[2] - triangle_[1]).normalize();
        closest_point = triangle_[1] + vec12 * clamp((point - triangle_[1]).inner_product(vec12),0.,length12);
      }
      else if(side == 4)
      {
        const double length20=(triangle_[0] - triangle_[2]).length();
        const auto vec20=(triangle_[0] - triangle_[2]).normalize();
        closest_point = triangle_[2] + vec20 * clamp((point - triangle_[2]).inner_product(vec20),0.,length20);
      }
    }
    unsigned int Id() const
    {
      return triangle_.id;
    }
  private:
    AABBTriangle triangle_;
  };

  AABBTriangleTree()noexcept:
    root_(nullptr){}
  ~AABBTriangleTree()
  {
    if(root_!=nullptr)
    {
      DestroySubTree(root_);
    }
  }
  void BuildTreeFromSurface(Surface& surface)
  {
    std::vector<AABBTriangle> triangles_;
    triangles_.clear();
    unsigned int id = 0;
    for (mesh_tools::FacetIterator i(surface); !i.IsEnd(); ++i,++id) {
      const auto a = i.halfedge().vertex().position();
      const auto b = i.halfedge().next().vertex().position();
      const auto c = i.halfedge().next().next().vertex().position();
      triangles_.emplace_back(AABBTriangle(a,b,c,id));
    }
    root_ = BuildSubTree(0,static_cast<unsigned int>(triangles_.size()),triangles_);
    triangles_.clear();
  }
  void BuildTreeFromTrianglePointVector(const std::vector<AABBTriangle> &triangles)
  {
    assert(triangles.size()%3==0);
    assert(!triangles.empty());
    std::vector<AABBTriangle> triangles_ = triangles;
    root_ = BuildSubTree(0,static_cast<unsigned int>(triangles_.size()),triangles_);
    triangles_.clear();
  }
  void FindClosestPoint(const Point3 &point, Point3 &closest_point, unsigned int &closest_triangle_id) const
  {
    double min_distance = DBL_MAX;
    closest_point = point;
    FindClosestPoint(point,root_,closest_point,closest_triangle_id,min_distance);
  }
  void ForEachNode(std::function<void(AABBTriangleTreeNode*)> action)const
  {
    ForEachNode(root_,action);
    
  }
private:
  void ForEachNode(AABBTriangleTreeNode* node,std::function<void(AABBTriangleTreeNode*)> action) const
  {
    action(node);
    if(node->size()>1)
    {
      const auto branch = dynamic_cast<AABBTriangleTreeBranchNode*>(node);
      ForEachNode(branch->left(),action);
      ForEachNode(branch->right(),action);
    }
  }
  void FindClosestPoint(const Point3 &point, AABBTriangleTreeNode* node, Point3 &closest_point, unsigned int &closest_triangle_id, double& min_squared_distance) const
  {
    if(node->size()==1)
    {
      auto triangle = dynamic_cast<AABBTriangleTreeLeafNode*>(node);
      Point3 cur_point;
      triangle->FindClosestPoint(point,cur_point);
      const double distance = (cur_point - point).length2();
      if(distance < min_squared_distance)
      {
        min_squared_distance = distance;
        closest_point = cur_point;
        closest_triangle_id=triangle->Id();
      }
      return;
    }
    const auto branch = dynamic_cast<AABBTriangleTreeBranchNode*>(node);
    double distance = branch->SquaredDistanceToPoint(point);
    if(distance<min_squared_distance)
    {
      FindClosestPoint(point,branch->left(),closest_point,closest_triangle_id,min_squared_distance);
      FindClosestPoint(point,branch->right(),closest_point,closest_triangle_id,min_squared_distance); 
    }
  }

  AABBTriangleTreeNode *BuildSubTree(unsigned int begin, unsigned int end,std::vector<AABBTriangle> &triangles)const
  {
    const unsigned int num_of_triangles = end - begin;
    if(num_of_triangles == 1)
    {
      const auto new_node = new AABBTriangleTreeLeafNode(triangles[begin][0],triangles[begin][1],triangles[begin][2],triangles[begin].id);
      return new_node;
    }
    BoundingBox box;
    for(unsigned int i = begin;i<end;i++)
    {
      box.AddPoint3(triangles[i][0]);
      box.AddPoint3(triangles[i][1]);
      box.AddPoint3(triangles[i][2]);
    }
    if(box.length()>= std::max(box.width(),box.height()))
    {
      std::nth_element(
        triangles.begin()+begin,
        triangles.begin()+begin+num_of_triangles/2,
        triangles.begin()+end,
        AABBTriangle::cmp_x()
      );
    }
    else if(box.width()>box.height())
    {
      std::nth_element(
        triangles.begin()+begin,
        triangles.begin()+begin+num_of_triangles/2,
        triangles.begin()+end,
        AABBTriangle::cmp_y()
      );
    }
    else
    {
      std::nth_element(
        triangles.begin()+begin,
        triangles.begin()+begin+num_of_triangles/2,
        triangles.begin()+end,
        AABBTriangle::cmp_z()
      );
    }

    const auto left = BuildSubTree(begin,begin+num_of_triangles/2,triangles);
    const auto right = BuildSubTree(begin+num_of_triangles/2,end,triangles);
    const auto new_node = new AABBTriangleTreeBranchNode(left,right);
    return new_node;
  }
  static void DestroySubTree(AABBTriangleTreeNode* node)
  {
    if(node->size()>1)
    {
      DestroySubTree(dynamic_cast<AABBTriangleTreeBranchNode*>(node)->left());
      DestroySubTree(dynamic_cast<AABBTriangleTreeBranchNode*>(node)->right());
    }
    delete node;
  }
  AABBTriangleTreeNode *root_;
};

}
