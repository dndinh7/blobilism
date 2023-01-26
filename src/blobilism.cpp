#include <iostream>
#include <vector>
#include <cmath>
#include "tinygl-cpp.h"
using namespace tinygl;

/*
  This program creates a window and allows
  the user to choose a color and draw using their
  mouse. The user can choose from the palette of
  colors shown in the bottom of the window.
  The user can adjust the size of the brush
  by using the up and down arrow keys. The user can 
  also adjust the transparency of the brush stroke
  by using the left and right arrow keys. The user can 
  change the brush to a circle by pressing '1', a square
  by pressing '2', and a triangle by pressing '3'. 
  The user can then clear the screen by pressing 'c'.

  @author: David Dinh
  @version: January 26, 2023
*/

enum Shape { Circle, Square, Triangle };

struct ColorVec {
  float r;
  float g;
  float b;
};

struct PalBrush {
  float x;
  float y;
  ColorVec myColor;
};

struct Brush {
  float x;
  float y;
  int size;
  ColorVec myColor;
  float alpha;
  Shape shape;
};

class MyWindow : public Window {
 public:
  MyWindow(int w, int h) : Window(w, h) {}

  void setup() override {
    std::cout << "Window size: " << width() << ", " << height() << std::endl;

    // reference: https://colorswall.com/palette/102
    // rainbow color palette
    float w= width();
    const float h= 35; // middle of the palette section

    // initializes the palette and spaces them accordingly
    // there will be paletteLength + 1 spacings in between the borders and circles
    const float spacing= w/(float)paletteLength;
    const float startingX= spacing*0.5f;

    for (int i= 0; i < paletteLength; i++) {
      palette.push_back(PalBrush{ startingX + spacing*i, h, palette_colors[i]});
    }
    curBrush= Brush { 0, 0, 5, palette[selectedColor].myColor, 1, Circle };
  }

  virtual void mouseMotion(int x, int y, int dx, int dy) {
    if (mouseIsDown(GLFW_MOUSE_BUTTON_LEFT)) {
      curBrush.x= mouseX();
      curBrush.y= mouseY();
      if (curBrush.y >= PALETTE_HEIGHT && curBrush.y <= height() && curBrush.x >= 0 && curBrush.x <= width()) {
        listBrushes.push_back(curBrush);
        std::cout << "Pressed LEFT_CLICK: stored ";
        switch (curBrush.shape) {
          case Circle:
            std::cout << "circle" << std::endl;
            break;
          case Square:
            std::cout << "square" << std::endl;
            break;
          case Triangle:
            std::cout << "triangle" << std::endl;
            break;
        }
      }
    }
  }
  
  virtual void mouseDown(int button, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
      float mx = mouseX();  // current mouse pos x
      float my = mouseY();  // current mouse pos y
      if (my >= 0 && my <= PALETTE_HEIGHT) {
        for (int i= 0; i < paletteLength; i++) {
          // checks if the click is in a circle
          float x= palette[i].x;
          float y= palette[i].y;
          if (std::sqrt(std::pow(x - mx, 2) + std::pow(y - my, 2)) <= PALETTE_RADIUS) {
            selectedColor= i;
            curBrush.myColor= palette[selectedColor].myColor;
            std::cout << "Pressed LEFT_CLICK: changed color to " << curBrush.myColor.r << " " << 
            curBrush.myColor.g << " " << curBrush.myColor.b << std::endl;
            break;
          }
        }
      }

    }
  }

  void keyDown(int key, int mods) {
    if (key == GLFW_KEY_UP) {
      // increase size of circle
      curBrush.size+= (curBrush.size == 1) ? 4 : 5;
      std::cout << "Pressed UP: Increase point size to " << curBrush.size << std::endl;
    }
    else if (key == GLFW_KEY_DOWN) {
      // decrease size of circle
      curBrush.size= std::max(1, curBrush.size - 5);
      std::cout << "Pressed DOWN: Decrease point size to " << curBrush.size << std::endl;
    }
    else if (key == GLFW_KEY_LEFT) {
      // decrease alpha
      curBrush.alpha= std::max(0.0f, curBrush.alpha - 0.05f);
      std::cout << "Pressed LEFT: Decrease transparency to " << curBrush.alpha << std::endl;
    }
    else if (key == GLFW_KEY_RIGHT) {
      // increase alpha
      curBrush.alpha= std::min(1.0f, curBrush.alpha + 0.05f);
      std::cout << "Pressed RIGHT: Increase transparency to " << curBrush.alpha << std::endl;
    }
    else if (key == GLFW_KEY_C) {
      // clear vector of circles
      listBrushes.clear();
      std::cout << "Pressed 'C': Clear canvas " << std::endl;
    } else if (key == GLFW_KEY_1) {
      curBrush.shape= Circle;
      std::cout << "Pressed '1': Change brush to Circle" << std::endl;
    } else if (key == GLFW_KEY_2) {
      curBrush.shape= Square;
      std::cout << "Pressed '2': Change brush to Square" << std::endl;
    } else if (key == GLFW_KEY_3) {
      curBrush.shape= Triangle;
      std::cout << "Pressed '3': Change brush to Triangle" << std::endl;
    }
  }

  void draw() override {
    background(0.95f, 0.95f, 0.95f); // parameters: r, g, b

    for (Brush brush: listBrushes) {
      color(brush.myColor.r, brush.myColor.g, brush.myColor.b, brush.alpha);
      switch (brush.shape) {
        case Circle:
          circle(brush.x, brush.y, brush.size);
          break;
        case Square:
          square(brush.x, brush.y, brush.size, brush.size);
          break;
        case Triangle:
          triangle(brush.x, brush.y, brush.size, brush.size);
          break;
      }
    }


    color(0.1f, 0.1f, 0.1f);
    square(width()/2.0f, 35, width(), 70);

    // this highlights the selected color
    color(48.0f/255.0f, 197.0f/255.0f, 1.0f); // this is selected blue color
    circle(palette[selectedColor].x, palette[selectedColor].y, PALETTE_DIAMETER+6);
    for (PalBrush brush: palette) {
      color(brush.myColor.r, brush.myColor.g, brush.myColor.b);
      circle(brush.x, brush.y, PALETTE_DIAMETER);
    }
  }


 private:
  Brush curBrush;
  std::vector<PalBrush> palette;
  std::vector<Brush> listBrushes;
  const int paletteLength= 8;
  const ColorVec palette_colors[8]{ColorVec{1.0f, 1.0f, 1.0f}, //white
      ColorVec{255.0f/255.0f, 0.0f, 0.0f}, //red
      ColorVec{255.0f/255.0f, 165.0f/255.0f, 0.0f}, //orange 
      ColorVec{1.0f, 1.0f, 0.0f}, //yellow 
      ColorVec{0.0f, 0.5f, 0.0f}, //green 
      ColorVec{0.0f, 0.0f, 1.0f}, //blue 
      ColorVec{75.0f/255.0f, 0.0f, 130.0f/255.0f}, // indigo
      ColorVec{238.0f/255.0f, 130.0f/255.0f, 238.0f/255.0f}}; //violet ;
  const float PALETTE_RADIUS= 28;
  const float PALETTE_DIAMETER= PALETTE_RADIUS*2;
  const float PALETTE_HEIGHT= 70;
  unsigned char selectedColor= 0;
};

int main() {
  MyWindow window(500, 500);
  window.run();
}
