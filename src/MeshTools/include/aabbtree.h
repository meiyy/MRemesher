#pragma once
#include "boundingbox.h"

namespace mesh_tools
{


struct TrianglePoint
{
  struct cmp_x
  {
    bool operator()(const TrianglePoint& a, const TrianglePoint& b)const
    {
      return a.box.x_min() < b.box.x_min();
    }
  };
  struct cmp_y
  {
    bool operator()(const TrianglePoint& a, const TrianglePoint& b)const
    {
      return a.box.y_min() < b.box.y_min();
    }
  };
  struct cmp_z
  {
    bool operator()(const TrianglePoint& a, const TrianglePoint& b)const
    {
      return a.box.z_min() < b.box.z_min();
    }
  };
  TrianglePoint(const Point3 &point_a,const Point3 &point_b,const Point3 &point_c):points{point_a,point_b,point_c}
  {
    box.AddPoint3(point_a);
    box.AddPoint3(point_b);
    box.AddPoint3(point_c);
  }
  auto& operator[](unsigned int i)
  {
    return points[i];
  }
  Point3 points[3];
  BoundingBox box;
}; 
class AABBTriangleTree
{
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
  private:
    BoundingBox box;
    AABBTriangleTreeNode *left_;
    AABBTriangleTreeNode *right_;
  };

  class AABBTriangleTreeLeafNode:public AABBTriangleTreeNode
  {
  public:
    AABBTriangleTreeLeafNode(const Point3& point_a, const Point3& point_b,const Point3& point_c):
      AABBTriangleTreeNode(1),triangle_(point_a,point_b,point_c){}
  
    BoundingBox GetBoundingBox() override
    {
      BoundingBox box;
      for(unsigned int i = 0;i<3;i++)
      {
        box.AddPoint3(triangle_[i]);
      }
      return box;
    }
  private:
    TrianglePoint triangle_;
  };

public:
  AABBTriangleTree():
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
    std::vector<TrianglePoint> triangles_;
    triangles_.clear();
    for (mesh_tools::FacetIterator i(surface, 0); !i.IsEnd(); ++i) {
      const auto a = i.halfedge().vertex().position();
      const auto b = i.halfedge().next().vertex().position();
      const auto c = i.halfedge().next().next().vertex().position();
      triangles_.emplace_back(a,b,c);
    }
    root_ = BuildSubTree(0,triangles_.size()/3,triangles_);
    triangles_.clear();
  }
  void BuildTreeFromTrianglePointVector(const std::vector<TrianglePoint> &triangles)
  {
    assert(triangles.size()%3==0);
    assert(!triangles.empty());
    std::vector<TrianglePoint> triangles_ = triangles;
    root_ = BuildSubTree(0,triangles_.size()/3,triangles_);
    triangles_.clear();
  }
private:
  AABBTriangleTreeNode *BuildSubTree(unsigned int begin, unsigned int end,std::vector<TrianglePoint> &triangles)
  {
    const unsigned int num_of_triangles = end - begin;
    if(num_of_triangles == 1)
    {
      const auto new_node = new AABBTriangleTreeLeafNode(triangles[begin][0],triangles[begin][1],triangles[begin][2]);
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
      std::sort(triangles.begin()+begin,triangles.begin()+end,TrianglePoint::cmp_x());
    }
    else if(box.width()>box.height())
    {
      std::sort(triangles.begin()+begin,triangles.begin()+end,TrianglePoint::cmp_y());
    }
    else
    {
      std::sort(triangles.begin()+begin,triangles.begin()+end,TrianglePoint::cmp_z());
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