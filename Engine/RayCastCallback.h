#pragma once
#include <Box2d.h>

class RayCastCallback : public b2RayCastCallback
{
public:
    RayCastCallback() : m_fixture(NULL)
    {};

    float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction);

    b2Fixture* m_fixture;
};

