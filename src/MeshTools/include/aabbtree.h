#pragma once
#include "boundingbox.h"

namespace mesh_tools
{
  
class AABBTriangleTree
{
  class AABBTriangleTreeNode
  {
  public:
    AABBTriangleTreeNode():left_(nullptr),right_(nullptr){}

    AABBTriangleTreeNode *left() const
    {
      return left_;
    }

    AABBTriangleTreeNode *right() const
    {
      return right_;
    }
  protected:
    AABBTriangleTreeNode *left_;
    AABBTriangleTreeNode *right_;
  };

  class AABBTriangleTreeBranchNode:public AABBTriangleTreeNode
  {
  public:

  private:
    BoundingBox box;
  };

public:

private:

};

}