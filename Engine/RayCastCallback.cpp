#include "precomp.h"
#include "RayCastCallback.h"

float32 RayCastCallback::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) {
    m_fixture = fixture;
    return fraction;
}