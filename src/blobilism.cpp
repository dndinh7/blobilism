#include <iostream>
#include <vector>
#include <cmath>
#include <glm/glm.hpp>
#include "tinygl-cpp.h"
using namespace tinygl;


struct Circle {
  float x;
  float y;
  float size;
  float r;
  float g;
  float b;
  float alpha;
};


class MyWindow : public Window {
 public:
  MyWindow(int w, int h) : Window(w, h) {}

  void setup() override {
    std::cout << "Window size: " << width() << ", " << height() << std::endl;
    cur_circle= Circle { 0, 0, 5, 0, 0, 0, 1 };
  }

  virtual void mouseMotion(int x, int y, int dx, int dy) {
    if (mouseIsDown(GLFW_MOUSE_BUTTON_LEFT)) {
      cur_circle.x= mouseX();
      cur_circle.y= mouseY();
      // todo: store a circle with the current color, size, x, y
      list_circles.push_back(cur_circle);
      std::cout << "Pressed LEFT_CLICK: stored circle" << std::endl;
    }
  }
  
  virtual void mouseDown(int button, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
      // todo: check if user clicked a color 
      // float mx = mouseX();  // current mouse pos x
      // float my = mouseY();  // current mouse pos y
    }
  }

  void keyDown(int key, int mods) {
    if (key == GLFW_KEY_UP) {
      // increase size of circle
      cur_circle.size+= 5.0f;
      std::cout << "Pressed UP: Increase point size to " << cur_circle.size << std::endl;
    }
    else if (key == GLFW_KEY_DOWN) {
      // decrease size of circle
      cur_circle.size= std::max(1.0f, cur_circle.size - 5);
      std::cout << "Pressed DOWN: Decrease point size to " << cur_circle.size << std::endl;
    }
    else if (key == GLFW_KEY_LEFT) {
      // decrease alpha
      cur_circle.alpha= std::max(0.0f, cur_circle.alpha - 0.05f);
      std::cout << "Pressed LEFT: Decrease transparency to " << cur_circle.alpha << std::endl;
    }
    else if (key == GLFW_KEY_RIGHT) {
      // increase alpha
      cur_circle.alpha= std::min(1.0f, cur_circle.alpha + 0.05f);
      std::cout << "Pressed RIGHT: Increase transparency to " << cur_circle.alpha << std::endl;
    }
    else if (key == GLFW_KEY_C) {
      // clear vector of circles
      list_circles.clear();
      std::cout << "Pressed 'C': Clear canvas " << std::endl;
    }
  }

  void draw() override {
    background(0.95f, 0.95f, 0.95f); // parameters: r, g, b

    //color(1.0f, 0.5f, 0.5f);
    //circle(width() * 0.5f, height() * 0.5, 300); // x, y, radius

    for (Circle c: list_circles) {
      color(c.r, c.g, c.b, c.alpha);
      circle(c.x, c.y, c.size);
    }

    // todo : draw pallet
    // reference: https://www.color-hex.com/color-palette/1021571
    // terracotta rave color palette
    color(0.1f, 0.1f, 0.1f);
    square(width()/2.0f, 35, width(), 70);
  }
 private:

  // todo: create member variables for 
  // current circle size
  // current transparency
  // current color
  // list of circles to draw each frame
  // color pallet

  Circle cur_circle;
  std::vector<glm::vec3> palette;
  std::vector<Circle> list_circles;
  
};

int main() {
  MyWindow window(500, 500);
  window.run();
}
