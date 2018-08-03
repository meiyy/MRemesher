#pragma once
#include "surface.h"
#include <map>
#include "boundingbox.h"

class DrawableObject
{
public:
  virtual void addToBoundingBox(mesh_tools::BoundingBox &box)const = 0;
  virtual void draw()const=0;
protected:
  void drawTriangle(const mesh_tools::Point3 &point_a, const mesh_tools::Point3 &point_b, const mesh_tools::Point3 &point_c)const;
  void setColor(double red, double green, double blue, double alpha)const;
  void drawLine(const mesh_tools::Point3 &point_a ,const mesh_tools::Point3 &point_b)const;
  void drawPoint(const mesh_tools::Point3 &point)const;
};

class Displayer
{
public:

  ~Displayer() = default;

  static Displayer& getInstance()
  {
    return instance_;
  }

  void init(int &argc, char** argv, const char *title, int width, int height);

  void runLoop();

  static void bindKey(unsigned char key, void function(int, int));

  static unsigned int addDrawableObject(const DrawableObject &object);

private:
  Displayer() = default;

  static void initCallBackFunctions();

  static void initParameters();

  static void getScaleSize();

  static void CallBackReshape(int width, int height);

  static void CallBackMouse(int status, int button, int x, int y);

  static void CallBackKeyboard(unsigned char key, int x, int y);

  static void CallBackDisplay();

  static void CallBackMotion(int x, int y);


  static Displayer instance_;
  static int width_, height_;
  static double scale_times;
  static std::vector<unsigned int> drawLists_;
  static std::map<unsigned char, void(*)(int, int)> keyboard_functions_;
  static int mouse_down_button_, mouse_down_x_, mouse_down_y_;
  static double mouse_down_a_, mouse_down_b_;
  static double rotate_a_, rotate_b_;
  static double translate_a_, translate_b_;
  static double modelSize;
  static mesh_tools::BoundingBox box;
};
