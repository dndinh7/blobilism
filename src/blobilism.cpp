#include <iostream>
#include <vector>
#include <cmath>
#include <glm/glm.hpp>
#include "tinygl-cpp.h"
using namespace tinygl;


struct vecColor {
  float r;
  float g;
  float b;
};

struct palColor {
  float x;
  float y;
  vecColor vColor;
};

struct Circle {
  float x;
  float y;
  float size;
  vecColor vColor;
  float alpha;
};

class MyWindow : public Window {
 public:
  MyWindow(int w, int h) : Window(w, h) {}

  void setup() override {
    std::cout << "Window size: " << width() << ", " << height() << std::endl;
    cur_circle= Circle { 0, 0, 5, vecColor {0, 0, 0}, 1 };


    // reference: https://colorswall.com/palette/102
    // rainbow color palette


    float w= width();
    const float h= 35; // middle of the palette section


    // there will be palette_length + 1 spacings in between the borders and circles

    const float spacing= w/(float)palette_length;
    const float starting_x= spacing*0.5f;

    for (int i= 0; i < palette_length; i++) {
      palette.push_back(palColor{ starting_x + spacing*i, h, palette_colors[i]});
    }
  }

  virtual void mouseMotion(int x, int y, int dx, int dy) {
    if (mouseIsDown(GLFW_MOUSE_BUTTON_LEFT)) {
      cur_circle.x= mouseX();
      cur_circle.y= mouseY();
      // todo: store a circle with the current color, size, x, y
      if (cur_circle.y >= pal_height && cur_circle.y <= height() && cur_circle.x >= 0 && cur_circle.x <= width()) {
        list_circles.push_back(cur_circle);
        std::cout << "Pressed LEFT_CLICK: stored circle" << std::endl;
      }
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
      color(c.vColor.r, c.vColor.g, c.vColor.b, c.alpha);
      circle(c.x, c.y, c.size);
    }


    color(0.1f, 0.1f, 0.1f);
    square(width()/2.0f, 35, width(), 70);

    // todo : draw pallet
    for (palColor c: palette) {
      color(c.vColor.r, c.vColor.g, c.vColor.b);
      circle(c.x, c.y, pal_diameter);
    }
  }
 private:

  // todo: create member variables for 
  // current circle size
  // current transparency
  // current color
  // list of circles to draw each frame
  // color pallet

  Circle cur_circle;
  std::vector<palColor> palette;
  std::vector<Circle> list_circles;
  const int palette_length= 8;
  const vecColor palette_colors[8]{vecColor{1.0f, 1.0f, 1.0f}, //white
      vecColor{255.0f/255.0f, 0.0f, 0.0f}, //red
      vecColor{255.0f/255.0f, 165.0f/255.0f, 0.0f}, //orange 
      vecColor{1.0f, 1.0f, 0.0f}, //yellow 
      vecColor{0.0f, 0.5f, 0.0f}, //green 
      vecColor{0.0f, 0.0f, 1.0f}, //blue 
      vecColor{75.0f/255.0f, 0.0f, 130.0f/255.0f}, // indigo
      vecColor{238.0f/255.0f, 130.0f/255.0f, 238.0f/255.0f}}; //violet ;
  const float pal_radius= 28;
  const float pal_diameter= pal_radius*2;
  const float pal_height= 70;
  
};

int main() {
  MyWindow window(500, 500);
  window.run();
}
