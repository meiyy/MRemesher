#include "LocalRemesher.h"
#include "surfacefile.h"
#include "displayer.h"
#include "surfacealgorithm.h"
#include "sample.h"
#include "aabbtree.h"

using namespace std;
using namespace mesh_tools;

class SampleDraw : public DrawableObject {
public:
  SampleDraw(Sample& sample)
    : sample_(sample) {
  }

  virtual ~SampleDraw() = default;

  void addToBoundingBox(mesh_tools::BoundingBox& box) const override {
    for (const auto i : sample_.Points())
      box.AddPoint3(i);
  }

  void draw() const override {
    setColor(1., 0., 0., 1.);
    for (const auto i : sample_.Points()) {
      drawPoint(i);
    }
  }

private:
  Sample& sample_;
};

class SurfaceDraw : public DrawableObject {
public:
  SurfaceDraw(Surface& surface)
    : surface_(surface) {
  }

  virtual ~SurfaceDraw() = default;

  void addToBoundingBox(mesh_tools::BoundingBox& box) const override {
    box.AddSurface(surface_);
  }

  void draw() const override {
    setColor(0.6, 0.6, 0.7, 1.);
    for (mesh_tools::FacetIterator i(surface_); !i.IsEnd(); ++i) {
      const auto a = i.halfedge().vertex().position();
      const auto b = i.halfedge().next().vertex().position();
      const auto c = i.halfedge().next().next().vertex().position();
      drawTriangle(a, b, c);
    }

    setColor(0.4, 1, 0.4, 1.);
    for (mesh_tools::HalfedgeIterator i(surface_); !i.IsEnd(); ++i) {
      const auto a = i.vertex().position();
      const auto b = i.opposite().vertex().position();
      drawLine(a, b);
    }

    setColor(1., 0., 0., 1.);
    for (mesh_tools::VertexIterator i(surface_); !i.IsEnd(); ++i) {
      const auto point = i.position();
      drawPoint(point);
    }
  }

private:
  Surface& surface_;
};

class TreeDraw : public DrawableObject {
public:
  TreeDraw(AABBTriangleTree& tree)
    : tree_(tree) {
  }

  virtual ~TreeDraw() = default;

  void addToBoundingBox(mesh_tools::BoundingBox& box) const override {
    tree_.ForEachNode(
                      [&box](AABBTriangleTree::AABBTriangleTreeNode* node)
                      {
                        auto box0 = node->GetBoundingBox();
                        Point3 p0(box0.x_min(), box0.y_min(), box0.z_min()),
                               p1(box0.x_min(), box0.y_min(), box0.z_max()),
                               p2(box0.x_min(), box0.y_max(), box0.z_min()),
                               p3(box0.x_min(), box0.y_max(), box0.z_max()),
                               p4(box0.x_max(), box0.y_min(), box0.z_min()),
                               p5(box0.x_max(), box0.y_min(), box0.z_max()),
                               p6(box0.x_max(), box0.y_max(), box0.z_min()),
                               p7(box0.x_max(), box0.y_max(), box0.z_max());
                        box.AddPoint3(p0);
                        box.AddPoint3(p1);
                        box.AddPoint3(p2);
                        box.AddPoint3(p3);
                        box.AddPoint3(p4);
                        box.AddPoint3(p5);
                        box.AddPoint3(p6);
                        box.AddPoint3(p7);
                      }
                     );
  }

  void draw() const override {
    setColor(0.6, 0.6, 0.9, 1.);
    tree_.ForEachNode(
      [&](AABBTriangleTree::AABBTriangleTreeNode* node)
      {
        auto box0 = node->GetBoundingBox();
      
        Point3 p0(box0.x_min(), box0.y_min(), box0.z_min()),
               p1(box0.x_min(), box0.y_min(), box0.z_max()),
               p2(box0.x_min(), box0.y_max(), box0.z_min()),
               p3(box0.x_min(), box0.y_max(), box0.z_max()),
               p4(box0.x_max(), box0.y_min(), box0.z_min()),
               p5(box0.x_max(), box0.y_min(), box0.z_max()),
               p6(box0.x_max(), box0.y_max(), box0.z_min()),
               p7(box0.x_max(), box0.y_max(), box0.z_max());
        drawLine(p0,p1);
      drawLine(p0,p2);
      drawLine(p0,p4);
      drawLine(p1,p3);
      drawLine(p1,p5);
      drawLine(p2,p3);
      drawLine(p2,p6);
      drawLine(p3,p7);
      drawLine(p4,p5);
      drawLine(p4,p6);
      drawLine(p5,p7);
      drawLine(p6,p7);
      }
     );
  }

private:
  AABBTriangleTree& tree_;
};



int main(int argc, char* argv[]) {

  Surface surface;
  if (argc > 1) {
    const std::string path(argv[1]);
    ObjFile oldfile(path);
    oldfile.ReadToSurface(surface);
  }
  Sample sample(surface);
  sample.FixedNumberRandomSample(1000);

  AABBTriangleTree aabbtree;
  aabbtree.BuildTreeFromSurface(surface);

  auto sample_points = sample.Points();
  std::vector<Point3> new_sample;
  for (auto& i : sample_points) {
    Point3 new_point;
    unsigned int id;
    aabbtree.FindClosestPoint(i, new_point, id);
    new_sample.push_back(new_point);
  }
  Sample sample2(surface, new_sample);

  auto displayer = Displayer::getInstance();
  displayer.init(argc, argv, "hello", 1024, 768);
  displayer.addDrawableObject(SampleDraw(sample));
  displayer.addDrawableObject(SampleDraw(sample2));
  //displayer.addDrawableObject(TreeDraw(aabbtree));
  displayer.addDrawableObject(SurfaceDraw(surface));
  displayer.runLoop();
  return 0;
}
