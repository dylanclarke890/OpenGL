#pragma once

#include "Test.h"

namespace test
{
  class Texture2D: public Test
  {
  public:
    Texture2D();
    ~Texture2D();

    void OnUpdate(float deltatime);
    void OnRender();
    void OnImGuiRender();
  };
}