#include "displayer.h"
#define FREEGLUT_LIB_PRAGMAS 0
#include <GL/glut.h>
#include "boundingbox.h"
#include "basicmath.h"
#include <iostream>
#include <cmath>

Displayer Displayer::instance_;
int Displayer::width_ = 860, Displayer::height_ = 640;
double Displayer::scale_times = 1.;
std::vector<unsigned int> Displayer::drawLists_;
std::map<unsigned char, void(*)(int, int)> Displayer::keyboard_functions_;
int Displayer::mouse_down_button_=GL_LEFT, Displayer::mouse_down_x_=0, Displayer::mouse_down_y_=0;
double Displayer::mouse_down_a_ = 0, Displayer::mouse_down_b_ = 0;
double Displayer::rotate_a_= M_PI/4, Displayer::rotate_b_= M_PI / 4;
double Displayer::translate_a_=0, Displayer::translate_b_=0;
mesh_tools::BoundingBox Displayer::box;
double Displayer::modelSize=1;

mesh_tools::Point3 camera,origin,mouse_down_origin;
GLfloat light_x_ = 0.1f, light_y_ = 0.1f, light_z_ = 0.2f;


void Displayer::init(int &argc, char** argv, const char *title, int width, int height)
{
  width_ = width;
  height_ = height;
  for(int i=0;i<argc;i++)
  {
    std::cerr << argv[i] << std::endl;
  }
  argc--;
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

  bindKey('a', [](int, int) {light_x_ -= 0.05f; std::cerr << light_x_ << "," << light_y_ << "," << light_z_ << std::endl; });
  bindKey('d', [](int, int) {light_x_ += 0.05f; std::cerr << light_x_ << "," << light_y_ << "," << light_z_ << std::endl; });
  bindKey('w', [](int, int) {light_y_ += 0.05f; std::cerr << light_x_ << "," << light_y_ << "," << light_z_ << std::endl; });
  bindKey('s', [](int, int) {light_y_ -= 0.05f; std::cerr << light_x_ << "," << light_y_ << "," << light_z_ << std::endl; });
  bindKey('z', [](int, int) {light_z_ -= 0.05f; std::cerr << light_x_ << "," << light_y_ << "," << light_z_ << std::endl; });
  bindKey('x', [](int, int) {light_z_ += 0.05f; std::cerr << light_x_ << "," << light_y_ << "," << light_z_ << std::endl; });
}

void Displayer::initParameters()
{
  GLfloat white_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
  GLfloat lmodel_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
  glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
  glLightfv(GL_LIGHT0, GL_AMBIENT, lmodel_ambient);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  
  glEnable(GL_AUTO_NORMAL);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_NORMALIZE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glEnable(GL_POINT_SMOOTH);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_POLYGON_SMOOTH);
  glEnable(GL_POLYGON_OFFSET_FILL);

  //glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
  //glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  //glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);

  glPolygonOffset(1.0, 2.0);
  
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDepthFunc(GL_LEQUAL);
  glFrontFace(GL_CCW);
  glCullFace(GL_BACK);
  glShadeModel(GL_FLAT);
  glPointSize(3);
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
      - 10000,
      + 10000
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
  glViewport(0, 0, width_, height_);
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
      mouse_down_origin = origin;
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
  glClearColor(1., 1., 1., 1.);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  GLfloat light_position[] = { light_x_, light_y_, light_z_, 0.0f };
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  camera = origin + mesh_tools::Point3(cos(rotate_a_)*cos(rotate_b_), sin(rotate_b_), sin(rotate_a_)*cos(rotate_b_));
  gluLookAt(camera[0], camera[1], camera[2], origin[0], origin[1], origin[2], 0, 1, 0);

  glPushMatrix();
  {
    glPushMatrix();
    glTranslated(0.3, 0.3, 0.3);
    //glutSolidSphere(0.05, 5, 5);
    glPopMatrix();

    glColor3d(1, 0, 0);
    glBegin(GL_LINES);
    glVertex3d(0, 0, 0);
    glVertex3d(10, 0, 0);
    glEnd();

    glColor3d(0, 1, 0);
    glBegin(GL_LINES);
    glVertex3d(0, 0, 0);
    glVertex3d(0, 10, 0);
    glEnd();

    glColor3d(0, 0, 1);
    glBegin(GL_LINES);
    glVertex3d(0, 0, 0);
    glVertex3d(0, 0, 10);
    glEnd();

    for (auto list : drawLists_)
      glCallList(list);
  }
  glPopMatrix();
  glutSwapBuffers();
}

void Displayer::CallBackMotion(int x, int y)
{
  int dx = x - mouse_down_x_, dy = y - mouse_down_y_;
  if (mouse_down_button_ == GLUT_LEFT_BUTTON)
  {
    auto camera_offset = mesh_tools::Point3(cos(rotate_a_)*cos(rotate_b_), sin(rotate_b_), sin(rotate_a_)*cos(rotate_b_));
    auto x_offset = mesh_tools::Point3(-sin(rotate_a_) , 0, cos(rotate_a_));
    auto y_offset = x_offset.outer_product(camera_offset);
    origin = mouse_down_origin + x_offset * dx*modelSize * scale_times*0.001 + y_offset * dy*modelSize * scale_times*0.001;
  }
  else if (mouse_down_button_ == GLUT_RIGHT_BUTTON)
  {
    constexpr double eps = 1e-8;
    rotate_a_ = mouse_down_a_ + dx * 0.01;
    rotate_b_ = mouse_down_b_ + dy * 0.01;
    while (rotate_a_ > M_PI*2)
      rotate_a_ -= M_PI*2;
    while (rotate_a_ < 0)
      rotate_a_ += M_PI*2;
    while (rotate_b_ > M_PI/2- eps)
      rotate_b_ = M_PI/2- eps;
    while (rotate_b_ < -M_PI / 2+ eps)
      rotate_b_ = -M_PI / 2+ eps;
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
  origin = box.center() * -1;
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
  auto normal = (point_b - point_a).outer_product(point_c - point_b);
  glNormal3d(normal[0] , normal[1], normal[2]);
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
  GLboolean isLightOpen;
  glGetBooleanv(GL_LIGHTING, &isLightOpen);
  glDisable(GL_LIGHTING);
 // glDepthRange(0.0, 0.998);
  glBegin(GL_LINES);
  glVertex3d(point_a[0], point_a[1], point_a[2]);
  glVertex3d(point_b[0], point_b[1], point_b[2]);
  glEnd();
 // glDepthRange(0.0, 1.);
  if (isLightOpen)
    glEnable(GL_LIGHTING);
}

void DrawableObject::drawPoint(const mesh_tools::Point3 & point)const
{
  GLboolean isLightOpen;
  glGetBooleanv(GL_LIGHTING, &isLightOpen);
  glBegin(GL_POINTS);
  glVertex3d(point[0], point[1], point[2]);
  glEnd();
  if (isLightOpen)
    glEnable(GL_LIGHTING);
}
