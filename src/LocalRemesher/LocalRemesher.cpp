#include "LocalRemesher.h"
#include <iostream>
#define FREEGLUT_LIB_PRAGMAS 0
#include "gl/glut.h"
#include "surfacefile.h"
#include "boundingbox.h"
#include "basicmath.h"
#include "math.h"

using namespace std;
using namespace mesh_tools;

Surface surface;
int window_width = 860, window_height = 640;
double scale_times = 1;


void getScaleSize()
{
  BoundingBox box(surface);
  double modelSize = sqrt(
    square(box.length())
    + square(box.width())
    + square(box.height())
  );
  auto center = box.center();
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  if (window_width >= window_height)
  {
    glOrtho(
      center[0] - scale_times*modelSize / 2* window_width/window_height, center[0] + scale_times * modelSize / 2* window_width / window_height,
      center[1] - scale_times * modelSize / 2 , center[1] + scale_times * modelSize / 2 ,
      center[2] - modelSize / 2, center[2] + modelSize / 2
    );
  }
  else
  {
    glOrtho(
      center[0] - scale_times * modelSize / 2, center[0] + scale_times * modelSize / 2,
      center[1] - scale_times * modelSize / 2 / window_width * window_height, center[1] + scale_times * modelSize / 2 / window_width * window_height,
      center[2] - modelSize / 2, center[2] + modelSize / 2
    );
  }
}

void reshape(int width, int height)
{
  window_width = width;
  window_height = height;
  getScaleSize();
  glutPostRedisplay();
}

void drawFunc()
{
  getScaleSize();

  glViewport(0, 0, window_width, window_height);
  glClearColor(1., 1., 1., 1.);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  glColor3d(0., 0., 1.);
  glBegin(GL_TRIANGLES);
  for (FacetIterator i(surface, 0); !i.IsEnd(); ++i)
  {
    auto a = i.halfedge().vertex().position();
    auto b = i.halfedge().next().vertex().position();
    auto c = i.halfedge().next().next().vertex().position();
    glVertex3d(a[0], a[1], a[2]);
    glVertex3d(b[0], b[1], b[2]);
    glVertex3d(c[0], c[1], c[2]);
  }
  glEnd();

  glColor3d(0., 1., 0.);
  glBegin(GL_LINES);
  for (HalfedgeIterator i(surface, 0); !i.IsEnd(); ++i)
  {
    auto a = i.vertex().position();
    auto b = i.opposite().vertex().position();
    glVertex3d(a[0], a[1], a[2]);
    glVertex3d(b[0], b[1], b[2]);
  }
  glEnd();

  glColor3d(1., 0., 0.);
  glBegin(GL_POINTS);
  for (VertexIterator i(surface, 0); !i.IsEnd(); ++i)
  {
    auto point = i.position();
    glVertex3d(point[0], point[1], point[2]);
  }
  glEnd();

  glutSwapBuffers();
  //glutPostRedisplay();
}

void kbfun(int button, int state, int x, int y)
{
  #define  GLUT_WHEEL_UP 3           //定义滚轮操作  
  #define  GLUT_WHEEL_DOWN 4  
  if (state == GLUT_UP && button == GLUT_WHEEL_UP)
  {
    scale_times += 0.03;
    glutPostRedisplay();
  }
  if (state == GLUT_UP && button == GLUT_WHEEL_DOWN)
  {
    scale_times -= 0.03;
    glutPostRedisplay();
  }
  glutPostRedisplay();
}
int main(int argc, char* argv[]) {
  if (argc > 1)
  {
    const std::string path(argv[1]);
    ObjFile oldfile(path);
    oldfile.ReadToSurface(surface);
    ObjFile newfile(path.substr(0, path.length() - 4) + "-new.obj");
    newfile.SaveToFile(surface);
  }
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(window_width, window_height);
  glutCreateWindow("MRemesher");
  glutReshapeFunc(reshape);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glutMouseFunc(kbfun);
  glutDisplayFunc(drawFunc);
  glutMainLoop();
  return 0;
}
