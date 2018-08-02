#include "displayer.h"
#define FREEGLUT_LIB_PRAGMAS 0
#include <GL/glut.h>
#include "boundingbox.h"
#include "basicmath.h"
#include <iostream>

Displayer Displayer::instance_;
int Displayer::width_ = 860, Displayer::height_ = 640;
double Displayer::scale_times = 1.;
std::vector<unsigned int> Displayer::drawLists_;
std::map<unsigned char, void(*)(int, int)> Displayer::keyboard_functions_;
int Displayer::mouse_down_button_=GL_LEFT, Displayer::mouse_down_x_=0, Displayer::mouse_down_y_=0;
double Displayer::mouse_down_a_ = 0, Displayer::mouse_down_b_ = 0;
double Displayer::rotate_a_= 0, Displayer::rotate_b_=0;
double Displayer::translate_a_=0, Displayer::translate_b_=0;
mesh_tools::BoundingBox Displayer::box;
double Displayer::modelSize=1;

void Displayer::init(int &argc, char** argv, const char *title, int width, int height)
{
  width_ = width;
  height_ = height;

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(width_, height_);
  glutCreateWindow(title);

  initCallBackFunctions();

  initParameters(); 
}

void Displayer::runLoop()
{
  glutMainLoop();
}

void Displayer::bindKey(unsigned char key, void function(int, int))
{
  keyboard_functions_.insert_or_assign(key, function);
}

void Displayer::initCallBackFunctions()
{
  glutReshapeFunc(CallBackReshape);
  glutMouseFunc(CallBackMouse);
  glutMotionFunc(CallBackMotion);
  glutKeyboardFunc(CallBackKeyboard);

  glutDisplayFunc(CallBackDisplay);
}

void Displayer::initParameters()
{
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);

  glLineWidth(3);
  glPointSize(4);
}

void Displayer::getScaleSize()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  if (width_ >= height_)
  {
    glOrtho(
      - scale_times * modelSize / 2 * width_ / height_, 
      + scale_times * modelSize / 2 * width_ / height_,
      - scale_times * modelSize / 2, 
      + scale_times * modelSize / 2,
      - modelSize / 2,
      + modelSize / 2
    );
  }
  else
  {
    glOrtho(
      - scale_times * modelSize / 2, 
      + scale_times * modelSize / 2,
      - scale_times * modelSize / 2 / width_ * height_, 
      + scale_times * modelSize / 2 / width_ * height_,
      - modelSize / 2, 
      + modelSize / 2
    );
  }
}

void Displayer::CallBackReshape(int width, int height)
{
  width_ = width;
  height_ = height;
  getScaleSize();
  glutPostRedisplay();
}

void Displayer::CallBackMouse(int button, int status, int x, int y)
{
  static const int GLUT_WHEEL_UP = 3;
  static const int GLUT_WHEEL_DOWN = 4;
  if (status == GLUT_UP)
  {
    if (button == GLUT_WHEEL_UP)
    {
      if (scale_times<100)
        scale_times *= 1.25;
      getScaleSize();
    }
    else if (button == GLUT_WHEEL_DOWN)
    {
      if (scale_times>0.01)
        scale_times *= 0.8;
      getScaleSize();
    }
  }
  else
  {
    if(button == GLUT_LEFT_BUTTON)
    {
      mouse_down_button_ = GLUT_LEFT_BUTTON;
      mouse_down_x_ = x;
      mouse_down_y_ = y;
      mouse_down_a_ = translate_a_;
      mouse_down_b_ = translate_b_;
    }
    else if (button == GLUT_RIGHT_BUTTON)
    {
      mouse_down_button_ = GLUT_RIGHT_BUTTON;
      mouse_down_x_ = x;
      mouse_down_y_ = y;
      mouse_down_a_ = rotate_a_;
      mouse_down_b_ = rotate_b_;
    }
  }
  glutPostRedisplay();
}

void Displayer::CallBackKeyboard(unsigned char key, int x, int y)
{
  auto function = keyboard_functions_.find(key);
  if(function!=keyboard_functions_.end())
  {
    function->second(x, y);
    glutPostRedisplay();
  }
  else
  {
    std::cerr << "Key \'" << key << "\'" << " is not bound." << std::endl;
  }
}

void Displayer::CallBackDisplay()
{
  glViewport(0, 0, width_, height_);
  glClearColor(1., 1., 1., 1.);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  auto center = box.center();
  glTranslated( translate_a_,-translate_b_, 0);
  glRotated(rotate_b_, 1, 0, 0);
  glRotated(rotate_a_, 0, 1, 0);
  glTranslated(-center[0], -center[1], -center[2]);

  for(auto list:drawLists_)
    glCallList(list);

  glutSwapBuffers();
}

void Displayer::CallBackMotion(int x, int y)
{
  int dx = x - mouse_down_x_, dy = y - mouse_down_y_;
  if (mouse_down_button_ == GLUT_LEFT_BUTTON)
  {
    translate_a_ = mouse_down_a_ + dx * modelSize * scale_times * 0.001;
    translate_b_ = mouse_down_b_ + dy * modelSize * scale_times * 0.001;
  }
  else if (mouse_down_button_ == GLUT_RIGHT_BUTTON)
  {
    rotate_a_ = mouse_down_a_ + dx * 0.3;
    rotate_b_ = mouse_down_b_ + dy * 0.3;
    while (rotate_a_ > 360)
      rotate_a_ -= 360;
    while (rotate_a_ < 0)
      rotate_a_ += 360;
    while (rotate_b_ > 360)
      rotate_b_ -= 360;
    while (rotate_b_ < 0)
      rotate_b_ += 360;
  }
  glutPostRedisplay();
}

unsigned Displayer::addDrawableObject(const DrawableObject &object)
{
  object.addToBoundingBox(box);
  modelSize = sqrt(
    mesh_tools::square(box.length())
    + mesh_tools::square(box.width())
    + mesh_tools::square(box.height())
  );
  unsigned int object_no = glGenLists(1);
  glNewList(object_no, GL_COMPILE);
  object.draw();
  glEndList();
  drawLists_.push_back(object_no); 
  getScaleSize();
  return object_no;
}

void DrawableObject::drawTriangle(const mesh_tools::Point3 & point_a, const mesh_tools::Point3 & point_b, const mesh_tools::Point3 & point_c)const
{
  glBegin(GL_TRIANGLES);
  glVertex3d(point_a[0], point_a[1], point_a[2]);
  glVertex3d(point_b[0], point_b[1], point_b[2]);
  glVertex3d(point_c[0], point_c[1], point_c[2]);
  glEnd();
}

void DrawableObject::setColor(double red, double green, double blue, double alpha)const
{
  glColor4d(red, green, blue, alpha);
}

void DrawableObject::drawLine(const mesh_tools::Point3 & point_a, const mesh_tools::Point3 & point_b)const
{
  glBegin(GL_LINES);
  glVertex3d(point_a[0], point_a[1], point_a[2]);
  glVertex3d(point_b[0], point_b[1], point_b[2]);
  glEnd();
}

void DrawableObject::drawPoint(const mesh_tools::Point3 & point)const
{
  glBegin(GL_POINTS);
  glVertex3d(point[0], point[1], point[2]);
  glEnd();
}
