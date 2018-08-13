#include "LocalRemesher.h"
#include "surfacefile.h"
#include "displayer.h"
#include "surfacealgorithm.h"
#include "sample.h"

using namespace std;
using namespace mesh_tools;

class SampleDraw:public  DrawableObject
{
public:
  SampleDraw(Sample &sample):sample_(sample){}
  virtual ~SampleDraw() = default;
  void addToBoundingBox(mesh_tools::BoundingBox& box) const override
  {
    for (const auto i : sample_.Points())
      box.AddPoint3(i);
  }
  void draw() const override
  {
    setColor(1., 0., 0., 1.);
    for (const auto i : sample_.Points())
    {
      drawPoint(i);
    }
  }
private:
  Sample & sample_;
};

class SurfaceDraw:public DrawableObject
{
public:
  SurfaceDraw(Surface &surface):surface_(surface){}
  virtual ~SurfaceDraw() = default;
  void addToBoundingBox(mesh_tools::BoundingBox& box)const override
  {
    box.AddSurface(surface_);
  }
  void draw() const override
  {
    setColor(0.6, 0.6, 0.7,1.);
    for (mesh_tools::FacetIterator i(surface_, 0); !i.IsEnd(); ++i)
    {
      const auto a = i.halfedge().vertex().position();
      const auto b = i.halfedge().next().vertex().position();
      const auto c = i.halfedge().next().next().vertex().position();
      drawTriangle(a, b, c);
    }
    
    setColor(0.4, 1, 0.4, 1.);
    for (mesh_tools::HalfedgeIterator i(surface_, 0); !i.IsEnd(); ++i)
    {
      const auto a = i.vertex().position();
      const auto b = i.opposite().vertex().position();
      drawLine(a, b);
    }

    setColor(1., 0., 0., 1.);
    for (mesh_tools::VertexIterator i(surface_, 0); !i.IsEnd(); ++i)
    {
      const auto point = i.position();
      drawPoint(point);
    }
  }
private:
  Surface & surface_;
};

int main(int argc, char* argv[]) {
  
  Surface surface;
  if (argc > 1)
  {
    const std::string path(argv[1]);
    ObjFile oldfile(path);
    oldfile.ReadToSurface(surface);
  }
  Sample sample(surface);
  sample.FixedNumberRandomSample(1000);

  auto displayer = Displayer::getInstance();
  displayer.init(argc, argv, "hello", 1024, 768);
  displayer.addDrawableObject(SampleDraw(sample));
  displayer.addDrawableObject(SurfaceDraw(surface));
  displayer.runLoop();
  return 0;
}
