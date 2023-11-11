#pragma once

#include "Test.h"

namespace test
{
  class ClearColor: public Test
  {
  private:
    float m_ClearColor[4];

  public:
    ClearColor();
    ~ClearColor();

    void OnUpdate(float deltatime);
    void OnRender();
    void OnImGuiRender();
  };
}