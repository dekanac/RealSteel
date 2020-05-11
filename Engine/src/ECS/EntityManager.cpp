#include "precomp.h"
#include "EntityManager.h"
#include "ECS/Entity.h"
#include "Render/Texture.h"

namespace Engine
{
    bool EntityManager::Init()
    {
        return true;
    }

    void EntityManager::Update(float dt)
    {
        // Post-physics update
    }

    void EntityManager::AddEntity(Entity* e)
    {
        m_Entities.emplace_back(e);
    }
    void EntityManager::AddEntity(std::unique_ptr<Entity>&& e)
    {
        m_Entities.push_back(std::move(e));
    }

    void EntityManager::RemoveEntity(Entity* e)
    {


        for (auto i = m_Entities.begin(); i < m_Entities.end(); i++)
        {
            if ((*i).get() == e)
            {
                if ((*i).get()->HasComponent<Box2dBodyComponent>())
                {
                    auto tmp = (*i).get()->GetComponent<Box2dBodyComponent>();
                    tmp->body->GetWorld()->DestroyBody(tmp->body);
                }
                m_Entities.erase(i);
                break;
            }

        }
    }

}
