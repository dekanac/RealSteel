#pragma once
#include "Engine.h"

namespace Game
{
	struct HealthBarComponent : public Engine::Component {
		Engine::Entity* healthBarFrame;
	};

	struct HealthBarFrameComponent : public Engine::Component {

	};

	struct PlayerGameComponent : public Engine::Component {
		Engine::Entity* tankEntity;
	};

	struct TankComponent : public Engine::Component {
		float tankSpeed{ 100.f };

		Engine::Entity* tankTurretEntity{};
		Engine::Entity* tankBodyEntity{};
	};

	struct TankBodyComponent : public Engine::Component {

	};

	struct TankTurretComponent : public Engine::Component {
				
	};

	struct ShootComponent : public Engine::Component {

	};

	struct BulletComponent : public Engine::Component {

	};

	struct WallComponent : public Engine::Component {

	};
}