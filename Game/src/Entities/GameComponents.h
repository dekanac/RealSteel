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
		bool speedBoosted{ false };
		bool speedReduced{ false };
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

	enum class PickupType {

		Health = 0,
		Ammunation,
		Speed
	};

	struct PickupComponent : public Engine::Component {

		PickupType m_Type{};

		PickupComponent() = default;
		PickupComponent(PickupType type_) : m_Type(type_) {}
	};

	struct PalmComponent : public Engine::Component {

		Engine::Entity* birdsAnimation{};
		bool m_Birds{ true };
	};

	struct LakeComponent : public Engine::Component {

	};
}