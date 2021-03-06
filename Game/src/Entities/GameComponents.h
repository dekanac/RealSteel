#pragma once
#include "Engine.h"
#include <Box2d.h>
#include <chrono>

namespace Game
{
	struct PhysicsComponent : public Engine::Component {
		b2World* world;
	};

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
		bool shieldActivated{ false };
		bool hasSpecial{ false };
		int missilePower{ 10 };
		float tankSpeed{ 100.f };

		Engine::Entity* tankTurretEntity{};
		Engine::Entity* tankBodyEntity{};
	};

	struct TankBodyComponent : public Engine::Component {

	};

	struct TankTurretComponent : public Engine::Component {
				
	};

	struct ShootComponent : public Engine::Component {
		bool cooldown = false;
		int ammo = 100;
		bool infinite_ammo = true;
		std::chrono::steady_clock::time_point last_fired;
	};

	struct BulletComponent : public Engine::Component {
		bool collided{false};
		bool m_specialBullet{ false };
		int m_power{ 10 };


		BulletComponent() = default;
	};

	struct OwnershipComponent : public Engine::Component {
		bool ownedByPlayer{false};

		OwnershipComponent(bool p_) : ownedByPlayer(p_ ) {};
	};

	struct WallComponent : public Engine::Component {

	};

	struct TimedComponent : public Engine::Component {
		std::chrono::steady_clock::time_point created;
		int lifeMillis = 0;

		TimedComponent(int m_) : lifeMillis(m_) {
			created = std::chrono::high_resolution_clock::now();
		};
	};

	enum class PickupType {

		Health = 0,
		Health_Help,
		Ammunation,
		Max_Ammunition,
		Speed,
		Missile_Powerup,
		Special_Missile, 
		Shield
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

	struct SolidObjectComponent : public Engine::Component {
		//objekti kroz koje ne moze da se prodje
	};

	struct MenuComponent : public Engine::Component {

	};

	struct BotComponent : public Engine::Component {
		Engine::Entity* tankEntity;
		vec2 m_CurrentPosition{};
		int m_GridX;
		int m_GridY;
		bool m_FiredRecently;
		bool m_isMoving = false;
	};
}